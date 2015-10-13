#pragma once

#include <time.h>

class Timer
{
public:
	Timer();


	float GetDeltaTime();

private:
	clock_t m_LastTimeStamp;
};