#ifndef PLATFORM_DEBUG_H
#define PLATFORM_DEBUG_H

namespace platform
{
#ifdef _WIN32
	static void breakpoint()
	{
		__debugbreak();
	}
#else
	static void breakpoint()
	{
		__builtin_trap();
	}
#endif
}; // namespace platform
#endif
