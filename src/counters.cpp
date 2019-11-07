#include "counters.h"
#include <iostream>
#include <vector>
#include <sys/time.h>

#define TIME_RESOLUTION 1000000	// time measuring resolution (us)
#define MAX_THREADS 48 // maximum number of threads to run

long long unsigned initial_time;
std::vector<long long unsigned> sequential_measurements;
std::vector<long long unsigned> measurements[MAX_THREADS/4 + 1];	// position 0 is for the sequential version

timeval t;

void start (void) {
	gettimeofday(&t, NULL);
	initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

long long unsigned stop (int thread) {
	gettimeofday(&t, NULL);
	long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;

	if (thread == -1)
		sequential_measurements.push_back(final_time - initial_time);
	else
		measurements[thread].push_back(final_time - initial_time);

	return final_time - initial_time;
}

long long unsigned minElement (int thread) {
	long long unsigned best = 0;

	if (thread == - 1) {
		for (auto i : sequential_measurements) {
			if (i < best || !best)
				best = i;
		}
	} else {
		for (auto i : measurements[thread]) {
			if (i < best || !best)
				best = i;
		}
	}

	return best;
}

void printResults (void) {
	std::cout << "Time: " << minElement(-1) << " usecs" << std::endl;
	// std::cout << "Parallel time for " << 2 << " threads: " << minElement(2) << " usecs" << std::endl;
}
