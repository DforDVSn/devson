#include <stdio.h>
#include "donut.h"
#include "try.h"

#define CLEAR_CMD "clear"
int main()
{
  int choice;
  int j;
  system(CLEAR_CMD);
  printf("\nDON'T ENTER 1 (ONE)\n");
  scanf("%d", &j);

  if (j == 1)
  {
    printf("\nTHANK YOU\n");
    donut();
    return 0;
  }

  do
  {
    system(CLEAR_CMD);
    printf("AUTHENTICATION\n");
    printf("1. ADMIN\n");
    printf("2. STUDENT\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice != 1 && choice != 2)
    {
      printf("Invalid choice, try again!\n\n");
    }
  } while (choice != 1 && choice != 2);

  switch (choice)
  {
  case 1:
    printf("Admin selected\n");
    admin_login();
    break;
  case 2:
    printf("Student selected\n");
    dev();
    break;
  }

  return 0;
}
