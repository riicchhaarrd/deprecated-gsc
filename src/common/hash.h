// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

#include <stdint.h>
#include <stdio.h>

typedef uint8_t u8;
typedef uint64_t u64;
typedef uint64_t hash_t;
typedef uint32_t u32;

constexpr u32 fnv1a_32(const char *str)
{
	u32 prime = 0x01000193;
	u32 offset = 0x811c9dc5;

	u32 hash = offset;
	while(*str)
	{
		hash ^= *str;
		hash *= prime;
		++str;
	}
	return hash;
}
constexpr u64 fnv1a_64(const char *str)
{
	u64 prime = 0x00000100000001B3;
	u64 offset = 0xcbf29ce484222325;

	u64 hash = offset;
	while(*str)
	{
		hash ^= *str;
		hash *= prime;
		++str;
	}
	return hash;
}

//static_hash
consteval hash_t string_hash(const char *str) // Compile-time
{
	return fnv1a_64(str);
}

//runtime_hash
constexpr hash_t hash_string(const char *str) // Runtime
{
	return fnv1a_64(str);
}

static char hexchar(int value, bool upper = false)
{
	if(value > 15)
		return 0;
	if(value < 10)
		return '0' + value;
	static const char uppercase[] = {'A','B','C','D','E','F'};
	static const char lowercase[] = {'a','b','c','d','e','f'};
	return upper ? uppercase[value - 10] : lowercase[value - 10];
}

template<size_t N>
void hash_to_hex_string(hash_t hash, char (&out_hex_string)[N], bool upper = false)
{
	constexpr size_t hash_len = sizeof(hash);
    static_assert(N >= (hash_len * 2) + 1, "out_hex_string must be at least sizeof hash value * 2 + 1");
	uint8_t *data = (uint8_t*)&hash;
    for(size_t i = 0; i < hash_len; ++i)
    {
		u8 byte = data[hash_len - i - 1] & 0xff;
		char nibble1 = hexchar(byte & 0xf, upper);
		char nibble2 = hexchar((byte >> 4) & 0xf, upper);
		//TODO: FIXME endianness
		out_hex_string[i * 2] = nibble2;
		out_hex_string[i * 2 + 1] = nibble1;
    }
	out_hex_string[hash_len * 2] = 0;
}