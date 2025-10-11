#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct StudentDetails
{
    int rollNo;
    char name[50];
    int Marks1, Marks2, Marks3;
};

void getRollNumbers(int index, int n)
{
    if (index > n)
    {
        return;
    }
    printf("%d", index);
    getRollNumbers(index + 1, n); // for recursion
}

int main()
{
    int n;
    printf("Number of student : ");
    scanf("%d", &n);

    struct StudentDetails student[n];

    for (int index = 0; index < n; index++)
    {
        printf("Roll no., Name , Marks1 Marks2 Marks3: ");
        scanf("%d %s %d %d %d", &student[index].rollNo, student[index].name, &student[index].Marks1, &student[index].Marks2, &student[index].Marks3);
    }

    for (int index = 0; index < n; index++)
    {
        int total = student[index].Marks1 + student[index].Marks2 + student[index].Marks3;

        float average = total / 3.0;

        char grade;
        int star = 0;

        if (average >= 85)
        {
            grade = 'A';
            star = 5;
        }
        else if (average >= 70)
        {
            grade = 'B';
            star = 4;
        }
        else if (average >= 50)
        {
            grade = 'C';
            star = 3;
        }
        else if (average >= 35)
        {
            grade = 'D';
            star = 2;
        }
        else
        {
            grade = 'F';
            star = 0;
        }

        printf("RollNumber:%d\n", student[index].rollNo);
        printf("Name:%s\n", student[index].name);
        printf("Total:%d\n", total);
        printf("Average:%.2f\n", average);
        printf("Grade: %c\n", grade);
        if (star > 0)
        {
            printf("Performance: ");
            for (int j = 0; j < star; j++)
            {
                printf("*");
            }
            printf("\n");
        }
    }

    printf("List of Roll Numbers (via recursion): \t ");
    getRollNumbers(1, n);
    printf("\n");

    return 0;
}
