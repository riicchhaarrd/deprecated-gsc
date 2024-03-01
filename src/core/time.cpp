#include "time.h"
#include <assert.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

namespace core
{
	void sleep(int ms)
	{
		#ifdef _WIN32
		Sleep(ms);
		#else
		usleep(ms * 1000);
		#endif
	}

	unsigned int time_milliseconds()
	{
#ifdef _WIN32
		//GetTickCount works weird, due to return frametime 0 and sometimes getting 15ms frametimes i dont know why
		//return GetTickCount();
		static thread_local clock_t start_time = clock();
		clock_t now = clock();
		return now - start_time;
#else
		struct timespec ts;
		unsigned int tick = 0;
		clock_gettime(CLOCK_REALTIME, &ts);
		tick = ts.tv_nsec / 1000000;
		tick += ts.tv_sec * 1000;
		return tick;
#endif
	}
};