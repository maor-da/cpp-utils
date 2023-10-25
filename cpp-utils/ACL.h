#pragma once

#include <AclAPI.h>
#include <sddl.h>
#include <windows.h>

#include <filesystem>
#include <iostream>
#include <string>

namespace __NAMESPACE__
{

template <SE_OBJECT_TYPE T = SE_OBJECT_TYPE::SE_FILE_OBJECT>
class AccessControlObject
{
public:
	AccessControlObject(std::string_view path) : m_Path(path)
	{
		query();
	}
	AccessControlObject(std::wstring_view path) : m_Path(path)
	{
		query();
	}
	AccessControlObject(std::filesystem::path& path) : m_Path(path)
	{
		query();
	}
	AccessControlObject()							= default;
	AccessControlObject(const AccessControlObject&) = default;

	void set_path(std::filesystem::path path)
	{
		m_Path = path;
	}

	std::filesystem::path get_path()
	{
		return m_Path;
	}

	AccessControlObject<T> duplicate(std::filesystem::path path)
	{
		AccessControlObject<T> acl(path);
		acl.set_sddl(get_sddl());
		return acl;
	}

	bool query()
	{
		PSECURITY_DESCRIPTOR pSecurityDescriptor = nullptr;

		if (GetNamedSecurityInfoW(m_Path.c_str(),
								  T,
								  DACL_SECURITY_INFORMATION,
								  nullptr,
								  nullptr,
								  nullptr,
								  nullptr,
								  &pSecurityDescriptor) == ERROR_SUCCESS) {
			m_SD.reset(pSecurityDescriptor);
			return true;
		}
		return false;
	}

	bool apply() {}

	std::wstring get_sddl();
	bool set_sddl(std::wstring sddl);

	// SECURITY_DESCRIPTOR_CONTROL get_control();
	// DWORD get_revision();
	// ACL

private:
	std::filesystem::path m_Path;
	std::unique_ptr<SECURITY_DESCRIPTOR, decltype(&LocalFree)> m_SD;
};

}  // namespace __NAMESPACE__
