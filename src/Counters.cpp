#include "Counters.h"
#include <iostream>
#include <vector>
#include <sys/time.h>

#define TIME_RESOLUTION 1000000	// time measuring resolution (us)

long long unsigned initial_time;
std::vector<long long unsigned> measurements;

timeval t;

long long unsigned getMeasurement(int i) {
	return measurements[i];
}

void start () {
	gettimeofday(&t, NULL);
	initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

long long unsigned stop () {
	gettimeofday(&t, NULL);
	long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;

	measurements.push_back(final_time - initial_time);

	return final_time - initial_time;
}

long long unsigned minElement (int thread) {
	long long unsigned best = 0;

	for (auto i : measurements) {
		if (i < best || !best)
			best = i;
	}
	return best;
}

void printResults (void) {
	std::cout << "Time: " << minElement(-1) << " usecs" << std::endl;
	measurements.clear();
}

void clearMeasurements() {
	measurements.clear();
}
