#pragma once
#include <cstdio>
#include <cstdlib>
#include <ctime>

namespace Helper {
	static float random(float min, float max)
	{
		return ((double)rand() / (RAND_MAX)) * ((double)max - (double)min) + (double)min;
	}

	static float maxF(float a, float b) {
		return (a > b) ? a : b;
	}
	
	static float minF(float a, float b) {
		return (a < b) ? a : b;
	}
};