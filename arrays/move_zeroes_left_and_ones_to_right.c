#include<stdio.h>

int main()
{
    int n;

    // Read number of elements
    printf("Enter the n value:");
    scanf("%d",&n);

    int a[n];

    // Read array elements (expected 0s and 1s)
    printf("Enter the elements:");
    for(int i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }

    // Initialize two pointers
    // i -> start of array
    // j -> end of array
    int i=0;
    int j=n-1;

    // Goal: Move all 0s to left and 1s to right
    while(i<j)
    {
        // If left side has 1
        if(a[i]==1)
        {
            // If right side has 0, swap
            if(a[j]==0)
            {
                int temp=a[i];
                a[i]=a[j];
                a[j]=temp;

                // Move both pointers after swap
                i++;
                j--;
            }
            else
            {
                // If right is also 1, just move j
                j--;
            }
        }
        else
        {
            // If left is already 0, move i
            i++;
        }
    }

    // Print result
    printf("After moving zeros to left:\n");
    for(int i=0;i<n;i++)
    {
        printf("%d ",a[i]);
    }

    return 0;
}

