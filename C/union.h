/**
 * \file union.h
 *
 * Header function for union.c. Provides set struct.
 *
 * \author mrenke
 * \date 28-10-2014
 */

struct set{
	int node;
	struct set* p;
	int r;
};

struct set* make_set(int node);

void union_set(struct set* x, struct set* y);

void link(struct set* x, struct set* y);

struct set* find_set(struct set* x);