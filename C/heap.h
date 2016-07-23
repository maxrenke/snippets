/*
 * Helper function to pull element up a heap.
 */
void pull_up(struct edge* heap[], unsigned int n,  unsigned int i);

/*
 * Helper function to find smallest index among parent and child.
 */
int smallest(struct edge* heap[], unsigned int n, unsigned int i);

/*
 * Helper function to push element down a heap.
 */
void push_down(struct edge* heap[], unsigned int n,  unsigned int i);

/*
 * Helper function to push element onto heap.
 * Element is added at the end of the heap and pushed up.
 */
struct edge ** push_edge(struct edge* heap[], unsigned int last,
				struct edge* edge);

/*
 * Helper function to extract min element and rebalance tree
 */
struct edge* pull_min(struct edge* heap[], unsigned int n, 
						unsigned int last);
