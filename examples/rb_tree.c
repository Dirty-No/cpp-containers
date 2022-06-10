/*
#include <stdlib.h>
#include <string.h>

typedef struct btree_node {
	int key;
	char *value;
	struct btree_node *left;
	struct btree_node *right;
} btree_node;

btree_node *create_btree_node(int key, char *value) {
	btree_node *node = malloc(sizeof(btree_node));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->value = strdup(value);
	return node;
}

void delete_btree_node(btree_node *node) {
	free(node->value);
	free(node);
}

char *btree_find(btree_node *root, int key) {
	if (root == NULL)
		return NULL;
	if (root->key == key)
		return root->value;
	if (key < root->key) {
		return btree_find(root->left, key);
	}
	return btree_find(root->right, key);
}

int btree_insert(btree_node *root, int key, char *value)
{
	if (root == NULL)
		return 1;
	if (root->key == key)
	{
		free(root->value);
		root->value = value;
	}
	if (key < root->key) {
		if (root->left)
			return btree_insert(root->left,  key, value);
		root->left = create_btree_node(key, value);
		return root->left ? 0 : 1;
	}
	if (root->right)
		return btree_insert(root->right,  key, value);
	root->right = create_btree_node(key, value);
	return root->right ? 0 : 1;
}

void btree_print_pretty_as_tree(btree_node *root, int level)
{
	if (root == NULL)
		return;
	btree_print_pretty_as_tree(root->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("\t");
	printf("%d: %s\n", root->key, root->value);
	btree_print_pretty_as_tree(root->left, level + 1);
}

int main(void)
{
	//test the tree
	btree_node *root = create_btree_node(5, "root");
	btree_insert(root, 3, "left3");
	btree_insert(root, 7, "right7");
	btree_insert(root, 2, "left2");
	btree_insert(root, 1, "left1");
	btree_insert(root, 4, "right4");
	btree_insert(root, 6, "right6");

	//print the tree
	btree_print_pretty_as_tree(root, 0);

	//find a value
	char *value = btree_find(root, 3);
	printf("3: %s\n", value);
}
*/

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
	return node;
}
void delete_rbtree_node(rbtree_node *node) {
	free(node->value);
	free(node);
}

// Search is the same algorithm for all btrees, but height is different
char *rb_tree_find(rbtree_node *root, int key) {
	if (root == NULL)
		return NULL;
	if (root->key == key)
		return root->value;
	if (key < root->key) {
		return rb_tree_find(root->left, key);
	}
	return rb_tree_find(root->right, key);
}

// void rb_tree_rotate_right(rbtree_node *node)
// {
// 	rbtree_node old = *node;
// 	rbtree_node *old_parent = node->parent;

// 	node->parent = node->left;
// 	node->parent->parent = old.parent;
// 	node->parent->right = node->left;
// 	node->left->parent = node;

// 	if (node->parent == NULL)
// 		global_root = node;
// 	else if (node->parent->parent == NULL)
// 		global_root = node->parent;
// 	if (old_parent)
// 	{
// 		if (isLeft)
// 			old_parent->left = node->parent;
// 		else
// 			old_parent->right = node->parent;
// 	}
// }

void rb_tree_rotate_left(rbtree_node *node) {
	printf("rotating left on %d\n", node->key);
	rbtree_node *B = node;
	rbtree_node *Y = B->right;
	rbtree_node *O = Y->left;

	Y->parent = B->parent;
	if (Y->parent == NULL)
	{
		global_root = Y;
		Y->color = BLACK;
	}
	else if (Y->parent->left == B)
		Y->parent->left = Y;
	else
		Y->parent->right = Y;
		
	Y->left = B;

	B->parent = Y;
	B->right = O;
	if (O)
		O->parent = B;
	if (B->left == NULL || B->left->color == BLACK || B->right == NULL || B->right->color == BLACK)
		B->color = RED;
	
	if (Y->color == RED)
		Y->color = BLACK;

	printf("B: %d, Y: %d, O: %d\n", B->key, Y->key, O ? O->key : 2727272);
}


