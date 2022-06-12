// Same thing, but with a red black tree:
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
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

typedef struct rbtree_op_result {
	rbtree_node *node;
	int result;
} rbtree_op_result;


rbtree_node *global_root;
rbtree_node *global_leftmost;
rbtree_node *global_rightmost;
size_t global_size = 0;


rbtree_node *rbtree_create_node(rbtree_node *parent, int key, char *value) {
	rbtree_node *node = malloc(sizeof(rbtree_node));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
	node->value = strdup(value);
	node->color = RED;
	return node;
}

rbtree_node *rbtree_create_root(int key, char *value) {
	rbtree_node *node = malloc(sizeof(rbtree_node));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->value = strdup(value);
	node->color = BLACK;
	global_root = node;
	global_size = 1;
	(void)(1||fprintf(stderr, "%d, ", key));
	global_leftmost = node;
	global_rightmost = node;
	return node;
}
void rbtree_destroy_node(rbtree_node *node) {
	printf("destoted node: %d\n", node->key);
	free(node->value);
	// if (node->key == 699)
	// 	*(node->value) = "XDDDDDD CRASH ME HARDER DADDY";
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

rbtree_node *rbtree_search(int key) {
	rbtree_node *x = global_root;
	while (x) {
		if (key < x->key)
			x = x->left;
		else if (key > x->key)
			x = x->right;
		else
			return x;
	}
	return NULL;
}

// Forward declaration
rbtree_node *rbtree_insert_node(rbtree_node *h, rbtree_node **__new_node_ptr, int key, char *value);

rbtree_op_result rbtree_insert(int key, char *value) {
	rbtree_node *inserted_node = NULL;
	size_t old_size = global_size;
	global_root = rbtree_insert_node(global_root, &inserted_node, key, value);

	rbtree_set_parent(global_root, NULL);
	rbtree_set_parent(global_root->left, global_root);
	rbtree_set_parent(global_root->right, global_root);

	global_root->color = BLACK;

	rbtree_op_result res;

	res.node = inserted_node;
	res.result = old_size != global_size;

	return res;
}

int rbtree_is_red(rbtree_node *h) {
	return h && h->color == RED;
}

rbtree_node *rbtree_fix_up(rbtree_node *h) {
	if (rbtree_is_red(h->right) && !rbtree_is_red(h->left))
		h = rbtree_rotate_left(h);
	if (rbtree_is_red(h->left) && rbtree_is_red(h->left->left))
		h = rbtree_rotate_right(h);
	if (rbtree_is_red(h->left) && rbtree_is_red(h->right))
		rbtree_flip_colors(h);

	return h;
}

rbtree_node *rbtree_insert_node(rbtree_node *h, rbtree_node **__new_node_ptr, int key, char *value) {
	if (!h)
	{
		global_size++;
		return (*__new_node_ptr = rbtree_create_node(NULL, key, value));
	}
	if (key < h->key) {
		h->left = rbtree_insert_node(h->left, __new_node_ptr, key, value);
		rbtree_set_parent(h->left, h);
		if (h == global_leftmost)
			global_leftmost = *__new_node_ptr;
	} else if (key > h->key) {
		h->right = rbtree_insert_node(h->right, __new_node_ptr, key, value);
		if (h == global_rightmost)
			global_rightmost = *__new_node_ptr;
		rbtree_set_parent(h->right, h);
	} else {
		// Key already exists, replace value
		free(h->value);
		h->value = strdup(value);
		*__new_node_ptr = h;
	}

	// Rebalance (Leaft-leaning Red-Black Tree implementation)
	return rbtree_fix_up(h);
}

rbtree_node *rbtree_move_red_left(rbtree_node *h) {
	rbtree_flip_colors(h);
	if (rbtree_is_red(h->right->left)) {
		h->right = rbtree_rotate_right(h->right);
		h = rbtree_rotate_left(h);
		rbtree_flip_colors(h);
	}
	return h;
}

rbtree_node *rbtree_move_red_right(rbtree_node *h) {
	rbtree_flip_colors(h);
	if (rbtree_is_red(h->left->left)) {
		h = rbtree_rotate_right(h);
		rbtree_flip_colors(h);
	}
	return h;
}

// Forward declaration
rbtree_node * rbtree_delete_node(rbtree_node *h, int key);

rbtree_op_result rbtree_delete(int key) {
	rbtree_op_result res;
	size_t old_size = global_size;

	res.node = NULL;

	global_root = rbtree_delete_node(global_root, key);
	global_root->color = BLACK;

	rbtree_set_parent(global_root, NULL);
	rbtree_set_parent(global_root->left, global_root);
	rbtree_set_parent(global_root->right, global_root);

	res.result = old_size == (global_size + 1);

	return res;
}

//rbtree_min
rbtree_node *rbtree_min(rbtree_node *h) {
	while (h->left)
		h = h->left;
	return h;
}

//rb_tree_max
rbtree_node *rbtree_max(rbtree_node *h) {
	while (h->right)
		h = h->right;
	return h;
}

void rbtree_swap_relink_nodes(rbtree_node *a, rbtree_node *b) {

	rbtree_node	*ap = a->parent;
	rbtree_node	*bp = b->parent;
	rbtree_node	*al = a->left;
	rbtree_node	*ar = a->right;
	rbtree_node	*bl = b->left;
	rbtree_node	*br = b->right;
	color 		ac 	= a->color;
	color 		bc 	= b->color;

	if (a->left == b)
	{
		a->left = b->left;
		b->left = a;
		b->parent = a->parent;
		a->parent = b;
		
		a->right = br;
		b->right = ar;
		
		rbtree_set_parent(a->right, a);
		rbtree_set_parent(b->right, b);

		a->color = bc;
		b->color = ac;

		return ;
	}
	if (a->right == b) {
		a->right = b->right;
		b->right = a;
		b->parent = a->parent;
		a->parent = b;

		a->left = bl;
		b->left = al;

		rbtree_set_parent(a->left, a);
		rbtree_set_parent(b->left, b);

		a->color = bc;
		b->color = ac;

		return ;
	}

	if (b->left == a || b->right == a) {
		rbtree_swap_relink_nodes(b, a);
		return ;
	}

	if (a == global_root)
		global_root = b;
	else if (b == global_root)
		global_root = a;
	
	if (ap) {
		if (ap->left == a)
			ap->left = b;
		else if (ap->right == a)
			ap->right = b;
		else {
			(void)(0||printf ("WTFFFFB ????? WHAT HAVE YOU DONE MORTYY?§§?§?D?§?§.?§.§? YHOUVE KILLED US ALLL"));
			exit(1);
		}
	}
	if (bp) {
		if (bp->left == b)
			bp->left = a;
		else if (bp->right == b)
			bp->right = a;
		else {
			(void)(0||printf ("WTFFFFB ????? WHAT HAVE YOU DONE MORTYY?§§?§?D?§?§.?§.§? YHOUVE KILLED US ALLL"));
			exit(1);
		}
		
	}

	if (al)
		al->parent = b;
	if (ar)
		ar->parent = b;

	if (bl)
		bl->parent = a;
	if (br)
		br->parent = a;

	a->parent = bp;
	b->parent = ap;

	a->left = bl;
	a->right = br;

	b->left = al;
	b->right = ar;

	a->color = bc;
	b->color = ac;
}

void rbtree_print_pretty_as_tree(rbtree_node *root, int level, char dir);
rbtree_node * rbtree_delete_node(rbtree_node *h, int key) {
	// left-leaning Red-Black Tree implementation
	if (key < h->key) {
		if (!rbtree_is_red(h->left) && !rbtree_is_red(h->left->left))
			h = rbtree_move_red_left(h);


		const rbtree_node *old_left = h->left;
	
		h->left = rbtree_delete_node(h->left, key);
		rbtree_set_parent(h->left, h);

		if (old_left == global_leftmost)
			global_leftmost = h->left ? h->left : h;
		
	} else {
		if (rbtree_is_red(h->left))
			h = rbtree_rotate_right(h);
		if (key == h->key && !h->right)
		{
			rbtree_destroy_node(h);

			global_size--;
			return NULL;
		}
		if (!rbtree_is_red(h->right) && !rbtree_is_red(h->right->left))
			h = rbtree_move_red_right(h);

		const rbtree_node *old_right = h->right; // save old right node to update global_rightmost

		if (key == h->key) {
			rbtree_node *x = rbtree_min(h->right);



			// (void)(0||printf("before: h->parent = %p, h->left = %p, h->right = %p, h->color = %d, x->parent = %p, x->left = %p, x->right = %p, x->color = %d\n", h->parent, h->left, h->right, h->color, x->parent, x->left, x->right, x->color));
			(void)(printf("h->key = %d, h->value = %s, h->color=%d, x->key = %d, x->value = %s, x->color=%d\n", h->key, h->value, h->color, x->key, x->value, x->color));
			
			
			// Life could be a dream
			// free(h->value);
			// h->key = x->key;
			// h->value = x->value;

			rbtree_swap_relink_nodes(h, x);
			rbtree_node *tmp = x;
			x = h;
			h = tmp;
			x->key = h->key;
			// x can keep its original value for deletion

			// (void)(0||printf("after: h->parent = %p, h->left = %p, h->right = %p, h->color = %d, x->parent = %p, x->left = %p, x->right = %p, x->color = %d\n", h->parent, h->left, h->right, h->color, x->parent, x->left, x->right, x->color));
			(void)(printf("h->key = %d, h->value = %s, h->color=%d, x->key = %d, x->value = %s, x->color=%d\n\n", h->key, h->value, h->color, x->key, x->value, x->color));

			// Original paper uses a deleteMin but this works too
			h->right = rbtree_delete_node(h->right, x->key);
		} else {
			h->right = rbtree_delete_node(h->right, key);
		}
		rbtree_set_parent(h->right, h);
		if (old_right == global_rightmost)
			global_rightmost = h->right ? h->right : h;
	}
	return rbtree_fix_up(h);
}

// Get next node in range
// ex: if key = 5 and 6 exist, returns 6
rbtree_node * rbtree_next(rbtree_node *pos) {
	if (pos->right)
		return rbtree_min(pos->right);
	rbtree_node *p = pos->parent;
	while (p && pos == p->right) {
		pos = p;
		p = p->parent;
	}
	return p;
}

// get previous node in range
rbtree_node * rbtree_prev(rbtree_node *pos) {
	if (pos->left)
		return rbtree_max(pos->left);
	rbtree_node *p = pos->parent;
	while (p && pos == p->left) {
		pos = p;
		p = p->parent;
	}
	return p;
}


const char ANSI_BLACK[] = "\x1b[30m";
const char ANSI_RED[] = "\x1b[31m";
const char ANSI_GREEN[] = "\x1b[32m";
const char ANSI_RESET[] = "\x1b[0m";

void check_if_rbtree_is_broken(rbtree_node *root);

void rbtree_print_pretty_as_tree(rbtree_node *root, int level, char dir)
{
	if (root == NULL)
		return;
	rbtree_print_pretty_as_tree(root->left, level + 1, 'l');
	for (int i = 0; i < level; i++)
		(void)(0||printf("\t"));
	(void)(0||printf("%c%s%d%s\n", dir, root->color == BLACK ? ANSI_BLACK : ANSI_RED, root->key, ANSI_RESET));
	rbtree_print_pretty_as_tree(root->right, level + 1, 'r');
}

// Iterates trough all nodes to check if their parent is set correctly
int check_if_rbtree_parents_are_broken(rbtree_node *root) {
	if (root == NULL)
		return 0;
	if (root->parent == NULL)
		return (root != global_root) || (check_if_rbtree_parents_are_broken(root->left) || check_if_rbtree_parents_are_broken(root->right));
	if (root->parent->left == root->parent->right)
		return 1;
	if (root->parent->left == root || root->parent->right == root)
		return check_if_rbtree_parents_are_broken(root->left) || check_if_rbtree_parents_are_broken(root->right);
	return 1;
}

int check_if_rbtree_has_double_reds(rbtree_node *root) {
	if (root == NULL)
		return 0;
	if (rbtree_is_red(root) && (rbtree_is_red(root->left) || rbtree_is_red(root->right)))
		return 1;
	return check_if_rbtree_has_double_reds(root->left) || check_if_rbtree_has_double_reds(root->right);
}

int count_black_nodes_from_leaf(rbtree_node *leaf) {
	if (leaf == NULL)
		return 0;
	int count = 0;
	while (leaf) {
		if (!rbtree_is_red(leaf))
			count++;
		leaf = leaf->parent;
	}
	return count;
}

struct count_test {
	int count;
	int isInitialized;
};

int check_black_nodes_helper(rbtree_node *root, struct count_test *count_test) {
	if (root == NULL)
		return 0;
	
	int count = count_black_nodes_from_leaf(root);
	if (root->left == NULL && root->right == NULL) {
		if (count_test->isInitialized) {
			if (count_test->count != count)
				return 1;
		} else {
			count_test->count = count;
			count_test->isInitialized = 1;
		}
	}
	return check_black_nodes_helper(root->left, count_test) || check_black_nodes_helper(root->right, count_test);
}

int check_if_all_path_have_same_number_of_black_nodes(rbtree_node *root) {
	struct count_test count_test;
	count_test.isInitialized = 0;
	return check_black_nodes_helper(root, &count_test);
}
		
void check_if_rbtree_is_broken(rbtree_node *root) {
	int res = 0;
	if ((res = check_if_rbtree_parents_are_broken(root)))
		(void)(0||printf("%s[KO] RBTree PARENTS ARE BROKEN%s\n", ANSI_RED, ANSI_RESET));
	if (rbtree_is_red(root) && (res = 1))
		(void)(0||printf("%s[KO] RBTree VIOLATES RULE 2 (root must be black)%s\n", ANSI_RED, ANSI_RESET));
	if (check_if_rbtree_has_double_reds(root) && (res = 1))
		(void)(0||printf("%s[KO] RBTree VIOLATES RULE 3 (no double red nodes)%s\n", ANSI_RED, ANSI_RESET));
	if (check_if_all_path_have_same_number_of_black_nodes(root)  && (res = 1))
		(void)(0||printf("%s[KO] RBTree VIOLATES RULE 4 (same number of black nodes for all paths to a leave)%s\n", ANSI_RED, ANSI_RESET));
	if (!res)
		(void)(0||printf("%s[OK] RBTree IS OK%s\n", ANSI_GREEN, ANSI_RESET));
	// else
	// 	exit(1);
}

int main(void)
{

	rbtree_op_result res;
	size_t old_size = 0;
	res.node = NULL;
	res.result = 0;

	//test the tree
	global_root = rbtree_create_root(0, "0");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	/*
															
	        		0													

	*/
	
	(void)(0||printf("-------1---------\n"));
	// normal insert
	rbtree_insert(30, "30");
	// (void)(0||printf ("%p %p\n", global_root->left, global_root->right));
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	/*
		  		
	        		0
					 \ (right)
					  3 																															
	*/
	
	(void)(0||printf("--------2--------\n"));
	// 3rd pic
	rbtree_insert(70, "70");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	/*                 
	                  0
		  		     /
	        		3
				    \ (right)
	     		     7 																															
	*/
	
	(void)(0||printf("--------3--------\n"));
	// recolor 2
	rbtree_insert(20, "20");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

		/*    
	                   0
		  		     /   \
	        		3     2
				     \ 
	     		       7
																																		
	*/
	
	(void)(0||printf("--------4--------\n"));
// 4th pic
	rbtree_insert(10, "10");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	/*    
	                     0
	                    /
	                   1
		  		     /   \
	        		3     2
				     \ 
	     		       7
																																		
	*/
	
	(void)(0||printf("-------5---------\n"));
	rbtree_insert(40, "40");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

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
	(void)(0||printf("-------6---------\n"));
	rbtree_insert(60, "60");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	
	// recolor
	(void)(0||printf("-------7---------\n"));
	rbtree_insert(50, "50");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	
	// normal insert
	(void)(0||printf("-------8---------\n"));
	rbtree_insert(80, "80");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	
	// 3rd pic
	(void)(0||printf("-------9---------\n"));
	rbtree_insert(90, "90");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	
	// recolor + rotate left on root
	(void)(0||printf("-------10---------\n"));
	rbtree_insert(100, "100");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	
	// last tested
	// 3rd pic
	(void)(0||printf("-------11---------\n"));
	rbtree_insert(110, "110");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);


	// recolor parent + uncle + grand parent + grand->
	(void)(0||printf("-------12---------\n"));
	rbtree_insert(120, "120");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);


	// last tested
	//3rd pic
	(void)(0||printf("-------13---------\n"));
	rbtree_insert(130, "130");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);


	// recolor + rotate left
	(void)(0||printf("-------14---------\n"));
	old_size = global_size;
	res = rbtree_insert(140, "140");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);
	assert (res.node->key == 140);
	assert(res.result);
	assert(global_size == old_size + 1);

	res = rbtree_insert(140, "140");
	assert (res.node->key == 140);
	assert(res.result == 0);

	(void)(0||printf("-------15---------\n"));
	rbtree_insert(35, "35");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	
	(void)(0||printf("-------16---------\n"));
	rbtree_insert(32, "32");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);


	(void)(0||printf("-------17---------\n"));
	rbtree_insert(33, "33");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);



	(void)(0||printf("-------18---------\n"));
	rbtree_insert(34, "34");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);


	(void)(0||printf("-------19---------\n"));
	rbtree_insert(1, "1");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);


	(void)(0||printf("-------20---------\n"));
	rbtree_insert(2, "2");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);


	// last tested
	(void)(0||printf("-------21---------\n"));
	rbtree_insert(3, "3");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	// (void)(0||printf("\n\n%d->%d->%d->%d->%d", global_root->left->key, global_root->left->left->key, global_root->left->left->left->key, global_root->left->left->left->left->key, global_root->left->left->left->left->left->key));

	(void)(0||printf("-------22---------\n"));
	rbtree_insert(4, "4");
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);



	for (int i = 500; i < 700; i++)
	{
		(void)(0||printf("-------%d---------\n", i - 500 + 23));
		rbtree_insert(i, "i");
		rbtree_print_pretty_as_tree(global_root, 0, 'R');
		check_if_rbtree_is_broken(global_root);
	}


	// test leftmost / rightmost
	(void)(0||printf("-------LEFT/RIGHT MOST---------\n"));
	assert(global_leftmost->key == 0);
	assert(global_rightmost->key == 699);

	(void)(0||printf("-------DELETIONS---------\n"));
	(void)(0||printf("-------1---------\n"));
	rbtree_delete(10);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);


	(void)(0||printf("-------2---------\n"));
	rbtree_delete(40);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	(void)(0||printf("-------3---------\n"));
	rbtree_delete(33);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	(void)(0||printf("-------4---------\n"));
	rbtree_delete(60);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	(void)(0||printf("-------5---------\n"));
	rbtree_delete(130);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	(void)(0||printf("-------6---------\n"));
	old_size = global_size;
	res = rbtree_delete(699);
	assert(old_size == global_size + 1);
	assert(res.result);
	// assert(res.node->key == 700);
	rbtree_print_pretty_as_tree(global_root, 0, 'R');
	check_if_rbtree_is_broken(global_root);

	rbtree_node *iterator = rbtree_search(501);
	// Test next and prev
	(void)(0||printf("-------ITERATORS---------\n"));
	(void)(0||printf("-------next---------\n"));
	rbtree_node *next = rbtree_next(iterator);
	printf ("%d\n", next->key);
	assert(next->key == 502);
	(void)(0||printf("-------prev---------\n"));
	rbtree_node *prev = rbtree_prev(iterator);
	printf ("%d\n", prev->key);
	assert(prev->key == 500);

	for (rbtree_node *it = rbtree_search(500); it != NULL; it = rbtree_next(it))
	{
		printf ("%d\n", it->key);
	}

	for (rbtree_node *it = rbtree_search(698); it != NULL; it = rbtree_prev(it))
	{
		printf ("%d\n", it->key);
	}

	// test leftmost / rightmost
	(void)(0||printf("-------LEFT/RIGHT MOST after delete ---------\n"));
	assert(global_leftmost->key == 0);
	assert(global_rightmost->key == 698);

	(void)(0||printf ("Rule 3 and/or 4 violations are expected\n"));
	global_root->right->color = RED; // Sabotage the tree to violate rule 4
	check_if_rbtree_is_broken(global_root);

	(void)(0||printf("Those two too\n"));
	global_root->right->left->color = RED; // Sabotage the tree to create double red
	check_if_rbtree_is_broken(global_root);

	(void)(0||printf("Those three also\n"));
	// Sabotage parents references
	global_root->right->right->left->parent = global_root->left;
	check_if_rbtree_is_broken(global_root);

	//print the tree
	//find a value
	char *value = rbtree_search(3)->value;
	(void)(0||printf("3: %s\n", value));
}