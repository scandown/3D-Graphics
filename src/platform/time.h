#pragma once
#include <sys/time.h>
#include <stddef.h>

float time_delta(struct timeval *time_previous_frame, double *elapsed_time);
