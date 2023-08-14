#pragma once
#include <memory>

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

	template <LOG_CHANNELS type>
	constexpr static inline void enable_channel()
	{
		LoggerImp::instance().enable_channel<type>();
	}

	template <LOG_CHANNELS type>
	constexpr static inline void disable_channel()
	{
		LoggerImp::instance().disable_channel<type>();
	}

	template <LOG_CHANNELS type>
	static inline void set_channel(std::unique_ptr<LogChannel<type>> channel)
	{
		LoggerImp::instance().set_channel<type>(std::move(channel));
	}
};
