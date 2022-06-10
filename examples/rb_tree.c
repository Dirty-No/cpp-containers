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

// Search is the same algorithm for all btrees, but height is different
rbtree_node *rbtree_find_node(rbtree_node *root, int key) {
	if (root == NULL)
		return NULL;
	if (root->key == key)
		return root;
	if (key < root->key) {
		return rbtree_find_node(root->left, key);
	}
	return rbtree_find_node(root->right, key);
}

char *rbtree_find(rbtree_node *root, int key) {
	const rbtree_node *found = rbtree_find_node(root, key);
	return found ? found->value : NULL;
}

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

void rb_tree_rotate_right(rbtree_node *node) {
	printf("rotating right on %d\n", node->key);
	rbtree_node *B = node;
	rbtree_node *Y = B->left;
	rbtree_node *O = Y->right;

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
		
	Y->right = B;

	B->parent = Y;
	B->left = O;
	if (O)
		O->parent = B;

	printf("B: %d, Y: %d, O: %d\n", B->key, Y->key, O ? O->key : 2727272);
}

void rbtree_fix_double_red_left(rbtree_node *pos)
{
	printf("fixing double red left on %d\n", pos->key);
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
			rbtree_fix_double_red_left(G);
	}
	else if (P->color == RED) {
		printf("rotating on %d\n", pos->key);
		if (G->left == P)
			rb_tree_rotate_right(G);
		else
			rb_tree_rotate_left(G);
	}
		// rb_tree_rotate_left(G);
}

void rbtree_fix_double_red_right(rbtree_node *pos)
{
	printf("fixing double red right on %d\n", pos->key);
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
			rbtree_fix_double_red_right(G);
	}
	else
	{
		rb_tree_rotate_right(G);
		rbtree_fix_double_red_left(G);
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

		fprintf(stderr, "%d, ", key);
		return 0;
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
				pos->parent->right->color = BLACK;
				pos->parent->left->color = BLACK;
				if (global_root != pos->parent)
				{
					pos->parent->color = RED;
					if (pos->parent->parent->color == RED)
						rbtree_fix_double_red_right(pos);
				}
				
				
			}
		}

		fprintf(stderr, "%d, ", key);
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
					
					rbtree_fix_double_red_left(pos->parent);
				}
			}
		}
	}
	fprintf(stderr, "%d, ", key);
	return 0;
}

rbtree_node *rbtree_find_inorder_predecessor(rbtree_node *node) {
	// rightmost internal position r of p's left subtreen
	node = node->left;
	while (node->right) {
		node = node->right;
	}
	return node;
}

void rbtree_swap_nodes(rbtree_node *__x, rbtree_node *__y) {
	const rbtree_node __x_tmp = *__x;
	__x->key = __y->key;
	__x->value = __y->value;
	__y->key = __x_tmp.key;
	__y->value = __x_tmp.value;
}

color rb_get_node_color(rbtree_node *node)
{
	return node ? node->color : BLACK;
}

int rb_is_black(rbtree_node *node) {
	return rb_get_node_color(node) == BLACK;
}

int rb_is_red(rbtree_node *node) {
	return !rb_is_black(node);
}

//https://www.youtube.com/watch?v=_ybZCHNSFOY
int rbtree_erase_node(rbtree_node *pos)
{
	if (pos == NULL)
		return 1;
	
	rbtree_node *K = pos;
	rbtree_node *P = K->parent;
	const int isLeft = P->left == K;
	// rbtree_node *S = isLeft ? P->right : P->left;
	// rbtree_node *G = P->parent;
	// const int isParentLeft = G->left == P;
	// rbtree_node *U = isParentLeft ? G->right : G->left;
	rbtree_node *CL = K->left;
	rbtree_node *CR = K->right;

	// case 1: K has at most one child
	if (CL == NULL || CR == NULL) {
		printf("Deleting case 1 on %d\n", K->key);
		// Promote the child
		rbtree_node *child = CL ? CL : CR;

		if (isLeft)
			P->left = child;
		else
			P->right = child;

		if (child)
		{		
			// child->color = P->color;
			child->parent = P;
		}

		if (global_root == K)
			global_root = child;
				
		delete_rbtree_node(K);
		
		return 0;
	}

	// case 2: K has two children
	printf("Deleting case 2 on %d\n", K->key);
	rbtree_node *R = rbtree_find_inorder_predecessor(K);
	printf("Found inorder predecessor %d\n", R->key);

	// printf("Before swap:\nK->left = %p, K->right= %p, K->parent= %p\nR->left= %p, R->right= %p, R->parent= %p\n", K->left, K->right, K->parent, R->left, R->right, R->parent);
	rbtree_swap_nodes(K, R);
	// printf("After swap:\nK->left = %p, K->right= %p, K->parent= %p\nR->left= %p, R->right= %p, R->parent= %p\n", K->left, K->right, K->parent, R->left, R->right, R->parent);

	// Back to case 1
	return rbtree_erase_node(R);
}	

int rbtree_erase(rbtree_node *root, int key)
{
	rbtree_node *pos = rbtree_find_node(root, key);
	if (pos == NULL)
		return 1;
	rbtree_erase_node(pos);
	return 0;
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

	// last tested
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

	// breaks here
	printf("-------21---------\n");
	rbtree_insert(global_root, 3, "3");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	// printf("\n\n%d->%d->%d->%d->%d", global_root->left->key, global_root->left->left->key, global_root->left->left->left->key, global_root->left->left->left->left->key, global_root->left->left->left->left->left->key);
	exit(0);

	printf("-------22---------\n");
	rbtree_insert(global_root, 4, "4");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');


	// for (int i = 500; i < 550; i++)
	// {
	// 	printf("-------%d---------\n", i - 500 + 23);
	// 	rbtree_insert(global_root, i, "i");
	// 	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	// }
	exit(0);
	printf("-------DELETIONS---------\n");
	printf("-------1---------\n");
	rbtree_erase(global_root, 4);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------2---------\n");
	rbtree_erase(global_root, 2);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------3---------\n");
	rbtree_erase(global_root, 1);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------4---------\n");
	rbtree_erase(global_root, 3);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------5---------\n");
	rbtree_erase(global_root, 20);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------6---------\n");
	rbtree_erase(global_root, 32);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------7---------\n");
	rbtree_erase(global_root, 33);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------8---------\n");
	rbtree_erase(global_root, 40);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	
	printf("-------9---------\n");
	rbtree_erase(global_root, 35);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------10---------\n");
	rbtree_erase(global_root, 34);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');

	printf("-------11---------\n");
	rbtree_erase(global_root, 50);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');


	//print the tree
	//find a value
	char *value = rbtree_find(global_root, 3);
	printf("3: %s\n", value);
}