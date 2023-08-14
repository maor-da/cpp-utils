#include <cpp-utils/Logger.h>
#include <cpp-utils/QueueManager.h>
#include <cpp-utils/WindowsDebugLogChannel.h>

#include <iostream>
#include <sstream>

class TestQ : public QueueManager<std::string, false>
{
public:
	using base_t = QueueManager<std::string, false>;

	TestQ() : base_t(8) {}
	~TestQ()
	{
		// must close all thread before destroyed
		shutdown_all();
	}

	virtual void thread_init() override {}

	bool worker(std::string& obj) override
	{
		std::stringstream ss;
		ss << "Thread: " << std::this_thread::get_id() << " message: " << obj.c_str();
		printf("%s\n", ss.str().c_str());
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return true;
	}

private:
};

void someFunc(int start, TestQ* q)
{
	for (int i = start; i < start + 1000; i++) {
		std::stringstream ss;
		ss << "{t: " << std::this_thread::get_id() << " id: " << i << "}";
		q->push(std::make_unique<std::string>(ss.str()));
	}
}

bool test_queue_manager()
{
	auto q = new TestQ();

	q->start();

	auto start = std::chrono::high_resolution_clock::now();
	std::thread t1(someFunc, 0, q), t2(someFunc, 1000, q), t3(someFunc, 2000, q);
	t1.join();
	t2.join();
	t3.join();
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	std::cout << "run for " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
			  << " microseconds\n";

	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("shutdown all\n");
	q->shutdown_all();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("add 2 worker thread\n");
	q->add_thread();
	q->add_thread();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("Stop\n");
	q->stop();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("start\n");
	q->start();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("shutdown one\n");
	q->shutdown_one();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("Stop and flush\n");
	q->stop();
	q->drop();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("destruct\n");
	delete q;

	return true;
}

bool test_logger()
{
	Logger::set_level(LOG_LEVEL::Debug);
	Logger::set_channel<LOG_CHANNELS::DEBUG>(std::make_unique<WindowsDebugLogChannel>());
	Logger::enable_channel<LOG_CHANNELS::DEBUG>();

	Logger::fatal << "3456" << 5 << "asdfga";
	Logger::warn("sdfg %d sfh", 356);
	Logger::debug("sdfg %d sfh", 356);
	Logger::trace("sdfg %5d sfh", 356);
	return true;
}

int main()
{
	// test_queue_manager();

	test_logger();

	getchar();
	return 0;
}
