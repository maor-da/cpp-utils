#pragma once
#include <stdint.h>

#include <iostream>
#include <utility>

#include "QueueManager.h"

enum class LOG_CHANNELS : uint8_t {
	STDOUT = 0x1 << 0,
	ETW	   = 0x1 << 1,
	FILE   = 0x1 << 2,
	DEBUG  = 0x1 << 3,
	ALL	   = 0xFF
};

enum class LOG_LEVEL : uint8_t {
	Fatal,
	Error,
	Warning,
	Info,
	Debug,
	Trace
};

union __channels {
	struct {
		uint8_t STDOUT : 1;
		uint8_t ETW : 1;
		uint8_t FILE : 1;
		uint8_t DEBUG : 1;
	};
	uint8_t all;
};

const char* LevelName(LOG_LEVEL level);

template <LOG_CHANNELS T>
class LogChannel
{
public:
	LogChannel()
	{
		static_assert("not implemented");
	}
};

template <>
class LogChannel<LOG_CHANNELS::STDOUT> : public QueueManager<std::pair<LOG_LEVEL, std::string>, true>
{
public:
	using base_t	  = QueueManager<std::pair<LOG_LEVEL, std::string>, true>;
	using container_t = std::pair<LOG_LEVEL, std::string>;

	LogChannel() : base_t(2) {}

private:
	virtual void thread_init() override {}

	// Inherited via QueueManager
	virtual bool worker(container_t& Obj) override
	{
		std::cout << LevelName(Obj.first) << ":: " << Obj.second.c_str() << std::endl;

		return true;
	}
};

template <>
class LogChannel<LOG_CHANNELS::DEBUG> : public QueueManager<std::pair<LOG_LEVEL, std::string>, false>
{
public:
	using base_t	  = QueueManager<std::pair<LOG_LEVEL, std::string>, false>;
	using container_t = std::pair<LOG_LEVEL, std::string>;

	LogChannel() : base_t(2) {}

private:
	virtual void thread_init() = 0;
	// Inherited via QueueManager
	virtual bool worker(container_t& Obj) = 0;
};