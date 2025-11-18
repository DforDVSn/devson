#ifndef BANK_H
#define BANK_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

void fix_fgets_input(char* string);
void input_password(char* password, size_t size);
void register_account();
int login_user(int *logged_acc_no, char *logged_username);
void banking_menu(int acc_no);
void deposit_money_for(int acc_no);
void withdraw_money_for(int acc_no);
void check_balance_for(int acc_no);
int dev();
void admin_login();

#endif /* BANK_H */