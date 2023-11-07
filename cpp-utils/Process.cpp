#include <cpputils.h>

std::shared_ptr<Process> AttachProcess(uint32_t pid)
{
	std::shared_ptr<Process> p;

#ifdef WIN32
	winutil::ImportFunction<bool(HANDLE hProcess, USHORT * pProcessMachine, USHORT * pNativeMachine)>
		IsWow64Process2(L"kernel32.dll", "IsWow64Process2");

	do {
		if (!IsWow64Process2.is_valid()) {
			break;
		}

		auto h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, pid);
		if (!h) {
			break;
		}

		uint16_t wow64Type = IMAGE_FILE_MACHINE_UNKNOWN;
		if (!IsWow64Process2(h, &wow64Type, nullptr)) {
			break;
		}

		if (wow64Type != IMAGE_FILE_MACHINE_UNKNOWN) {
			p = std::make_shared<__NAMESPACE__::WindowsWoWProcess>(pid);
		} else {
			p = std::make_shared<__NAMESPACE__::WindowsNativeProcess>(pid);
		}

	} while (false);

#endif	// WIN32

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
