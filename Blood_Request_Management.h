#pragma once

#include "Common.h"

typedef struct Blood_Request
{
    int Request_ID;

    char Patient_Name[50];
    char Hospital_Name[100];

    char Blood_Type[5];

    int Units_Required;

    Date Request_Date;

    char Status[20];

} Blood_Request;


void Register_Blood_Request(void);
void View_Requests(void);


Blood_Request Search_Request_By_ID(int Request_ID);


void Approve_Request(int Request_ID);
void Reject_Request(int Request_ID);

void View_Pending_Requests(void);

/* Utility */

int Generate_Request_ID(void);