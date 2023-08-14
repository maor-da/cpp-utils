#pragma once
#include <Windows.h>

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
		OutputDebugStringA((std::string(LevelName(Obj.first)) + " :: " + Obj.second + '\n').c_str());

		return true;
	}
};
