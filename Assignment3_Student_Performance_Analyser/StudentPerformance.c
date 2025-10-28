#include <stdio.h>

typedef struct
{
    int rollNo;
    char name[50];
    int marks[3];
} StudentDetails;

void getRollNumbers(StudentDetails student[], int n)
{
    printf("List of Roll Numbers:\t");
    for (int index = 0; index < n; index++)
    {
        printf("%d ", student[index].rollNo);
    }
    printf("\n");
}

typedef struct
{
    char grade;
    int star;
} StudentGrades;

StudentGrades getGrade(float average)
{
    StudentGrades grades;
    if (average >= 85)
    {
        grades.grade = 'A';
        grades.star = 5;
    }
    else if (average >= 70)
    {
        grades.grade = 'B';
        grades.star = 4;
    }
    else if (average >= 50)
    {
        grades.grade = 'C';
        grades.star = 3;
    }
    else if (average >= 35)
    {
        grades.grade = 'D';
        grades.star = 2;
    }
    else
    {
        grades.grade = 'F';
        grades.star = 0;
    }
    return grades;
}

int main()
{
    int n;
    printf("Number of student : ");
    scanf("%d", &n);

    StudentDetails student[n];

    for (int index = 0; index < n; index++)
    {
        printf("Roll no., Name , Marks1 Marks2 Marks3: ");
        scanf("%d", &student[index].rollNo);
        scanf("%s", student[index].name);

        for (int j = 0; j < 3; j++)
        {
            do
            {
                printf("Enter marks %d (0 to 100): ", j + 1);
                scanf("%d", &student[index].marks[j]);

                if (student[index].marks[j] < 0 || student[index].marks[j] > 100)
                {
                    printf("Invalid marks entered , please try between 0 and 100. \n");
                }
            } while (student[index].marks[j] < 0 || student[index].marks[j] > 100);
        }
    }

    for (int index = 0; index < n; index++)
    {
        int total = 0;
        for (int j = 0; j < 3; j++)
        {
            total += student[index].marks[j];
        }

        float average = total / 3.0;

        char grade;
        int star = 0;

        StudentGrades grades = getGrade(average);
        grade = grades.grade;
        star = grades.star;

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

    getRollNumbers(student, n);

    return 0;
}
