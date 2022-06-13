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
