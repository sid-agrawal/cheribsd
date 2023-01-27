// From:
// https://www.geeksforgeeks.org/construct-complete-binary-tree-given-array/
// CPP program to construct binary
// tree from given array in level
// order fashion Tree Node
#include <bits/stdc++.h>
using namespace std;

/* A binary tree node has data_int, and data_char
pointer to left child and a
pointer to right child */
struct Node
{
	int data_int;
	char data_char[4096 - sizeof(Node *) * 2 - sizeof(int)];
	Node* left, * right;
};

/* Helper function that allocates a
new node */
Node* newNode(int data_int)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->data_int = data_int;
	node->left = node->right = NULL;
	return (node);
}

// Function to insert nodes in level order
Node* insertLevelOrder(int arr[], int i, int n)
{
	Node *root = nullptr;
	// Base case for recursion
	if (i < n)
	{
		root = newNode(arr[i]);
		
		// insert left child
		root->left = insertLevelOrder(arr,
				2 * i + 1, n);

		// insert right child
		root->right = insertLevelOrder(arr,
				2 * i + 2, n);
	}
	return root;
}

// Function to print tree nodes in
// InOrder fashion
void inOrder(Node* root)
{
	if (root != NULL)
	{
		inOrder(root->left);
		cout << root->data_int <<" ";
		inOrder(root->right);
	}
}

// Driver program to test above function
int main()
{
	int arr[] = { 1, 2, 3, 4, 5, 6, 6, 6, 6 };
	int n = sizeof(arr)/sizeof(arr[0]);
	Node* root = insertLevelOrder(arr, 0, n);
	inOrder(root);
}
