#pragma once
#include <cstdint>

#if !_DEBUG

namespace cx
{
	namespace detail
	{
		namespace fnv
		{
			constexpr uint64_t fnv1(uint64_t h, const char* s)
			{
				return (*s == 0) ? h :
					fnv1((h * 1099511628211ull) ^ static_cast<uint64_t>(*s), s + 1);
			}
			constexpr uint64_t fnv1a(uint64_t h, const char* s)
			{
				return (*s == 0) ? h :
					fnv1a((h ^ static_cast<uint64_t>(*s)) * 1099511628211ull, s + 1);
			}
		}
	}
	constexpr uint64_t fnv1(const char* s)
	{
		return detail::fnv::fnv1(14695981039346656037ull, s);
	}
	constexpr uint64_t fnv1a(const char* s)
	{
		return detail::fnv::fnv1a(14695981039346656037ull, s);
	}
}

#define hashed_string uint64_t
#define hash( s ) cx::fnv1( s )

__forceinline bool hash_compare(hashed_string l, const char* str)
{
	return (l == hash(str));
}

#else

#define hashed_string const char*
#define hash( s ) ( s )

#define hash_compare(l,r) ((!strcmp(l,r)))

#endif
