#pragma once

#include "Common.h"
#include "Donor_Management.h"
#include "Donation_Management.h"
#include "Blood_Inventory_Management.h"
#include "Blood_Request_Management.h"
#include "Blood_Distribution_Management.h"

/* Common Print Functions */

void Print_Line(void);

void Print_Donor(Donor d);

void Print_Donation(Donation d);

void Print_Inventory(Blood_Inventory b);

void Print_Request(Blood_Request r);

void Print_Distribution(Blood_Distribution d);