#pragma once

#include <stddef.h>
#include <stdlib.h>



#define da_push(arr, number) \
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
