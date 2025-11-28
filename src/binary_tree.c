#include "binary_tree.h"


BST *createnode(float value, float value2, float linked_number) {
	BST *result = malloc(sizeof(BST));
	result->left = NULL;
	result->right = NULL;
	result->value.vertex = value;
	result->value.texture = value2;
	result->linked = linked_number;

	return result;
}

bool insertnumber(BST **rootptr, float value, float value2, float linked_number) {
	BST *root = *rootptr;
	if (root == NULL) {
		*rootptr = createnode(value, value2, linked_number);
		return true;
	}
	if (value == root->value.vertex && value2 == root->value.texture) {
		return false;
	}

	if (value == root->value.vertex) {
		if (value2 < root->value.texture) {
			return insertnumber(&(root->left), value, value2, linked_number);
		}
		else if (value2 > root->value.texture) {
			return insertnumber(&(root->right), value, value2, linked_number);
		}
	} else {
		if (value < root->value.vertex) {
			return insertnumber(&(root->left), value, value2, linked_number);	
		} else {
			// if all else is false then this must be true
			return insertnumber(&(root->right), value, value2, linked_number);	
		}
	}

	return false;
}

bool getnumber(BST *rootptr, float value, float value2) {
	if (rootptr == NULL) {
		return false;
	}

	if (rootptr->value.vertex == value && rootptr->value.texture == value2) {
		return true;
	}

	if (value == rootptr->value.vertex) {
		if (value2 < rootptr->value.texture) {
			return getnumber(rootptr->left, value, value2);
		}
		else if (value2 > rootptr->value.texture) {
			return getnumber(rootptr->right, value, value2);
		}
	} else {
		if (value < rootptr->value.vertex) {
			return getnumber(rootptr->left, value, value2);	
		} else {
			// if all else is false then this must be true
			return getnumber(rootptr->right, value, value2);	
		}
	}

	return false;
}

BST *getvalue(BST *rootptr, float value, float value2) {
	if (rootptr == NULL) {
		return NULL;
	}

	if (rootptr->value.vertex == value && rootptr->value.texture == value2) {
		return rootptr;
	}

	if (value == rootptr->value.vertex) {
		if (value2 < rootptr->value.texture) {
			return getvalue(rootptr->left, value, value2);
		}
		else if (value2 > rootptr->value.texture) {
			return getvalue(rootptr->right, value, value2);
		}
	} else {
		if (value < rootptr->value.vertex) {
			return getvalue(rootptr->left, value, value2);	
		} else {
			// if all else is false then this must be true
			return getvalue(rootptr->right, value, value2);	
		}
	}

	return NULL;
}

bool free_bst(BST **rootptr) {
	BST *root = *rootptr;
	if (root == NULL) {
		return false;
	}

	if (root->left != NULL) {
		free_bst(&root->left);
		root->left = NULL;
	}

	if (root->right != NULL) {
		free_bst(&root->right);
		root->right = NULL;
	}

	free(root);
	*rootptr = NULL;
	return true;
}

void print_tree(BST *rootptr) {
    if (rootptr == NULL) return;
    
    print_tree(rootptr->left);
    printf("Vertex: %d, Texture: %d / Linked: %d\n", rootptr->value.vertex, rootptr->value.texture, rootptr->linked);
    print_tree(rootptr->right);
}
