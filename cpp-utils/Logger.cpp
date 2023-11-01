#include <cpputils.h>

LoggerImp& LoggerImp::instance()
{
	static LoggerImp instance;
	return instance;
}

LoggerImp::LoggerImp()
{
	//set_channel<LOG_CHANNELS::STDOUT>(std::make_unique<LogChannel<LOG_CHANNELS::STDOUT>>());
	//enable_channel<LOG_CHANNELS::STDOUT>();
}

inline void LoggerImp::set_channels(__channels cnls)
{
	m_Channels = cnls;
}

void LoggerImp::log(LOG_LEVEL level, std::string_view str)
{
	auto msg = std::make_shared<std::pair<LOG_LEVEL, std::string>>(level, str);
	for (uint8_t c = 1; c > 0; c <<= 1) {
		switch (static_cast<LOG_CHANNELS>(c & m_Channels.all)) {
			case LOG_CHANNELS::STDOUT:
				if (m_Ch_stdout) {
					m_Ch_stdout->push(msg);
				}
				break;
			case LOG_CHANNELS::NO_QUEUE_STDOUT:
				if (m_Ch_no_queue_stdout) {
					m_Ch_no_queue_stdout->log(level, str);
				}
				break;
			case LOG_CHANNELS::ETW:
				break;
			case LOG_CHANNELS::FILE:
				break;
			case LOG_CHANNELS::DEBUG:
				if (m_Ch_debug) {
					m_Ch_debug->push(msg);
				}
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

#ifdef __NO_TRACE_LOGS__
VoidLogStream Logger::debug;
VoidLogStream Logger::trace;
#else
LogStream<LOG_LEVEL::Debug> Logger::debug;
LogStream<LOG_LEVEL::Trace> Logger::trace;
#endif	// __NO_TRACE_LOGS__

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
