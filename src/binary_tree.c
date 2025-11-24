#include "binary_tree.h"

BST *createnode(int value) {
	BST *result = malloc(sizeof(BST));
	result->left = NULL;
	result->right = NULL;
	result->value = value;

	return result;
}

bool insertnumber(BST **rootptr, int value) {
	BST *root = *rootptr;
	if (root == NULL) {
		*rootptr = createnode(value);
		return true;
	}
	if (value == root->value) {
		return false;
	}

	if (value < root->value) {
		return insertnumber(&(root->left), value);	
	} else {
		return insertnumber(&(root->right), value);	
	}
}

bool getnumber(BST *rootptr, int value) {
	if (rootptr == NULL) {
		return false;
	}

	if (rootptr->value == value) {
		return true;
	}

	if (value < rootptr->value) {
		return getnumber(rootptr->left, value);	
	} else {
		return getnumber(rootptr->right, value);	
	}
}

bool free_bst(BST *root) {
	if (root == NULL) {
		return true;
	}

	if (root->left != NULL) {
		free_bst(root->left);
		root->left = NULL;
	}

	if (root->right != NULL) {
		free_bst(root->right);
		root->right = NULL;
	}

	free(root);
	return true;
}