// Strategy
// 1. Ins
int rbtree_insert(rbtree_node *pos, int key, char *value)
{
	if (pos == NULL)
		return 1;

	if (pos->key == key)
	{
		free(pos->value);
		pos->value = value;
	}

	// false: case 1, true: case 2
	const int isPosLeftChild = pos->parent == NULL || pos->parent->left == pos;
	const int isSimpleRecolor = (pos->parent == NULL) ||
		(isPosLeftChild ? pos->parent->right == NULL || pos->parent->right->color == BLACK : pos->parent->left == NULL || pos->parent->left->color == BLACK);

	if (key < pos->key) {
		if (pos->left)
			return rbtree_insert(pos->left,  key, value);
		// 1. Insert new RED node 
		pos->left = create_rbtree_node(pos, key, value);
		
		if (pos->left == NULL)
			return 1;

		if (pos->color == RED)
		{
			// 2. Recolor and rotates nodes to fix violation
			
			// rbtree_rebalance(pos->left);

			// We need to determine if we're in case 2a or case 2b
			// https://pages.cs.wisc.edu/~cs400/readings/Red-Black-Trees/
			// Case 2a: P's sibling S is black or null
			if (isSimpleRecolor) {
				if (isPosLeftChild)
				{
					printf("1th pic\n");
					//1st pic
					rbtree_node *P = pos;
					rbtree_node *G = P->parent;
					rbtree_node *S = G->right;
					rbtree_node *K = P->left;
					rbtree_node *ext1 = P->right;
					rbtree_node *ext2 = S->left;
					rbtree_node *ext3 = S->right;

					P->parent = G->parent;
					P->left = K;
					P->right = G;
					P->color = BLACK;
					if (P->parent == NULL)
						global_root = P;
					else {
						if (P->parent->left == G)
							P->parent->left = P;
						else
							P->parent->right = P;
					}

					K->left = NULL;
					K->right = NULL;
					K->parent = P;
					K->color = RED;
					
					G->parent = P;
					G->left = ext1;
					G->right = S;
					G->color = RED;
					
					S->left = ext2;
					S->right = ext3;
					S->color = BLACK;

					if (ext1)
						ext1->parent = G;
					if (ext2)
						ext2->parent = S;
					if (ext3)
						ext3->parent = S;
				}
				else
				{
					printf("4th pic\n");
					// 4th pic
					rbtree_node *P = pos;
					rbtree_node *G = P->parent;
					rbtree_node *S = G->left;
					rbtree_node *K = P->left;
					rbtree_node *ext1 = P->right;
					rbtree_node *ext2 = S ? S->left : NULL;
					rbtree_node *ext3 = S ? S->right : NULL;

					K->parent = G->parent;
					K->left = G;
					K->right = P;
					K->color = BLACK;
					if (K->parent == NULL)
						global_root = K;
					else {
						if (K->parent->left == G)
							K->parent->left = K;
						else
							K->parent->right = K;
					}


					G->parent = K;
					G->left = S;
					G->right = NULL;
					G->color = RED;

					if (S)
					{
						S->parent = G;
						S->left = ext2;
						S->right = ext3;
						S->color = BLACK;
					}

					P->parent = K;
					P->left = NULL,
					P->right = ext1;

					if (ext1)
						ext1->parent = P;
					if (ext2)
						ext2->parent = S;
					if (ext3)
						ext3->parent = S;
				}
			}
			else {
				printf("simple recolor 1 /!\\DANGER /!\\\n");
				// Case 2b: P's sibling S is red
				// We simply recolor
				// WIP, this case has never ocurred right now, so if it happens, we need to fix it
				if (global_root != pos->parent)
					pos->parent->color = RED;
				pos->parent->right->color = BLACK;
				pos->parent->left->color = BLACK;
			}
		}
		return 0;
	}

	if (pos->right)
		return rbtree_insert(pos->right,  key, value);

	// 1. Insert new RED node 
	pos->right = create_rbtree_node(pos, key, value);

	if (pos->right == NULL)
		return 1;

	// false: case 1, true: case 2
	if (pos->color == RED)
	{
		// 2. Recolor and rotates nodes to fix violation
		// rbtree_rebalance(pos->right);

		// We need to determine if we're in case 2a or case 2b
		// https://pages.cs.wisc.edu/~cs400/readings/Red-Black-Trees/

		// Case 2a: P's sibling S is black or null
		if (isSimpleRecolor) {

			if (isPosLeftChild)
			{
				printf("2th pic\n");
				//2nd pic
				rbtree_node *P = pos;
				rbtree_node *G = P->parent;
				rbtree_node *S = G->right;
				rbtree_node *K = P->right;
				rbtree_node *ext1 = P->left;
				rbtree_node *ext2 = S ? S->left : NULL;
				rbtree_node *ext3 = S ? S->right : NULL;

				K->parent = G->parent;
				K->left = P;
				K->right = G;
				K->color = BLACK;

				if (K->parent == NULL)
					global_root = K;
				else {
					if (K->parent->left == G)
						K->parent->left = K;
					else
						K->parent->right = K;
				}

				P->parent = K;
				P->left = ext1;
				P->right = NULL;
				P->color = RED;
				
				G->parent = K;
				G->left = NULL;
				G->right = S;
				G->color = RED;

				if (S)
				{
					S->parent = G;
					S->left = ext2;
					S->right = ext3;
					S->color = BLACK;
				}

				// Shoud not actually change anything
				if (ext1)
					ext1->parent = P;
				if (ext2)
					ext2->parent = S;
				if (ext3)
					ext3->parent = S;
			}
			else
			{
				printf("3rd pic\n");
				// 3rd pic
				rbtree_node *P = pos;
				rbtree_node *G = P->parent;
				rbtree_node *S = G->left;
				rbtree_node *K = P->right;
				rbtree_node *ext1 = S ? P->left : NULL;
				rbtree_node *ext2 = S ? S->left : NULL;
				rbtree_node *ext3 = S ? S->right : NULL;

				P->parent = G->parent;
				P->left = G;
				P->right = K;
				P->color = BLACK;
				if (P->parent == NULL)
					global_root = P;
				else {
					if (P->parent->left == G)
						P->parent->left = P;
					else
						P->parent->right = P;
				}

				G->parent = P;
				G->left = S;
				G->right = ext1;
				G->color = RED;

				if (S)
				{
					S->parent = G;
					S->left = ext2;
					S->right = ext3;
					S->color = BLACK;
				}

				K->parent = P;
				K->left = NULL;
				K->right = NULL;
				K->color = RED;
	
				// Should not actually change anything
				if (ext1)
					ext1->parent = G;
				if (ext2)
					ext2->parent = S;
				if (ext3)
					ext3->parent = S;
			}
		}
		else {
			// Case 2b: P's sibling S is red
			// We simply recolor 
			printf("simple recolor 2\n");
			pos->parent->right->color = BLACK;
			pos->parent->left->color = BLACK;
			if (global_root != pos->parent)
			{
				pos->parent->color = RED;
				// We may have introduced double reds, so we need check if they exist and fix them
				if (pos->parent->color == RED && pos->parent->parent->color == RED)
				{
					printf("created double red on %d -> %d\n", pos->parent->parent->key, pos->parent->key);
					// printf("double red, rotating left on %d\n", pos->parent->parent->parent->key);
					// rb_tree_rotate_left(pos->parent->parent->parent);
					rbtree_fix_double_red(pos->parent);
				}
			}
		}
	}
	return 0;
}

