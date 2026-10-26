#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Donor_Management.h"
#include "Helper.h"

void Add_Donor()
{
    FILE *F = fopen("Donor_Details.dat","ab");

    if(F == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    Donor d = {0};

    printf("Enter Donor ID : ");
    scanf("%d",&d.Donor_ID);

    if(Search_By_ID(d.Donor_ID).Donor_ID != -1)
    {
        printf("Donor ID already exists\n");
        fclose(F);
        return;
    }

    getchar();

    printf("Enter Name : ");
    fgets(d.Name,sizeof(d.Name),stdin);
    d.Name[strcspn(d.Name,"\n")] = '\0';

    printf("Enter Blood Type : ");
    scanf("%4s",d.Blood_Type);

    printf("Enter Mobile Number : ");
    scanf("%14s",d.Mobile_Number);

    getchar();

    printf("Enter Address : ");
    fgets(d.Address,sizeof(d.Address),stdin);
    d.Address[strcspn(d.Address,"\n")] = '\0';

    printf("Enter Age : ");
    scanf("%d",&d.Age);

    printf("Enter Gender : ");
    scanf("%9s",d.Gender);

    printf("Enter Weight : ");
    scanf("%f",&d.Weight);

    fwrite(&d,sizeof(Donor),1,F);

    fclose(F);

    printf("\nDonor Added Successfully\n");
}

void View_Donors()
{
    FILE *F = fopen("Donor_Details.dat","rb");

    if(F == NULL)
    {
        printf("No donor records found\n");
        return;
    }

    Donor d = {0};

    while(fread(&d,sizeof(Donor),1,F) == 1)
    {
        Print_Donor(d);
        Print_Line();
    }

    fclose(F);
}

Donor Search_By_ID(int ID)
{
    Donor d = {0};

    d.Donor_ID = -1;

    FILE *F = fopen("Donor_Details.dat","rb");

    if(F == NULL)
        return d;

    while(fread(&d,sizeof(Donor),1,F) == 1)
    {
        if(d.Donor_ID == ID)
        {
            fclose(F);
            return d;
        }
    }

    fclose(F);

    d.Donor_ID = -1;

    return d;
}

void Search_By_Name(const char *Name)
{
    FILE *F = fopen("Donor_Details.dat","rb");

    if(F == NULL)
    {
        printf("File Not Found\n");
        return;
    }

    Donor d = {0};
    int found = 0;

    while(fread(&d,sizeof(Donor),1,F) == 1)
    {
        if(strcmp(d.Name,Name) == 0)
        {
            Print_Donor(d);
            Print_Line();
            found = 1;
        }
    }

    if(!found)
        printf("No donor found\n");

    fclose(F);
}

void Search_By_Blood_Type(const char *Blood_Type)
{
    FILE *F = fopen("Donor_Details.dat","rb");

    if(F == NULL)
    {
        printf("File Not Found\n");
        return;
    }

    Donor d = {0};
    int found = 0;

    while(fread(&d,sizeof(Donor),1,F) == 1)
    {
        if(strcmp(d.Blood_Type,Blood_Type) == 0)
        {
            Print_Donor(d);
            Print_Line();
            found = 1;
        }
    }

    if(!found)
        printf("No donor found\n");

    fclose(F);
}

void Search_Donor()
{
    int choice;

    printf("\n1. Search By ID");
    printf("\n2. Search By Name");
    printf("\n3. Search By Blood Type");

    printf("\n\nEnter Choice : ");
    scanf("%d",&choice);

    switch(choice)
    {
        case 1:
        {
            int ID;

            printf("Enter ID : ");
            scanf("%d",&ID);

            Donor d = Search_By_ID(ID);

            if(d.Donor_ID == -1)
                printf("Donor Not Found\n");
            else
                Print_Donor(d);

            break;
        }

        case 2:
        {
            char Name[50];

            getchar();

            printf("Enter Name : ");
            fgets(Name,sizeof(Name),stdin);

            Name[strcspn(Name,"\n")] = '\0';

            Search_By_Name(Name);

            break;
        }

        case 3:
        {
            char Blood_Type[5];

            printf("Enter Blood Type : ");
            scanf("%4s",Blood_Type);

            Search_By_Blood_Type(Blood_Type);

            break;
        }

        default:
            printf("Invalid Choice\n");
    }
}

void Update_Donor_Details(int ID)
{
    FILE *F = fopen("Donor_Details.dat","rb+");

    if(F == NULL)
    {
        printf("File Not Found\n");
        return;
    }

    Donor d = {0};
    int found = 0;

    while(fread(&d,sizeof(Donor),1,F) == 1)
    {
        if(d.Donor_ID == ID)
        {
            found = 1;

            int choice;

            printf("\n1. Name");
            printf("\n2. Mobile Number");
            printf("\n3. Address");
            printf("\n4. Age");
            printf("\n5. Weight");

            printf("\n\nEnter Choice : ");
            scanf("%d",&choice);

            getchar();

            switch(choice)
            {
                case 1:
                    printf("Enter New Name : ");
                    fgets(d.Name,sizeof(d.Name),stdin);
                    d.Name[strcspn(d.Name,"\n")] = '\0';
                    break;

                case 2:
                    printf("Enter New Mobile Number : ");
                    scanf("%14s",d.Mobile_Number);
                    break;

                case 3:
                    printf("Enter New Address : ");
                    fgets(d.Address,sizeof(d.Address),stdin);
                    d.Address[strcspn(d.Address,"\n")] = '\0';
                    break;

                case 4:
                    printf("Enter New Age : ");
                    scanf("%d",&d.Age);
                    break;

                case 5:
                    printf("Enter New Weight : ");
                    scanf("%f",&d.Weight);
                    break;

                default:
                    printf("Invalid Choice\n");
                    fclose(F);
                    return;
            }

            fseek(F,-sizeof(Donor),SEEK_CUR);
            fwrite(&d,sizeof(Donor),1,F);

            printf("Updated Successfully\n");

            break;
        }
    }

    if(!found)
        printf("Donor Not Found\n");

    fclose(F);
}

void Delete_Donor(int ID)
{
    FILE *F = fopen("Donor_Details.dat","rb");

    if(F == NULL)
    {
        printf("File Not Found\n");
        return;
    }

    FILE *Temp = fopen("Temp.dat","wb");

    Donor d = {0};
    int found = 0;

    while(fread(&d,sizeof(Donor),1,F) == 1)
    {
        if(d.Donor_ID == ID)
        {
            found = 1;
            continue;
        }

        fwrite(&d,sizeof(Donor),1,Temp);
    }

    fclose(F);
    fclose(Temp);

    remove("Donor_Details.dat");
    rename("Temp.dat","Donor_Details.dat");

    if(found)
        printf("Donor Deleted Successfully\n");
    else
        printf("Donor Not Found\n");
}