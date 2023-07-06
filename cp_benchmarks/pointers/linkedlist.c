#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysctl.h>
struct node {
	struct node *next; 
	int arr[1020];
}__attribute__((aligned (4096)));

#define num_nodes 51201
// #define num_nodes 1310721
//#define num_nodes 524288
struct node *ll[num_nodes];
struct node *ll_head, *ll_curr;

#include "stats.h"
#include "shuffle.h"

#define NODE_ARRAY_SIZE 1020
struct node {
	struct node *next; 
	int arr[NODE_ARRAY_SIZE];
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
	printf("Running\n"); 
	// ll = (struct node *) malloc(sizeof(malloc(sizeof(struct node) * num_nodes)));
	FILE* ptr; 
	char ch; 
	ptr = fopen("random_array_5G", "r");

	if(ptr == NULL) {
		printf("File not found\n");
		return 0;
	}
        /* Get the Random ordering */
        int *randArr = getShuffled( num_nodes );

        /* Shuffle the List */
	int first = 1;
        int num  = 0;
	struct timeval begin, end;
    	gettimeofday(&begin, 0);
	printf("Constructing LL\n");
	// Read from file one by one to make LL.
	do {
		ch = fgetc(ptr);
		if(ch == ',') {
			// Make num.
			str[count] = '\0';
			num = atoi(str);
			count = 0;
			// printf("Num is %d\n", num);
			if(first) {
				first = 0;
				memset(ll[num], 0, 4096);
				ll_head = ll[num];
				ll_curr = ll_head;
			} else {
				memset(ll[num], 0, 4096);
				ll_curr->next = ll[num];
				ll_curr = ll_curr->next;
			}
		} else {
			str[count++] = ch; 
		}

	} while(ch != '\n');
	gettimeofday(&end, 0);
    	long seconds = end.tv_sec - begin.tv_sec;
	printf("Linkedlist construction time %ld seconds\n", seconds);
	
	int mib_soft[2], mib_major[2], mib_blocked[2], mib_prefetch[2], softfaults, majorfaults, blocked_faults, prefetches; 
	size_t mib_soft_len, mib_major_len, mib_blocked_len, mib_prefetch_len, len; 
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
	

	printf("Softfaults %d, majorfaults %d, Blocked faults %d, prefetches %d\n", softfaults, majorfaults, blocked_faults, prefetches);


    	// Start measuring trial time
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
    	int final_softfaults, final_majorfaults, final_blocked_faults, final_prefetches;
	mib_soft_len = 2; 
	mib_major_len = 2;
	mib_blocked_len = 2;
	mib_prefetch_len = 2;
	
	sysctlnametomib("vm.v_softfault", mib_soft, &mib_soft_len);
	sysctlnametomib("vm.v_majorfault", mib_major, &mib_major_len);
	sysctlnametomib("vm.v_blocked_softfault", mib_blocked, &mib_blocked_len);
	sysctlnametomib("vm.v_prefetches", mib_prefetch, &mib_prefetch_len);
	
	len = sizeof(softfaults);
	newp1 = 0, newp2 = 0; 
	sysctl(mib_soft, 2, &final_softfaults, &len, NULL, 0);
	sysctl(mib_major, 2, &final_majorfaults, &len, NULL, 0);
	sysctl(mib_blocked, 2, &final_blocked_faults, &len, NULL, 0);
	sysctl(mib_prefetch, 2, &final_prefetches, &len, NULL, 0);

	printf("Softfaults %d, majorfaults %d, Blocked faults %d, prefetches %d\n", final_softfaults - softfaults, final_majorfaults - majorfaults, final_blocked_faults - blocked_faults, final_prefetches - prefetches);


	printf("Count is %d, start is %lu\n", count, counter); 

	return 0;	

}
