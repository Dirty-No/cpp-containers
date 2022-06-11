// Same thing, but with a red black tree:
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
	RED,
	BLACK
} color;


typedef struct rbtree_node {
	int key;
	char *value;
	color color;
	struct rbtree_node *left;
	struct rbtree_node *right;
	struct rbtree_node *parent;
} rbtree_node;

rbtree_node *global_root;

rbtree_node *create_rbtree_node(rbtree_node *parent, int key, char *value) {
	rbtree_node *node = malloc(sizeof(rbtree_node));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
	node->value = strdup(value);
	node->color = RED;
	return node;
}

rbtree_node *create_rbtree_root(int key, char *value) {
	rbtree_node *node = malloc(sizeof(rbtree_node));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->value = strdup(value);
	node->color = BLACK;
	global_root = node;
	fprintf(stderr, "%d, ", key);
	return node;
}
void delete_rbtree_node(rbtree_node *node) {
	free(node->value);
	free(node);
}

// Node rotateLeft(Node h)
// {
//  x = h.right;
//  h.right = x.left;
//  x.left = h;
//  x.color = h.color;
//  h.color = RED;
//  return x;
// }

void rbtree_set_parent(rbtree_node *node, rbtree_node *parent) {
	if (node)
		node->parent = parent;
}

rbtree_node *rbtree_rotate_left(rbtree_node *h)
{
	// Left-leaning Red-Black Tree implementation

	rbtree_node *x = h->right;
	h->right = x->left;
	x->left = h;
	x->color = h->color;
	h->color = RED;

	// Update parent pointers
	rbtree_set_parent(x, h->parent);
	rbtree_set_parent(x->left, x); // x->left == h
	rbtree_set_parent(x->right, x);
	rbtree_set_parent(h->left, h);
	rbtree_set_parent(h->right, h);

	return x;
}

rbtree_node *rbtree_rotate_right(rbtree_node *h) {
	// Right-leaning Red-Black Tree implementation
	rbtree_node *x = h->left;
	h->left = x->right;
	x->right = h;
	x->color = h->color;
	h->color = RED;

	// Update parent pointers
	rbtree_set_parent(x, h->parent);
	rbtree_set_parent(x->left, x);
	rbtree_set_parent(x->right, x);
	rbtree_set_parent(h->left, h);
	rbtree_set_parent(h->right, h);

	return x;
}

void rbtree_flip_colors(rbtree_node *h) {
	h->color = h->color == RED ? BLACK : RED;
	h->left->color = h->left->color == RED ? BLACK : RED;
	h->right->color = h->right->color == RED ? BLACK : RED;
}

char *rbtree_search(int key) {
	rbtree_node *x = global_root;
	while (x) {
		if (key < x->key)
			x = x->left;
		else if (key > x->key)
			x = x->right;
		else
			return x->value;
	}
	return NULL;
}

// Forward declaration
rbtree_node *rbtree_insert_node(rbtree_node *h, int key, char *value);

void rbtree_insert(int key, char *value) {
	global_root = rbtree_insert_node(global_root, key, value);

	rbtree_set_parent(global_root, NULL);
	rbtree_set_parent(global_root->left, global_root);
	rbtree_set_parent(global_root->right, global_root);

	global_root->color = BLACK;
}

int rbtree_is_red(rbtree_node *h) {
	return h && h->color == RED;
}

rbtree_node *rbtree_insert_node(rbtree_node *h, int key, char *value) {
	if (!h)
		return create_rbtree_node(NULL, key, value);
	
	if (key < h->key) {
		h->left = rbtree_insert_node(h->left, key, value);
		rbtree_set_parent(h->left, h);
	} else if (key > h->key) {
		h->right = rbtree_insert_node(h->right, key, value);
		rbtree_set_parent(h->right, h);
	} else {
		// Key already exists, replace value
		free(h->value);
		h->value = strdup(value);
	}

	// Rebalance (Leaft-leaning Red-Black Tree implementation)
	if (rbtree_is_red(h->right) && !rbtree_is_red(h->left))
		h = rbtree_rotate_left(h);
	if (rbtree_is_red(h->left) && rbtree_is_red(h->left->left))
		h = rbtree_rotate_right(h);
	if (rbtree_is_red(h->left) && rbtree_is_red(h->right))
		rbtree_flip_colors(h);
	
	return h;
}



