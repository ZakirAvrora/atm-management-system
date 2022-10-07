#include "header.h"

const char *RECORDS = "./data/records.txt";


int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %s %lf %s",
                  &r->id,
		          &r->userId,
		          r->name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %s %.2lf %s\n\n",
            r.id,
	        r.userId,
	        r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    

noAccount: ;

    FILE *pf = fopen(RECORDS, "a+");
    
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);


    while (getAccountFromFile(pf, userName, &cr))
    {   
      
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            printf("✖ Try again in 2 seconds\n\n");
            sleep(2);
            fclose(pf);
            goto noAccount;
        }
    }

    u.id = getUserID(u);
    if (u.id == -1) {
        printf("Error on the obtaining userId");
    }
    r.userId = u.id;
    r.id = cr.id+1;
    strcpy(r.name, u.name);
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%s", r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);


    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}


void updateAcc(struct User u)
{
    
invalidAccount:;
    struct Record r;
    struct Record cr;
    int flagFound;
    int lineNum = 0;

 
    printf("\n\nWhat is the account number you want to change:");
    scanf("%d", &r.accountNbr);
    
    FILE *pf = fopen(RECORDS, "r");

    while (getAccountFromFile(pf, u.name, &cr))
    {   
        lineNum++;
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {   
            printf("\nAccountNum %d, %d:", r.accountNbr,cr.accountNbr);
            flagFound = 1;
            break;
        }
    }
    fclose(pf);

    if (flagFound != 1) {
        system("clear");
        printf("There is no such account for user, try again");
        goto invalidAccount;
    }

invalid:
    printf("\n\nWhich information do you want to update?");
    int option;
    printf("\n1-> phone number");
    printf("\n2->country\n");
    printf("\n3->Main menu\n");
    scanf("%d", &option);
    if(option == 1)
    {   
        system("clear");
        printf("Enter the new phone number:");
        scanf("%s", r.phone);
    }else if (option == 2)
    {
        system("clear");
        printf("Enter the new country:");
        scanf("%s", r.country);
    }else if (option == 3)
    {
        system("clear");
        mainMenu(u);
    }else
    {
        system("clear");
        printf("Invalid option, try again");
        goto invalid;
    }

    updateRecord(u,r,lineNum,option);
    success(u);
}


void updateRecord(struct User u, struct Record r, int line, int option){
    
    struct Record cr;

    FILE *fp1;
    FILE *fp2;
    fp2 = fopen("./data/copy.txt", "w");
    fp1 = fopen(RECORDS, "r");

    int count = 0;
    while (getAccountFromFile(fp1, u.name, &cr))
    {   
        count++;
        if (count==line) {
            if(option==1) 
            {
                strcpy(cr.phone, r.phone);
            } 
            else if (option == 2) 
            {
                strcpy(cr.country, r.country);
            }
        }
    
        saveAccountToFile(fp2, u, cr);
    }
    fclose(fp1);
    fclose(fp2);
    remove (RECORDS);
    rename("./data/copy.txt", RECORDS);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(r.name, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}


void checkAccount(struct User u)
{
    struct Record r;
    struct Record cr;
    int flagFound = 0;
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);


    FILE *pf = fopen(RECORDS, "r");

    while (getAccountFromFile(pf, u.name, &cr))
    {
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {   
            flagFound = 1;
            system("clear");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   cr.accountNbr,
                   cr.deposit.month,
                   cr.deposit.day,
                   cr.deposit.year,
                   cr.country,
                   cr.phone,
                   cr.amount,
                   cr.accountType);

            if (strcmp(cr.accountType, "saving") == 0 ) {
                 printf("\n\n->You will get $%.2f as interest on day 10 of every month", cr.amount*0.07/12);
            }else if (strcmp(cr.accountType, "fixed01") == 0 ) {
                cr.deposit.year++;
                printf("\n\n->You will get $%.2f as interest on %d/%d/%d", cr.amount*0.04,cr.deposit.month, cr.deposit.day, cr.deposit.year);
            }else if (strcmp(cr.accountType, "fixed02") == 0 ) {
                cr.deposit.year+=2;
                printf("\n\n->You will get $%.2f as interest on %d/%d/%d", cr.amount*0.05*2,cr.deposit.month, cr.deposit.day, cr.deposit.year);
            }else if (strcmp(cr.accountType, "fixed03") == 0 ) {
                cr.deposit.year+=3;
                printf("\n\n->You will get $%.2f as interest on %d/%d/%d", cr.amount*0.08*3,cr.deposit.month, cr.deposit.day, cr.deposit.year);
            }else if (strcmp(cr.accountType, "current") == 0 ) {
                printf("\n\n->You will not get interests because the account is of type current");
            }
        }
        
    }
    
    fclose(pf);

    if (flagFound) {
        success(u);
    }else{
        int option;
        system("clear");
        printf("\n✔ No such account\n\n");
invalid:
        printf("Enter 1 to go to the main menu and 0 to exit!\n");
        scanf("%d", &option);
        system("clear");
        if (option == 1)
        {
            mainMenu(u);
        }
        else if (option == 0)
        {
            exit(1);
        }
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
}


