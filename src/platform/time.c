#include "time.h"



float time_delta(struct timeval *time_previous_frame, double *elapsed_time) {
	double dummy_elapsed_time;
	if (elapsed_time == NULL) {
		elapsed_time = &dummy_elapsed_time;	
	}
	struct timeval t2;
	gettimeofday(&t2, NULL);

	*elapsed_time = (t2.tv_sec - time_previous_frame->tv_sec) * 1000.0;
	*elapsed_time += (t2.tv_usec - time_previous_frame->tv_usec) / 1000.0;

	gettimeofday(time_previous_frame, NULL);
	float delta = *elapsed_time / 1000;

	return delta;
}
