// From:
// https://www.geeksforgeeks.org/construct-complete-binary-tree-given-array/
// CPP program to construct binary of a given depth 
// Traverse: in order
//#include <bits/stdc++.h>
#include <iostream>
#include <chrono>
#include <sys/sysctl.h>
#include <assert.h>
using namespace std;

int node_count = 0;
int cycle_per_node;

#define PAGE_SIZE 4096

/* Forward Defs */
void printRunStats();

void handleRV(int rv, 
                const char * name,
                int value, size_t len) {
        if (rv) {
                printf("%s V: %d Len: %zu\n", name, value, len);
                printf("error no: %d\n", errno);
                exit(1);
        }


}



void printStats() {
	int mib_soft[2], mib_major[2], mib_blocked[2], mib_prefetch[2], softfaults, majorfaults, blocked_faults, prefetches; 
	size_t mib_soft_len, mib_major_len, mib_blocked_len, mib_prefetch_len, len; 

	mib_soft_len = 2; 
	mib_major_len = 2;
	mib_blocked_len = 2;
	mib_prefetch_len = 2;
	
	sysctlnametomib("vm.v_softfault", mib_soft, &mib_soft_len);
	sysctlnametomib("vm.v_majorfault", mib_major, &mib_major_len);
	sysctlnametomib("vm.v_blocked_softfault", mib_blocked, &mib_blocked_len);
	sysctlnametomib("vm.v_prefetches", mib_prefetch, &mib_prefetch_len);
	
	len = sizeof(softfaults);
	int newp1 = 0, newp2 = 0; 
	sysctl(mib_soft, 2, &softfaults, &len, NULL, 0);
	sysctl(mib_major, 2, &majorfaults, &len, NULL, 0);
	sysctl(mib_blocked, 2, &blocked_faults, &len, NULL, 0);
	sysctl(mib_prefetch, 2, &prefetches, &len, NULL, 0);

	printf("Softfaults %d, majorfaults %d, Blocked faults %d, prefetches %d\n", softfaults, majorfaults, blocked_faults, prefetches);
}

void clearStats() {
	int mib_soft[2], mib_major[2], mib_blocked[2], mib_prefetch[2];
        int softfault, majorfault, blocked_fault, prefetch; 
	size_t mib_soft_len, mib_major_len, mib_blocked_len, mib_prefetch_len, len; 

        int rv;
	
	rv = sysctlnametomib("vm.v_softfault", mib_soft, &mib_soft_len);
        assert(rv==0);
	rv = sysctlnametomib("vm.v_majorfault", mib_major, &mib_major_len);
        assert(rv==0);
	rv = sysctlnametomib("vm.v_blocked_softfault", mib_blocked, &mib_blocked_len);
        assert(rv==0);
	rv = sysctlnametomib("vm.v_prefetches", mib_prefetch, &mib_prefetch_len);
        assert(rv==0);
	
	len = sizeof(softfault);
        int new_softfault=0;
	rv = sysctl(mib_soft, mib_soft_len, &softfault, &len, &new_softfault, len);
        handleRV(rv, "vm.v_softfault", softfault, len);

	len = sizeof(majorfault);
        int new_majorfault=0;
	rv = sysctl(mib_major, mib_major_len, &majorfault, &len, &new_majorfault, len);
        handleRV(rv, "vm.v_majorfault", majorfault, len);

        len = sizeof(blocked_fault);
        int new_blocked_fault=0;
	rv = sysctl(mib_blocked, mib_major_len, &blocked_fault, &len, &new_blocked_fault, len);
        handleRV(rv, "vm.v_blocked_softfault", blocked_fault, len);
	
        len = sizeof(prefetch);
        int new_prefetch=0;
	rv = sysctl(mib_prefetch, mib_prefetch_len, &prefetch, &len, &new_prefetch, len);
        handleRV(rv, "vm.v_prefetches", prefetch, len);

        printf("All stats cleared. ");
        printf("the stats below should be very close to zero\n");
        printStats();
}
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
        printRunStats();
        exit(1);
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
        cout << "Depth is: " << depth << "\n";  
        cout << "Cycle Per Node is: " << cycle_per_node << "\n";  
        cout << "Count is: " << n << "\n";  
        cout << "Approx Memory consumption: " << PAGE_SIZE*n/1024/1024 << " MB\n";  

        std::chrono::steady_clock::time_point begin,end;
        begin = std::chrono::steady_clock::now();

        cout << "START of Construction\n";
        begin = std::chrono::steady_clock::now();
        Node* root = insertLevelOrder(0, n);
        end = std::chrono::steady_clock::now();
        cout << "END   of Construction. Duration: " << 
                std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() 
                << "[s]" << std::endl;

        cout << "START of Traversal\n";
        begin = std::chrono::steady_clock::now();
        clearStats();
        inOrder(root);
        printStats();
        end = std::chrono::steady_clock::now();
        cout << "END   of Traversal. Duration: " << 
                std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() 
                << "[s]" << std::endl;
}


struct run_stat{
        char *name;
        bool cheri_picking_enabled;
        int cycle_per_node;
        int depth;
        struct counter{
                char *name;
                int value;
        } counters[5];
        int duration_second;
};

void printRunStats(struct run_stat *rs){

        rs->name = "bfs";
        rs->cheri_picking_enabled = true;
        rs->cycle_per_node = 1000;
        rs->depth = 10;
        rs->counters[0].name = "v_prefetches";
        rs->counters[0].value = 1098790;
        rs->counters[1].name = "v_majorfaults";
        rs->counters[1].value = 1098790;
        rs->duration_second = 14999;


        printf("%s\n", rs->name);
        printf("\t CHERI: %s \tCycle: %d \tDepth: %d \tDuration: %d\n",
                        rs->cheri_picking_enabled ? "TRUE" : "FALSE",
                        rs->cycle_per_node,
                        rs->depth,
                        rs->duration_second);
        for (int i = 0; i < 5; i++) {
                printf("%s \t %d\n", rs->counters[i].name, rs->counters[i].value);

        }
                        
}
