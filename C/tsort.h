/**
 * \file tsort.h
 *
 * Header for tsort.c.
 *
 * \author mrenke
 * \date 28-10-2014
 */
 
void dfs_visit(struct node_list **headp, struct node_list* nodes,
				struct node* u);