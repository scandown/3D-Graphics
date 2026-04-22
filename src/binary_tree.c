#include "binary_tree.h"


void bst_create_node(BST_dynamic_arr bst_array, int value, int value2, int value3, int linked_number) {
	DA_PUSH(bst_array, 0);
	bst_array.items[bst_array.index].left = NULL;
	bst_array.items[bst_array.index].right = NULL;
	bst_array.items[bst_array.index].value.vertex = value;
	bst_array.items[bst_array.index].value.texture = value2;
	bst_array.items[bst_array.index].value.normal = value3;
	bst_array.items[bst_array.index].linked = linked_number;
}

bool bst_insert(BST_dynamic_arr rootptr, int value, int value2, int value3, int linked_number) {
	BST *root = rootptr.items[0];

	while (root != NULL) {
		if (value == root->value.vertex && value2 == root->value.texture && value3 == root->value.normal) {
			return false;
		}

		if (value > root->value.vertex) {
			root = root->right;
		}
		else if (value < root->value.vertex) {
			root = root->left;
		}
		else if (value2 > root->value.texture) {
			root = root->right;
		}
		else if (value2 < root->value.texture) {
			root = root->left;
		}
		else if (value3 > root->value.normal) {
			root = root->right;
		}
		else if (value3 < root->value.normal) {
			root = root->left;
		}
	}
	if (root == NULL) {
		bst_create_node(rootptr, value, value2, value3, linked_number);
		return true;
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
