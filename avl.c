#include "avl.h"

static struct avl_node *avl_leftmost(struct avl_node *node)
{
	while (node->left)
		node = node->left;
	return node;
}

static struct avl_node *avl_rightmost(struct avl_node *node)
{
	while (node->right)
		node = node->right;
	return node;
}

struct avl_node *avl_next(struct avl_node *node)
{
	if (node->right)
		return avl_leftmost(node->right);

	while (node->parent && node->parent->right == node)
		node = node->parent;
	return node->parent;
}

struct avl_node *avl_prev(struct avl_node *node)
{
	if (node->left)
		return avl_rightmost(node->left);

	while (node->parent && node->parent->left == node)
		node = node->parent;
	return node->parent;
}

struct avl_node *avl_first(struct avl_tree *tree)
{
	if (tree->root)
		return avl_leftmost(tree->root);
	return 0;
}

struct avl_node *avl_last(struct avl_tree *tree)
{
	if (tree->root)
		return avl_rightmost(tree->root);
	return 0;
}

static int avl_height(struct avl_node *node)
{ return node ? node->height : 0; }

static int avl_balance(struct avl_node *node)
{
	if (!node)
		return 0;
	return avl_height(node->left) - avl_height(node->right);
}

#define MAX(a, b) (((a) < (b)) ? (b) : (a))
void avl_update(struct avl_node *node)
{ node->height = 1 + MAX(avl_height(node->left), avl_height(node->right)); }
#undef MAX

static void left_rotate(struct avl_node *node, struct avl_tree *tree)
{
	struct avl_node *x = node->right;

	node->right = x->left;
	x->left = node;
	x->parent = node->parent;
	node->parent = x;

	if (node->right)
		node->right->parent = node;

	if (x->parent) {
		if (x->parent->left == node)
			x->parent->left = x;
		else
			x->parent->right = x;
	} else {
		tree->root = x;
	}
	avl_update(node);
	avl_update(x);
}

static void right_rotate(struct avl_node *node, struct avl_tree *tree)
{
	struct avl_node *x = node->left;

	node->left = x->right;
	x->right = node;
	x->parent = node->parent;
	node->parent = x;

	if (node->left)
		node->left->parent = node;

	if (x->parent) {
		if (x->parent->left == node)
			x->parent->left = x;
		else
			x->parent->right = x;
	} else {
		tree->root = x;
	}
	avl_update(node);
	avl_update(x);
}

#include <stdio.h>

static void avl_fixup(struct avl_node *node, struct avl_tree *tree)
{
	while (node) {
		avl_update(node);
		if (avl_balance(node) > 1) {
			if (avl_balance(node->left) < 0)
				left_rotate(node->left, tree);
			right_rotate(node, tree);
		} else if (avl_balance(node) < -1) {
			if (avl_balance(node->right) > 0)
				right_rotate(node->right, tree);
			left_rotate(node, tree);
		}
		node = node->parent;
	}
}

void avl_insert(struct avl_node *node, struct avl_tree *tree)
{ avl_fixup(node, tree); }

void avl_erase(struct avl_node *node, struct avl_tree *tree)
{
	struct avl_node *p = node->parent;
	struct avl_node **pl;

	if (p)
		pl = p->left == node ? &p->left : &p->right;
	else
		pl = &tree->root;

	if (!node->left) {
		if (node->right)
			node->right->parent = p;
		*pl = node->right;
	} else if (!node->right) {
		node->left->parent = p;
		*pl = node->left;
	} else {
		struct avl_node *next = avl_leftmost(node->right);

		*pl = next;
		if (next->parent == node) {
			next->left = node->left;
			next->parent = p;
			p = next;
		} else {
			p = next->parent;
			p->left = 0;

			next->parent = node->parent;
			next->right = node->right;
			next->left = node->left;
		}
	}

	if (p)
		avl_fixup(p, tree);
}

void avl_link(struct avl_node *node, struct avl_node *parent,
			struct avl_node **plink)
{
	node->parent = parent;
	node->height = 1;
	node->left = node->right = 0;
	*plink = node;
}
