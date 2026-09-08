#include "platform/time.h"

double time_get_current() {
	struct timespec tp = {0};
	clock_gettime(CLOCK_MONOTONIC, &tp);


	const int NANOSECOND_TO_SEC = 1.0E9;

	double decimal_part = tp.tv_nsec / NANOSECOND_TO_SEC;
	double time_double = decimal_part + tp.tv_sec;

	return time_double;
}


float time_delta(struct timeval *time_previous_frame, double *elapsed_time) {
	double dummy_elapsed_time;
	if (elapsed_time == NULL) {
		elapsed_time = &dummy_elapsed_time;	
	}
	struct timeval t2;
	gettimeofday(&t2, NULL);

	*elapsed_time = (t2.tv_sec - time_previous_frame->tv_sec) * 1000.0;
	*elapsed_time += (t2.tv_usec - time_previous_frame->tv_usec) / 1000.0;

	*time_previous_frame = t2;
	float delta = *elapsed_time / 1000;

	return delta;
}
