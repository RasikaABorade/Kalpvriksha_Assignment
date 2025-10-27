#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **originalMatrix(int n)
{
    int **matrix = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        *(matrix + i) = (int *)malloc(n * sizeof(int));
    }
    return matrix;
}

void randomMatrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(*(matrix + i) + j) = rand() % 256;
        }
    }
}

void printMatrix(int **matrix, int n)
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

    for (int i = 0; i < n; i++)
    {
        int *left = *(matrix + i);
        int *right = *(matrix + i) + (n - 1);

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

    int **temp = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        *(temp + i) = (int *)malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int sum = 0, count = 0;

            for (int neighbourOfI = i - 1; neighbourOfI <= i + 1; neighbourOfI++)
            {
                for (int neighbourOfJ = j - 1; neighbourOfJ <= j + 1; neighbourOfJ++)
                {
                    if (neighbourOfI >= 0 && neighbourOfI < n && neighbourOfJ >= 0 && neighbourOfJ < n)
                    {
                        sum += *(*(matrix + neighbourOfI) + neighbourOfJ);
                        count++;
                    }
                }
            }

            *(*(temp + i) + j) = sum / count;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(*(matrix + i) + j) = *(*(temp + i) + j);
        }
    }

    for (int i = 0; i < n; i++)
    {
        free(*(temp + i));
    }
    free(temp);
}

int main()
{
    int n;
    srand(time(NULL) + clock());
    do
    {
        printf("Enter the size of the matrix (between 2 and 10): ");
        scanf("%d", &n);

        if (n < 2 || n > 10)
        {
            printf("Invalid size. Please enter a valid size between 2 and 10.\n");
        }
    } while (n < 2 || n > 10);

    int **matrix = originalMatrix(n);
    randomMatrix(matrix, n);

    printf("The original matrix is: \n");
    printMatrix(matrix, n);

    rotatedMatrix(matrix, n);

    printf("90 degree rotated matrix is: \n");
    printMatrix(matrix, n);

    smoothing(matrix, n);

    printf("The value after smoothing is: \n");
    printMatrix(matrix, n);

    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
