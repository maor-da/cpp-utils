#pragma once

#include <stdint.h>

#include <memory>

#include "LogChannels.h"

// single instace implementation
class LoggerImp
{
public:
	static LoggerImp& instance();

	template <LOG_CHANNELS type>
	constexpr void set_channel(std::unique_ptr<LogChannel<type>> channel)
	{
		if constexpr (type == LOG_CHANNELS::STDOUT) {
			m_Ch_stdout.swap(channel);
		} else if constexpr (type == LOG_CHANNELS::ETW) {
		} else if constexpr (type == LOG_CHANNELS::FILE) {
		} else if constexpr (type == LOG_CHANNELS::DEBUG) {
			m_Ch_debug.swap(channel);
		}
	}

	template <LOG_CHANNELS type>
	constexpr void enable_channel()
	{
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::STDOUT) {
			if (m_Ch_stdout) {
				m_Ch_stdout->start();
				m_Channels.STDOUT = 1;
			}
		}
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::ETW) {
		}
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::FILE) {
		}
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::DEBUG) {
			if (m_Ch_debug) {
				m_Ch_debug->start();
				m_Channels.DEBUG = 1;
			}
		}
	}

	template <LOG_CHANNELS type>
	constexpr void disable_channel()
	{
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::STDOUT) {
			if (m_Ch_stdout) {
				m_Ch_stdout->shutdown_all();
				m_Ch_stdout->drop();
				m_Channels.STDOUT = 0;
			}
		}
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::ETW) {
		}
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::FILE) {
		}
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::DEBUG) {
			if (m_Ch_debug) {
				m_Ch_debug->shutdown_all();
				m_Ch_debug->drop();
				m_Channels.DEBUG = 0;
			}
		}
	}

	void set_channels(__channels cnls);

	void log(LOG_LEVEL level, std::string_view str);

private:
	LoggerImp();
	static std::unique_ptr<LoggerImp> m_Instance;
	__channels m_Channels;

	std::unique_ptr<LogChannel<LOG_CHANNELS::STDOUT>> m_Ch_stdout;
	std::unique_ptr<LogChannel<LOG_CHANNELS::DEBUG>> m_Ch_debug;
};
