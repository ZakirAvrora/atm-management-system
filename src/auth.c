#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
}

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {   
      
        
        if (strcmp(userChecker.name, u.name) == 0)
        {   
            fclose(fp);
            decrypt(userChecker.password, 0xFACA);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

const int getUserID(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {   
        if (strcmp(userChecker.name, u.name) == 0)
        {   
            fclose(fp);
            return userChecker.id;
        }
    }

    fclose(fp);
    return -1;
}

void registerMenu(char a[50], char pass[50])
{   
    char confirmationPassword[50];
    struct User userChecker;

tryAgain:

    system("clear");
    printf("\n\n\n\t\t\t\t   Registration Menu\n\t\t\t\t\t User Name:");
    scanf("%s", a);

    FILE *fp;
    FILE *fw;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {   
      
        
        if (strcmp(userChecker.name, a) == 0)
        {   
            fclose(fp);
            int option;
            system("clear");
            printf("\n✔ The user with the same name already exists\n\n");
    invalid:
            printf("Enter 1 to try again and 0 to exit!\n");
            scanf("%d", &option);
            system("clear");
            if (option == 1)
            {
               goto tryAgain;
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
    
    fclose(fp);

tryAgainPasw:

    printf("\n\t\t\t\t\t Password:");
    scanf("%s", pass);


    printf("\n\t\t\t\t\t Confirmation Password:");
    scanf("%s", confirmationPassword);

    printf("\n\t%s, %s", pass, confirmationPassword);
    printf("\n\t%d",strcmp(pass,confirmationPassword));

    if (strcmp(pass, confirmationPassword) != 0)
    { 
        int option;
            system("clear");

            printf("\nPassword: %s, ConfiPass: %s", pass, confirmationPassword);
            printf("\n✔ The passwords do not match\n\n");
    invalid1:
            printf("Enter 1 to try again and 0 to exit!\n");
            scanf("%d", &option);
            system("clear");
            if (option == 1)
            {
               goto tryAgainPasw;
            }
            else if (option == 0)
            {
                exit(1);
            }
            else
            {
                printf("Insert a valid operation!\n");
                goto invalid1;
            }
            
    }else{
        fw = fopen(USERS, "a+");
        encrypt(pass, 0xFACA);
        userChecker.id++;
        fprintf(fw, "%d %s %s\n\n",userChecker.id , a, pass);
        fclose(fw);
    }
}

