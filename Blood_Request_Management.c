#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Blood_Request_Management.h"
#include "Blood_Inventory_Management.h"
#include "Helper.h"

int Generate_Request_ID()
{
    FILE *F = fopen("Requests.dat","rb");

    if(F == NULL)
        return 1;

    Blood_Request r = {0};

    int Last_ID = 0;

    while(fread(&r,sizeof(Blood_Request),1,F) == 1)
    {
        Last_ID = r.Request_ID;
    }

    fclose(F);

    return Last_ID + 1;
}

Blood_Request Search_Request_By_ID(int Request_ID)
{
    Blood_Request r = {0};

    r.Request_ID = -1;

    FILE *F = fopen("Requests.dat","rb");

    if(F == NULL)
        return r;

    while(fread(&r,sizeof(Blood_Request),1,F) == 1)
    {
        if(r.Request_ID == Request_ID)
        {
            fclose(F);

            return r;
        }
    }

    fclose(F);

    r.Request_ID = -1;

    return r;
}

void Register_Blood_Request()
{
    FILE *F = fopen("Requests.dat","ab");

    if(F == NULL)
    {
        printf("Error Opening File\n");
        return;
    }

    Blood_Request r = {0};

    r.Request_ID = Generate_Request_ID();

    getchar();

    printf("Enter Patient Name : ");
    fgets(r.Patient_Name,sizeof(r.Patient_Name),stdin);
    r.Patient_Name[strcspn(r.Patient_Name,"\n")] = '\0';

    printf("Enter Hospital Name : ");
    fgets(r.Hospital_Name,sizeof(r.Hospital_Name),stdin);
    r.Hospital_Name[strcspn(r.Hospital_Name,"\n")] = '\0';

    printf("Enter Blood Type : ");
    scanf("%4s",r.Blood_Type);

    printf("Enter Units Required : ");
    scanf("%d",&r.Units_Required);
    
    if(r.Units_Required <= 0)
    {
        printf("Invalid Units\n");
        fclose(F);
        return;
    }

    printf("Enter Request Date\n");

    printf("Day : ");
    scanf("%d",&r.Request_Date.day);

    printf("Month : ");
    scanf("%d",&r.Request_Date.month);

    printf("Year : ");
    scanf("%d",&r.Request_Date.year);

    strcpy(r.Status,"Pending");

    fwrite(&r,sizeof(Blood_Request),1,F);

    fclose(F);

    printf("\nRequest Registered Successfully\n");
    printf("Request ID : %d\n",r.Request_ID);
}

void View_Requests()
{
    FILE *F = fopen("Requests.dat","rb");

    if(F == NULL)
    {
        printf("No Requests Found\n");
        return;
    }

    Blood_Request r = {0};

    while(fread(&r,sizeof(Blood_Request),1,F) == 1)
    {
        Print_Request(r);

        Print_Line();
    }

    fclose(F);
}

void View_Pending_Requests()
{
    FILE *F = fopen("Requests.dat","rb");

    if(F == NULL)
    {
        printf("No Requests Found\n");
        return;
    }

    Blood_Request r = {0};

    while(fread(&r,sizeof(Blood_Request),1,F) == 1)
    {
        if(strcmp(r.Status,"Pending") == 0)
        {
            Print_Request(r);

            Print_Line();
        }
    }

    fclose(F);
}

void Approve_Request(int Request_ID)
{
    FILE *F = fopen("Requests.dat","rb+");

    if(F == NULL)
    {
        printf("No Requests Found\n");
        return;
    }

    Blood_Request r = {0};

    while(fread(&r,sizeof(Blood_Request),1,F) == 1)
    {
        if(r.Request_ID == Request_ID)
        {
            Blood_Inventory b =
                Search_Blood_Group(r.Blood_Type);

            if(b.Available_Units < r.Units_Required)
            {
                printf("Insufficient Stock\n");

                fclose(F);

                return;
            }

            strcpy(r.Status,"Approved");

            fseek(F,-(long)sizeof(Blood_Request),SEEK_CUR);

            fwrite(&r,sizeof(Blood_Request),1,F);

            fclose(F);

            printf("Request Approved\n");

            return;
        }
    }

    fclose(F);

    printf("Request Not Found\n");
}

void Reject_Request(int Request_ID)
{
    FILE *F = fopen("Requests.dat","rb+");

    if(F == NULL)
    {
        printf("No Requests Found\n");
        return;
    }

    Blood_Request r = {0};

    while(fread(&r,sizeof(Blood_Request),1,F) == 1)
    {
        if(r.Request_ID == Request_ID)
        {
            strcpy(r.Status,"Rejected");

            fseek(F,-(long)sizeof(Blood_Request),SEEK_CUR);

            fwrite(&r,sizeof(Blood_Request),1,F);

            fclose(F);

            printf("Request Rejected\n");

            return;
        }
    }

    fclose(F);

    printf("Request Not Found\n");
}