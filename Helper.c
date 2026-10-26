#include <stdio.h>
#include "Helper.h"
#include "Donation_Management.h"


void Print_Line()
{
    printf("\n------------------------------------------------------------\n");
}

void Print_Donor(Donor d)
{
    printf("\nDonor ID      : %d", d.Donor_ID);
    printf("\nName          : %s", d.Name);
    printf("\nBlood Type    : %s", d.Blood_Type);
    printf("\nMobile Number : %s", d.Mobile_Number);
    printf("\nAddress       : %s", d.Address);
    printf("\nAge           : %d", d.Age);
    printf("\nGender        : %s", d.Gender);
    printf("\nWeight        : %.2f", d.Weight);
}



void Print_Donation(Donation d)
{
    printf("\nDonation ID   : %d",d.Donation_ID);

    printf("\nDonor ID      : %d",d.Donor_ID);

    printf("\nDate          : %02d/%02d/%04d",
           d.Donation_Date.day,
           d.Donation_Date.month,
           d.Donation_Date.year);

    printf("\nUnits Donated : %.2f",d.Units_Donated);

    printf("\nLocation      : %s",d.Location);

    printf("\nRemarks       : %s",d.Remarks);
}

void Print_Inventory(Blood_Inventory b)
{
    printf("\nBlood Type      : %s",
           b.Blood_Type);

    printf("\nAvailable Units : %.2f",
           b.Available_Units);

    printf("\nReserved Units  : %.2f",
           b.Reserved_Units);

    printf("\nIssued Units    : %.2f",
           b.Issued_Units);
}

void Print_Request(Blood_Request r)
{
    printf("\nRequest ID      : %d",
           r.Request_ID);

    printf("\nPatient Name    : %s",
           r.Patient_Name);

    printf("\nHospital Name   : %s",
           r.Hospital_Name);

    printf("\nBlood Type      : %s",
           r.Blood_Type);

    printf("\nUnits Required  : %d",
           r.Units_Required);

    printf("\nRequest Date    : %02d/%02d/%04d",
           r.Request_Date.day,
           r.Request_Date.month,
           r.Request_Date.year);

    printf("\nStatus          : %s",
           r.Status);
}

void Print_Distribution(
        Blood_Distribution d)
{
    printf("\nDistribution ID : %d",
           d.Distribution_ID);

    printf("\nRequest ID      : %d",
           d.Request_ID);

    printf("\nPatient Name    : %s",
           d.Patient_Name);

    printf("\nBlood Type      : %s",
           d.Blood_Type);

    printf("\nUnits Issued    : %d",
           d.Units_Issued);

    printf("\nIssue Date      : %02d/%02d/%04d",
           d.Issue_Date.day,
           d.Issue_Date.month,
           d.Issue_Date.year);
}

