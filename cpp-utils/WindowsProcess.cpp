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

std::shared_ptr<__NAMESPACE__::WindowsProcess> __NAMESPACE__::WindowsProcess::cast(std::shared_ptr<Process> p)
{
	return std::dynamic_pointer_cast<winutil::WindowsProcess>(p);
}

inline std::shared_ptr<winutil::Token> __NAMESPACE__::WindowsProcess::get_token()
{
	return m_Token;
}

// template <class... Args>
//__NAMESPACE__::WindowsNativeProcess::WindowsNativeProcess(Args... args) : base_t(args...)
//{
// }

bool __NAMESPACE__::WindowsNativeProcess::is_wow64()
{
	return false;
}

// template <class... Args>
//__NAMESPACE__::WindowsWoWProcess::WindowsWoWProcess(Args... args) : base_t(args...)
//{
// }

bool __NAMESPACE__::WindowsWoWProcess::is_wow64()
{
	return true;
}
