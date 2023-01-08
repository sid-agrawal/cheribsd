#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
struct node {
	int arr[1020];
	struct node *next; 
}__attribute__((aligned (4096)));

#define num_nodes 1310721
struct node *ll[num_nodes];
struct node *ll_head, *ll_curr;

int main() {
	void *temp;	
	clock_t start = clock();
	for(int i = 0; i < num_nodes; i++) {
		//if(i % 100000) 
		//	printf("Something\n");
		ll[i] = malloc(sizeof(struct node));
		
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

	int first = 1;
	char * str = (char *)malloc(sizeof(char) * 15);
	int count = 0;
	int num;
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
	
	printf("LL constructed\n");
	ll_curr->next = ll[num];
	ll_curr = ll[num];
	ll_curr->next = NULL;
	count = 0;
	ll_curr = ll_head;

	unsigned long counter = 0;
	while(ll_curr != NULL) {
		ll_curr = ll_curr->next;
		count++;
		if(ll_curr != NULL) {
			for (int i = 0; i < 1000; i++){
				counter++;
				//ll_curr->arr[i] = count;
			}
		} 
	}

	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("Count is %d and time taken is %fs, start is %lu\n", count, seconds, counter); 
	return 0;	

}
