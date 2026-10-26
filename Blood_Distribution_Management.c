#include <stdio.h>
#include <string.h>

#include "Blood_Distribution_Management.h"
#include "Blood_Request_Management.h"
#include "Blood_Inventory_Management.h"
#include "Helper.h"

int Generate_Distribution_ID()
{
    FILE *F = fopen("Distribution_History.dat","rb");

    if(F == NULL)
        return 1;

    Blood_Distribution d = {0};

    int Last_ID = 0;

    while(fread(&d,sizeof(Blood_Distribution),1,F) == 1)
    {
        Last_ID = d.Distribution_ID;
    }

    fclose(F);

    return Last_ID + 1;
}

Blood_Distribution Search_Distribution_By_ID(int Distribution_ID)
{
    Blood_Distribution d = {0};

    d.Distribution_ID = -1;

    FILE *F = fopen("Distribution_History.dat","rb");

    if(F == NULL)
        return d;

    while(fread(&d,sizeof(Blood_Distribution),1,F) == 1)
    {
        if(d.Distribution_ID == Distribution_ID)
        {
            fclose(F);

            return d;
        }
    }

    fclose(F);

    d.Distribution_ID = -1;

    return d;
}

void Issue_Blood_To_Request(int Request_ID)
{
    Blood_Request Request =
        Search_Request_By_ID(Request_ID);

    if(Request.Request_ID == -1)
    {
        printf("Request Not Found\n");
        return;
    }

    if(strcmp(Request.Status,"Approved") != 0)
    {
        printf("Request Must Be Approved Before Distribution\n");
        return;
    }
    
    if(Request.Units_Required <= 0)
    {
        printf("Request Has Invalid Units\n");
        return;
    }

    FILE *Inventory_File = fopen("Inventory.dat","rb+");

    if(Inventory_File == NULL)
    {
        printf("Inventory Not Available\n");
        return;
    }

    Blood_Inventory Inventory;
    int Found = 0;

    while(fread(&Inventory,sizeof(Blood_Inventory),1,Inventory_File) == 1)
    {
        if(strcmp(Inventory.Blood_Type,
                  Request.Blood_Type) == 0)
        {
            Found = 1;

            if(Inventory.Available_Units <
               Request.Units_Required)
            {
                printf("Insufficient Stock\n");

                fclose(Inventory_File);

                return;
            }

            Inventory.Available_Units -=
                Request.Units_Required;

            Inventory.Issued_Units +=
                Request.Units_Required;

            fseek(Inventory_File,
                  -sizeof(Blood_Inventory),
                  SEEK_CUR);

            fwrite(&Inventory,
                   sizeof(Blood_Inventory),
                   1,
                   Inventory_File);

            break;
        }
    }

    fclose(Inventory_File);

    if(!Found)
    {
        printf("Blood Type Not Found\n");
        return;
    }

    FILE *Distribution_File =
        fopen("Distribution_History.dat","ab");

    if(Distribution_File == NULL)
    {
        printf("Error Opening Distribution File\n");
        return;
    }

    Blood_Distribution Distribution;

    Distribution.Distribution_ID =
        Generate_Distribution_ID();

    Distribution.Request_ID =
        Request.Request_ID;

    strcpy(Distribution.Patient_Name,
           Request.Patient_Name);

    strcpy(Distribution.Blood_Type,
           Request.Blood_Type);

    Distribution.Units_Issued =
        Request.Units_Required;

    printf("Enter Issue Date\n");

    printf("Day : ");
    scanf("%d",&Distribution.Issue_Date.day);

    printf("Month : ");
    scanf("%d",&Distribution.Issue_Date.month);

    printf("Year : ");
    scanf("%d",&Distribution.Issue_Date.year);

    fwrite(&Distribution,
           sizeof(Blood_Distribution),
           1,
           Distribution_File);

    fclose(Distribution_File);

    FILE *Request_File =
        fopen("Requests.dat","rb+");

    if(Request_File != NULL)
    {
        Blood_Request r;

        while(fread(&r,sizeof(Blood_Request),1,Request_File) == 1)
        {
            if(r.Request_ID == Request_ID)
            {
                strcpy(r.Status,"Completed");

                fseek(Request_File,
                      -sizeof(Blood_Request),
                      SEEK_CUR);

                fwrite(&r,
                       sizeof(Blood_Request),
                       1,
                       Request_File);

                break;
            }
        }

        fclose(Request_File);
    }

    printf("Blood Issued Successfully\n");
}

void View_Distribution_History()
{
    FILE *F =
        fopen("Distribution_History.dat","rb");

    if(F == NULL)
    {
        printf("No Distribution Records Found\n");
        return;
    }

    Blood_Distribution d = {0};

    while(fread(&d,sizeof(Blood_Distribution),1,F) == 1)
    {
        Print_Distribution(d);

        Print_Line();
    }

    fclose(F);
}

void View_Transaction_History()
{
    View_Distribution_History();
}