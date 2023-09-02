#pragma once

#include <string>

namespace __NAMESPACE__
{
std::string to_string(std::wstring_view wstring);
std::wstring to_wstring(std::string_view string);

}  // namespace __NAMESPACE__