
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "avl_tree.h"

/*
		x					z
	y		z	  =>	x		B
          A   B		  y	  A
*/
void rotate_left(avl_tree* x) {
	avl_tree* z = x->right;
	avl_tree* A = x->right->left;
	x->right = A;
	z->left = x;
	x->height = fmax(height(x->left), height(x->right)) + 1;
	z->height = fmax(height(z->left), height(z->right)) + 1;
}

/*
		x					z
	y		z	  <=	x		B
          A   B		  y	  A
*/
void rotate_right(avl_tree* z) {
	avl_tree* x = z->left;
	avl_tree* A = z->left->right;
	z->left = A;
	x->right = z;
	z->height = fmax(height(z->left), height(z->right)) + 1;
	x->height = fmax(height(z->left), height(z->right)) + 1;
}

