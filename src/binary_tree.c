#include "binary_tree.h"


BST *bst_create_node(int value, int value2, int value3, int linked_number) {
	BST *result = malloc(sizeof(BST));
	result->left = NULL;
	result->right = NULL;
	result->value.vertex = value;
	result->value.texture = value2;
	result->value.normal = value3;
	result->linked = linked_number;

	return result;
}

bool bst_insert(BST **rootptr, int value, int value2, int value3, int linked_number) {
	BST *root = *rootptr;
	if (root == NULL) {
		*rootptr = bst_create_node(value, value2, value3, linked_number);
		return true;
	}
	if (value == root->value.vertex && value2 == root->value.texture && value3 == root->value.normal) {
		return false;
	}

	if (value > root->value.vertex) {
		return bst_insert(&(root->right), value, value2, value3, linked_number);
	}
	else if (value < root->value.vertex) {
		return bst_insert(&(root->left), value, value2, value3, linked_number);
	}
	else if (value2 > root->value.texture) {
		return bst_insert(&(root->right), value, value2, value3, linked_number);
	}
	else if (value2 < root->value.texture) {
		return bst_insert(&(root->left), value, value2, value3, linked_number);
	}
	else if (value3 > root->value.normal) {
		return bst_insert(&(root->right), value, value2, value3, linked_number);
	}
	else if (value3 < root->value.normal) {
		return bst_insert(&(root->left), value, value2, value3, linked_number);
	}

	return false;
}

bool bst_get_number(BST *rootptr, int value, int value2, int value3) {
	if (rootptr == NULL) {
		return false;
	}

	if (rootptr->value.vertex == value && rootptr->value.texture == value2 && rootptr->value.normal == value3) {
		return true;
	}

	if (value > rootptr->value.vertex) {
		return bst_get_number(rootptr->right, value, value2, value3);
	}
	else if (value < rootptr->value.vertex) {
		return bst_get_number(rootptr->left, value, value2, value3);
	}
	else if (value2 > rootptr->value.texture) {
		return bst_get_number(rootptr->right, value, value2, value3);
	}
	else if (value2 < rootptr->value.texture) {
		return bst_get_number(rootptr->left, value, value2, value3);
	}
	else if (value3 > rootptr->value.normal) {
		return bst_get_number(rootptr->right, value, value2, value3);
	}
	else if (value3 < rootptr->value.normal) {
		return bst_get_number(rootptr->left, value, value2, value3);
	}


	return false;
}

BST *bst_get_value(BST *rootptr, int value, int value2, int value3) {
	if (rootptr == NULL) {
		return NULL;
	}

	if (rootptr->value.vertex == value && rootptr->value.texture == value2 && rootptr->value.normal == value3) {
		return rootptr;
	}

	if (value > rootptr->value.vertex) {
		return bst_get_value(rootptr->right, value, value2, value3);
	}
	else if (value < rootptr->value.vertex) {
		return bst_get_value(rootptr->left, value, value2, value3);
	}
	else if (value2 > rootptr->value.texture) {
		return bst_get_value(rootptr->right, value, value2, value3);
	}
	else if (value2 < rootptr->value.texture) {
		return bst_get_value(rootptr->left, value, value2, value3);
	}
	else if (value3 > rootptr->value.normal) {
		return bst_get_value(rootptr->right, value, value2, value3);
	}
	else if (value3 < rootptr->value.normal) {
		return bst_get_value(rootptr->left, value, value2, value3);
	}

	return NULL;
}

bool bst_delete(BST **rootptr) {
	BST *root = *rootptr;
	if (root == NULL) {
		return false;
	}

	if (root->left != NULL) {
		bst_delete(&root->left);
		root->left = NULL;
	}

	if (root->right != NULL) {
		bst_delete(&root->right);
		root->right = NULL;
	}

	free(root);
	*rootptr = NULL;
	return true;
}

void bst_print(BST *rootptr) {
    if (rootptr == NULL) return;
    
    bst_print(rootptr->left);
    printf("Vertex: %d, Texture: %d, Normal: %d / Linked: %d\n", rootptr->value.vertex, rootptr->value.texture, rootptr->value.normal, rootptr->linked);
    bst_print(rootptr->right);
}
