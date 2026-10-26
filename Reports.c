#include <stdio.h>
#include <string.h>

#include "Reports.h"
#include "Donor_Management.h"
#include "Donation_Management.h"
#include "Blood_Inventory_Management.h"
#include "Blood_Request_Management.h"
#include "Blood_Distribution_Management.h"
#include "Helper.h"

void Donor_Report()
{
    FILE *F = fopen("Donor_Details.dat","rb");

    if(F == NULL)
    {
        printf("No Donor Records Found\n");
        return;
    }

    Donor d;
    int Count = 0;

    while(fread(&d,sizeof(Donor),1,F) == 1)
    {
        Print_Donor(d);
        Print_Line();

        Count++;
    }

    fclose(F);

    printf("\nTotal Donors : %d\n",Count);
}

void Donation_Report()
{
    FILE *F = fopen("Donation_History.dat","rb");

    if(F == NULL)
    {
        printf("No Donation Records Found\n");
        return;
    }

    Donation d;
    int Count = 0;
    float Total_Units = 0;

    while(fread(&d,sizeof(Donation),1,F) == 1)
    {
        Print_Donation(d);
        Print_Line();

        Total_Units += d.Units_Donated;
        Count++;
    }

    fclose(F);

    printf("\nTotal Donations : %d",Count);
    printf("\nTotal Units Collected : %.2f\n",Total_Units);
}

void Inventory_Report()
{
    FILE *F = fopen("Inventory.dat","rb");

    if(F == NULL)
    {
        printf("Inventory Empty\n");
        return;
    }

    Blood_Inventory b;

    while(fread(&b,sizeof(Blood_Inventory),1,F) == 1)
    {
        Print_Inventory(b);
        Print_Line();
    }

    fclose(F);
}

void Low_Stock_Report_Display()
{
    FILE *F = fopen("Inventory.dat","rb");

    if(F == NULL)
    {
        printf("Inventory Empty\n");
        return;
    }

    Blood_Inventory b;

    printf("\n========== LOW STOCK REPORT ==========\n");

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

void Out_Of_Stock_Report_Display()
{
    FILE *F = fopen("Inventory.dat","rb");

    if(F == NULL)
    {
        printf("Inventory Empty\n");
        return;
    }

    Blood_Inventory b;

    printf("\n========== OUT OF STOCK REPORT ==========\n");

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

void Request_Report()
{
    FILE *F = fopen("Requests.dat","rb");

    if(F == NULL)
    {
        printf("No Requests Found\n");
        return;
    }

    Blood_Request r;

    while(fread(&r,sizeof(Blood_Request),1,F) == 1)
    {
        Print_Request(r);
        Print_Line();
    }

    fclose(F);
}

void Pending_Request_Report()
{
    FILE *F = fopen("Requests.dat","rb");

    if(F == NULL)
    {
        printf("No Requests Found\n");
        return;
    }

    Blood_Request r;

    printf("\n========== PENDING REQUESTS ==========\n");

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

void Approved_Request_Report()
{
    FILE *F = fopen("Requests.dat","rb");

    if(F == NULL)
    {
        printf("No Requests Found\n");
        return;
    }

    Blood_Request r;

    printf("\n========== APPROVED REQUESTS ==========\n");

    while(fread(&r,sizeof(Blood_Request),1,F) == 1)
    {
        if(strcmp(r.Status,"Approved") == 0)
        {
            Print_Request(r);
            Print_Line();
        }
    }

    fclose(F);
}

void Rejected_Request_Report()
{
    FILE *F = fopen("Requests.dat","rb");

    if(F == NULL)
    {
        printf("No Requests Found\n");
        return;
    }

    Blood_Request r;

    printf("\n========== REJECTED REQUESTS ==========\n");

    while(fread(&r,sizeof(Blood_Request),1,F) == 1)
    {
        if(strcmp(r.Status,"Rejected") == 0)
        {
            Print_Request(r);
            Print_Line();
        }
    }

    fclose(F);
}

void Distribution_Report()
{
    FILE *F = fopen("Distribution_History.dat","rb");

    if(F == NULL)
    {
        printf("No Distribution Records Found\n");
        return;
    }

    Blood_Distribution d;

    while(fread(&d,sizeof(Blood_Distribution),1,F) == 1)
    {
        Print_Distribution(d);
        Print_Line();
    }

    fclose(F);
}

void System_Summary_Report()
{
    int Donor_Count = 0;
    int Donation_Count = 0;
    int Request_Count = 0;
    int Distribution_Count = 0;

    FILE *F;

    Donor donor;
    Donation donation;
    Blood_Request request;
    Blood_Distribution distribution;

    F = fopen("Donor_Details.dat","rb");

    if(F != NULL)
    {
        while(fread(&donor,sizeof(Donor),1,F) == 1)
            Donor_Count++;

        fclose(F);
    }

    F = fopen("Donation_History.dat","rb");

    if(F != NULL)
    {
        while(fread(&donation,sizeof(Donation),1,F) == 1)
            Donation_Count++;

        fclose(F);
    }

    F = fopen("Requests.dat","rb");

    if(F != NULL)
    {
        while(fread(&request,sizeof(Blood_Request),1,F) == 1)
            Request_Count++;

        fclose(F);
    }

    F = fopen("Distribution_History.dat","rb");

    if(F != NULL)
    {
        while(fread(&distribution,sizeof(Blood_Distribution),1,F) == 1)
            Distribution_Count++;

        fclose(F);
    }

    printf("\n========================================");
    printf("\n        BLOOD BANK SYSTEM SUMMARY");
    printf("\n========================================");

    printf("\nTotal Donors           : %d",Donor_Count);
    printf("\nTotal Donations        : %d",Donation_Count);
    printf("\nTotal Requests         : %d",Request_Count);
    printf("\nTotal Distributions    : %d",Distribution_Count);

    printf("\n========================================\n");
}