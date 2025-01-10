// ++ implementation of the approach
#include <bits/stdc++.h>
using namespace std;

// Function to return the next random number
int getNum(vector<int>& v)
{

	// Size of the vector
	int n = v.size();

	// Generate a random number
	srand(time(NULL));

	// Make sure the number is within
	// the index range
	int index = rand() % n;

	// Get random number from the vector
	int num = v[index];

	// Remove the number from the vector
	swap(v[index], v[n - 1]);
	v.pop_back();

	// Return the removed number
	return num;
}

// Function to generate n non-repeating random numbers
void generateRandom(int n)
{
	vector<int> v(n);

	// Fill the vector with the values
	// 1, 2, 3, ..., n
	for (int i = 0; i < n; i++)
		v[i] = i + 1;

	// While vector has elements
	// get a random number from the vector and print it
	while (v.size()) {
		cout << getNum(v) << ",";
	}
}

// Driver code
int main()
{
	int n = 8650752;
	generateRandom(n);

	return 0;
}
