#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "Blood_Inventory_Management.h"
#include "Helper.h"

pthread_mutex_t inventory_mutex = PTHREAD_MUTEX_INITIALIZER;

Blood_Inventory Search_Blood_Group(const char *Blood_Type)
{
    Blood_Inventory b = {0};
    b.Available_Units = -1;

    FILE *F = fopen("Inventory.dat","rb");

    if(F == NULL)
        return b;

    while(fread(&b,sizeof(Blood_Inventory),1,F) == 1)
    {
        if(strcmp(b.Blood_Type,Blood_Type) == 0)
        {
            fclose(F);
            return b;
        }
    }

    fclose(F);

    b.Available_Units = -1;
    return b;
}

void Add_Stock()
{
    char Blood_Type[5];
    float Units;

    printf("Enter Blood Type : ");
    scanf("%4s",Blood_Type);

    printf("Enter Units : ");
    scanf("%f",&Units);
    
    if(Units <= 0)
    {
        printf("Invalid Units\n");
        return;
    }

    pthread_mutex_lock(&inventory_mutex);

    FILE *F = fopen("Inventory.dat","rb+");

    if(F == NULL)
    {
        F = fopen("Inventory.dat","wb");
        if(F == NULL){
            printf("Error unable to create inventory.dat\n");
            pthread_mutex_unlock(&inventory_mutex);
            return;
        }

        Blood_Inventory b = {0};
        strcpy(b.Blood_Type,Blood_Type);
        b.Available_Units = Units;

        fwrite(&b,sizeof(Blood_Inventory),1,F);
        fclose(F);

        pthread_mutex_unlock(&inventory_mutex);
        printf("Stock Added Successfully\n");
        return;
    }

    Blood_Inventory b = {0};
    int Found = 0;

    while(fread(&b,sizeof(Blood_Inventory),1,F) == 1)
    {
        if(strcmp(b.Blood_Type,Blood_Type) == 0)
        {
            b.Available_Units += Units;
            fseek(F,-(long)sizeof(Blood_Inventory),SEEK_CUR);
            fwrite(&b,sizeof(Blood_Inventory),1,F);
            Found = 1;
            break;
        }
    }

    if(!Found)
    {
        strcpy(b.Blood_Type,Blood_Type);
        b.Available_Units = Units;
        b.Reserved_Units = 0;
        b.Issued_Units = 0;
        fwrite(&b,sizeof(Blood_Inventory),1,F);
    }

    fclose(F);
    pthread_mutex_unlock(&inventory_mutex);

    printf("Stock Added Successfully\n");
}

void Reserve_Blood()
{
    char Blood_Type[5];
    float Units;

    printf("Enter Blood Type : ");
    scanf("%4s",Blood_Type);

    printf("Enter Units : ");
    scanf("%f",&Units);
    
    if(Units <= 0)
    {
        printf("Invalid Units\n");
        return;
    }
    
    pthread_mutex_lock(&inventory_mutex);

    FILE *F = fopen("Inventory.dat","rb+");

    if(F == NULL)
    {
        printf("Inventory Empty\n");
        pthread_mutex_unlock(&inventory_mutex);
        return;
    }

    Blood_Inventory b = {0};

    while(fread(&b,sizeof(Blood_Inventory),1,F) == 1)
    {
        if(strcmp(b.Blood_Type,Blood_Type) == 0)
        {
            if(b.Available_Units < Units)
            {
                printf("Insufficient Stock\n");
                fclose(F);
                pthread_mutex_unlock(&inventory_mutex);
                return;
            }

            b.Available_Units -= Units;
            b.Reserved_Units += Units;

            fseek(F,-(long)sizeof(Blood_Inventory),SEEK_CUR);
            fwrite(&b,sizeof(Blood_Inventory),1,F);

            fclose(F);
            pthread_mutex_unlock(&inventory_mutex);
            printf("Blood Reserved Successfully\n");
            return;
        }
    }

    fclose(F);
    pthread_mutex_unlock(&inventory_mutex);
    printf("Blood Type Not Found\n");
}

