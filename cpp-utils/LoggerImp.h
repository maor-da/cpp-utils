#pragma once

#include <stdint.h>

#include <memory>

#include "LogChannels.h"

// single instace
class LoggerImp
{
public:
	static LoggerImp& instance();

	void set_channels(__channels cnls)
	{
		m_Channels = cnls;
	}

	void log(LOG_LEVEL level, std::string_view str);

private:
	using stdout_channel_t = LogChannel<LOG_CHANNELS::STDOUT>;

	LoggerImp();
	static std::unique_ptr<LoggerImp> m_Instance;
	__channels m_Channels;

	stdout_channel_t m_Ch_stdout;

	void activate_channels();

};
