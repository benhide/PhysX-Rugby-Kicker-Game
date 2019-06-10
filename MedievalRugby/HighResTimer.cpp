#include "HighResTimer.h"

// Reset timer
void HighResTimer::ResetHighResTimer()
{
	startChrono = Clock::now();
}

// Get timer
float HighResTimer::GetHighResTimer()
{
	std::chrono::high_resolution_clock::time_point now = Clock::now();
	auto timeDiff = std::chrono::duration_cast<std::chrono::microseconds>(now - startChrono).count();
	return (float)timeDiff;
}