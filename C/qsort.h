/*
 * Swap
 * Swap two elements in an array based on inex.
 */
void swap( struct edge* ary[],
				  int i, int j);

/*
 * Partition
 * Swap elements on pivot. Return middle index.
 */
int partition( struct edge* ary[],
				  int left, int right);

/*
 * Quicksort Helper
 * Helper method to recurse through array.
 */
void quicksort_help( struct edge* *ary,
				  int left, int right);

/*
 * Quicksort
 * Return 0 on success and 1 on failure.
 */
unsigned long do_quicksort(struct edge* ary[],
				  unsigned long n);