#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;

// Merge two sorted parts
void merge(vector<int>& a, int low, int mid, int high)
{
    vector<int> temp;
    int i = low, j = mid + 1;

    while (i <= mid && j <= high)
    {
        if (a[i] < a[j])
            temp.push_back(a[i++]);
        else
            temp.push_back(a[j++]);
    }

    while (i <= mid) temp.push_back(a[i++]);
    while (j <= high) temp.push_back(a[j++]);

    for (int k = low; k <= high; k++)
        a[k] = temp[k - low];
}

// Sequential Merge Sort
void mergeSortSeq(vector<int>& a, int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;

        mergeSortSeq(a, low, mid);
        mergeSortSeq(a, mid + 1, high);

        merge(a, low, mid, high);
    }
}

// Parallel Merge Sort
void mergeSortPar(vector<int>& a, int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;

        #pragma omp task shared(a)
        mergeSortPar(a, low, mid);

        #pragma omp task shared(a)
        mergeSortPar(a, mid + 1, high);

        #pragma omp taskwait

        merge(a, low, mid, high);
    }
}

int main()
{
    int n;
    cout << "Enter size: ";
    cin >> n;

    vector<int> arr(n), temp(n);

//    cout << "Enter elements: ";
    for (int i = 0; i < n; i++)
//        cin >> arr[i];
		arr[i] = rand()%1000;

    temp = arr;

    // Sequential Time
    auto start = chrono::high_resolution_clock::now();
    mergeSortSeq(arr, 0, n - 1);
    auto end = chrono::high_resolution_clock::now();

	cout << "Sequential Merge Sort Time (ms): "
	 << chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;

    // Parallel Time
    start = chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        #pragma omp single
        mergeSortPar(temp, 0, n - 1);
    }

    end = chrono::high_resolution_clock::now();

    cout << "Parallel Merge Sort Time (ms): "
 	<< chrono::duration_cast<chrono::microseconds>(end - start).count()<< endl;

    return 0;
}
