#include <cpp-utils/Winutil.h>
#include <winrt/base.h>

namespace __NAMESPACE__
{

std::string to_string(std::wstring_view wstring)
{
	return winrt::to_string(wstring);
}

std::wstring to_wstring(std::string_view string)
{
	return std::wstring_view{winrt::to_hstring(string)}.data();
}

}  // namespace __NAMESPACE__
