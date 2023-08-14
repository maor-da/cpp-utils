#pragma once
#include <Windows.h>

#include <sstream>

#include "LogChannels.h"

class WindowsDebugLogChannel : public LogChannel<LOG_CHANNELS::DEBUG>
{
public:
	using base_t = LogChannel<LOG_CHANNELS::DEBUG>;

private:
	virtual void thread_init() override
	{
		SetThreadDescription(GetCurrentThread(), L"WindowsDebugLogChannel");
	}

	// Inherited via QueueManager
	virtual bool worker(container_t& Obj) override
	{
		std::ostringstream oss;
		oss << LevelName(Obj.first) << " :: " << Obj.second << std::endl;
		OutputDebugStringA(oss.str().c_str());

		return true;
	}
};
