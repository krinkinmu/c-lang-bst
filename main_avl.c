#include <stdlib.h>
#include <stdio.h>
#include "avl.h"

struct avl_int_node {
	struct avl_node node;
	int value;
};

static struct avl_int_node *alloc_node(int value)
{
	struct avl_int_node *node = malloc(sizeof(*node));

	node->value = value;
	return node;
}

static void free_node(struct avl_int_node *node)
{
	free(node);
}

static void __print_tree(struct avl_node *node)
{
	struct avl_int_node *inode = (struct avl_int_node *)node;

	if (!inode) {
		printf("NIL (0)");
		return;
	}
	printf("( ");
	__print_tree(node->left);
	printf(" %d (%d) ", inode->value, node->height);
	__print_tree(node->right);
	printf(" )");
}

static void print_tree(struct avl_tree *tree)
{
	__print_tree(tree->root);
	printf("\n");
}

static void __release_tree(struct avl_node *node)
{
	while (node) {
		struct avl_int_node *inode = (struct avl_int_node *)node;

		__release_tree(node->right);
		node = node->left;
		free_node(inode);
	}
}

static void release_tree(struct avl_tree *tree)
{
	__release_tree(tree->root);
}

static void test_insert(struct avl_tree *tree, int value)
{
	struct avl_int_node *node = alloc_node(value);
	struct avl_node **plink = &tree->root;
	struct avl_node *parent = 0;

	while (*plink) {
		struct avl_int_node *x = (struct avl_int_node *)*plink;

		parent = *plink;
		if (x->value < node->value)
			plink = &parent->right;
		else
			plink = &parent->left;
	}

	avl_link(&node->node, parent, plink);
	avl_insert(&node->node, tree);
}

static void test_remove(struct avl_tree *tree, int value)
{
	struct avl_node **plink = &tree->root;
	struct avl_node *parent = 0;

	while (*plink) {
		struct avl_int_node *x = (struct avl_int_node *)*plink;

		parent = *plink;

		if (x->value == value) {
			avl_erase(&x->node, tree);
			free_node(x);
			return;
		}

		if (x->value < value)
			plink = &parent->right;
		else
			plink = &parent->left;
	}
}

int main()
{
	struct avl_tree tree = { 0 };
	int i;

	print_tree(&tree);
	for (i = 0; i != 7; ++i) {
		test_insert(&tree, i + 1);
		print_tree(&tree);
	}

	for (i = 0; i != 7; ++i) {
		test_remove(&tree, i + 1);
		print_tree(&tree);
	}
	release_tree(&tree);

	return 0;
}
