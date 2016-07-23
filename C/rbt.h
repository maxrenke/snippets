/**
 * \file rbst.h
 *
 * A red/black search tree on disk locations.
 *
 * \author mrenke
 * \date 09-23-2014
 */

#if !defined(_RBST_H_)
#define _RBST_H_

#define BLACK 0x1
#define RED 0x0
#define NIL -1

#include "disk_loc.h"

/* A simple red-black tree node of disk locations. */
struct rbtree_node {
	struct disk_location loc;
	struct rbtree_node *left;
	struct rbtree_node *right;
	struct rbtree_node *parent;
	int color;
};

struct rbtree {
	struct rbtree_node *root;
	struct rbtree_node *nil;
};

/*
 * Inserts a location into the binary tree.
 *
 * Returns 0 on success and 1 on failure.
 */
int insert_rbtree_location(struct rbtree_node **root, struct disk_location *loc);

/* Frees the memory allocated for the location tree. */
void free_rbtree(struct rbtree_node *root);

/*
 * Removes the given node from the tree.
 *
 * Blame Cormen, Leiserson, Rivest and Stein for the awful variable
 * names.
 */
void remove_rbtree_node(struct rbtree_node **root, struct rbtree_node *z);

/*
 * Remove the given location from the tree if it is there.
 *
 * Returns 1 if the location was removed and 0 if not (because it was
 * not found).
 */
int remove_rbtree_location(struct rbtree_node **root, struct disk_location *loc);

/*
 * Gets up to 'n' elements that come directly after (and including)
 * 'loc'.
 */
int rbtree_n_after(struct rbtree_node *node, struct disk_location *loc,
		 struct disk_location *locs[], unsigned int n,
		 unsigned int fill);

/*
 * Gets up to 'n' elements that come directly before (and including)
 * 'loc'.
 */
int rbtree_n_before(struct rbtree_node *node, struct disk_location *loc,
		  struct disk_location *locs[], unsigned int n,
		  unsigned int fill);

/*
 * Output the tree to the given file.
 */
void output_rbtree(FILE *outfile, int depth, struct rbtree_node *root);

#endif /* !_RBST_H_ */
