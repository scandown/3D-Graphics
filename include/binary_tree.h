#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


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

BST *bst_create_node(int value, int value2, int value3, int linked_number);
bool bst_insert(BST **rootptr, int value, int value2, int value3, int linked_number);
bool bst_get_number(BST *rootptr, int value, int value2, int value3);
BST *bst_get_value(BST *rootptr, int value, int value2, int value3);
bool bst_delete(BST **rootptr);
void bst_print(BST *rootptr);
