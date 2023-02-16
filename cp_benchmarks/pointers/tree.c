// From:
// https://www.geeksforgeeks.org/construct-complete-binary-tree-given-array/
#include <sys/sysctl.h>
#include <sys/time.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "stats.h"

int node_count = 0;
int cycle_per_node;

#define PAGE_SIZE 4096

/* A binary tree node has data_int, and data_char
pointer to left child and a
pointer to right child */
struct Node
{
	int data_int;
	char data_char[PAGE_SIZE - sizeof(struct Node *) * 2 - sizeof(int)];
	struct Node* left, * right;
};

/* Helper function that allocates a
new node */
struct Node* newNode(int data_int)
{
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
        if (node == NULL){
                exit(1);
        }
	node->data_int = data_int;
	node->left = node->right = NULL;
	return (node);
}

// Function to insert nodes in level order
struct Node* insertLevelOrder(int i, int n)
{
	struct Node *root = NULL;
	// Base case for recursion
	if (i < n)
	{ root = newNode(node_count);
                node_count++;
		
		// insert left child
		root->left = insertLevelOrder(
				2 * i + 1, n);

		// insert right child
		root->right = insertLevelOrder(
				2 * i + 2, n);
	}
	return root;
}

// Function to print tree nodes in
// InOrder fashion
void inOrder(struct Node* root)
{ 
        /* Volatile so that it doesn't get optimized out */
        volatile int busy_work;
	if (root != NULL)
	{
                for (int i = 0; i < cycle_per_node; i++) {
                        busy_work++;
                }
		inOrder(root->left);
		inOrder(root->right);
	}
}

//  main 
int main(int argc,char* argv[])
{
        if (argc != 3){
                printf("Usage: tree <depth> <cyclesPerNode>\n");
                exit(1);
        }

        /* Get Depth */
        int depth = atoi(argv[1]); 

        /* Get cycle count */
        cycle_per_node = atoi(argv[2]);

        /* Calculate Total Number of Nodes*/
        long n = 1ULL << (depth-1);


        struct timeval CBegin, CEnd, TBegin, TEnd;

        /* Make the Tree */
        gettimeofday(&CBegin, 0);
        struct Node* root = insertLevelOrder(0, n);
        gettimeofday(&CEnd, 0);
        long CDur = CEnd.tv_sec - CBegin.tv_sec;
              

        /* Traverse the Tree inorder */
        clearCounters();
        gettimeofday(&TBegin, 0);
        inOrder(root);
        gettimeofday(&TEnd, 0);
        long TDur = TEnd.tv_sec - TBegin.tv_sec;

        /* Print Summary */
        printf("{");
        printf("\"Test\": \"%s\",", "tree");
        printf("\"CPEnabled\": %d,", sysctlRead("vm.v_cheri_prefetch"));
        printf("\"Depth\": %d,", depth);
        printf("\"Delay\": %d,", cycle_per_node);
        printf("\"NumNodes\": %ld,", n);
        printf("\"ApproxMemoryConsumptionMB\": %ld,", PAGE_SIZE*n/1024/1024);
        printCounters();
        printf("\"ConstructionSecond\": %ld,", CDur); 
        printf("\"TraversalSecond\": %ld", TDur); 
        printf("}");
}