void deleteAccount(struct User u)
{
  
TryAgain: ;
    struct Record r;
    struct Record cr;
    int lineNum = 0;
    int found = 0;

    printf("\nEnter the account number you want to delete: ");
    scanf("%d", &r.accountNbr);

    FILE *pf = fopen(RECORDS, "r");

    while (getAccountFromFile(pf, u.name, &cr))
    {   
        lineNum++;
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {   
            found = 1;
            break;
        }
    }
    fclose(pf);
    if (found !=1) {
        system("clear");
        printf("\nThere is no such account");
        int option;
invalid:
        printf("\n\nEnter 0 to go to the main menu and 1 to try again!\n");
        scanf("%d", &option);
        system("clear");
        if (option == 0)
        {
            mainMenu(u);
        }
        else if (option == 1)
        {
            goto TryAgain;
        }
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }

    FILE *fp2;
    fp2 = fopen("./data/copy.txt", "w");
    pf = fopen(RECORDS, "r");
    int count = 0;
    int flagAfter = 0;
    while (getAccountFromFile(pf, u.name, &cr))
    {   
        count++;
        if (count==lineNum) {
            flagAfter = 1;
            continue;
        }
        if (flagAfter == 1){
            cr.id--;
        }
        saveAccountToFile(fp2, u, cr);
    }
    fclose(pf);
    fclose(fp2);
    remove (RECORDS);
    rename("./data/copy.txt", RECORDS);

    success(u);
}

void  makeTransaction(struct User u) 
{   
TryAgain:;
    struct Record r;
    struct Record cr;
    int option;
    double flowMoney;
    int lineNum = 0;
    int flagFound = 0;

    printf("\nEnter the account number of the customer: ");
    scanf("%d", &r.accountNbr);

    FILE *pf = fopen(RECORDS, "r");

    while (getAccountFromFile(pf, u.name, &cr))
    {   
        lineNum++;
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {   
            flagFound = 1;
            break;
        }
    }
    fclose(pf);

    if (flagFound !=1) {
        printf("Account was not found");
         system("clear");
        printf("\nThere is no such account");
        int option;
invalid:
        printf("\n\nEnter 0 to go to the main menu and 1 to try again!\n");
        scanf("%d", &option);
        system("clear");
        if (option == 0)
        {
            mainMenu(u);
        }
        else if (option == 1)
        {
            goto TryAgain;
        }
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }

invalidWithdraw:
    printf("\n\nDo you want to:");
    printf("\n1->Withdraw");
    printf("\n2->Deposit");
    printf("\n3->Main menu");
    printf("\n\nEnter your choise:");
    scanf("%d", &option);

    if (strcmp(cr.accountType, "fixed01") == 0 || strcmp(cr.accountType, "fixed02")==0 || strcmp(cr.accountType, "fixed03")==0)
    {   
        printf("\nYou cannot deposit or withdraw cash in fixed accounts!");
        mainMenu(u);
    }

    if (option==1){
        printf("\n\nEnter the amount you want to withdraw: $");
        scanf("%lf", &flowMoney);
        if(cr.amount < flowMoney){
            system("clear");
            printf("\nThe amount you chose to withdraw is superior to your available balance!");
            printf("\nTry again");
            goto invalidWithdraw;
        }
        r.amount = cr.amount-flowMoney;
    }else if (option==2){
        printf("\n\nEnter the amount you want to deposit: $");
        scanf("%lf", &flowMoney);    
        r.amount = cr.amount+flowMoney;
    }else if (option==3){
        mainMenu(u);
    }else{
        printf("Invalid option");
    }

    updateAmount(u,r,lineNum);
    success(u);
}


