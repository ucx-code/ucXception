// Copied and adapted from https://www.geeksforgeeks.org/merge-sort/
/* C program for Merge Sort */
#include<stdlib.h> 
#include<stdio.h> 

// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(int arr[], int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 = r - m; 

	/* create temp arrays */
	int L[n1], R[n2]; 

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1+ j]; 

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 
	while (i < n1 && j < n2) 
	{ 
		if (L[i] <= R[j]) 
		{ 
			arr[k] = L[i]; 
			i++; 
		} 
		else
		{ 
			arr[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 

	/* Copy the remaining elements of L[], if there 
	are any */
	while (i < n1) 
	{ 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 

	/* Copy the remaining elements of R[], if there 
	are any */
	while (j < n2) 
	{ 
		arr[k] = R[j]; 
		j++; 
		k++; 
	} 
} 

/* l is for left index and r is right index of the 
sub-array of arr to be sorted */
void mergeSort(int arr[], long l, long r) 
{ 
	if (l < r) 
	{ 
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h 
		long m = l+(r-l)/2; 

		// Sort first and second halves 
		mergeSort(arr, l, m); 
		mergeSort(arr, m+1, r); 

		merge(arr, l, m, r); 
	} 
} 

/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size) 
{ 
	int i; 
	for (i=0; i < size; i++) 
		printf("%d ", A[i]); 
	printf("\n"); 
} 

/* Driver program to test above functions */
int main(int argc, char * argv[]) 
{ 
	if (argc != 4) {
		printf("Usage: mergesort <repeats> <array size> <verbose=[1|0]>\n");
		return -1;
	}
	int repeats;
	long arr_size;
	int verbose;

	sscanf(argv[1], "%d", &repeats);
	sscanf(argv[2], "%ld", &arr_size);
	sscanf(argv[3], "%d", &verbose);
	
	srand(1337); // fixed seed
	for (int j = 0; j < repeats; j++) {
		int * arr = malloc(sizeof(int) * arr_size);

		for (long i = 0; i < arr_size; i++)
		{
			arr[i] = rand();
		}

		if (verbose == 1) {
			printf("Given array is \n"); 
			printArray(arr, arr_size); 
		}

		mergeSort(arr, 0, arr_size - 1); 

		if (verbose == 1) {
			printf("\nSorted array is \n"); 
			printArray(arr, arr_size); 
		}
		free(arr);
	}
	return 0; 
} 
