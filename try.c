#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "conversion.h"
#include "placement.h"

#define ACCOUNT_FILE "data.txt"
#define NAME_LEN 50
#define USERNAME_LEN 30
#define PASSWORD_LEN 30
#ifdef _WIN32
#define CLEAR_CMD "cls"
#else
#define CLEAR_CMD "clear"
#endif

typedef struct
{
    char name[NAME_LEN];
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    int acc_no;
    float balance;
} Account;

/* Prototypes */
void fix_fgets_input(char *string);
void input_password(char *password, size_t size);
void register_account();
int login_user(int *logged_acc_no, char *logged_username);
void banking_menu(int acc_no);
void deposit_money_for(int acc_no);
void withdraw_money_for(int acc_no);
void check_balance_for(int acc_no);
void admin_login(); /* new admin prototype */

/* Helper: remove trailing newline from fgets */
void fix_fgets_input(char *string)
{
    int index = strcspn(string, "\n");
    string[index] = '\0';
}

/* Read password with masking (supports backspace) */
void input_password(char *password, size_t size)
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON); /* disable echo and canonical mode */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int i = 0;
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF && i < (int)size - 1)
    {
        if (ch == 127 || ch == '\b')
        { /* handle backspace */
            if (i > 0)
            {
                i--;
                /* erase star from terminal */
                printf("\b \b");
                fflush(stdout);
            }
        }
        else
        {
            password[i++] = (char)ch;
            printf("*");
            fflush(stdout);
        }
    }
    password[i] = '\0';
    printf("\n");

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

