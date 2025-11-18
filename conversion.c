
#include <stdio.h>
#include <stdlib.h>

#define CLEAR_CMD "clear"

void calculateGPA(void);
void calculateCGPA(void);
void method(void);
void flush_input(void);

void sgpa()
{
    int input;

    while (1)
    {
        system(CLEAR_CMD);
        printf("--------------------------------------------------------------------------\n");
        printf("                    GPA & CGPA Calculator (Developed by DVSN)             \n");
        printf("--------------------------------------------------------------------------\n\n");
        printf("            MENU:\n");
        printf("                   1. Calculate GPA (Grade Point Average)\n");
        printf("                   2. Calculate CGPA (Cummulative Grade Point Average)\n");
        printf("                   3. Method that is applied here for calclating GPA & CGPA\n");
        printf("                   4. Exit Application\n");
        printf("--------------------------------------------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &input) != 1)
        {
            flush_input();
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (input)
        {
        case 1:
            calculateGPA();
            break;
        case 2:
            calculateCGPA();
            break;
        case 3:
            method();
            break;
        case 4:
            exit(EXIT_SUCCESS);
        default:
            printf("You have entered wrong input. Try again!\n");
            break;
        }
    }

}

void calculateGPA(void)
{
    int q;
    system(CLEAR_CMD);
    printf("-------------- GPA Calculating -----------------\n");
    printf("How many subject's points do you want to calculate? : ");

    if (scanf("%d", &q) != 1 || q <= 0)
    {
        flush_input();
        printf("Invalid number of subjects. Returning to menu.\n");
        return;
    }

    float *credit = (float *)malloc(q * sizeof(float));
    float *point = (float *)malloc(q * sizeof(float));
    if (!credit || !point)
    {
        printf("Memory allocation failed.\n");
        free(credit);
        free(point);
        return;
    }

    for (int i = 0; i < q; ++i)
    {
        printf("\nEnter the credit for the subject %d: ", i + 1);
        if (scanf("%f", &credit[i]) != 1)
        {
            flush_input();
            credit[i] = 0.0f;
        }
        printf("Enter the point of the subject %d: ", i + 1);
        if (scanf("%f", &point[i]) != 1)
        {
            flush_input();
            point[i] = 0.0f;
        }
        printf("-----------------------------------\n\n");
    }

    float sum = 0.0f;
    for (int j = 0; j < q; ++j)
        sum += credit[j] * point[j];

    float totCr = 0.0f;
    for (int k = 0; k < q; ++k)
        totCr += credit[k];

    if (totCr == 0.0f)
        printf("\n\nTotal Points: %.2f . Total Credits: %.2f . Total GPA: undefined (total credits is zero).\n", sum, totCr);
    else
        printf("\n\nTotal Points: %.2f . Total Credits: %.2f . Total GPA: %.2f .\n", sum, totCr, sum / totCr);

    free(credit);
    free(point);

    while (1)
    {
        int inmenu;
        printf("\n\n1. Calculate Again\n2. Go Back to Main Menu\n3. Exit This App\n\nYour Input: ");
        if (scanf("%d", &inmenu) != 1)
        {
            flush_input();
            printf("Invalid input. Please choose again!\n");
            continue;
        }

        switch (inmenu)
        {
        case 1:
            calculateGPA();
            return;
        case 2:
            return;
        case 3:
            exit(EXIT_SUCCESS);
        default:
            printf("You have Entered Wrong Input! Please Choose Again!\n");
            break;
        }
    }
}

void calculateCGPA(void)
{
    system(CLEAR_CMD);
    int l;
    printf("-------------- CGPA Calculating -----------------\n\n");
    printf("How many semester results do you want input? : ");

    if (scanf("%d", &l) != 1 || l <= 0)
    {
        flush_input();
        printf("Invalid number of semesters. Returning to menu.\n");
        return;
    }

    float *semrs = (float *)malloc(l * sizeof(float));
    if (!semrs)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < l; ++i)
    {
        printf(" Enter Semester %d Result(GPA): ", i + 1);
        if (scanf("%f", &semrs[i]) != 1)
        {
            flush_input();
            semrs[i] = 0.0f;
        }
        printf("\n");
    }

    float semtot = 0.0f;
    for (int j = 0; j < l; ++j)
        semtot += semrs[j];

    printf("******** Your CGPA is %.2f **********\n", semtot / l);

    free(semrs);

    while (1)
    {
        int inmenu;
        printf("\n\n1. Calculate Again\n2. Go Back to Main Menu\n3. Exit This App\n\nYour Input: ");
        if (scanf("%d", &inmenu) != 1)
        {
            flush_input();
            printf("Invalid input. Please choose again!\n");
            continue;
        }

        switch (inmenu)
        {
        case 1:
            calculateCGPA();
            return;
        case 2:
            return;
        case 3:
            exit(EXIT_SUCCESS);
        default:
            printf("You have Entered Wrong Input! Please Choose Again!\n");
            break;
        }
    }
}

void method(void)
{
    system(CLEAR_CMD);
    printf("--------------- Method of Calculating GPA & CGPA ---------------\n\n");
    printf(" GPA = Sum of (Credit * Point) / total of credits\n");
    printf(" CGPA = Sum of GPA / number of semesters\n");
    printf("-----------------------------------------------------------------\n\n");

    while (1)
    {
        int inmenu;
        printf("1. Go Back to Main Menu\n2. Exit This App\n\nYour Input: ");
        if (scanf("%d", &inmenu) != 1)
        {
            flush_input();
            printf("Invalid input. Please choose again!\n");
            continue;
        }

        switch (inmenu)
        {
        case 1:
            return;
        case 2:
            exit(EXIT_SUCCESS);
        default:
            printf("You have Entered Wrong Input! Please Choose Again!\n");
            break;
        }
    }
}

void flush_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    { /* discard */
    }
}
