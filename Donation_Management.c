#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Donation_Management.h"
#include "Donor_Management.h"
#include "Blood_Inventory_Management.h"
#include "Helper.h"

int Generate_Donation_ID()
{
    FILE *F = fopen("Donation_History.dat","rb");

    if(F == NULL)
        return 1;

    Donation d = {0};
    int Last_ID = 0;

    while(fread(&d,sizeof(Donation),1,F) == 1)
    {
        Last_ID = d.Donation_ID;
    }

    fclose(F);

    return Last_ID + 1;
}

Donation Search_Donation_By_ID(int Donation_ID)
{
    Donation d = {0};

    d.Donation_ID = -1;

    FILE *F = fopen("Donation_History.dat","rb");

    if(F == NULL)
        return d;

    while(fread(&d,sizeof(Donation),1,F) == 1)
    {
        if(d.Donation_ID == Donation_ID)
        {
            fclose(F);
            return d;
        }
    }

    fclose(F);

    d.Donation_ID = -1;

    return d;
}

void Record_Donation()
{
    FILE *F = fopen("Donation_History.dat","ab");

    if(F == NULL)
    {
        printf("Error Opening File\n");
        return;
    }

    Donation d = {0};

    d.Donation_ID = Generate_Donation_ID();

    printf("Enter Donor ID : ");
    scanf("%d",&d.Donor_ID);

    Donor donor = Search_By_ID(d.Donor_ID);

    if(donor.Donor_ID == -1)
    {
        printf("Donor Not Found\n");
        fclose(F);
        return;
    }

    printf("Enter Donation Date\n");

    printf("Day : ");
    scanf("%d",&d.Donation_Date.day);

    printf("Month : ");
    scanf("%d",&d.Donation_Date.month);

    printf("Year : ");
    scanf("%d",&d.Donation_Date.year);

    printf("Enter Units Donated : ");
    scanf("%f",&d.Units_Donated);
    
    if(d.Units_Donated <= 0)
    {
        printf("Invalid Units\n");
        fclose(F);
        return;
    }

    getchar();

    printf("Enter Location : ");
    fgets(d.Location,sizeof(d.Location),stdin);
    d.Location[strcspn(d.Location,"\n")] = '\0';

    printf("Enter Remarks : ");
    fgets(d.Remarks,sizeof(d.Remarks),stdin);
    d.Remarks[strcspn(d.Remarks,"\n")] = '\0';

    fwrite(&d,sizeof(Donation),1,F);

    fclose(F);

    /* Inventory Auto Update */

    Update_Stock_After_Donation(
        donor.Blood_Type,
        d.Units_Donated
    );

    printf("\nDonation Recorded Successfully\n");
    printf("Donation ID : %d\n",d.Donation_ID);
}

void View_Donation()
{
    FILE *F = fopen("Donation_History.dat","rb");

    if(F == NULL)
    {
        printf("No Donation Records Found\n");
        return;
    }

    Donation d = {0};

    while(fread(&d,sizeof(Donation),1,F) == 1)
    {
        Print_Donation(d);
        Print_Line();
    }

    fclose(F);
}

void Search_Donation_By_DonorID(int Donor_ID)
{
    FILE *F = fopen("Donation_History.dat","rb");

    if(F == NULL)
    {
        printf("No Donation Records Found\n");
        return;
    }

    Donation d = {0};
    int Found = 0;

    while(fread(&d,sizeof(Donation),1,F) == 1)
    {
        if(d.Donor_ID == Donor_ID)
        {
            Print_Donation(d);
            Print_Line();

            Found = 1;
        }
    }

    fclose(F);

    if(!Found)
        printf("No Donations Found\n");
}

void Search_Donation_By_Date(Date date)
{
    FILE *F = fopen("Donation_History.dat","rb");

    if(F == NULL)
    {
        printf("No Donation Records Found\n");
        return;
    }

    Donation d = {0};
    int Found = 0;

    while(fread(&d,sizeof(Donation),1,F) == 1)
    {
        if(
            d.Donation_Date.day == date.day &&
            d.Donation_Date.month == date.month &&
            d.Donation_Date.year == date.year
        )
        {
            Print_Donation(d);
            Print_Line();

            Found = 1;
        }
    }

    fclose(F);

    if(!Found)
        printf("No Donations Found\n");
}