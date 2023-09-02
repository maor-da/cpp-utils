#include <cpp-utils/NamedpipeServer.h>

NamedpipeServer::~NamedpipeServer()
{
	m_Running = false;
	for (auto& pipe : m_Servers) {
		if (pipe) {
			// try wake up the pipe
			CreateFileW(m_config.lpName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		}
	}
}

NamedpipeServer::NamedpipeServer(uint8_t nThreads, Configuration& config, callback_t callback)
	: base_t(nThreads), m_config(config), m_Callback(callback)
{
	uint8_t instances = min(get_poolsize(), static_cast<uint8_t>(config.nMaxInstances));
	for (uint8_t i = 0; i < instances; i++) {
		create_server(m_config);
	}

	m_Running = true;
	this->start();

	// TODO: add thread with event signaling to replace server on error
}

inline void NamedpipeServer::create_server(Configuration& config)
{
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	hPipe		 = CreateNamedPipeW(config.lpName,
								config.dwOpenMode,
								config.dwPipeMode,
								config.nMaxInstances,
								config.nOutBufferSize,
								config.nInBufferSize,
								config.nDefaultTimeOut,
								config.lpSecurityAttributes);

	if (hPipe == INVALID_HANDLE_VALUE) {
		Logger::warn << "unable to create namedpipe server";
	}

	m_Servers.push_back(hPipe);
	this->push(std::make_unique<HANDLE>(hPipe));
}

inline bool NamedpipeServer::worker(HANDLE& pipe)
{
	SetThreadDescription(GetCurrentThread(), L"NamedpipeServer");

	Logger::debug << "Start namedpipe server on thread: " << std::this_thread::get_id();

	DWORD written, readed;
	std::wstring message;
	BOOL ret;
	auto readBuf = std::make_unique<wchar_t[]>(1024);

	while ((ConnectNamedPipe(pipe, NULL) || (GetLastError() == ERROR_PIPE_CONNECTED))) {
		if (!m_Running) {
			// Close pipe
			break;
		}

		message.clear();

		do {
			readed = 0;
			ret	   = ReadFile(pipe, readBuf.get(), 1024 * sizeof(wchar_t), &readed, nullptr);

			if (!ret) {
				if (GetLastError() != ERROR_MORE_DATA) {
					// Failed read
					break;
				}
			}

			if (readed) {
				message += std::wstring_view(readBuf.get(), readed / sizeof(wchar_t));
			}
		} while (!ret);

		if (!ret) {
			if (GetLastError() == ERROR_BROKEN_PIPE) {
				// server disconnected
				FlushFileBuffers(pipe);
				DisconnectNamedPipe(pipe);
				continue;
			}
		}

		message = m_Callback(message);

		if (message.empty()) {
			FlushFileBuffers(pipe);
			DisconnectNamedPipe(pipe);
			continue;
		}

		WriteFile(pipe, message.c_str(), message.length() * sizeof(wchar_t), &written, nullptr);
	}

	FlushFileBuffers(pipe);
	DisconnectNamedPipe(pipe);
	CloseHandle(pipe);

	Logger::debug << "Namedpipe server closed on thread: " << std::this_thread::get_id();

	return true;
}
