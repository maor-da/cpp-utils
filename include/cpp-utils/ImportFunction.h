#pragma once
#include <Windows.h>

#include <functional>
#include <string_view>

namespace __NAMESPACE__
{

template <class T>
class ImportFunction : public std::function<T>
{
public:
	using base_t = std::function<T>;
	using func_t = T;

	ImportFunction(std::wstring_view lib, std::string_view function)
	{
		HMODULE hModule = nullptr;

		do {
			m_hModule = LoadLibraryW(lib.data());
			if (!m_hModule) {
				break;
			}

			auto proc = GetProcAddress(m_hModule, function.data());

			if (!proc) {
				break;
			}

			base_t(reinterpret_cast<func_t*>(proc)).swap(*static_cast<base_t*>(this));

		} while (false);
	}

	~ImportFunction()
	{
		if (m_hModule) {
			FreeLibrary(m_hModule);
		}
	}

	constexpr bool is_valid() const
	{
		// TODO: debug log
		return this->target<func_t*>() != nullptr;
	}

private:
	HMODULE m_hModule = nullptr;
};

}  // namespace Winutil