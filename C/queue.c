/*****************
       QUEUE 
 *****************/
#include "flow_net.h"
#include <limits.h>
#include <stdlib.h>
#include "queue.h"

void enqueue(struct queue* Q, struct node* x){
	Q->q[Q->tail] = x;
	if( Q->tail == Q->length ){
		Q->tail = 0;
	}
	else{
		Q->tail = Q->tail + 1;
	}
}

struct node* dequeue(struct queue* Q){
	struct node* x = Q->q[Q->head];
	Q->q[Q->head] = NULL;
	if( Q->head == Q->length ){
		Q->head = 0;
	}
	else{
		Q->head = Q->head + 1;
	}
	return x;
}