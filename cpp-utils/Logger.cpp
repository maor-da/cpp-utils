#include "Logger.h"

#include "LoggerImp.h"

std::unique_ptr<LoggerImp> LoggerImp::m_Instance = nullptr;

static std::mutex _mtx;	 // limit scope
LoggerImp& LoggerImp::instance()
{
	std::lock_guard<std::mutex> g(_mtx);
	if (!m_Instance) {
		m_Instance.reset(new LoggerImp);
	}
	return *m_Instance;
}

LoggerImp::LoggerImp() : m_Channels(Logger::Channels{.STDOUT = 1})
{
	activate_channels();
}

void LoggerImp::activate_channels()
{
	m_Ch_stdout.start();
}

void LoggerImp::log(LOG_LEVEL level, std::string_view str)
{
	for (uint8_t c = 1; c != 0x80; c <<= 1) {
		switch (static_cast<LOG_CHANNELS>(c & m_Channels.all)) {
			case LOG_CHANNELS::STDOUT: {
				m_Ch_stdout.push(
					std::make_unique<stdout_channel_t::container_t>(level, str));
				break;
			}
			case LOG_CHANNELS::ETW:
				break;
			case LOG_CHANNELS::FILE:
				break;
			case LOG_CHANNELS::DEBUG:
				break;
			default:
				break;
		}
	}
}

__flush Logger::flush;

LogStream<LOG_LEVEL::Fatal> Logger::fatal;
LogStream<LOG_LEVEL::Error> Logger::error;
LogStream<LOG_LEVEL::Warning> Logger::warn;
LogStream<LOG_LEVEL::Info> Logger::info;
LogStream<LOG_LEVEL::Debug> Logger::debug;
LogStream<LOG_LEVEL::Trace> Logger::trace;

LOG_LEVEL LogStreamStorage::m_MaxLevel = LOG_LEVEL::Warning;

const char* LevelName(LOG_LEVEL level)
{
	switch (level) {
		case LOG_LEVEL::Fatal:
			return "Fatal";
		case LOG_LEVEL::Error:
			return "Error";
		case LOG_LEVEL::Warning:
			return "Warning";
		case LOG_LEVEL::Info:
			return "Info";
		case LOG_LEVEL::Debug:
			return "Debug";
		case LOG_LEVEL::Trace:
			return "Trace";
		default:
			return "Unknown";
	}
}
