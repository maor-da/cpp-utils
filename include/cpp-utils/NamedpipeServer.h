#pragma once
#include <Windows.h>

#include <functional>
#include <string>
#include <vector>

#include "Logger.h"
#include "QueueManager.h"
#include "guid.h"



class NamedpipeServer : private QueueManager<HANDLE>
{
public:
	using base_t	 = QueueManager<HANDLE>;
	using callback_t = std::function<std::wstring(std::wstring_view)>;

	struct Configuration {
		LPCWSTR lpName;	 // \\.\pipe\pipename
		DWORD dwOpenMode = PIPE_ACCESS_DUPLEX;
		DWORD dwPipeMode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS;
		DWORD nMaxInstances						   = 16;
		DWORD nOutBufferSize					   = 4096;
		DWORD nInBufferSize						   = 4096;
		DWORD nDefaultTimeOut					   = NMPWAIT_USE_DEFAULT_WAIT;
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
	};

	NamedpipeServer(uint8_t nThreads, Configuration& config, callback_t callback);
	~NamedpipeServer();

private:
	std::vector<HANDLE> m_Servers;
	Configuration m_config;
	callback_t m_Callback;
	bool m_Running;

	void create_server(Configuration& config);
	bool worker(HANDLE& pipe) override;
};
