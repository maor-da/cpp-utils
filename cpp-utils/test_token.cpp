#include <cpputils.h>

bool test_token()
{
	winutil::Token t(winutil::Token::ACCESS::adjust_privileges);
	if (!t.adjust().add(L"SeManageVolumePrivilege")) {
		return false;
	}

	return true;
}
