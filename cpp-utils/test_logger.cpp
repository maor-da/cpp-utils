#include <cpp-utils/Logger.h>
#include <cpp-utils/WindowsDebugLogChannel.h>

#include "tests.h"

bool test_logger()
{
	Logger::set_level(LOG_LEVEL::Debug);
	Logger::set_channel<LOG_CHANNELS::DEBUG>(std::make_unique<WindowsDebugLogChannel>());
	Logger::enable_channel<LOG_CHANNELS::DEBUG>();

	Logger::fatal << "3456" << 5 << "asdfga";
	Logger::warn("sdfg %d sfh", 356);
	Logger::debug("DEBUGGGGG %d sfh", 356);
	Logger::trace("TRACEEEEE %5d sfh", 356);
	return true;
}