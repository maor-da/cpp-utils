#pragma once
#include <stdint.h>

#include <memory>

template <size_t N>
struct ENCRYPTED_DATA {
	using type = ENCRYPTED_DATA<N>;

	constexpr ENCRYPTED_DATA(uint64_t salt, char const (&plaintext)[N]) : data()
	{
		crypt(plaintext, salt ^ 0xcbf29ce484222325);
	}

	constexpr ENCRYPTED_DATA(uint64_t salt, uint8_t const (&ciphertext)[N]) : data()
	{
		crypt(ciphertext, salt ^ 0xcbf29ce484222325);
	}

	uint8_t const (&get() const)[N]
	{
		return data;
	}

	template <typename T>
	constexpr void crypt(T const (&text)[N], const uint64_t key)
	{
#pragma warning(disable : 4244)
		for (size_t i = 0; i < N; i++) {
			uint8_t k = key >> i % sizeof(key);
			data[i]	  = text[i] ^ k;
		}
#pragma warning(default : 4244)
	}

	// const uint64_t key;
	uint8_t data[N];
	const size_t size = N;
};

#pragma optimize("", off)
auto decrypt(auto& ct, uint64_t salt)
{
	auto pt = std::make_unique<decltype(ct)>(salt, ct.data);
	return std::move(pt);
}
