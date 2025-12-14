#pragma once

#include <stddef.h>
#include <stdlib.h>


typedef struct {
	float *items;
	size_t count;
	size_t capacity;
} Array;

void push(Array *arr, float number);
