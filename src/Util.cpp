#include "Util.h"
#include <stdlib.h> 

#define MAX_INT 10
#define MIN_INT -10

#include <limits> 
#include <thread>

namespace Util
{
	void WaitFor(std::function<bool()> p_fnWait)
	{
		while(p_fnWait())
		{
			Sleep(100);
		}
	}

	void Sleep(const unsigned long p_culMilisec)
	{
    	std::this_thread::sleep_for(std::chrono::milliseconds(p_culMilisec));
	}
}