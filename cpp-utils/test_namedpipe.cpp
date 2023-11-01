#include <cpp-utils/NamedpipeClient.h>
#include <cpp-utils/NamedpipeServer.h>
#include <cpputils.h>

#include "tests.h"

bool test_namedpipe()
{
	Logger::set_level(LOG_LEVEL::Debug);

	NamedpipeServer::Configuration config;
	auto pipeName = L"//./pipe/" + winutil::guid::generate_uuid();
	config.lpName = pipeName.c_str();
	WaitNamedPipeW(config.lpName, NMPWAIT_WAIT_FOREVER);  // not exist - not blocking
	NamedpipeServer nps(16, config, [](std::wstring_view ws) {
		Logger::info << ws;
		auto val = std::wcstol(ws.data(), nullptr, 10);

		if (val) {
			return std::to_wstring(++val);
		}

		return std::wstring{};
	});

	NamedpipeClient client(config.lpName);

	client << L"I am the client";
	client << L"and this is counter ping pong";

	client(L"1", [](std::wstring_view ws) {
		auto val = std::wcstol(ws.data(), nullptr, 10);
		if (val > 10) {
			return std::wstring{};
		}

		return std::to_wstring(++val);
	});

	getchar();

	return true;
}