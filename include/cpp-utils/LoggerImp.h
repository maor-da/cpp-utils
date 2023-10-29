#pragma once

#include <stdint.h>

#include <memory>

#include "LogChannels.h"

#if defined(API_EXPORT)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

// single instace implementation
class LoggerImp
{
public:
	static API LoggerImp& instance();

	template <LOG_CHANNELS type>
	constexpr void set_channel(std::unique_ptr<LogChannel<type>> channel)
	{
		if constexpr (type == LOG_CHANNELS::STDOUT) {
			m_Ch_stdout.swap(channel);
		} else if constexpr (type == LOG_CHANNELS::NO_QUEUE_STDOUT) {
			m_Ch_no_queue_stdout.swap(channel);
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
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::NO_QUEUE_STDOUT) {
			if (m_Ch_no_queue_stdout) {
				m_Channels.NO_QUEUE_STDOUT = 1;
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
				m_Ch_stdout->stop();
			}
			m_Channels.STDOUT = 0;
		}
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::NO_QUEUE_STDOUT) {
			m_Channels.NO_QUEUE_STDOUT = 0;
		}
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::ETW) {
		}
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::FILE) {
		}
		if constexpr (type == LOG_CHANNELS::ALL || type == LOG_CHANNELS::DEBUG) {
			if (m_Ch_debug) {
				m_Ch_debug->stop();
			}
			m_Channels.DEBUG = 0;
		}
	}

	void set_channels(__channels cnls);

	API void log(LOG_LEVEL level, std::string_view str);

private:
	LoggerImp();
	__channels m_Channels;

	std::unique_ptr<LogChannel<LOG_CHANNELS::STDOUT>> m_Ch_stdout;
	std::unique_ptr<LogChannel<LOG_CHANNELS::NO_QUEUE_STDOUT>> m_Ch_no_queue_stdout;
	std::unique_ptr<LogChannel<LOG_CHANNELS::DEBUG>> m_Ch_debug;
};
