#pragma once
#include <stdint.h>

#include <functional>
#include <string>
#include <utility>

#include "QueueManager.h"

// Message type namedpipe client
class NamedpipeClient
	: private QueueManager<std::pair<std::wstring, std::function<std::wstring(std::wstring_view)>>>
{
public:
	using callback_t  = std::function<std::wstring(std::wstring_view)>;
	using container_t = std::pair<std::wstring, callback_t>;
	using base_t	  = QueueManager<container_t>;

	NamedpipeClient(std::wstring_view name, uint8_t nThreads = 4);

	NamedpipeClient& operator<<(std::wstring_view message);

	void operator()(std::wstring_view message, callback_t cb);

private:
	std::wstring m_PipeName;

	bool worker(container_t& obj) override;
};
