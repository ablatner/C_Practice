
typedef struct avl_tree avl_tree;
struct avl_tree {
	int value;
	avl_tree* left;
	avl_tree* right;
	int height;
};

void print_tree(avl_tree* root);

avl_tree* avl_tree_int_new_empty();

avl_tree* avl_tree_int_new(int x);

bool avl_tree_int_contains(avl_tree* root, int x);

avl_tree* avl_tree_int_get(avl_tree* root, int x);

avl_tree* avl_tree_int_insert(avl_tree* root, int x);

bool avl_tree_int_remove(avl_tree* root, int x);
