#pragma once
#include "stb_ds.h"


#define arrput_vector(array, vector, vector_dimensions) do { \
	arraddn(array, 1); \
	unsigned int arr_len = arrlen(array); \
	for (int __macro_iter = 0; __macro_iter < vector_dimensions; ++__macro_iter) { \
		array[arr_len - 1][__macro_iter] = vector[__macro_iter]; \
	} \
} while (0)


