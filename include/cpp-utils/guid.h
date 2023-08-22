#pragma once
#include <windows.h>

#include <string>

namespace __NAMESPACE__
{
namespace guid
{

template <class T>
concept GUID_CLASS = std::same_as<T, GUID> || std::same_as<T, IID> || std::same_as<T, CLSID>;

inline std::wstring uuid_to_guid_wstring(std::wstring_view guid)
{
	if (guid.size() == 36) {
		return std::wstring(L"{") + guid.data() + L'}';
	}

	return {};
}

inline std::wstring guid_to_uuid_wstring(std::wstring_view guid)
{
	if (guid.size() == 38) {
		if (guid[0] != L'{' || guid[guid.size() - 1] != L'}') {
			return {};	// no guid brackets
		}
		return {guid.substr(1, 36).data(), 36};
	} else if (guid.size() == 36) {
		return guid.data();
	} else {
		return {};
	}
}

// TODO: constexpr
inline constexpr std::wstring binary_to_wstring(const uint8_t* guid)
{
	const wchar_t hex[] = {L"0123456789ABCDEF"};
	wchar_t str[]		= {L"00000000-0000-0000-0000-000000000000"};
	uint8_t idx			= 0;

	for (uint8_t i = 0; i < 4; i++) {
		uint8_t b		   = guid[idx++];
		str[6 - 2 * i]	   = hex[b >> 4];
		str[6 - 2 * i + 1] = hex[b & 0x0F];
	}

	for (uint8_t i = 0; i < 2; i++) {
		uint8_t b			= guid[idx++];
		str[11 - 2 * i]		= hex[b >> 4];
		str[11 - 2 * i + 1] = hex[b & 0x0F];
	}

	for (uint8_t i = 0; i < 2; i++) {
		uint8_t b			= guid[idx++];
		str[16 - 2 * i]		= hex[b >> 4];
		str[16 - 2 * i + 1] = hex[b & 0x0F];
	}

	for (uint8_t i = 0; i < 2; i++) {
		uint8_t b			= guid[idx++];
		str[19 + 2 * i]		= hex[b >> 4];
		str[19 + 2 * i + 1] = hex[b & 0x0F];
	}

	for (size_t i = 0; i < 6; i++) {
		uint8_t b			= guid[idx++];
		str[24 + 2 * i]		= hex[b >> 4];
		str[24 + 2 * i + 1] = hex[b & 0x0F];
	}
	return str;
}

template <class T>
inline T generate_guid()
{
	static_assert("Not implemented");
}

template <>
inline GUID generate_guid()
{
	GUID g;

	if (CoCreateGuid(&g) != S_OK) {
		return {};
	}

	return g;
}

template <GUID_CLASS T>
inline std::wstring guid_to_wstring(const T& guid)
{
	auto str = std::make_unique<wchar_t[]>(40);

	if (!StringFromGUID2(guid, str.get(), 40)) {
		return {};
	}

	return str.get();
}

template <>
inline std::wstring generate_guid()
{
	return guid_to_wstring(generate_guid<GUID>());
}

inline std::wstring generate_uuid()
{
	return guid_to_uuid_wstring(generate_guid<std::wstring>());
}

}  // namespace guid

}  // namespace __NAMESPACE__
