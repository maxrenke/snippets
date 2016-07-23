/**
 * \file tsort.c
 *
 * Helper function to implement a DFS to perform a topological search.
 *
 * \author mrenke
 * \date 28-10-2014
 */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/time.h>

#include "trie.h"
#include "depgraph.h"
#include "nodelist.h"
#include "depfile.h"
#include "debug.h"
#include "tsort.h"

#define WHITE 0
#define  GRAY 1
#define BLACK 2

void dfs_visit(struct node_list **headp, struct node_list* nodes, struct node* u){
	struct node_list* v;
	u->priv = (void*)GRAY;
	for (v = u->kids; v; v = v->next) {
		if( v->n->priv == WHITE){
			dfs_visit(headp, nodes, v->n);
		}
	}
	u->priv = (void*)BLACK;
	node_list_add(headp, u);
}