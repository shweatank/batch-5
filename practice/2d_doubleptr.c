#include <stdio.h>
#include <stdlib.h>

int main()
{
    int rows, cols, i, j;

    printf("Enter rows and columns: ");
    scanf("%d %d", &rows, &cols);

    /* allocate array of row pointers */
    int **arr = (int **)malloc(rows * sizeof(int *));

    /* allocate each row */
    for (i = 0; i < rows; i++)
        arr[i] = (int *)malloc(cols * sizeof(int));

    /* scan */
    printf("Enter elements:\n");
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            scanf("%d", &arr[i][j]);

    /* print */
    printf("\n2D Array:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++)
            printf("%4d", arr[i][j]);
        printf("\n");
    }

    /* free */
    for (i = 0; i < rows; i++)
        free(arr[i]);
    free(arr);

    return 0;
}
