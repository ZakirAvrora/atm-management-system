#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    char phone[100];
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};


// authentication functions
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);
const int getUserID(struct User u);


//encryption functions
void encrypt(char password[],int key);
void decrypt(char password[],int key);


// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
void checkAccount(struct User u);
void updateAcc(struct User u);
void updateRecord(struct User u, struct Record r, int line, int option);
void deleteAccount(struct User u);
void makeTransaction(struct User u);
void updateAmount(struct User u, struct Record r, int line);
void transferOwner(struct User u);
void updateOwner(struct User u, struct Record r, int line);
