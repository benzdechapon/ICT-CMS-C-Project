#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void initialize();
void update_data();
void update_file(int);
void menu_Main();
struct userInfo {
    char id[8];
    char firstName[30];
    char lastName[30];
    char phoneNumber[11];
    char email[50];
    char password[15];
};
struct userInfo user_all[50];
int total_user=0;


int main()
{
    initialize();
    delay(2000);
    menu_Main();
    return 0;
}

void initialize()
{
    //File Checking
    FILE *fdata;
    fdata = fopen("userInfo.txt","r");
    if (fdata == NULL)
    {
        printf("[SYSTEM MESSAGE] File not found!\n");
        fclose(fdata);
        delay(500);
        printf("[SYSTEM MESSAGE] Creating file...\n");
        delay(500);
        fdata = fopen("userInfo.txt", "w");
        if (fdata != NULL)
        {
            printf("[SYSTEM MESSAGE] File Creating Success! :D\n");
            fclose(fdata);
        }
        else
        {
            printf("[SYSTEM MESSAGE] File Creating Fail! :(\n");
            fclose(fdata);
            exit(0);
        }
    }
    else
    {
        update_data();
        printf("[SYSTEM MESSAGE] File Loaded!\n");
        delay(500);
        printf("[SYSTEM MESSAGE] %d User\n", total_user);
        fclose(fdata);
    }
}

void update_data()
{
    FILE *freader;
    freader = fopen("userInfo.txt", "r");
    int i=0;
    if (freader == NULL)
    {
        printf("[SYSTEM MESSAGE] File Loading Fail!");
    }

    while (fscanf(freader,"%s %s %s %s %s %s", user_all[i].id, user_all[i].firstName, user_all[i].lastName,
        user_all[i].phoneNumber, user_all[i].email, user_all[i].password) != EOF)
    {
        i++;
    }

    total_user = i;
    fclose(freader);
}

void update_file(int times)
{
    FILE *fwriter;
    int i;
    fwriter = fopen("userInfo.txt", "w");
    if (fwriter == NULL)
    {
        printf("[SYSTEM MESSAGE] File Loading Fail!");
    }
    for (i = 0; i < times; i++)
    {
        fprintf(fwriter,"%s %s %s %s %s %s", user_all[i].id, user_all[i].firstName, user_all[i].lastName,
            user_all[i].phoneNumber, user_all[i].email, user_all[i].password);
    }
}

void menu_Main()
{
    //Initialize
    char choice;
    system("cls");
    printf("===============================\n");
    printf("ICT Contact Management Systems\n");
    printf("===============================\n");
    printf("[1] User Registration\n");
    printf("[2] Login\n");
    printf("[0] Exit\n");
    printf("-----------------------\n");

    //Menu Select
    do {
        printf("Enter the choice: ");
        choice = getchar();
        switch (choice)
        {
            case '0':
                printf("\nGood Bye! :)\n");
                exit(0);
            case '1':
                break;
            case '2':
                break;
            default:
                printf("[ERROR] Wrong Number! Please Enter the choice again.\n");
                printf("[Re-type]");
        }
    } while (choice!='0'&&choice!='1'&&choice!='2');
}
