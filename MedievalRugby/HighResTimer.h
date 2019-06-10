#pragma once

#include <chrono>

// Timer class
class HighResTimer
{
public:
	public:
		// Constructor
		HighResTimer() {}

		// Reset timer
		void ResetHighResTimer();

		// Get time
		float GetHighResTimer();

private:
	// Type definitions
	typedef std::chrono::high_resolution_clock Clock;

	// Start time
	std::chrono::high_resolution_clock::time_point startChrono;
};