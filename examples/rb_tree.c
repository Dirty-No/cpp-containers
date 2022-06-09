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

void rb_tree_rotate_right(rbtree_node *node, int isLeft)
{
	rbtree_node old = *node;
	rbtree_node *old_parent = node->parent;

	node->parent = node->left;
	node->parent->parent = old.parent;
	node->parent->right = node->left;
	node->left->parent = node;

	if (node->parent == NULL)
		global_root = node;
	else if (node->parent->parent == NULL)
		global_root = node->parent;
	if (old_parent)
	{
		if (isLeft)
			old_parent->left = node->parent;
		else
			old_parent->right = node->parent;
	}
}

void rb_tree_rotate_left(rbtree_node *node, int isLeft) {
	rbtree_node old = *node;
	rbtree_node *old_parent = node->parent;

	node->parent = node->right;
	node->right->parent = old.parent;
	node->right = node->parent->left;

	if (node->parent == NULL)
		global_root = node;
	else if (node->parent->parent == NULL)
		global_root = node->parent;

	if (old_parent)
	{
		if (isLeft)
			old_parent->left = node->parent;
		else
			old_parent->right = node->parent;
	}
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

	if (key < pos->key) {
		if (pos->left)
			return rbtree_insert(pos->left,  key, value);
		// 1. Insert new RED node 
		pos->left = create_rbtree_node(pos, key, value);
		
		if (pos->left == NULL)
			return 1;

		// false: case 1, true: case 2
		if (pos->color == RED)
		{
			// 2. Recolor and rotates nodes to fix violation
			
			// rbtree_rebalance(pos->left);

			// We need to determine if we're in case 2a or case 2b
			// https://pages.cs.wisc.edu/~cs400/readings/Red-Black-Trees/
			// Case 2a: P's sibling S is black or null
			if (pos->right == NULL || pos->right->color == BLACK) {
				const int isPosLeftChild = pos->parent->left == pos;


				if (isPosLeftChild)
				{
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
					// 4th pic
					rbtree_node *P = pos;
					rbtree_node *G = P->parent;
					rbtree_node *S = G->left;
					rbtree_node *K = P->left;
					rbtree_node *ext1 = P->right;
					rbtree_node *ext2 = S->left;
					rbtree_node *ext3 = S->right;

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

					S->parent = G;
					S->left = ext2;
					S->right = ext3;

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
				// Case 2b: P's sibling S is red
				// We simply recolor 
				pos->parent = RED;
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
		if (pos->left == NULL || pos->left->color == BLACK) {
			const int isPosLeftChild = pos->parent->left == pos;

			if (isPosLeftChild)
			{
				//2nd pic
				rbtree_node *P = pos;
				rbtree_node *G = P->parent;
				rbtree_node *S = G->right;
				rbtree_node *K = P->right;
				rbtree_node *ext1 = P->left;
				rbtree_node *ext2 = S->left;
				rbtree_node *ext3 = S->right;

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

				S->parent = G;
				S->left = ext2;
				S->right = ext3;
				S->color = BLACK;

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
				// 3th pic
				rbtree_node *P = pos;
				rbtree_node *G = P->parent;
				rbtree_node *S = G->left;
				rbtree_node *K = P->right;
				rbtree_node *ext1 = P->left;
				rbtree_node *ext2 = S->left;
				rbtree_node *ext3 = S->right;

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

				S->parent = G;
				S->left = ext2;
				S->right = ext3;
				S->color = BLACK;

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
			pos->parent = RED;
			pos->parent->right->color = BLACK;
			pos->parent->left->color = BLACK;
		}
	}
	return 0;
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

void rbtree_print_pretty_as_tree(rbtree_node *root, int level)
{
	if (root == NULL)
		return;
	rbtree_print_pretty_as_tree(root->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("\t");
	printf("%d: %s\n", root->key, root->value);
	rbtree_print_pretty_as_tree(root->left, level + 1);
}

int main(void)
{
	//test the tree
	rbtree_node *root = create_rbtree_root(0, "root");
	rbtree_print_pretty_as_tree(root, 0);
	/*
															
	        		0													

	*/
	printf("----------------\n");

	rbtree_insert(root, 3, "right3");
	rbtree_print_pretty_as_tree(root, 0);
	/*
		  		
	        		0
					 \ (right)
					  3 																															
	*/
	printf("----------------\n");

	rbtree_insert(root, 7, "right7");
	rbtree_print_pretty_as_tree(root, 0);
	/*                 
	                  0
		  		     /
	        		3
				    \ (right)
	     		     7 																															
	*/
	printf("----------------\n");

	rbtree_insert(root, 2, "left2");
	rbtree_print_pretty_as_tree(root, 0);
	printf("----------------\n");

	rbtree_insert(root, 1, "left1");
	rbtree_print_pretty_as_tree(root, 0);
	/*    
	                   0
		  		     /   \
	        		3     2
				     \ 
	     		       7
																																		
	*/
	printf("----------------\n");

	rbtree_insert(root, 4, "right4");
	rbtree_print_pretty_as_tree(root, 0);
	/*
		  		
	        		3
				   /  \ (right)
	     		  0    7
				   \   /
				    2 4																															
	*/
	printf("----------------\n");

	rbtree_insert(root, 6, "right6");
	rbtree_print_pretty_as_tree(root, 0);
	printf("----------------\n");

	//print the tree
	//find a value
	char *value = rbtree_find(root, 3);
	printf("3: %s\n", value);
}