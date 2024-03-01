#pragma once
#include "hash.h"

#undef UNIQUE_FUNCTION_ID

#if defined(_MSC_VER)
#define UNIQUE_FUNCTION_ID __FUNCSIG__
#else     
#if defined( __GNUG__ )
#define UNIQUE_FUNCTION_ID __PRETTY_FUNCTION__
#endif
#endif

using type_id_t = std::size_t;

template<typename T>
class type_id
{
public:
	static constexpr std::size_t id()
	{
		/*static*/constexpr std::size_t s_id = fnv1a_64(UNIQUE_FUNCTION_ID);
		return s_id;
	}
};