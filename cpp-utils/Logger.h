#pragma once
#include "LogStream.h"

class Logger
{
public:
	using Channels = __channels;
	static __flush flush;

	static LogStream<LOG_LEVEL::Fatal> fatal;
	static LogStream<LOG_LEVEL::Error> error;
	static LogStream<LOG_LEVEL::Warning> warn;
	static LogStream<LOG_LEVEL::Info> info;
	static LogStream<LOG_LEVEL::Debug> debug;
	static LogStream<LOG_LEVEL::Trace> trace;

	static inline void set_level(LOG_LEVEL level)
	{
		LogStreamStorage::set_level(level);
	}

	static inline void set_channels(Channels channels)
	{
		LoggerImp::instance().set_channels(channels);
	}
};
