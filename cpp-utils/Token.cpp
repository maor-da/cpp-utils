#include <cpputils.h>

namespace __NAMESPACE__
{

Token::Token(ACCESS access)
{
	OpenProcessToken(GetCurrentProcess(), static_cast<DWORD>(access), &m_TokenHandle);
}

Token::Token() : Token(ACCESS::all_access) {}

Token::~Token()
{
	if (m_TokenHandle) {
		CloseHandle(m_TokenHandle);
	}
}

// Must have PROCESS_QUERY_INFORMATION access
Token::Token(HANDLE hProcess, ACCESS access)
{
	if (hProcess) {
		OpenProcessToken(hProcess, static_cast<DWORD>(access), &m_TokenHandle);
	}
	// throw?
}

Token::Token(uint32_t pid, ACCESS access) : Token(OpenProcess(PROCESS_QUERY_INFORMATION, false, pid), access)
{
}

Token::Token(uint32_t pid) : Token(pid, ACCESS::all_access) {}

constexpr bool Token::is_valid()
{
	return m_TokenHandle != nullptr;
}

HANDLE Token::get()
{
	return m_TokenHandle;
}

Privilege Token::adjust()
{
	if (is_valid()) {
		return m_TokenHandle;
	}

	return nullptr;
}

Privilege::Privilege(HANDLE token) : m_Token(token) {}

bool Privilege::set_privilege(std::wstring_view privilege, bool enable)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(nullptr, privilege.data(), &luid)) {
		return false;
	}

	tp.PrivilegeCount			= 1;
	tp.Privileges[0].Luid		= luid;
	tp.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : NULL;

	if (!AdjustTokenPrivileges(m_Token, false, &tp, sizeof(tp), nullptr, NULL)) {
		return false;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		return false;
	}

	return true;
}

}  // namespace __NAMESPACE__
