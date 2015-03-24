
typedef struct {
	int value;
	avl_tree left;
	avl_tree right
} avl_tree;

void new_avl_tree();

void new_avl_tree(int x);

void insert_int(avl_tree* root, int x);

bool remove_int(avl_trees* root, int x);
