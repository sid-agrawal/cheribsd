// From:
// https://www.geeksforgeeks.org/construct-complete-binary-tree-given-array/
// CPP program to construct binary of a given depth 
// Traverse: in order
//#include <bits/stdc++.h>
#include <iostream>
#include <chrono>
#include <sys/sysctl.h>
#include <assert.h>

#include "stats.h"
using namespace std;

int node_count = 0;
int cycle_per_node;

#define PAGE_SIZE 4096

/* A binary tree node has data_int, and data_char
pointer to left child and a
pointer to right child */
struct Node
{
	int data_int;
	char data_char[PAGE_SIZE - sizeof(Node *) * 2 - sizeof(int)];
	Node* left, * right;
};

/* Helper function that allocates a
new node */
Node* newNode(int data_int)
{
	Node* node = (Node*)malloc(sizeof(Node));
        if (node == NULL){
                cout <<  "malloc failed\n";
                exit(1);
        }
        //cout << "Allocating: " << sizeof(Node) << "Bytes";
	node->data_int = data_int;
	node->left = node->right = NULL;
	return (node);
}

// Function to insert nodes in level order
Node* insertLevelOrder(int i, int n)
{
	Node *root = nullptr;
	// Base case for recursion
	if (i < n)
	{
		root = newNode(node_count);
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
void inOrder(Node* root)
{ 
        /* Volatile so that it doesn't get optimized out */
        volatile int busy_work;
	if (root != NULL)
	{
                for (int i = 0; i < cycle_per_node; i++) {
                        busy_work++;
                }
		inOrder(root->left);
		//cout << "data: " << root->data_int <<"\n ";
		inOrder(root->right);
	}
}

//  main 
int main(int argc,char* argv[])
{
        //struct run_stat rv;
        //printRunStats(&rv);
        
        if (argc != 3){
                cout << "Usage: tree <depth> <cyclesPerNode>\n";
                exit(1);
        }

        /* Get Depth */
        int depth = atoi(argv[1]); 

        /* Get cycle count */
        cycle_per_node = atoi(argv[2]);

        /* Calculate Total Number of Nodes*/
        long n = 1ULL << (depth-1);

        /* Print Summary */
        cout << "Depth: " << depth << "\n";  
        cout << "Delay: " << cycle_per_node << " loop \n";  
        cout << "Num Nodes: " << n << "\n";  
        cout << "Approx Memory consumption: " << PAGE_SIZE*n/1024/1024 << " MB\n";  

        std::chrono::steady_clock::time_point begin,end;
        begin = std::chrono::steady_clock::now();

        //cout << "START of Construction\n";
        begin = std::chrono::steady_clock::now();
        Node* root = insertLevelOrder(0, n);
        end = std::chrono::steady_clock::now();
        cout << "Construction: " << 
                std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() 
                << "[s]" << std::endl;

        //cout << "START of Traversal\n";
        begin = std::chrono::steady_clock::now();
        inOrder(root);
        end = std::chrono::steady_clock::now();
        cout << "Traversal: " << 
                std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() 
                << "[s]" << std::endl;
}

