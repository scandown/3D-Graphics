#include "dynamic_array.h"

void push(Array *arr, float number) {
	if (arr->count >= arr->capacity) {
		if (arr->capacity == 0) {
			arr->capacity = 256;
		}
		else {
			arr->capacity *= 2;
		}
		arr->items = realloc(arr->items, arr->capacity * sizeof(*(arr->items)));
	}
	arr->items[arr->count++] = number;
}
