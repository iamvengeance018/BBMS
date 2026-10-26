#pragma once

typedef struct Blood_Inventory
{
    char Blood_Type[5];

    float Available_Units;
    float Reserved_Units;
    float Issued_Units;

} Blood_Inventory;

/* Core Operations */

void Add_Stock(void);
void Reserve_Blood(void);
void Issue_Blood(void);
void View_Inventory(void);

/* Reports */

void Low_Stock_Report(void);
void Out_Of_Stock_Report(void);

/* Search */

Blood_Inventory Search_Blood_Group(const char *Blood_Type);

/* Donation Integration */

void Update_Stock_After_Donation(
    const char *Blood_Type,
    float Units
);