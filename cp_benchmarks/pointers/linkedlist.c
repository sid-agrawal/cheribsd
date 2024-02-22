#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

//#include "stats.h"
#include "shuffle.h"
#define PAGE_SIZE 4096
#define NODE_ARRAY_SIZE 1020
struct node {
	struct node *next; 
	int arr[NODE_ARRAY_SIZE];
}__attribute__((aligned (PAGE_SIZE)));

int main(int argc,char* argv[])
{
        if (argc != 4){
                printf("Usage: ll <log base 2 of num_nodes> <cyclesPerNode> <randomize traversal>\n");
                exit(1);
        }

        /* Get num_nodes */
        int num_nodes = atoi(argv[1]); 
       
	/* Set RSS limit to wss/2 */
	/* struct rlimit *constrain_memory = (struct rlimit *) malloc(sizeof(struct rlimit));
    	constrain_memory->rlim_cur = (1UL << (num_nodes - 1)) * sizeof(struct node);
    	constrain_memory->rlim_max = (1UL << (num_nodes - 1)) * sizeof(struct node);
   	setrlimit(RLIMIT_RSS, constrain_memory);

	if (constrain_memory->rlim_cur == 0)
		return 0; 
	*/
	/* Set total memory consumption */
	num_nodes = 1UL << num_nodes;
	
        
	/* Get cyclesPerNode */
        int cyclesPerNode = atoi(argv[2]); 
    
	int randomize_traversal = atoi(argv[3]);
        /* Allocate and initialize the LL */
        struct node **ll = (struct node **) malloc (sizeof(struct node *) * num_nodes);
        assert(ll != NULL);
	for(int i = 0; i < num_nodes; i++) {
		ll[i] = malloc(sizeof(struct node));
                assert(ll[i] != NULL);
		
	}

        /* Get the Random ordering */
        int *randArr = getShuffled( num_nodes , randomize_traversal);

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
	

	printf("Traversal\n");

    	/* Traverse the List */
    	gettimeofday(&begin, 0);
        //clearCounters();
	int a;
	// scanf("%d", &a);
	ll_curr->next = ll[num];
	ll_curr = ll[num];
	ll_curr->next = NULL;
	int count = 0;
	ll_curr = ll_head;

	while(ll_curr != NULL) {
		ll_curr = ll_curr->next;
		// printf("%p\n", ll_curr);
		count++;
		if(ll_curr != NULL) {
                        for (int i = 0; i < cyclesPerNode ; i++){
                                ll_curr->arr[i % NODE_ARRAY_SIZE] = count;
                        }
		} 
	}

	gettimeofday(&end, 0);
    	long TDur = end.tv_sec - begin.tv_sec;
	printf("Done\n");
        /* Print the stats 
        printf("{");
        printf("\"Test\": \"%s\",", "ll");
        printf("\"CPEnabled\": %d,", sysctlRead("vm.v_cheri_prefetch"));
        printf("\"NumNodes\": %d,", num_nodes);
        printf("\"Delay\": %d,", cyclesPerNode);
        printf("\"ApproxMemoryConsumptionMB\": %d,", 
                        PAGE_SIZE*num_nodes/1024/1024);
        printCounters();
        printf("\"ConstructionSecond\": %ld,", CDur); 
        printf("\"TraversalSecond\": %ld", TDur); 
        printf("}");
	printf("\n");*/
	return 0;	

}
