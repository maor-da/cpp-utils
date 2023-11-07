#pragma once
#include "Process.h"

namespace __NAMESPACE__
{

class CPP_UTILS_API WindowsProcess : public Process
{
public:
	using base_t = Process;

	enum class ACCESS : uint32_t {
		terminate		   = PROCESS_TERMINATE,
		create_thread	   = PROCESS_CREATE_THREAD,
		set_sessionid	   = PROCESS_SET_SESSIONID,
		vm_operation	   = PROCESS_VM_OPERATION,
		vm_read			   = PROCESS_VM_READ,
		vm_write		   = PROCESS_VM_WRITE,
		dup_handle		   = PROCESS_DUP_HANDLE,
		create_process	   = PROCESS_CREATE_PROCESS,
		set_quota		   = PROCESS_SET_QUOTA,
		set_info		   = PROCESS_SET_INFORMATION,
		query_info		   = PROCESS_QUERY_INFORMATION,
		suspend_resume	   = PROCESS_SUSPEND_RESUME,
		query_limited_info = PROCESS_QUERY_LIMITED_INFORMATION,
		set_limited_info   = PROCESS_SET_LIMITED_INFORMATION,
		all_access		   = PROCESS_ALL_ACCESS
	};

	// open process
	WindowsProcess(uint32_t pid, uint32_t access);

	WindowsProcess(uint32_t pid);

	// create process
	WindowsProcess(std::filesystem::path path);

	WindowsProcess(WindowsProcess&&) = default;

	~WindowsProcess();

	static std::shared_ptr<WindowsProcess> cast(std::shared_ptr<Process> p);

	std::shared_ptr<winutil::Token> get_token();

	virtual bool is_wow64() = 0;

protected:
#pragma warning(disable : 4251)
	std::shared_ptr<winutil::Token> m_Token;
#pragma warning(default : 4251)
};

class WindowsNativeProcess : public WindowsProcess
{
public:
	using base_t = WindowsProcess;
	using base_t::base_t;

	bool is_wow64() override;
};

class WindowsWoWProcess : public WindowsProcess
{
public:
	using base_t = WindowsProcess;
	using base_t::base_t;

	bool is_wow64() override;
};

}  // namespace __NAMESPACE__
