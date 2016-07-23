#include "graph.h"
#include <assert.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Swap
 * Swap two elements in an array based on inex.
 */
void swap( struct edge* ary[],
				  int i, int j)
{
	struct edge* temp = ary[i];
	ary[i] = ary[j];
	ary[j] = temp;
}

/*
 * Partition
 * Swap elements on pivot. Return middle index.
 */
int partition( struct edge* ary[],
				  int left, int right)
{
	struct edge* pivot = ary[left]; /* pivot point to swap elements */
	int l = left; /* left head */
	int r = right+1; /* right head */
	
	while( l < r) /* iterate through partition */
	{
		do ++l; while( ary[l]->cost <= pivot->cost && l < right ); /* approach pivot from left */
		do --r; while( ary[r]->cost > pivot->cost ); /* approach pivot from right */
		if( l < r ) /* if not middle */
		{
			swap(ary,l,r); /* swap elements */
		}
   }
   
   swap(ary,left,r); /* swap final elements */
   
   return r; /* return middle index */
   
}

/*
 * Quicksort Helper
 * Helper method to recurse through array.
 */
void quicksort_help( struct edge* *ary,
				  int left, int right)
{
	if( left < right) /* base case */
	{
		int median = partition(ary, left, right); /* get middle index */
		quicksort_help(ary, left, median-1); /* sort left half */
		quicksort_help(ary, median+1, right); /* sort right half */
	}
}

/*
 * Quicksort
 * Return 0 on success and 1 on failure.
 */
unsigned long do_quicksort(struct edge* ary[],
				  unsigned long n)
{	
	quicksort_help( ary, 0, n-1); /* call helper method */
	
	return 0; /* success */
}