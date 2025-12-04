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


BST *createnode(int value, int value2, int value3, int linked_number);
bool insertnumber(BST **rootptr, int value, int value2, int value3, int linked_number);
bool getnumber(BST *rootptr, int value, int value2, int value3);
BST *getvalue(BST *rootptr, int value, int value2, int value3);
bool free_bst(BST **rootptr);
void print_tree(BST *rootptr);
