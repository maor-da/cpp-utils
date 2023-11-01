#pragma once
#include <memory>

#include "LogStream.h"

/**
 * This is a static methods class to instantiate log stream
 * for every log level which use a storage and queue the message to the
 * logger implementation message queue.
 * It could control the logging channels and log level
 */
class Logger
{
public:
	using Channels = __channels;
	static __flush flush;

	static CPP_UTILS_API LogStream<LOG_LEVEL::Fatal> fatal;
	static CPP_UTILS_API LogStream<LOG_LEVEL::Error> error;
	static CPP_UTILS_API LogStream<LOG_LEVEL::Warning> warn;
	static CPP_UTILS_API LogStream<LOG_LEVEL::Info> info;

#ifdef __NO_TRACE_LOGS__
	static CPP_UTILS_API VoidLogStream debug;
	static CPP_UTILS_API VoidLogStream trace;
#else
	static CPP_UTILS_API LogStream<LOG_LEVEL::Debug> debug;
	static CPP_UTILS_API LogStream<LOG_LEVEL::Trace> trace;
#endif	// __NO_TRACE_LOGS__

	static void set_level(LOG_LEVEL level)
	{
		LogStreamStorage::set_level(level);
	}

	template <LOG_CHANNELS type>
	static void enable_channel()
	{
		LoggerImp::instance().enable_channel<type>();
	}

	template <LOG_CHANNELS type>
	static void disable_channel()
	{
		LoggerImp::instance().disable_channel<type>();
	}

	template <LOG_CHANNELS type>
	static void set_channel(std::unique_ptr<LogChannel<type>> channel)
	{
		LoggerImp::instance().set_channel<type>(std::move(channel));
	}
};
