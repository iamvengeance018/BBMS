#pragma once

#include "Common.h"

typedef struct Blood_Distribution
{
    int Distribution_ID;

    int Request_ID;

    char Patient_Name[50];
    char Blood_Type[5];

    int Units_Issued;

    Date Issue_Date;

} Blood_Distribution;

/* Distribution Operations */

void Issue_Blood_To_Request(int Request_ID);
void View_Distribution_History(void);

/* Search */

Blood_Distribution Search_Distribution_By_ID(int Distribution_ID);

/* Utility */

int Generate_Distribution_ID(void);

/* Reports */

void View_Transaction_History(void);