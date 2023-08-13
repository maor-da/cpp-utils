#pragma once
#include <type_traits>

template <class T, std::enable_if<std::is_enum<T>::value, bool>::type = true>
constexpr std::underlying_type_t<T> operator|(T lhs, T rhs)
{
	return static_cast<std::underlying_type_t<T>>(lhs) | static_cast<std::underlying_type_t<T>>(rhs);
}

template <class T, std::enable_if<std::is_enum<T>::value, bool>::type = true>
constexpr void operator|=(std::underlying_type_t<T>& lhs, T rhs)
{
	lhs = lhs | static_cast<std::underlying_type_t<T>>(rhs);
}

template <class T, std::enable_if<std::is_enum<T>::value, bool>::type = true>
constexpr std::underlying_type_t<T> operator&(T lhs, T rhs)
{
	return static_cast<std::underlying_type_t<T>>(lhs) & static_cast<std::underlying_type_t<T>>(rhs);
}

template <class T, std::enable_if<std::is_enum<T>::value, bool>::type = true>
constexpr std::underlying_type_t<T> operator&(std::underlying_type_t<T> lhs, T rhs)
{
	return lhs & static_cast<std::underlying_type_t<T>>(rhs);
}

template <class T, std::enable_if<std::is_enum<T>::value, bool>::type = true>
constexpr std::underlying_type_t<T> operator&=(std::underlying_type_t<T>& lhs, T rhs)
{
	lhs = lhs & static_cast<std::underlying_type_t<T>>(rhs);
}

template <class T, std::enable_if<std::is_enum<T>::value, bool>::type = true>
constexpr std::underlying_type_t<T> operator&(T lhs, std::underlying_type_t<T> rhs)
{
	return static_cast<std::underlying_type_t<T>>(lhs) & rhs;
}

template <class T, std::enable_if<std::is_enum<T>::value, bool>::type = true>
constexpr std::underlying_type_t<T> operator~(T rhs)
{
	return ~static_cast<std::underlying_type_t<T>>(rhs);
}

template <class T, std::enable_if<std::is_integral<T>::value, bool>::type = true>
constexpr bool operator!(T lhs)
{
	return lhs == T(0);
}

template <typename T, bool U>
inline constexpr T _max_val = static_cast<T>(0x0);

// unsigned
template <typename T>
inline constexpr T _max_val<T, true> = static_cast<T>(0xFFFFFFFFFFFFFFFF);

// signed
template <typename T>
inline constexpr T _max_val<T, false> = static_cast<T>(0xFFFFFFFFFFFFFFFF >> ((8 - sizeof(T)) * 8 + 1));

template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
inline constexpr T max_val = _max_val<T, std::is_unsigned<T>>;

template <typename T, bool U>
inline constexpr T _min_val = static_cast<T>(0x0);

// signed
template <typename T>
inline constexpr T _min_val<T, true> = -_max_val<T, false>;

template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
inline constexpr T min_val = _min_val<T, std::is_signed<T>>;