void Issue_Blood()
{
    char Blood_Type[5];
    float Units;

    printf("Enter Blood Type : ");
    scanf("%4s",Blood_Type);

    printf("Enter Units : ");
    scanf("%f",&Units);
    
    if(Units <= 0)
    {
        printf("Invalid Units\n");
        return;
    }

    pthread_mutex_lock(&inventory_mutex);

    FILE *F = fopen("Inventory.dat","rb+");

    if(F == NULL)
    {
        printf("Inventory Empty\n");
        pthread_mutex_unlock(&inventory_mutex);
        return;
    }

    Blood_Inventory b = {0};

    while(fread(&b,sizeof(Blood_Inventory),1,F) == 1)
    {
        if(strcmp(b.Blood_Type,Blood_Type) == 0)
        {
            if(b.Reserved_Units < Units)
            {
                printf("Not Enough Reserved Blood\n");
                fclose(F);
                pthread_mutex_unlock(&inventory_mutex);
                return;
            }

            b.Reserved_Units -= Units;
            b.Issued_Units += Units;

            fseek(F,-(long)sizeof(Blood_Inventory),SEEK_CUR);
            fwrite(&b,sizeof(Blood_Inventory),1,F);

            fclose(F);
            pthread_mutex_unlock(&inventory_mutex);
            printf("Blood Issued Successfully\n");
            return;
        }
    }

    fclose(F);
    pthread_mutex_unlock(&inventory_mutex);
    printf("Blood Type Not Found\n");
}

void Update_Stock_After_Donation(const char *Blood_Type, float Units)
{
    pthread_mutex_lock(&inventory_mutex);

    FILE *F = fopen("Inventory.dat","rb+");

    if(F == NULL)
    {
        F = fopen("Inventory.dat","wb");
        if(F == NULL) {
            pthread_mutex_unlock(&inventory_mutex);
            return;
        }

        Blood_Inventory b = {0};
        strcpy(b.Blood_Type,Blood_Type);
        b.Available_Units = Units;

        fwrite(&b,sizeof(Blood_Inventory),1,F);
        fclose(F);
        pthread_mutex_unlock(&inventory_mutex);
        return;
    }

    Blood_Inventory b = {0};
    int Found = 0;

    while(fread(&b,sizeof(Blood_Inventory),1,F) == 1)
    {
        if(strcmp(b.Blood_Type,Blood_Type) == 0)
        {
            b.Available_Units += Units;
            fseek(F,-(long)sizeof(Blood_Inventory),SEEK_CUR);
            fwrite(&b,sizeof(Blood_Inventory),1,F);
            Found = 1;
            break;
        }
    }

    if(!Found)
    {
        strcpy(b.Blood_Type,Blood_Type);
        b.Available_Units = Units;
        b.Reserved_Units = 0;
        b.Issued_Units = 0;
        fwrite(&b,sizeof(Blood_Inventory),1,F);
    }

    fclose(F);
    pthread_mutex_unlock(&inventory_mutex);
}

void View_Inventory()
{
    FILE *F = fopen("Inventory.dat","rb");

    if(F == NULL)
    {
        printf("Inventory Empty\n");
        return;
    }

    Blood_Inventory b = {0};

    while(fread(&b,sizeof(Blood_Inventory),1,F) == 1)
    {
        Print_Inventory(b);
        Print_Line();
    }

    fclose(F);
}

void Low_Stock_Report()
{
    FILE *F = fopen("Inventory.dat","rb");

    if(F == NULL)
    {
        printf("Inventory Empty\n");
        return;
    }

    Blood_Inventory b = {0};

    printf("\nLOW STOCK REPORT\n");

    while(fread(&b,sizeof(Blood_Inventory),1,F) == 1)
    {
        if(b.Available_Units < 5)
        {
            Print_Inventory(b);
            Print_Line();
        }
    }

    fclose(F);
}

void Out_Of_Stock_Report()
{
    FILE *F = fopen("Inventory.dat","rb");

    if(F == NULL)
    {
        printf("Inventory Empty\n");
        return;
    }

    Blood_Inventory b = {0};

    printf("\nOUT OF STOCK REPORT\n");

    while(fread(&b,sizeof(Blood_Inventory),1,F) == 1)
    {
        if(b.Available_Units <= 0)
        {
            Print_Inventory(b);
            Print_Line();
        }
    }

    fclose(F);
}