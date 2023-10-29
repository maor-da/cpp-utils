#include <cpp-utils/Logger.h>
#include <cpp-utils/WindowsDebugLogChannel.h>

#include "tests.h"

bool test_logger()
{
	Logger::set_level(LOG_LEVEL::Debug);
	Logger::disable_channel<LOG_CHANNELS::ALL>();

	Logger::set_channel<LOG_CHANNELS::DEBUG>(std::make_unique<WindowsDebugLogChannel>());
	Logger::enable_channel<LOG_CHANNELS::DEBUG>();

	Logger::set_channel<LOG_CHANNELS::NO_QUEUE_STDOUT>(
		std::make_unique<LogChannel<LOG_CHANNELS::NO_QUEUE_STDOUT>>());
	Logger::enable_channel<LOG_CHANNELS::NO_QUEUE_STDOUT>();

	Logger::fatal << "3456" << 5 << "asdfga";
	Logger::warn("sdfg %d sfh", 356);
	Logger::debug("DEBUGGGGG %d sfh", 356);
	Logger::trace("TRACEEEEE %5d sfh", 356);
	return true;
}