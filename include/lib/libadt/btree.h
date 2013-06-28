#ifndef __BTREE_H
#define __BTREE_H

#include <lib/libc/stdint.h>
#include <lib/libc/stdbool.h>

/* Binary tree node structure */
typedef struct btree_node
{
	/* Value */
	void *value;

	/* Left, right, and parent nodes */
	struct btree_node *left, *right;
	struct btree_node *parent;

	/* The tree that the node belongs to */
	void *tree;

	/* Node exists */
	bool exists;
} btree_node_t;

/* Binary tree structure */
typedef struct btree
{
	/* Root node and maximum nodes */
	btree_node_t *root;
	uint32_t max_nodes;
} btree_t;

/* Create, place, destroy, insert an object into, and search a binary tree */
btree_t create_btree(uint32_t max_size);
btree_t place_btree(void *addr, uint32_t max_size);
void destroy_btree(btree_t tree);
void insert_btree(btree_t tree, void *value);
btree_node_t *search_btree(btree_t tree, void *value);

/* Create, destroy, insert an object at, and search at a binary tree node */
btree_node_t *create_btree_node(btree_t *tree);
void destroy_btree_node(btree_node_t *node);
void insert_btree_node(btree_node_t *node, void *value);
btree_node_t *search_btree_node(btree_node_t *node, void *value);

#endif
