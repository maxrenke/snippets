/**
 * \file union.c
 *
 * Implementation of union-find data structure.
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

#include "union.h"

struct set* make_set(int node){
	struct set* set = malloc(sizeof(struct set*));
	set->node = node;
	set->p = set;
	set->r = 0;
	return set;
}

struct set* find_set(struct set* x){
	if( x != x->p)
		x->p = find_set(x->p);
	return x->p;
}

void union_set(struct set* x, struct set* y){
	link(find_set(x),find_set(y));
}

void link(struct set* x, struct set* y){
	if( x->r > y->r ){
		y->p = x;
	}
	else{
		x->p = y;
		if( x->r == y->r ){
			y->r++;
		}
	}
}
