
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "avl_tree.h"


int height(avl_tree* root) {
	if (root == NULL) {
		return 0;
	}
	return fmax(height(root->left), height(root->right)) + 1;
}


avl_tree* avl_tree_int_new(int x) {
	avl_tree* new = malloc(sizeof(avl_tree));
	new->value = x;
	new->left = NULL;
	new->right = NULL;
	new->height = 1;
	return new;
}

avl_tree* avl_tree_int_new_empty() {
	avl_tree* new = avl_tree_int_new(0);
	new->height = 0;
	return new;
}

/*
		x					z
	y		z	  =>	x		B
          A   B		  y	  A
*/
avl_tree* rotate_left(avl_tree* x) {
	avl_tree* z = x->right;
	avl_tree* A = x->right->left;
	x->right = A;
	z->left = x;
	x->height = fmax(height(x->left), height(x->right)) + 1;
	z->height = fmax(height(z->left), height(z->right)) + 1;
	return z;
}

/*
		x					z
	y		z	  <=	x		B
          A   B		  y	  A
*/
avl_tree* rotate_right(avl_tree* z) {
	avl_tree* x = z->left;
	avl_tree* A = z->left->right;
	z->left = A;
	x->right = z;
	z->height = fmax(height(z->left), height(z->right)) + 1;
	x->height = fmax(height(z->left), height(z->right)) + 1;
	return x;
}

avl_tree* balance_avl_node(avl_tree* root, avl_tree* inserted_subtree) {
	int balance = height(root->left) - height(root->right);
	int sub_balance = height(inserted_subtree->left) - height(inserted_subtree->right);
	if (balance == 2) {
		if (sub_balance == -1) {
			root->left = root->left->right;
			rotate_left(inserted_subtree);
		}
		root = rotate_right(root);
	}
	else if (balance == -2) {
		if (sub_balance == 1) {
			root->right = root->right->left;
			rotate_right(inserted_subtree);
		}
		root = rotate_left(root);
	}
	return root;
}

avl_tree* avl_tree_int_insert(avl_tree* root, int x) {
	avl_tree* inserted = NULL;
	if (root->height == 0) {
		root->value = x;
		root->height = 1;
		return root;
	}
	else if (x <= root->value) {
		if (root->left == NULL) {
			root->left = avl_tree_int_new(x);
		}
		else {
			root->left = avl_tree_int_insert(root->left, x);
		}
		inserted = root->left;
	}
	else {
		if (root->right == NULL) {
			root->right = avl_tree_int_new(x);
		}
		else {
			root->right = avl_tree_int_insert(root->right, x);
		}
		inserted = root->right;
	}
	root->height = fmax(height(root->left), height(root->right)) + 1;
	return balance_avl_node(root, inserted);
}

avl_tree* avl_tree_int_gets(avl_tree* root, int x) {
	avl_tree* ret;
	if (root == NULL || x== root->value) {
		ret = root;
	}
	else if (x < root->value) {
		ret = avl_tree_int_gets(root->left, x);
	}
	else {
		ret = avl_tree_int_gets(root->right, x);
	}
	root->height = fmax(height(root->left), height(root->right)) + 1;
	return ret;
}

bool avl_tree_int_contains(avl_tree* root, int x) {
	if (avl_tree_int_gets(root, x) == NULL) {
		return false;
	}
	return true;
}

void swap_int_nodes(avl_tree* first, avl_tree* second) {
	int temp = first->value;
	first->value = second->value;
	second->value = temp;
}

void remove_node(avl_tree* node, avl_tree* parent) {
	if (node->left != NULL || node->right != NULL) {
		if (height(node->left) >= height(node->right)) {
			avl_tree* rightmost = node->left;
			avl_tree* rightmost_parent = node;
			while (rightmost->right != NULL) {
				rightmost_parent = rightmost;
				rightmost = rightmost->right;
			}
			swap_int_nodes(rightmost, node);
			avl_tree* new_rightmost = rightmost->left;
			rightmost_parent->right = new_rightmost;
			free(rightmost);
		}
		else {
			avl_tree* leftmost = node->right;
			avl_tree* leftmost_parent = node;
			while (leftmost->left != NULL) {
				leftmost_parent = leftmost;
				leftmost = leftmost->left;
			}
			swap_int_nodes(leftmost, node);
			avl_tree* new_leftmost = leftmost->right;
			leftmost_parent->left = new_leftmost;
			free(leftmost);
		}
	}
	else {
		if (parent == NULL) {
			node->value = 0;
			node->height = 0;
		}
		else {
			if (parent->left == node) {
				parent->left = NULL;
			}
			else{
				parent->right = NULL;
			}
			free(node);
		}
	}
}

bool avl_tree_int_remove_helper(avl_tree* root, avl_tree* parent, int x) {
	bool ret;
	if (x == root->value) {
		remove_node(root, parent);
		return true;
	}
	else if (x < root->value) {
		if (root->left == NULL) {
			ret = false;
		}
		else {
			ret = avl_tree_int_remove_helper(root->left, root, x);
		}
	}
	else {
		if (root->right == NULL) {
			ret = false;
		}
		else {
			ret = avl_tree_int_remove_helper(root->right, root, x);
		}
	}
	root->height = fmax(height(root->left), height(root->right)) + 1;
	return ret;
}

bool avl_tree_int_remove(avl_tree* root, int x) {
	return avl_tree_int_remove_helper(root, NULL, x);
}

void print_tree_helper(avl_tree* root, int depth) {
	if (root == NULL) {
		printf("\n");
		return;
	}
	print_tree_helper(root->right, depth+1);
	for (int i = 0; i < depth; i++) {
		printf("\t");
	}
	printf("%d\n", root->value);
	print_tree_helper(root->left, depth+1);
}

void print_tree(avl_tree* root) {
	print_tree_helper(root, 0);
}

int main(void) {
	printf("%s\n", "TESTING AVL TREE");
	avl_tree* tree = avl_tree_int_new_empty();
	tree = avl_tree_int_insert(tree, 100);
	printf("%s\n", "inserted 100");
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	tree = avl_tree_int_insert(tree, 50);
	printf("%s\n", "inserted 50");
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	tree = avl_tree_int_insert(tree, 150);
	printf("%s\n", "inserted 150");
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	tree = avl_tree_int_insert(tree, 125);
	printf("%s\n", "inserted 125");
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	tree = avl_tree_int_insert(tree, 25);
	printf("%s\n", "inserted 25");
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	tree = avl_tree_int_insert(tree, 40);
	printf("%s\n", "inserted 40");
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	tree = avl_tree_int_insert(tree, 19);
	printf("%s\n", "inserted 19");
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	tree = avl_tree_int_insert(tree, 42);
	printf("%s\n", "inserted 42");
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	tree = avl_tree_int_insert(tree, 30);
	printf("%s\n", "inserted 30");
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	tree = avl_tree_int_insert(tree, 38);
	printf("%s\n", "inserted 38");
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	printf("Should be 1: %d\n", avl_tree_int_contains(tree, 42));
	printf("Should be 1: %d\n", avl_tree_int_contains(tree, 125));
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	printf("Removed 42: %d\n", avl_tree_int_remove(tree, 42));
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	printf("Removed 100: %d\n", avl_tree_int_remove(tree, 100));
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	printf("Removed 40: %d\n", avl_tree_int_remove(tree, 40));
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	printf("Removed 38: %d\n", avl_tree_int_remove(tree, 38));
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	printf("Removed 50: %d\n", avl_tree_int_remove(tree, 50));
	print_tree(tree);
	printf("%s\n", "=-=-=-=-=-=-=-=-");
	return 0;
}
