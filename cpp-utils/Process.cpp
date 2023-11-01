#include <cpputils.h>

std::shared_ptr<Process> AttachProcess(uint32_t pid)
{
	std::shared_ptr<Process> p = std::make_shared<__NAMESPACE__::WindowsNativeProcess>(pid);
	return p;
}

std::shared_ptr<Process> ListProcesses(ATTACH_PROCESS_FILTER&& filter)
{
	return std::shared_ptr<Process>();
}

Process::~Process() {}

std::list<handle_t> Process::get_handles()
{
	return std::list<handle_t>();
}

std::list<handle_t> Process::get_modules()
{
	return std::list<handle_t>();
}

std::list<handle_t> Process::get_threads()
{
	return std::list<handle_t>();
}
