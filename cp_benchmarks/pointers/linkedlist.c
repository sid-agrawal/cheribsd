#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysctl.h>

#include "stats.h"
#include "shuffle.h"

#define NODE_ARRAY_SIZE 1020
struct node {
	struct node *next; 
	int arr[node_array_size];
}__attribute__((aligned (PAGE_SIZE)));

int main(int argc,char* argv[])
{
        if (argc != 3){
                printf("Usage: ll <log base 2 of num_nodes> <cyclesPerNode> \n");
                exit(1);
        }

        /* Get num_nodes */
        int num_nodes = atoi(argv[1]); 
        num_nodes = 1UL << num_nodes;

        /* Get cyclesPerNode */
        int cyclesPerNode = atoi(argv[2]); 
    	
        /* Allocate and initialize the LL */
        struct node **ll = (struct node **) malloc (sizeof(struct node *) * num_nodes);
        assert(ll != NULL);
	for(int i = 0; i < num_nodes; i++) {
		ll[i] = malloc(sizeof(struct node));
                assert(ll[i] != NULL);
		
	}

        /* Get the Random ordering */
        int *randArr = getShuffled( num_nodes );

        /* Shuffle the List */
	int first = 1;
        int num  = 0;
	struct timeval begin, end;
    	gettimeofday(&begin, 0);

        struct node *ll_head, *ll_curr;
        for (int i = 0; i < num_nodes; i++ ) {

                num = randArr[i];
                if(first) {
                        first = 0;
                        memset(ll[num], 0, PAGE_SIZE);
                        ll_head = ll[num];
                        ll_curr = ll_head;
                } else {
                        memset(ll[num], 0, PAGE_SIZE);
                        ll_curr->next = ll[num];
                        ll_curr = ll_curr->next;
                }

        }
	gettimeofday(&end, 0);
    	long CDur = end.tv_sec - begin.tv_sec;
        free(randArr);
	


    	/* Traverse the List */
    	gettimeofday(&begin, 0);
        clearCounters();

	ll_curr->next = ll[num];
	ll_curr = ll[num];
	ll_curr->next = NULL;
	int count = 0;
	ll_curr = ll_head;

	while(ll_curr != NULL) {
		ll_curr = ll_curr->next;
		count++;
		if(ll_curr != NULL) {
                        for (int i = 0; i < cyclesPerNode ; i++){
                                ll_curr->arr[i % NODE_ARRAY_SIZE] = count;
                        }
		} 
	}

	gettimeofday(&end, 0);
    	long TDur = end.tv_sec - begin.tv_sec;

        /* Print the stats */
        printf("{");
        printf("\"Test\": \"%s\",", "tree");
        printf("\"NumNodes\": %d,", num_nodes);
        printf("\"Delay\": %d,", cyclesPerNode);
        printf("\"ApproxMemoryConsumptionMB\": %d,", 
                        PAGE_SIZE*num_nodes/1024/1024);
        printCounters();
        printf("\"ConstructionSecond\": %ld,", CDur); 
        printf("\"TraversalSecond\": %ld", TDur); 
        printf("}");
	return 0;	

}
