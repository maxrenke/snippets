/**
 * \file map_route.c
 *
 * Plans a route in a roadmap.
 *
 * \author eaburns
 * \date 18-08-2010
 * \modified
 * \author mrenke
 * \date 07-11-2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "roadmap.h"
#include "system.h"
#include "heap.h"

/* Simple recursive function to print path */
void print_out(FILE * outfile, struct node* n){
	if( n == NULL) return;
	print_out(outfile, n->pi);
	fprintf(outfile, "%d\n",n->num+1);
}

/*
 * Single source shortest path solver using Djikstra's algorithm.
 * 'source' is the ID value of the source node (its node structure can
 * be found at nodes[source - 1]) and 'target' is the ID value of the
 * target node.  The 'costp' argument should be used to return the
 * final path cost value.
 *
 * Return 0 on success and 1 on failure.
 */
int djikstra(FILE * outfile, struct node nodes[], unsigned int nnodes,
	     unsigned int source, unsigned int target, unsigned int *costp)
{	
	struct node** Q = malloc(sizeof(struct node)*nnodes);
	struct node* u = malloc(sizeof(struct node*));
	unsigned int last = nnodes-1;
	unsigned int i = 0;
		
	/**Dijkstra's algorithm**/
	for( i=0; i<nnodes; i++){
		Q[i] = &nodes[i];
		nodes[i].d = UINT_MAX;
		nodes[i].q = i;
		nodes[i].pi = NULL;
	}
	nodes[source].d = 0;
	
	/*swap faster than pull_up*/
	swap(Q,source,0);
		
	while( u->num != target ){
		u = pull_min(Q,last--);
		for( i=0; i<u->narcs; i++){
			struct arc* a = &u->arcv[i];
			struct node* v = &nodes[a->target];
			if( v->d > u->d + a->wt ){
				v->d = u->d + a->wt;
				v->pi = u;
				pull_up(Q,v->q,v->q);
			}
		}
	}
	
	/* find and output path */
	u = &nodes[target];
	
	/* recursive call to print path */
	print_out(outfile,u);
		
	/* set 'costp' */
	(*costp) = u->d;
	
	/* cleanup */
	free(Q);
	
	/* success */
    return 0;
}

/* Read the user's input and call the search. */
static int input_and_search(FILE * infile, struct node nodes[],
			    unsigned int nnodes)
{
    int err = 0;
    unsigned int s, t;
    unsigned int cost = 0;
    double start, end;

    while (fscanf(infile, "%u %u", &s, &t) == 2) {
	s = s - 1; /* avoiding 1-indexing */
	t = t - 1;
	if (s >= nnodes) {
	    fprintf(stderr, "Start node is invalid\n");
	    continue;
	}
	if (t >= nnodes) {
	    fprintf(stderr, "Target node is invalid\n");
	    continue;
	}
	printf("finding a route from %d to %d\n", s, t);
	start = get_current_seconds();
	err = djikstra(stdout, nodes, nnodes, s, t, &cost);
	end = get_current_seconds();
	if (err)
	    break;
	printf("cost: %u\n", cost);
	printf("time: %f seconds\n", end - start);
    }

    return err;
}


/* Print the usage string. */
static void usage(void)
{
    fprintf(stderr, "Usage:\nmap-route <datafile> <file>\n");
    exit(EXIT_FAILURE);
}


int main(int argc, const char *const argv[])
{
    int err, ret = EXIT_FAILURE;
    FILE *f, *infile = stdin;
    double start, end;
    unsigned int nnodes;
    struct node *nodes;

    if (argc != 3)
	usage();

    f = fopen(argv[1], "r");
    if (!f) {
	perror("Error opening data file");
	return EXIT_FAILURE;
    }
    if (strcmp(argv[2], "-") != 0) {
	infile = fopen(argv[2], "r");
	if (!infile) {
	    perror("Error opening data file");
	    fclose(f);
	    return EXIT_FAILURE;
	}
    }

    start = get_current_seconds();
    err = load_map(f, &nodes, &nnodes);
    if (err)
	goto out_map;
    end = get_current_seconds();
    fclose(f);
    if (err)
	return EXIT_FAILURE;
    printf("Loaded %d nodes in %f seconds\n", nnodes, end - start);
    printf("Using %d MB\n", peak_memory_usage());

    err = input_and_search(infile, nodes, nnodes);
    if (err)
	goto out;

    printf("Peak memory usage %d MB\n", peak_memory_usage());

    ret = EXIT_SUCCESS;
  out:
    free_map(nodes, nnodes);
  out_map:
    if (infile != stderr)
	fclose(infile);
    return ret;
}
