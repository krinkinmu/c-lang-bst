#ifndef __AVL_H__
#define __AVL_H__

struct avl_node {
	struct avl_node *left;
	struct avl_node *right;
	struct avl_node *parent;
	int height;
};

struct avl_tree {
	struct avl_node *root;
};

struct avl_node *avl_next(struct avl_node *node);
struct avl_node *avl_prev(struct avl_node *node);

struct avl_node *avl_first(struct avl_tree *tree);
struct avl_node *avl_last(struct avl_tree *tree);

void avl_insert(struct avl_node *node, struct avl_tree *tree);
void avl_erase(struct avl_node *node, struct avl_tree *tree);
void avl_link(struct avl_node *node, struct avl_node *parent,
		struct avl_node **plink);

#endif /*__AVL_H__*/
