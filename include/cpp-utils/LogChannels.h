#pragma once
#include <Windows.h>
#include <stdint.h>

#include <iostream>
#include <utility>

#include "QueueManager.h"

union __channels {
	struct {
		uint8_t STDOUT : 1;
		uint8_t ETW : 1;
		uint8_t FILE : 1;
		uint8_t DEBUG : 1;
		uint8_t NO_QUEUE_STDOUT : 1;
	};
	uint8_t all;
};

enum class LOG_CHANNELS : uint8_t {
	STDOUT			= 0x1 << 0,
	ETW				= 0x1 << 1,
	FILE			= 0x1 << 2,
	DEBUG			= 0x1 << 3,
	NO_QUEUE_STDOUT = 0x1 << 4,
	ALL				= 0xFF
};

enum class LOG_LEVEL : uint8_t {
	Fatal,
	Error,
	Warning,
	Info,
	Debug,
	Trace
};

CPP_UTILS_API const char* LevelName(LOG_LEVEL level);

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
class LogChannel<LOG_CHANNELS::STDOUT>
	: public QueueManager<std::pair<LOG_LEVEL, std::string>, true, std::shared_ptr>
{
public:
	using base_t	  = QueueManager<std::pair<LOG_LEVEL, std::string>, true, std::shared_ptr>;
	using container_t = std::pair<LOG_LEVEL, std::string>;

	LogChannel() : base_t(1) {}

private:
	// Inherited via QueueManager
	virtual bool worker(container_t& Obj) override
	{
		static int c = 0;
		std::cout << LevelName(Obj.first) << " (" << c++ << ") "
				  << ":: " << Obj.second.c_str() << std::endl;
		return true;
	}
};

template <>
class LogChannel<LOG_CHANNELS::NO_QUEUE_STDOUT>
{
public:
	void log(LOG_LEVEL level, std::string_view str)
	{
		static int c = 0;
		std::cout << LevelName(level) << " (" << c++ << ") "
				  << ":: " << str.data() << std::endl;
	}
};

template <>
class LogChannel<LOG_CHANNELS::DEBUG>
	: public QueueManager<std::pair<LOG_LEVEL, std::string>, true, std::shared_ptr>
{
public:
	using base_t	  = QueueManager<std::pair<LOG_LEVEL, std::string>, true, std::shared_ptr>;
	using container_t = std::pair<LOG_LEVEL, std::string>;

	LogChannel() : base_t(1) {}

private:
	// Inherited via QueueManager
	virtual bool worker(container_t& Obj) = 0;
};