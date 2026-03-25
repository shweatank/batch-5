#include <stdio.h>

int main()
{
    int n, j;

    // Read number of elements
    printf("Enter the n value: ");
    scanf("%d", &n);

    int a[n];

    // Read array elements
    for(int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    // Initialize pointer to last index
    j = n - 1;

    /*
     * Step 1:
     * Traverse from right to left
     * Move all non-zero elements to the end of array
     * This preserves the relative order of non-zero elements
     */
    for(int i = n - 1; i >= 0; i--)
    {
        if(a[i] != 0)
        {
            a[j] = a[i];
            j--;
        }
    }

    /*
     * Step 2:
     * Fill remaining positions (left side) with zeros
     */
    while(j >= 0)
    {
        a[j] = 0;
        j--;
    }

    // Print result
    printf("After moving zeroes to left without changing order:\n");

    for(int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }

    return 0;
}