void rbtree_fix_double_red(rbtree_node *pos)
{
	// pos and pos->parent are red
	// If uncle of pos is red, push blackness down from grandparent

	rbtree_node *P = pos->parent;
	rbtree_node *G = P->parent;
	rbtree_node *U = G->right == P ? G->left : G->right;

	if (U && U->color == RED) {
		printf("recoloring on %d\n", pos->key);
		G->color = RED;
		if (G->left)
			G->left->color = BLACK;
		if (G->right)
			G->right->color = BLACK;
		if (G->parent == NULL) // root case
			G->color = BLACK;
		else if (G->parent->color == RED)
			rbtree_fix_double_red(G);
	}
	else
		rb_tree_rotate_left(G);
}
	

char *rbtree_find(rbtree_node *root, int key) {
	if (root == NULL)
		return NULL;
	if (root->key == key)
		return root->value;
	if (key < root->key) {
		return rbtree_find(root->left, key);
	}
	return rbtree_find(root->right, key);
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
	rbtree_insert(global_root, 30, "30");
	// printf ("%p %p\n", global_root->left, global_root->right);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	/*
		  		
	        		0
					 \ (right)
					  3 																															
	*/
	
	printf("--------2--------\n");
	// 3rd pic
	rbtree_insert(global_root, 70, "70");
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
	rbtree_insert(global_root, 20, "20");
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
	rbtree_insert(global_root, 10, "10");
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
	rbtree_insert(global_root, 40, "40");
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
	rbtree_insert(global_root, 60, "60");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	// recolor
	printf("-------7---------\n");
	rbtree_insert(global_root, 50, "50");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	// normal insert
	printf("-------8---------\n");
	rbtree_insert(global_root, 80, "80");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	// 3rd pic
	printf("-------9---------\n");
	rbtree_insert(global_root, 90, "90");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	// recolor + rotate left on root
	printf("-------10---------\n");
	rbtree_insert(global_root, 100, "100");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	// last tested
	// 3rd pic
	printf("-------11---------\n");
	rbtree_insert(global_root, 110, "110");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	// recolor parent + uncle + grand parent + grand->
	printf("-------12---------\n");
	rbtree_insert(global_root, 120, "120");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	// last tested
	//3rd pic
	printf("-------13---------\n");
	rbtree_insert(global_root, 130, "130");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	// last tested
	// recolor + rotate left
	printf("-------14---------\n");
	rbtree_insert(global_root, 140, "140");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------15---------\n");
	rbtree_insert(global_root, 35, "35");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	printf("-------16---------\n");
	rbtree_insert(global_root, 32, "32");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------17---------\n");
	rbtree_insert(global_root, 33, "33");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------18---------\n");
	rbtree_insert(global_root, 34, "34");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------19---------\n");
	rbtree_insert(global_root, 1, "1");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------20---------\n");
	rbtree_insert(global_root, 2, "2");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------21---------\n");
	rbtree_insert(global_root, 3, "3");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------22---------\n");
	rbtree_insert(global_root, 4, "4");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	for (int i = 500; i < 506; i++)
	{
		printf("-------%d---------\n", i - 500 + 23);
		rbtree_insert(global_root, i, "i");
		rbtree_print_pretty_as_tree(global_root, 0, 'R');
	}
	


	//print the tree
	//find a value
	char *value = rbtree_find(global_root, 3);
	printf("3: %s\n", value);
}