#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b)
{
    return (*(int*)a - *(int*)b);
}

int find_unique_sorted(int arr[], int n)
{
    qsort(arr, n, sizeof(int), cmp);

    for(int i = 0; i < n - 1; i += 2)
    {
        if(arr[i] != arr[i+1])
            return arr[i];
    }

    return arr[n-1];   // if unique is last element
}

int main()
{
    int arr[] = {1,4,1,5,4,8,8,9,5,9,3};
    int n = sizeof(arr)/sizeof(arr[0]);

    int result = find_unique_sorted(arr, n);
    printf("Unique element: %d\n", result);

    return 0;
}

