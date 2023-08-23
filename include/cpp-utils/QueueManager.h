#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

/**
 *        +-----------+    +-----------+    +-----------+  +-----------+  +-----------+  +-----------+
 *        |           |    |           |    |           |  |           |  |           |  |           |
 * +------+   Push    |    |   Flush   |    |   Start   |  |   Stop    |  | Shutdown  |  | Shutdown  |
 * |      |           |    |           |    |           |  |           |  |    all    |  | by tid    |
 * |      +-----------+    +-----+-----+    +-----+-----+  +-----+-----+  +-----+-----+  +-----+-----+
 * |                             |                |              |              |              |
 * |      +----------------------v-----+    +-----v-----+        |              |              |
 * |      |                            |    |           <--------+              |              |
 * +------>         Queue              +----> Dispatcher<-----------------------+              |
 *        |                            |    |           <--------------------------------------+
 *        +----------------------------+    +-----+-----+
 *                                                |
 *                                                |
 *                                          +-----v-----+
 *                                          |           |
 *                                          |  Worker   |
 *                                          |           |
 *                                          +-----------+
 *
 */
template <class Container, template <class> class PT = std::unique_ptr>
class pointer_queue : public std::queue<PT<Container>>
{
public:
	using base_t = std::queue<PT<Container>>;
	using elem_t = PT<Container>;

	constexpr inline elem_t move_front()
	{
		if (base_t::empty()) {
			return {};
		}
		elem_t t = std::move(this->front());
		this->pop();
		return t;
	}
};

class ThreadExit : public std::exception
{
};

template <class Container, bool SYNC = false, template <class> class PT = std::unique_ptr>	// TODO: concept
class QueueManager
{
public:
	using elem_t = PT<Container>;

	QueueManager(uint8_t poolSize, uint32_t maxQueue = 2048, uint8_t retries = 3, uint8_t interval = 1)
		: m_PoolSize(poolSize), m_Retries(retries), m_Interval(interval), m_MaxQueue(maxQueue)
	{
		for (uint8_t i = 0; i < poolSize; i++) {
			add_thread();
		}
	}

	// Push to the queue and notify the thread
	bool push(elem_t Obj)
	{  // implicit move semantics with unique_ptr
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			if (m_Queue.size() >= m_MaxQueue) {
				return false;  // The queue is full
			}
			m_Queue.push(std::move(Obj));
		}
		m_ConditionVariable.notify_all();
		return true;
	}

	// Start the worker
	void start()
	{
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_Working = true;
		}
		m_ConditionVariable.notify_all();
	}

	// stop by set the flag to false
	void stop()
	{
		std::lock_guard<std::mutex> lock(m_QueueMutex);
		m_Working = false;
	}

	void drop()
	{
		std::lock_guard<std::mutex> lock(m_QueueMutex);
		pointer_queue<Container, PT> empty;
		m_Queue.swap(empty);  // will free the memory;
	}

	void shutdown_all()
	{
		if (m_Pool.empty()) {
			return;
		}

		m_Pool.clear();
	}

	void shutdown_one()
	{
		if (m_Pool.empty()) {
			return;
		}

		m_Pool.pop_back();
	}

	void add_thread()
	{
		std::lock_guard<std::mutex> poolLock(m_PoolMutex);
		m_Pool.emplace_back(new std::thread(&QueueManager<Container, SYNC, PT>::dispatcher, this), m_Deleter);
	}

	uint8_t get_poolsize()
	{
		return m_PoolSize;
	}

private:
	std::mutex m_QueueMutex;   // queue, scheduling, signals mutex
	std::mutex m_WorkerMutex;  // workers mutex
	std::condition_variable m_ConditionVariable;
	pointer_queue<Container, PT> m_Queue;

	std::vector<std::unique_ptr<std::thread, std::function<void(std::thread*)>>> m_Pool;
	uint8_t m_PoolSize;
	uint8_t m_Retries;
	uint8_t m_Interval;
	uint32_t m_MaxQueue;
	std::mutex m_PoolMutex;

	bool m_Working	= false;
	bool m_Shutdown = false;
	std::thread::id m_ShutdownByTID;

	std::function<void(std::thread*)> m_Deleter = [this](std::thread* T) {
		if (T) {
			this->shutdown_thread(T->get_id());
			if (T->joinable()) {
				T->join();
			}

			m_Shutdown		= false;
			m_ShutdownByTID = std::thread::id();

			delete T;
		}
	};

	void shutdown_thread(std::thread::id id)
	{
		std::lock_guard<std::mutex> poolLock(m_PoolMutex);

		m_ShutdownByTID = id;
		m_Shutdown		= true;

		m_ConditionVariable.notify_all();
	}

	elem_t get_task()
	{
		std::unique_lock<std::mutex> queueLock(m_QueueMutex);
		// queueLock.unlock();
		m_ConditionVariable.wait(queueLock, [this]() {
			return ((m_Queue.size() > 0) && m_Working) ||							 // work
				   (m_Shutdown && (std::this_thread::get_id() == m_ShutdownByTID));	 // shutdown
		});

		if (m_Shutdown && (std::this_thread::get_id() == m_ShutdownByTID)) {
			throw ThreadExit();
		}

		return m_Queue.move_front();
	}

	void dispatcher()
	{
		while (true) {
			try {
				std::unique_lock<std::mutex> syncLock(m_WorkerMutex, std::defer_lock);
				if (SYNC) {	 // synchronize between workers
					syncLock.lock();
				}

				elem_t&& obj = get_task();

				if (!obj) {
					continue;
				}

				uint8_t counter = 0;
				do {
					if (worker(*obj)) {
						break;
					}

					counter++;

					if (m_Shutdown && (std::this_thread::get_id() == m_ShutdownByTID)) {
						throw ThreadExit();
					}

					if (m_Retries == -1) {	// infinite numbers of retries
						std::this_thread::sleep_for(std::chrono::seconds(m_Interval * (1 + counter % 10)));
					} else if (counter < m_Retries) {
						std::this_thread::sleep_for(std::chrono::seconds(m_Interval));
					} else {
						break;	// failed
					}
				} while (true);

				//if (ulock) {
				//	ulock.unlock();
				//}

			} catch ([[maybe_unused]] const ThreadExit& e) {
				// handle thread exit
				return;
			} catch ([[maybe_unused]] const std::exception& e) {
				// printf("%s\n", e.what());
			}

			_Releases_lock_(m_WorkerMutex);
		}

		return;
	}
	virtual bool worker(Container& Obj) = 0;
};
