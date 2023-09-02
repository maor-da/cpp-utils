#pragma once
#include <Windows.h>
#include <stdint.h>

#include <string_view>
#include <type_traits>

namespace __NAMESPACE__
{

template <class T, class... Ts>
concept all_convertable = sizeof
...(Ts) < 2 || (sizeof...(Ts) > 1 && std::conjunction_v<std::is_convertible<Ts, T>...>);

template <class T, class... Ts>
concept all_same = sizeof
...(Ts) < 2 || (sizeof...(Ts) > 1 && std::conjunction_v<std::is_same<Ts, T>...>);

class Token;

class Privilege
{
public:
	Privilege(HANDLE token);

	template <class... Args>
	requires all_convertable<std::wstring_view, Args...> bool
	add(Args... privilege)
	{
		if (!m_Token) {
			return false;
		}
		return (set_privilege(privilege, true) && ...);
	}

	template <class... Args>
	requires all_convertable<std::wstring_view, Args...> bool
	remove(Args... privilege)
	{
		if (!m_Token) {
			return false;
		}
		return (set_privilege(privilege, false) && ...);
	}

private:
	HANDLE m_Token;

	bool set_privilege(std::wstring_view privilege, bool enable);
};

class Token
{
public:
	enum class ACCESS : uint32_t {
		assign_primary	  = TOKEN_ASSIGN_PRIMARY,
		duplicate		  = TOKEN_DUPLICATE,
		impersonate		  = TOKEN_IMPERSONATE,
		query			  = TOKEN_QUERY,
		query_source	  = TOKEN_QUERY_SOURCE,
		adjust_privileges = TOKEN_ADJUST_PRIVILEGES,
		adjust_groups	  = TOKEN_ADJUST_GROUPS,
		adjust_default	  = TOKEN_ADJUST_DEFAULT,
		adjust_sessionid  = TOKEN_ADJUST_SESSIONID,
		read			  = TOKEN_READ,
		write			  = TOKEN_WRITE,
		all_access		  = TOKEN_ALL_ACCESS
	};

	Token(ACCESS access);

	Token();

	Token(uint32_t pid, ACCESS access);

	Token(uint32_t pid);

	inline constexpr bool is_valid();

	HANDLE get();

	Privilege adjust();

private:
	HANDLE m_TokenHandle = nullptr;
};

}  // namespace __NAMESPACE__
