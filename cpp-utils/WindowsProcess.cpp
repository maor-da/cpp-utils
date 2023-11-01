#include <cpputils.h>

// open process

__NAMESPACE__::WindowsProcess::WindowsProcess(uint32_t pid, uint32_t access)
{
	if (pid == 0) {
		m_Handle = GetCurrentProcess();
	} else {
		m_Handle = OpenProcess(access, false, pid);
	}

	m_Token = std::make_shared<winutil::Token>(m_Handle, winutil::Token::ACCESS::all_access);
}

__NAMESPACE__::WindowsProcess::WindowsProcess(uint32_t pid)
	: WindowsProcess(pid, static_cast<uint32_t>(ACCESS::all_access))
{
}

__NAMESPACE__::WindowsProcess::WindowsProcess(std::filesystem::path path) {}

__NAMESPACE__::WindowsProcess::~WindowsProcess()
{
	if (m_Handle) {
		CloseHandle(m_Handle);
	}
}

inline std::shared_ptr<winutil::Token> __NAMESPACE__::WindowsProcess::get_token()
{
	return m_Token;
}

// create process

__NAMESPACE__::WindowsNativeProcess::WindowsNativeProcess(uint32_t pid, uint32_t access) : base_t(pid, access)
{
}

__NAMESPACE__::WindowsNativeProcess::WindowsNativeProcess(uint32_t pid) : base_t(pid) {}

__NAMESPACE__::WindowsNativeProcess::WindowsNativeProcess(std::filesystem::path path) : base_t(path) {}

bool __NAMESPACE__::WindowsNativeProcess::is_wow64()
{
	return false;
}

__NAMESPACE__::WindowsWoWProcess::WindowsWoWProcess(uint32_t pid, uint32_t access) : base_t(pid, access) {}

__NAMESPACE__::WindowsWoWProcess::WindowsWoWProcess(uint32_t pid) : base_t(pid) {}

__NAMESPACE__::WindowsWoWProcess::WindowsWoWProcess(std::filesystem::path path) : base_t(path) {}

bool __NAMESPACE__::WindowsWoWProcess::is_wow64()
{
	return true;
}
