#pragma once

#include <stddef.h>
#include <stdlib.h>


#define DA_PUSH(arr, number) \
	do { \
	if (arr.count >= arr.capacity) { \
		if (arr.capacity == 0) { \
			arr.capacity = 256; \
		} \
		else { \
			arr.capacity *= 2; \
		} \
		arr.items = realloc(arr.items, arr.capacity * sizeof(*arr.items)); \
	} \
	arr.items[arr.count++] = number; \
	} while (0)

#define DA_PUSH_VEC(arr, vec, amount) \
	do { \
	if (arr.count >= arr.capacity) { \
		if (arr.capacity == 0) { \
			arr.capacity = 256; \
		} \
		else { \
			arr.capacity *= 2; \
		} \
		arr.items = realloc(arr.items, arr.capacity * sizeof(*arr.items)); \
	} \
	for (int __DA_ITER__ = 0; __DA_ITER__ < amount; ++__DA_ITER__) { \
		arr.items[arr.count][__DA_ITER__] = vec[__DA_ITER__]; \
	} \
	arr.count++; \
	} while (0)

#define DA_PUSH_VEC_SCALAR(arr, scalar, amount) \
	do { \
	if (arr.count >= arr.capacity) { \
		if (arr.capacity == 0) { \
			arr.capacity = 256; \
		} \
		else { \
			arr.capacity *= 2; \
		} \
		arr.items = realloc(arr.items, arr.capacity * sizeof(*arr.items)); \
	} \
	for (int __DA_ITER__ = 0; __DA_ITER__ < amount; ++__DA_ITER__) { \
		arr.items[arr.count][__DA_ITER__] = scalar; \
	} \
	arr.count++; \
	} while (0)
