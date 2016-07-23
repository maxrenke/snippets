#include "graph.h"
#include <assert.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
/*
 * Helper function to pull element up a heap.
 */
void pull_up(struct edge* heap[], unsigned int n,  unsigned int i)
{
	int parent = (i-1)/2;
	if( i > 0 && heap[i]->cost < heap[parent]->cost ){
		struct edge* temp = heap[parent];
		heap[parent] = heap[i];
		heap[i] = temp;
		pull_up(heap, n, parent);
	}
}

/*
 * Helper function to find smallest index among parent and child.
 */
int smallest(struct edge* heap[], unsigned int n, unsigned int i)
{
	unsigned int parent = i;
	int r = parent;
	
	unsigned int left_child = (2*i) + 1;
	unsigned int right_child = (2*i) + 2;
	
	struct edge* min_parent = heap[i];
	struct edge* min_left_child = heap[left_child];
	struct edge* min_right_child = heap[right_child];
	
	if( left_child >= n || right_child >= n ) return i;
			
	if( min_parent->cost <= min_left_child->cost && 
		min_parent->cost <= min_right_child->cost ) r = parent;
	if( min_left_child->cost <= min_parent->cost && 
		min_left_child->cost <= min_right_child->cost ) r = left_child;
	if( min_right_child->cost <= min_parent->cost && 
		min_right_child->cost <= min_left_child->cost ) r = right_child;
		
	return r;
}

/*
 * Helper function to push element down a heap.
 */
void push_down(struct edge* heap[], unsigned int n,  unsigned int i)
{
	unsigned int smallesti = smallest(heap,n,i);
	if( smallesti != i ){
		struct edge* temp = heap[smallesti];
		heap[smallesti] = heap[i];
		heap[i] = temp;
		push_down(heap, n, smallesti);
	}
}

/*
 * Helper function to push element onto heap.
 * Element is added at the end of the heap and pushed up.
 */
struct edge ** push_edge(struct edge* heap[], unsigned int last,
				struct edge* edge)
{
	heap[last]= edge;

	if( last > 0 ) pull_up(heap, last, last);
	
	return heap;
}

/*
 * Helper function to extract min element and rebalance tree
 */
struct edge* pull_min(struct edge* heap[], unsigned int n, 
						unsigned int last)
{
	struct edge* min = heap[0];
	heap[0] = heap[last];
	heap[last] = 0;
	
	push_down(heap, n, 0);
	
	return min;
}
