#pragma once
#include <Windows.h>

#include <functional>
#include <string>
#include <utility>

#include "Logger.h"
#include "QueueManager.h"

// Message type namedpipe client
class NamedpipeClient
	: private QueueManager<std::pair<std::wstring, std::function<std::wstring(std::wstring_view)>>>
{
public:
	using callback_t  = std::function<std::wstring(std::wstring_view)>;
	using container_t = std::pair<std::wstring, callback_t>;
	using base_t	  = QueueManager<container_t>;

	NamedpipeClient(std::wstring_view name, uint8_t nThreads = 4) : base_t(nThreads), m_PipeName(name)
	{
		this->start();
	}

	NamedpipeClient& operator<<(std::wstring_view message)
	{
		this->push(
			std::make_unique<container_t>(message, [](std::wstring_view ws) { return std::wstring{}; }));
		return *this;
	}

	void operator()(std::wstring_view message, callback_t cb)
	{
		this->push(std::make_unique<container_t>(message, cb));
	}

private:
	std::wstring m_PipeName;

	bool worker(container_t& obj) override
	{
		SetThreadDescription(GetCurrentThread(), L"NamedpipeClient");

		if (!WaitNamedPipeW(m_PipeName.c_str(), NMPWAIT_WAIT_FOREVER)) {
			// If the pipe is not exist, use the queuemanager retries
			return false;
		}

		// There is pipe instance available, try to get it
		auto pipe =
			CreateFileW(m_PipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

		if (pipe == INVALID_HANDLE_VALUE) {
			// instance was taken, try again or too many tries
			return false;
		}

		DWORD mode = PIPE_READMODE_MESSAGE;
		if (!SetNamedPipeHandleState(pipe, &mode, NULL, NULL)) {
			// The pipe is missed configured
			DisconnectNamedPipe(pipe);
			CloseHandle(pipe);
			return false;
		}

		DWORD written, readed;
		std::wstring message = obj.first;
		BOOL ret;
		auto readBuf = std::make_unique<wchar_t[]>(1024);

		do {
			ret = WriteFile(pipe, message.c_str(), message.length() * sizeof(wchar_t), &written, nullptr);

			if (!ret) {
				// failed write to pipe
				return false;
			}

			message.clear();

			do {
				readed = 0;
				ret	   = ReadFile(pipe, readBuf.get(), 1024 * sizeof(wchar_t), &readed, nullptr);

				if (!ret) {
					if (GetLastError() == ERROR_BROKEN_PIPE) {
						// server disconnected
						return true;
					}

					if (GetLastError() != ERROR_MORE_DATA) {
						// server disconnected
						break;
					}
				}

				if (readed) {
					message += std::wstring_view(readBuf.get(), readed / sizeof(wchar_t));
				}
			} while (!ret);

			message = obj.second(message);

			if (message.empty()) {
				break;
			}

		} while (true);

		FlushFileBuffers(pipe);
		DisconnectNamedPipe(pipe);
		CloseHandle(pipe);

		return true;
	}
};