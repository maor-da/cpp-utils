#include "NamedpipeServer.h"

NamedpipeServer::~NamedpipeServer()
{
	m_Running = false;
	for (auto& pipe : m_Servers) {
		if (pipe) {
			if (CreateFileW(m_config.lpName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL) !=
				INVALID_HANDLE_VALUE) {
			}
			CloseHandle(pipe);
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

	Logger::info << "Start namedpipe server on thread: " << std::this_thread::get_id();

	while ((ConnectNamedPipe(pipe, NULL) || (GetLastError() == ERROR_PIPE_CONNECTED)) && m_Running) {
		Sleep(50);
		m_Callback(L"message");
	}

	return true;
}
