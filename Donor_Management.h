#pragma once
#include "Common.h"
typedef struct Donor
{
    int Donor_ID;
    char Name[50];
    char Blood_Type[5];
    char Mobile_Number[15];
    char Address[100];
    int Age;
    char Gender[10];
    float Weight;
} Donor;

void Add_Donor(void);

void View_Donors(void);

Donor Search_By_ID(int ID);
void Search_By_Name(const char *Name);
void Search_By_Blood_Type(const char *Blood_Type);
void Search_Donor(void);

void Update_Donor_Details(int ID);

void Delete_Donor(int ID);