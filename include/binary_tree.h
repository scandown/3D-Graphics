#pragma once
#include <stdlib.h>
#include <stdbool.h>

typedef struct bst{
	float value;
	struct bst *left;
	struct bst *right;
} BST;

BST *createnode(int value);
bool insertnumber(BST **rootptr, int value);
bool getnumber(BST *rootptr, int value);
bool free_bst(BST *rootptr);
