#pragma once

#include "Common.h"

typedef struct Donation
{
    int Donation_ID;
    int Donor_ID;
    Date Donation_Date;
    float Units_Donated;
    char Location[50];
    char Remarks[100];
} Donation;

/* Donation Operations */
void Record_Donation(void);
void View_Donation(void);

/* Search Operations */
void Search_Donation_By_DonorID(int Donor_ID);
void Search_Donation_By_Date(Date date);
Donation Search_Donation_By_ID(int Donation_ID);

/* Utility */
int Generate_Donation_ID(void);