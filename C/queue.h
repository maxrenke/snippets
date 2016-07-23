/*****************
       QUEUE 
 *****************/
struct queue{
	struct node** q;
	int head;
	int tail;
	int length;
};

void enqueue(struct queue* Q, struct node* x);
struct node* dequeue(struct queue* Q);