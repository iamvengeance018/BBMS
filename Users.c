#include <stdio.h>
#include <string.h>

#include "Users.h"

void Initialize_Admin()
{
    FILE *F = fopen(ADMIN_FILE, "rb");

    if(F != NULL)
    {
        fclose(F);
        return;
    }

    F = fopen(ADMIN_FILE, "wb");

    if(F == NULL)
    {
        printf("Failed To Create Admin File\n");
        return;
    }

    Admin admin = {0};

    strcpy(admin.Username, "team27");
    strcpy(admin.Password, "kuruvi");

    fwrite(&admin, sizeof(Admin), 1, F);

    fclose(F);
}

int Admin_Login()
{
    FILE *F = fopen(ADMIN_FILE, "rb");

    if(F == NULL)
    {
        printf("Admin File Not Found\n");
        return 0;
    }

    Admin admin = {0};

    fread(&admin, sizeof(Admin), 1, F);

    fclose(F);

    char Username[50];
    char Password[50];

    printf("Enter Username : ");
    scanf("%49s", Username);

    printf("Enter Password : ");
    scanf("%49s", Password);

    if(strcmp(Username, admin.Username) == 0 &&
       strcmp(Password, admin.Password) == 0)
    {
        printf("\nLogin Successful\n");
        return 1;
    }

    printf("\nInvalid Username Or Password\n");

    return 0;
}

void Change_Admin_Password()
{
    FILE *F = fopen(ADMIN_FILE, "rb+");

    if(F == NULL)
    {
        printf("Admin File Not Found\n");
        return;
    }

    Admin admin;

    fread(&admin, sizeof(Admin), 1, F);

    char Current_Password[50];
    char New_Password[50];

    printf("Enter Current Password : ");
    scanf("%49s", Current_Password);

    if(strcmp(Current_Password, admin.Password) != 0)
    {
        printf("Incorrect Password\n");

        fclose(F);
        return;
    }

    printf("Enter New Password : ");
    scanf("%49s", New_Password);

    strcpy(admin.Password, New_Password);

    rewind(F);

    fwrite(&admin, sizeof(Admin), 1, F);

    fclose(F);

    printf("Password Changed Successfully\n");
}

void View_Login_Details()
{
    FILE *F = fopen(ADMIN_FILE, "rb");

    if(F == NULL)
    {
        printf("Admin File Not Found\n");
        return;
    }

    Admin admin;

    fread(&admin, sizeof(Admin), 1, F);

    fclose(F);

    printf("\nUsername : %s\n",
           admin.Username);
}