#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node {
	int arr[1020];
	struct node *next; 
};

#define num_nodes 786433
struct node *ll[num_nodes];
struct node *ll_head, *ll_curr;

int main() {
	sleep(10);
	void *temp;	
	for(int i = 0; i < num_nodes; i++) {
		//if(i % 100000) 
		//	printf("Something\n");
		ll[i] = malloc(sizeof(struct node));
		
	}
	printf("Running\n"); 
	// ll = (struct node *) malloc(sizeof(malloc(sizeof(struct node) * num_nodes)));
	FILE* ptr; 
	char ch; 
	ptr = fopen("random_array", "r");

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

	} while(ch != EOF);
	
	printf("LL constructed\n");
	ll_curr->next = ll[num];
	ll_curr = ll[num];
	ll_curr->next = NULL;
	count = 0;
	ll_curr = ll_head;

	while(ll_curr != NULL) {
		ll_curr = ll_curr->next;
		count++;
	}

	printf("Count is %d\n", count); 
	return 0;	

}
