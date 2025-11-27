#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


typedef struct {
	float vertex;
	float texture;
} Val;

typedef struct bst{
	Val value;
	struct bst *left;
	struct bst *right;
	float linked;
} BST;


BST *createnode(float value, float value2, float linked_number);
bool insertnumber(BST **rootptr, float value, float value2, float linked_number);
bool getnumber(BST *rootptr, float value, float value2);
bool free_bst(BST **rootptr);
void print_tree(BST *rootptr);