void updateAmount(struct User u, struct Record r, int line){
    
    struct Record cr;

    FILE *fp1;
    FILE *fp2;
    fp2 = fopen("./data/copy.txt", "w");
    fp1 = fopen(RECORDS, "r");

    int count = 0;
   
    while (getAccountFromFile(fp1, u.name, &cr))
    {   
        count++;
        if (count==line) {
            cr.amount = r.amount;
        }
    
        saveAccountToFile(fp2, u, cr);
    }
    fclose(fp1);
    fclose(fp2);
    remove (RECORDS);
    rename("./data/copy.txt", RECORDS);

    success(u);
}

void transferOwner(struct User u)
{

TryAgain:;
    struct Record r;
    struct Record cr;
    char newOwner[50];
    int lineNum = 0; 
    int flagFound = 0;

    printf("\nEnter the account number you want to transfere ownership:");
    scanf("%d",&r.accountNbr);


    FILE *fp = fopen(RECORDS, "r");

    while (getAccountFromFile(fp, u.name, &cr))
    {   
        lineNum++;
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {   
            flagFound = 1;
            break;
        }
    }

    fclose(fp);

    if (flagFound !=1){
        system("clear");
        printf("\nThere is no such account");
        int option;
invalid:
        printf("\n\nEnter 0 to go to the main menu and 1 to try again!\n");
        scanf("%d", &option);
        system("clear");
        if (option == 0)
        {
            mainMenu(u);
        }
        else if (option == 1)
        {
            goto TryAgain;
        }
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }

 TryAgainUser:
    printf("\nWhich user you want transfer ownership to (user name):");
    scanf("%s",newOwner);

    struct User userChecker;
    flagFound = 0;
    fp = fopen("./data/users.txt", "r");

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {   
      
        
        if (strcmp(userChecker.name, newOwner) == 0)
        {   
            flagFound = 1;
            break;
        }
    }

    fclose(fp);
    
    if (flagFound != 1){
        system("clear");
        printf("\nThere is no such user");
        int option;
invalid2:
        printf("\n\nEnter 0 to go to the main menu and 1 to try again!\n");
        scanf("%d", &option);
        system("clear");
        if (option == 0)
        {
            mainMenu(u);
        }
        else if (option == 1)
        {
            goto TryAgainUser;
        }
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid2;
        }
    }
     
    r.userId = userChecker.id;
    strcpy(r.name, userChecker.name);

    updateOwner(u, r, lineNum);


    success(u);

}


void updateOwner(struct User u, struct Record r, int line){
    
    struct Record cr;

    FILE *fp1;
    FILE *fp2;
    fp2 = fopen("./data/copy.txt", "w");
    fp1 = fopen(RECORDS, "r");

    int count = 0;
   
    while (getAccountFromFile(fp1, u.name, &cr))
    {   
        count++;
        if (count==line) {
            cr.userId = r.userId;
            strcpy(cr.name, r.name);
        }
    
        saveAccountToFile(fp2, u, cr);
    }
    fclose(fp1);
    fclose(fp2);
    remove (RECORDS);
    rename("./data/copy.txt", RECORDS);

}