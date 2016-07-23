/**
 * \file rbst.c
 *
 * A red/black search tree on disk locations.
 *
 * \author mrenke
 * \date 09-23-2014
 */

#include <assert.h>
#include <stdlib.h>

#include "rbt.h"
#include "disk_loc.h"
#include "valid.h"

#define BLACK 0x1
#define RED 0x0

void left_rotate(struct rbtree_node **root, struct rbtree_node *x){
	struct rbtree_node *y;
	if( x == NULL) return;
	y = x->right;
	if( y == NULL ) return;
	x->right = y->left;
	if( y->left != NULL ){
		y->left->parent = x;
	}
	y->parent = x->parent;
	if( x->parent == NULL ){
		*root = y;
	}
	else if( x == x->parent->left){
		x->parent->left = y;
	}
	else{
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;	
}

void right_rotate(struct rbtree_node **root, struct rbtree_node *x){
	struct rbtree_node *y;
	if( x == NULL ) return;
	y = x->left;
	if( y == NULL ) return;
	x->left = y->right;
	if( y->right != NULL ){
		y->right->parent = x;
	}
	y->parent = x->parent;
	if( x->parent == NULL ){
		*root = y;
	}
	else if( x == x->parent->right){
		x->parent->right = y;
	}
	else{
		x->parent->left = y;
	}
	y->right = x;
	x->parent = y;	
}

int rbtree_insert_fixup(struct rbtree_node **root, struct rbtree_node *z){
	struct rbtree_node *y;
	/*validate(&root);*/
	
	while( z->parent != NULL && z->parent->color == RED ){
		if( z->parent == z->parent->parent->left){
			y = z->parent->parent->right;
			if( y != NULL && y->color == RED ){
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else{
				if( z == z->parent->right){
					z = z->parent;
					left_rotate(root, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				right_rotate(root, z->parent->parent);
			}
		}
		/* below same as above with left and right exchanged */
		else if( z->parent == z->parent->parent->right){
			y = z->parent->parent->left;
			if( y != NULL && y->color == RED ){
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				if( z->parent->parent == NULL ) printf("null");
				z = z->parent->parent;
			}
			else{
				if( z == z->parent->left){
					z = z->parent;
					right_rotate(root, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				left_rotate(root, z->parent->parent);
			}
		}
	}
	
	(*root)->color = BLACK;
	/*validate(root);*/
	return 0;
}

int insert_rbtree_location(struct rbtree_node **root, struct disk_location *loc)
{
	struct rbtree_node *y, *x, *z;
	
	z = malloc(sizeof(*z));
	
	y = NULL;
	x = *root;

	while( x != NULL){
		y = x;
		if( compare_locations(loc, &x->loc) < 0 ){
			x = x->left;
		} 
		else{
			x = x->right;
		}
	}
	z->parent = y;
	if( y == NULL){
		*root = z;
	}
	else if (compare_locations(loc, &y->loc) < 0 ){
		y->left = z;
	} 
	else{
		y->right = z;
	}
	z->left = NULL;
	z->right = NULL;
	z->color = RED;
	z->loc = *loc;
	rbtree_insert_fixup(root,z);
	
	/*validate(root);*/
	
	return 0;
}

void rbtree_transplant(struct rbtree_node **root, struct rbtree_node *u, 
					   struct rbtree_node *v)
{
	if( u->parent == NULL){
		*root = v;
	}
	else if ( u == u->parent->left ){
		u->parent->left = v;
	}
	else u->parent->right = v;
	if( v != NULL ) v->parent = u->parent;
}

struct rbtree_node * rbtree_minimum(struct rbtree_node *x){
	struct rbtree_node *y;
	if( x == NULL ) return NULL;
	y = NULL;
	x = x->left;
	while( x != NULL){
		y = x;
		x = x->left;
	}
	return y;
}

void rbtree_delete_fixup(struct rbtree_node **root, struct rbtree_node *x){
	struct rbtree_node *w;
	
	while( x != *root && x->color == BLACK && x->parent != NULL){
		if ( x == x->parent->left){			
			w = x->parent->right;
			if( w != NULL){
				if( w->color == RED ){
					w->color = BLACK;
					if( x->parent != NULL )x->parent->color = RED;
					left_rotate(root,x->parent);
					if( x->parent != NULL ) w = x->parent->right;
					else w = NULL;
				}
				if( w != NULL){
					if( w->left == NULL && w->right == NULL){
						w->color = RED;
						x = x->parent;
					}
					else if( w->left->color == BLACK && 
							 w->right->color == BLACK){
						w->color = RED;
						x = x->parent;
					}
					else{
						if( w->right == NULL){
							if( w->left != NULL ) w->left->color = BLACK;
							w->color = RED;
							right_rotate(root,w);
							if( x->parent != NULL ) w = x->parent->right;
							else w = NULL;
						}
						else if( w->right->color == BLACK){
							if( w->left != NULL ) w->left->color = BLACK;
							w->color = RED;
							right_rotate(root,w);
							if( x->parent != NULL ) w = x->parent->right;
							else w = NULL;
						}
						if ( w != NULL ){
							if( x->parent != NULL ) w->color = x->parent->color;
							else w->color = BLACK;
							if( x->parent != NULL ) x->parent->color = BLACK;
							if( w->right != NULL ) w->right->color = BLACK;
							else w = NULL;
						}
						left_rotate(root,x->parent);
						x = *root;	
					}
				}
			}
		}
		else if ( x == x->parent->right) { 
			w = x->parent->left;
			if( w != NULL){
				if( w->color == RED ){
					w->color = BLACK;
					if( x->parent != NULL ) x->parent->color = RED;
					left_rotate(root,x->parent);
					if( x->parent != NULL ) w = x->parent->left;
					else w = NULL;
				}
				if( w != NULL){
					if( w->left == NULL && w->right == NULL){
						w->color = RED;
						x = x->parent;
					}
					else if( w->left->color == BLACK && 
							 w->right->color == BLACK){
						w->color = RED;
						x = x->parent;
					}
					else{
						if( w->left == NULL){
							if( w->right != NULL ) w->right->color = BLACK;
							w->color = RED;
							left_rotate(root,w);
							if( x->parent != NULL ) w = x->parent->left;
							else w = NULL;
						}
						else if( w->left->color == BLACK){
							if( w->right != NULL ) w->right->color = BLACK;
							w->color = RED;
							left_rotate(root,w);
							if( x->parent != NULL ) w = x->parent->left;
							else w = NULL;
						}
						if ( w != NULL ){
							if( x->parent != NULL ) w->color = x->parent->color;
							else w->color = BLACK;
							if( x->parent != NULL ) x->parent->color = BLACK;
							if( w->left != NULL ) w->left->color = BLACK;
							else w = NULL;
						}
						right_rotate(root,x->parent);
						x = *root;
					}
				}
			}
		}
	}
	
	if( x != NULL ) x->color = BLACK;
}

void remove_rbtree_node(struct rbtree_node **root, struct rbtree_node *z)
{
	struct rbtree_node *y, *x;
	int y_original_color;
		
	if( *root == z ){
		*root = NULL;
		return;
	}
	
	/*validate(root); printf("\n");*/
	y = z;
	y_original_color = y->color;
	if( z->left == NULL){
		x = z->right;
		rbtree_transplant(root,z,z->right);
	}
	else if( z->right == NULL){
		x = z->left;
		rbtree_transplant(root,z,z->left);
	}
	else{
		y = rbtree_minimum(z->right);
		y_original_color = y->color;
		x = y->right;
		if( y->parent == z ){
			x->parent = y;
		}
		else{
			rbtree_transplant(root,y,y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		rbtree_transplant(root,z,y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	if( y_original_color == BLACK ){
		if( *root != NULL && x != NULL ) rbtree_delete_fixup(root,x);
	}
	
	/*validate(root);*/
}

int remove_rbtree_location(struct rbtree_node **root, struct disk_location *loc)
{
	struct rbtree_node *x, *y;
	
	y = NULL;
	x = *root;

	while( x != NULL){
		y=x;
		if( compare_locations(loc, &x->loc) < 0 ){
			x = x->left;
		} 
		else{
			x = x->right;
		}
	}
	if( y != NULL ){
		remove_rbtree_node(root,y);
		return 1;
	}

	return 0;
}

/**
 * Below is essentially copied from bst.c,
 * modified to work with a red-black tree
 */

void free_rbtree(struct rbtree_node *root)
{
	if (root) {
		free_rbtree(root->left);
		free_rbtree(root->right);
		free(root);
	}
}

int rbtree_n_after(struct rbtree_node *node, struct disk_location *loc,
		   struct disk_location *locs[], unsigned int n,
		   unsigned int fill)
{
	int c;

	if (!node)
		return fill;

	c = compare_locations(&node->loc, loc);
	if (c < 0) {
		return rbtree_n_after(node->right, loc, locs, n, fill);
	} else if (c >= 0) {
		int f = rbtree_n_after(node->left, loc, locs, n, fill);
		if (f < n) {
			locs[f] = &node->loc;
			f += 1;
			f = rbtree_n_after(node->right, loc, locs, n, f);
		}

		return f;
	}

	return fill;
}

int rbtree_n_before(struct rbtree_node *node, struct disk_location *loc,
		    struct disk_location *locs[], unsigned int n,
		    unsigned int fill)
{
	int c;

	if (!node)
		return fill;

	c = compare_locations(&node->loc, loc);
	if (c > 0) {
		return rbtree_n_before(node->left, loc, locs, n, fill);
	} else if (c <= 0) {
		int f = rbtree_n_before(node->right, loc, locs, n, fill);
		if (f < n) {
			locs[f] = &node->loc;
			f += 1;
			f = rbtree_n_before(node->left, loc, locs, n, f);
		}

		return f;
	}

	return fill;
}

void output_rbtree(FILE *outfile, int depth, struct rbtree_node *root)
{
	unsigned int i;
	for (i = 0; i < depth; i += 1)
		fprintf(outfile, " ");
	if (root) {
		output_location(outfile, &root->loc);
		fprintf(outfile, "\n");
		output_rbtree(outfile, depth + 1, root->left);
		output_rbtree(outfile, depth + 1, root->right);
	} else {
		fprintf(outfile, "(nil)\n");
	}
}