void rbtree_print_pretty_as_tree(rbtree_node *root, int level, char dir)
{
	if (root == NULL)
		return;

	const char ANSI_BLACK[] = "\x1b[30m";
	const char ANSI_RED[] = "\x1b[31m";
	const char ANSI_RESET[] = "\x1b[0m";


	rbtree_print_pretty_as_tree(root->left, level + 1, 'l');
	for (int i = 0; i < level; i++)
		printf("\t");
	printf("%c%s%d%s\n", dir, root->color == BLACK ? ANSI_BLACK : ANSI_RED, root->key, ANSI_RESET);
	rbtree_print_pretty_as_tree(root->right, level + 1, 'r');
}

int main(void)
{
	//test the tree
	global_root = create_rbtree_root(0, "0");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	/*
															
	        		0													

	*/
	
	printf("-------1---------\n");
	// normal insert
	rbtree_insert(30, "30");
	// printf ("%p %p\n", global_root->left, global_root->right);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	/*
		  		
	        		0
					 \ (right)
					  3 																															
	*/
	
	printf("--------2--------\n");
	// 3rd pic
	rbtree_insert(70, "70");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	/*                 
	                  0
		  		     /
	        		3
				    \ (right)
	     		     7 																															
	*/
	
	printf("--------3--------\n");
	// recolor 2
	rbtree_insert(20, "20");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
		/*    
	                   0
		  		     /   \
	        		3     2
				     \ 
	     		       7
																																		
	*/
	
	printf("--------4--------\n");
// 4th pic
	rbtree_insert(10, "10");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	/*    
	                     0
	                    /
	                   1
		  		     /   \
	        		3     2
				     \ 
	     		       7
																																		
	*/
	
	printf("-------5---------\n");
	rbtree_insert(40, "40");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	/*    
	                     0
	                    /
	                   1
		  		     /   \
	        		3     2
				     \           
					  \         
					   \       
					    \     4
						 \   /
						   7
																																		
	*/

	//2nd pic
	printf("-------6---------\n");
	rbtree_insert(60, "60");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	// recolor
	printf("-------7---------\n");
	rbtree_insert(50, "50");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	// normal insert
	printf("-------8---------\n");
	rbtree_insert(80, "80");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	// 3rd pic
	printf("-------9---------\n");
	rbtree_insert(90, "90");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	// recolor + rotate left on root
	printf("-------10---------\n");
	rbtree_insert(100, "100");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	// last tested
	// 3rd pic
	printf("-------11---------\n");
	rbtree_insert(110, "110");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	// recolor parent + uncle + grand parent + grand->
	printf("-------12---------\n");
	rbtree_insert(120, "120");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	// last tested
	//3rd pic
	printf("-------13---------\n");
	rbtree_insert(130, "130");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	// recolor + rotate left
	printf("-------14---------\n");
	rbtree_insert(140, "140");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------15---------\n");
	rbtree_insert(35, "35");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	printf("-------16---------\n");
	rbtree_insert(32, "32");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------17---------\n");
	rbtree_insert(33, "33");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');


	printf("-------18---------\n");
	rbtree_insert(34, "34");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------19---------\n");
	rbtree_insert(1, "1");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------20---------\n");
	rbtree_insert(2, "2");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	// last tested
	printf("-------21---------\n");
	rbtree_insert(3, "3");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	// printf("\n\n%d->%d->%d->%d->%d", global_root->left->key, global_root->left->left->key, global_root->left->left->left->key, global_root->left->left->left->left->key, global_root->left->left->left->left->left->key);

	printf("-------22---------\n");
	rbtree_insert(4, "4");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');


	for (int i = 500; i < 700; i++)
	{
		printf("-------%d---------\n", i - 500 + 23);
		rbtree_insert(i, "i");
		rbtree_print_pretty_as_tree(global_root, 0, 'R');
	}

	/*
	printf("-------DELETIONS---------\n");
	printf("-------1---------\n");
	rbtree_erase(global_root, 10);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------2---------\n");
	rbtree_erase(global_root, 40);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------3---------\n");
	rbtree_erase(global_root, 33);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------4---------\n");
	rbtree_erase(global_root, 60);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------5---------\n");
	rbtree_erase(global_root, 130);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');


	exit(0);
*/
	//print the tree
	//find a value
	char *value = rbtree_search(3);
	printf("3: %s\n", value);
}