#pragma once
#include <stdint.h>

template <size_t N>
struct ENCRYPTED_DATA {
	constexpr ENCRYPTED_DATA(uint64_t salt, char const (&plaintext)[N])
		: key(salt ^ 0xcbf29ce484222325), data()
	{
		crypt(plaintext);
	}

	constexpr ENCRYPTED_DATA(uint64_t salt, uint8_t const (&ciphertext)[N])
		: key(salt ^ 0xcbf29ce484222325), data()
	{
		crypt(ciphertext);
	}

	template <typename T>
	constexpr void crypt(T const (&text)[N])
	{
#pragma warning(disable : 4244)
		for (size_t i = 0; i < N; i++) {
			uint8_t k = key >> i % sizeof(key);
			data[i]	  = text[i] ^ k;
		}
#pragma warning(default : 4244)
	}

	uint64_t key;
	uint8_t data[N];
};
