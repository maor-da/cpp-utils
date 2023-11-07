#pragma once

#include <cpp-utils/TypeHelpers.h>
#include <cpp-utils/internals/PE.h>
#include <stdint.h>

#include <filesystem>
#include <list>
#include <memory>
#include <string>

struct ATTACH_PROCESS_FILTER {
	std::string name;
	// TODO: add capabilities filter
};

class CPP_UTILS_API Process
{
public:
// TODO: Make it cross platform
#ifdef WIN32
	using handle_t = HANDLE;
#endif	// WIN32

	virtual ~Process() = 0;

	std::list<handle_t> get_handles();
	std::list<handle_t> get_modules();
	std::list<handle_t> get_threads();

	inline operator handle_t() const
	{
		return m_Handle;
	}

	inline operator bool() const
	{
		return (m_Handle != nullptr) /* && (m_Handle != INVALID_HANDLE_VALUE)*/;
	}

protected:
#pragma warning(disable : 4251)
	handle_t m_Handle;
	std::filesystem::path m_Path;
	std::string m_Name;
	std::string m_Commandline;
	uint32_t m_Pid;

#pragma warning(default : 4251)
};

CPP_UTILS_API std::shared_ptr<Process> AttachProcess(uint32_t pid);
CPP_UTILS_API std::shared_ptr<Process> ListProcesses(ATTACH_PROCESS_FILTER&& filter);
