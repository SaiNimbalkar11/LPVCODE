#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;
using namespace std :: chrono;

void bubbleSortSequential(vector<int>& arr) 
{
	int n = arr.size();
	for (int i = 0; i < n - 1; i++) {
		bool swapped = false;
		for (int j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(arr[j], arr[j + 1]);
				swapped = true;
			}
		}
		if (!swapped) break;
	}
}

void bubbleSortParallel(vector<int>& arr) {
	int n = arr.size();
	for (int i = 0; i < n; i++) {
		#pragma omp parallel for
		for (int j = 0; j < n - 1; j += 2) {
			if (arr[j] > arr[j + 1])
				swap(arr[j], arr[j + 1]);
		}
		#pragma omp parallel for
		for (int j = 1; j < n - 1; j += 2) {
			if (arr[j] > arr[j + 1])
				swap(arr[j], arr[j + 1]);
		}
	}
}

int main() {
	int N;
	cout << "Enter the size of the vector: ";
	cin >> N;
	vector<int> arr(N), original(N);
	cout << "Enter the elements of the vector (space-separated): ";
	for (int i = 0; i < N; i++) {
		cin >> arr[i];
		original[i] = arr[i];
	}

	auto start = high_resolution_clock::now();
	bubbleSortSequential(arr);
	auto end = high_resolution_clock::now();
	double bubbleSortSequentialTime = duration<double, nano>(end - start).count();
	
	cout<<"\nSorted elements: \n";
	for (int i = 0; i < arr.size(); i++) 
	{
        cout << arr[i] << " ";
    }
	cout << "\nSequential Bubble Sort Time: "<< bubbleSortSequentialTime << '\n';

	arr = original;
	start = high_resolution_clock::now();
	bubbleSortParallel(arr);
	end = high_resolution_clock::now();
	double bubbleSortParallelTime = duration<double,nano>(end - start).count();
	
	cout<<"\n\nSorted elements: \n";
	for (int i = 0; i < arr.size(); i++) 
	{
        cout << arr[i] << " ";
    }
	cout << "\nParallel Bubble Sort Time: "<< bubbleSortParallelTime << '\n';
	cout <<"Speedup Factor: "<< bubbleSortSequentialTime/bubbleSortParallelTime;

	return 0;
}

