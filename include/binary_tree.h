#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "dynamic_array.h"


typedef struct {
	int vertex;
	int texture;
	int normal;
} Val;

typedef struct bst{
	Val value;
	struct bst *left;
	struct bst *right;
	int linked;
} BST;

typedef struct {
	BST *items;
	unsigned int index;
	unsigned int count;
} BST_dynamic_arr;

void bst_create_node(BST_dynamic_arr bst_array, int value, int value2, int value3, int linked_number);
bool bst_insert(BST_dynamic_arr rootptr, int value, int value2, int value3, int linked_number);
bool bst_get_number(BST *rootptr, int value, int value2, int value3);
BST *bst_get_value(BST *rootptr, int value, int value2, int value3);
bool bst_delete(BST **rootptr);
void bst_print(BST *rootptr);
