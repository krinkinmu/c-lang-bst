#include <stdlib.h>
#include <stdio.h>
#include "rb.h"

struct rb_int_node {
	struct rb_node node;
	int value;
};

static struct rb_int_node *alloc_node(int value)
{
	struct rb_int_node *node = malloc(sizeof(*node));

	node->value = value;
	return node;
}

static void free_node(struct rb_int_node *node)
{
	free(node);
}

static void __print_tree(struct rb_node *node)
{
	struct rb_int_node *inode = (struct rb_int_node *)node;

	if (!inode) {
		printf("NIL (B)");
		return;
	}
	printf("( ");
	__print_tree(node->left);
	printf(" %d (%c) ", inode->value, node->parent & 1 ? 'B' : 'R');
	__print_tree(node->right);
	printf(" )");
}

static void print_tree(struct rb_tree *tree)
{
	__print_tree(tree->root);
	printf("\n");
}

static void __release_tree(struct rb_node *node)
{
	while (node) {
		struct rb_int_node *inode = (struct rb_int_node *)node;

		__release_tree(node->right);
		node = node->left;
		free_node(inode);
	}
}

static void release_tree(struct rb_tree *tree)
{
	__release_tree(tree->root);
}

static void test_insert(struct rb_tree *tree, int value)
{
	struct rb_int_node *node = alloc_node(value);
	struct rb_node **plink = &tree->root;
	struct rb_node *parent = 0;

	while (*plink) {
		struct rb_int_node *x = (struct rb_int_node *)*plink;

		parent = *plink;
		if (x->value < node->value)
			plink = &parent->right;
		else
			plink = &parent->left;
	}

	rb_link(&node->node, parent, plink);
	rb_insert(&node->node, tree);
}

static void test_remove(struct rb_tree *tree, int value)
{
	struct rb_node **plink = &tree->root;
	struct rb_node *parent = 0;

	while (*plink) {
		struct rb_int_node *x = (struct rb_int_node *)*plink;

		parent = *plink;

		if (x->value == value) {
			rb_erase(&x->node, tree);
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
	struct rb_tree tree = { 0 };
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