/* helper: return 1 if username exists, 0 otherwise */
static int username_exists(const char *username)
{
    FILE *f = fopen(ACCOUNT_FILE, "rb");
    if (!f)
        return 0; /* no file yet -> no duplicates */

    Account tmp;
    while (fread(&tmp, sizeof(tmp), 1, f) == 1)
    {
        if (strcmp(tmp.username, username) == 0)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

/* helper: return 1 if account number exists, 0 otherwise */
static int accno_exists(int acc_no)
{
    FILE *f = fopen(ACCOUNT_FILE, "rb");
    if (!f)
        return 0;

    Account tmp;
    while (fread(&tmp, sizeof(tmp), 1, f) == 1)
    {
        if (tmp.acc_no == acc_no)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

/* Register a new account and store it in ACCOUNT_FILE */
void register_account()
{
    Account acc;
    int c;

    /* clear leftover newline */
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }

    printf("\n--- Register New Account ---\n");

    printf("Enter your full name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    fix_fgets_input(acc.name);

    /* get username and ensure uniqueness */
    while (1)
    {

        printf("Choose a username: ");
        fgets(acc.username, sizeof(acc.username), stdin);
        fix_fgets_input(acc.username);

        if (strlen(acc.username) == 0)
        {
            printf("Username cannot be empty. Try again.\n");
            continue;
        }

        if (username_exists(acc.username))
        {
            printf("Username '%s' already exists. Choose a different username.\n", acc.username);
            continue;
        }
        break;
    }

    printf("Choose a password (input will be masked): ");
    input_password(acc.password, sizeof(acc.password));

    /* get account number and ensure uniqueness */
    while (1)
    {
        system(CLEAR_CMD);

        printf("Enter your account number (integer): ");
        if (scanf("%d", &acc.acc_no) != 1)
        {
            printf("Invalid account number. Try again.\n");
            /* consume rest of line */
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }
        /* consume newline after scanf */
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }

        if (accno_exists(acc.acc_no))
        {
            printf("Account number %d already exists. Choose a different account number.\n", acc.acc_no);
            continue;
        }
        break;
    }

    acc.balance = 0.0f;

    /* Append to file */
    FILE *file = fopen(ACCOUNT_FILE, "ab");
    if (!file)
    {
        perror("Unable to open account file for writing");
        return;
    }

    if (fwrite(&acc, sizeof(acc), 1, file) != 1)
    {
        perror("Failed to write account to file");
    }
    else
    {
        printf("\nAccount created successfully for username '%s' with account no %d\n",
               acc.username, acc.acc_no);
    }

    fclose(file);
}

/* Login by username + password. On success returns 1 and sets logged_acc_no and logged_username */
int login_user(int *logged_acc_no, char *logged_username)
{
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];

    /* clear leftover newline */
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }

    printf("\n--- Login ---\n");
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    fix_fgets_input(username);

    printf("Password (masked): ");
    input_password(password, sizeof(password));

    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (!file)
    {
        printf("No accounts found. Please register first.\n");
        return 0;
    }

    Account acc;
    while (fread(&acc, sizeof(acc), 1, file) == 1)
    {
        if (strcmp(username, acc.username) == 0 && strcmp(password, acc.password) == 0)
        {
            *logged_acc_no = acc.acc_no;
            strncpy(logged_username, acc.username, USERNAME_LEN);
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

/* Admin login: id == 9999 and password == "admin" */
void admin_login()
{        system(CLEAR_CMD);

    int admin_id;
    char password[PASSWORD_LEN];
    int c;

    /* clear leftover newline */
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }

    printf("\n--- Admin Login ---\n");
    printf("Admin ID: ");
    if (scanf("%d", &admin_id) != 1)
    {
        printf("Invalid ID input.\n");
        /* consume rest of line */
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
        return;
    }
    /* consume newline after scanf */
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }

    printf("Password (masked): ");
    input_password(password, sizeof(password));

    if (admin_id == 9999 && strcmp(password, "admin") == 0)
    {
        FILE *file = fopen(ACCOUNT_FILE, "rb");
        if (!file)
        {
            perror("Unable to open students file");
            return;
        }

        printf("\n--- Students List ---\n");
        Account acc;
        int found = 0;
        while (fread(&acc, sizeof(acc), 1, file) == 1)
        {
            found = 1;
            printf("Name: %s\nUsername: %s\nAccount No: %d\nBalance: Rs.%.2f\n---------------------\n",
                   acc.name, acc.username, acc.acc_no, acc.balance);
        }
        if (!found)
        {
            printf("No student records found.\n");
        }
        fclose(file);
    }
    else
    {
        printf("Access denied: incorrect admin ID or password.\n");
    }
}

/* Banking menu for a logged-in account */
void banking_menu(int acc_no)
{
    int choice;
    while (1)
    {        system(CLEAR_CMD);

        printf("💼 >> Account Dashboard (ID %d) << 💼\n", acc_no);
        printf("1. Deposit Money\n");
        printf("2. Withdraw Money\n");
        printf("3. Check Balance\n");
        printf("4. GPA & CGPA Calculator\n");
        printf("5. Placement Likelihood\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            /* consume invalid input */
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            deposit_money_for(acc_no);
            break;
        case 2:
            withdraw_money_for(acc_no);
            break;
        case 3:
            check_balance_for(acc_no);
            break;
        case 4:
            sgpa();
            break;
        case 5:
            placement();
            break;
        case 6:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}

/* Deposit for a specific account number */
void deposit_money_for(int acc_no)
{
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (!file)
    {
        perror("Unable to open account file");
        return;
    }

    float money;
    Account acc_r;
    printf("Enter amount to deposit: ");
    if (scanf("%f", &money) != 1 || money <= 0.0f)
    {
        printf("Invalid amount.\n");
        /* consume rest of line */
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
        fclose(file);
        return;
    }

    while (fread(&acc_r, sizeof(acc_r), 1, file) == 1)
    {
        if (acc_r.acc_no == acc_no)
        {
            acc_r.balance += money;
            fseek(file, -(long)sizeof(acc_r), SEEK_CUR);
            if (fwrite(&acc_r, sizeof(acc_r), 1, file) != 1)
            {
                perror("Failed to update balance");
            }
            else
            {
                printf("Successfully deposited Rs.%.2f. New balance: Rs.%.2f\n", money, acc_r.balance);
            }
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Account number %d not found.\n", acc_no);
}

/* Withdraw for a specific account number */
void withdraw_money_for(int acc_no)
{
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (!file)
    {
        perror("Unable to open account file");
        return;
    }

    float money;
    Account acc_r;
    printf("Enter amount to withdraw: ");
    if (scanf("%f", &money) != 1 || money <= 0.0f)
    {
        printf("Invalid amount.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
        fclose(file);
        return;
    }

    while (fread(&acc_r, sizeof(acc_r), 1, file) == 1)
    {
        if (acc_r.acc_no == acc_no)
        {
            if (acc_r.balance >= money)
            {
                acc_r.balance -= money;
                fseek(file, -(long)sizeof(acc_r), SEEK_CUR);
                if (fwrite(&acc_r, sizeof(acc_r), 1, file) != 1)
                {
                    perror("Failed to update balance");
                }
                else
                {
                    printf("Successfully withdrawn Rs.%.2f. Remaining balance: Rs.%.2f\n", money, acc_r.balance);
                }
            }
            else
            {
                printf("Insufficient balance! Current balance: Rs.%.2f\n", acc_r.balance);
            }
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Account number %d not found.\n", acc_no);
}

/* Check balance for a specific account number */
void check_balance_for(int acc_no)
{
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (!file)
    {
        perror("Unable to open account file");
        return;
    }

    Account acc_read;
    while (fread(&acc_read, sizeof(acc_read), 1, file) == 1)
    {
        if (acc_read.acc_no == acc_no)
        {
            printf("Your current balance is Rs.%.2f\n", acc_read.balance);
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Account number %d not found.\n", acc_no);
}

/* Main: top-level register/login then banking */
int dev()
{
    int option;
    int logged_acc_no = -1;
    char logged_username[USERNAME_LEN] = {0};

    while (1)
    {
        printf("\n\n=== WELCOME ===\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &option) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (option)
        {
        case 1:
            register_account();
            break;
        case 2:
            if (login_user(&logged_acc_no, logged_username))
            {
                printf("Login successful! Welcome, %s\n", logged_username);
                banking_menu(logged_acc_no);
            }
            else
            {
                printf("Login failed: incorrect username or password.\n");
            }
            break;
        case 3:
            printf("Exiting program. Goodbye.\n");
            return 0;
        default:
            printf("Invalid option. Try again.\n");
        }
    }

    return 0;
}
