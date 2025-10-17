#include <stdio.h>
#include <stdlib.h>

int **OriginalMatrix(int n)
{
    int **matrix = (int **)malloc(n * sizeof(int *)); // rows
    for (int i = 0; i < n; i++)
    {
        *(matrix + i) = (int *)malloc(n * sizeof(int *)); // columns
    }
    return matrix;
}

void RandomMatrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(*(matrix + i) + j) = rand() % 256;
        }
    }
}

void PrintMatrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d", *(*(matrix + i) + j));
            if (j < n - 1)
                printf(" ");
        }
        printf("\n");
    }
}

void rotatedMatrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            int *ptr1 = *(matrix + i) + j;
            int *ptr2 = *(matrix + j) + i;

            int temp = *ptr1;
            *ptr1 = *ptr2;
            *ptr2 = temp;
        }
    }

    // reverse
    for (int i = 0; i < n; i++)
    {
        int *left = *(matrix + i);            // start
        int *right = *(matrix + i) + (n - 1); // end of matrix

        while (left < right)
        {
            int temp = *left;
            *left = *right;
            *right = temp;
            left++;
            right--;
        }
    }
}

void smoothing(int **matrix, int n)
{
    int *temp = (int *)malloc(n * n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(temp + i * n + j) = *(*(matrix + i) + j);
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int sum = 0;
            int count = 0;

            for (int neighbourOfI = i - 1; neighbourOfI <= i + 1; neighbourOfI++)
            {
                for (int neighbourOfJ = j - 1; neighbourOfJ <= j + 1; neighbourOfJ++)
                {
                    if (neighbourOfI >= 0 && neighbourOfI < n && neighbourOfJ >= 0 && neighbourOfJ < n)
                    {
                        sum += *(temp + neighbourOfI * n + neighbourOfJ);
                        count++;
                    }
                }
            }
            int average = sum / count;
            *(*(matrix + i) + j) = average;
        }
    }
}

int main()
{
    int n;
    printf("Enter the size of the matrix: ");
    scanf("%d", &n);

    if (n > 2 || n < 10)
    {
        printf("Please enter a valid size between 2 and 10\n");
        return 1;
    }

    int **matrix = OriginalMatrix(n);
    RandomMatrix(matrix, n);

    printf("The original matrix is: \n");
    PrintMatrix(matrix, n);

    rotatedMatrix(matrix, n);

    printf("90 degree rotated matrix is: \n");
    PrintMatrix(matrix, n);

    smoothing(matrix, n);

    printf("The value after smoothing is: \n");
    PrintMatrix(matrix, n);

    return 0;
}