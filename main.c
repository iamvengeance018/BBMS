#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
volatile int keep_running = 1;

#include "Common.h"

#include "Donor_Management.h"
#include "Donation_Management.h"
#include "Blood_Inventory_Management.h"
#include "Blood_Request_Management.h"
#include "Blood_Distribution_Management.h"

#include "Reports.h"
#include "Users.h"
#include "Helper.h"

/* Function Prototypes */

void User_Portal(void);
void Admin_Portal(void);

void Donate_Menu(void);
void Request_Menu(void);

void Donor_Management_Menu(void);
void Donation_Management_Menu(void);
void Inventory_Management_Menu(void);
void Request_Management_Menu(void);
void Distribution_Management_Menu(void);
void Blood_Availability_Menu(void);

void Reports_Menu(void);
void User_Management_Menu(void);

void* Monitor_Stock_Thread(void* arg) {
    (void)arg;
    while(keep_running) {
        sleep(1);
        FILE *F = fopen("Inventory.dat", "rb");
        if(F) {
            Blood_Inventory b;
            while(fread(&b, sizeof(Blood_Inventory), 1, F) == 1) {
                if(b.Available_Units < 5.0f) {
                    FILE *log = fopen("bg_alerts.log", "a");
                    if(log) {
                        fprintf(log, "ALERT: Low stock for %s (%.1f units left)\n", b.Blood_Type, b.Available_Units);
                        fclose(log);
                    }
                }
            }
            fclose(F);
        }
    }
    return NULL;
}

int main()
{
    Initialize_Admin();
    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, Monitor_Stock_Thread, NULL);

    int choice;

    do
    {
        printf("\n=========================================================\n");
        printf("              BLOOD BANK MANAGEMENT SYSTEM\n");
        printf("=========================================================\n");
        printf("1. User Portal\n");
        printf("2. Admin Portal\n");
        printf("0. Exit\n");
        printf("=========================================================\n");
        printf("Enter Choice : ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                User_Portal();
                break;

            case 2:
                if(Admin_Login())
                    Admin_Portal();
                break;

            case 0:
                keep_running = 0;
                pthread_join(monitor_thread, NULL);
                printf("\nThank You For Using Blood Bank Management System\n");
                break;

            default:
                printf("Invalid Choice\n");
        }

    } while(choice != 0);

    return 0;
}

/* ========================================================= */
/* USER PORTAL                                               */
/* ========================================================= */

void User_Portal()
{
    int choice;

    do
    {
        printf("\n=========================================================\n");
        printf("                    USER PORTAL\n");
        printf("=========================================================\n");

        printf("1. Donate Blood\n");
        printf("2. Request Blood\n");
        printf("0. Back\n");

        printf("=========================================================\n");
        printf("Enter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                Donate_Menu();
                break;

            case 2:
                Request_Menu();
                break;

            case 0:
                break;

            default:
                printf("Invalid Choice\n");
        }

    } while(choice != 0);
}

/* ========================================================= */
/* DONATE MENU                                               */
/* ========================================================= */

void Donate_Menu()
{
    int choice;

    do
    {
        printf("\n=========================================================\n");
        printf("                    DONATE BLOOD\n");
        printf("=========================================================\n");

        printf("1. Register New Donor\n");
        printf("2. Record Donation\n");
        printf("0. Back\n");

        printf("=========================================================\n");
        printf("Enter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                Add_Donor();
                break;

            case 2:
                Record_Donation();
                break;

            case 0:
                break;

            default:
                printf("Invalid Choice\n");
        }

    } while(choice != 0);
}

/* ========================================================= */
/* REQUEST MENU                                              */
/* ========================================================= */

void Request_Menu()
{
    int choice;

    do
    {
        printf("\n=========================================================\n");
        printf("                    REQUEST BLOOD\n");
        printf("=========================================================\n");

        printf("1. Register Blood Request\n");
        printf("2. Check Blood Availability\n");
        printf("3. View Request Status\n");
        printf("0. Back\n");

        printf("=========================================================\n");
        printf("Enter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                Register_Blood_Request();
                break;

            case 2:
            {
                char Blood_Type[5];

                printf("Enter Blood Type : ");
                scanf("%4s",Blood_Type);

                Blood_Inventory b =
                    Search_Blood_Group(Blood_Type);

                if(b.Available_Units == -1)
                    printf("Blood Type Not Found\n");
                else
                    Print_Inventory(b);

                break;
            }

            case 3:
            {
                int Request_ID;

                printf("Enter Request ID : ");
                scanf("%d",&Request_ID);

                Blood_Request r =
                    Search_Request_By_ID(Request_ID);

                if(r.Request_ID == -1)
                    printf("Request Not Found\n");
                else
                    Print_Request(r);

                break;
            }

            case 0:
                break;

            default:
                printf("Invalid Choice\n");
        }

    } while(choice != 0);
}

/* ========================================================= */
/* ADMIN PORTAL                                              */
/* ========================================================= */

void Admin_Portal()
{
    int choice;

    do
    {
        printf("\n=========================================================\n");
        printf("                    ADMIN PORTAL\n");
        printf("=========================================================\n");

        printf("1. Donor Management\n");
        printf("2. Donation Management\n");
        printf("3. Inventory Management\n");
        printf("4. Request Management\n");
        printf("5. Distribution Management\n");
        printf("6. Blood Availability\n");
        printf("7. Reports\n");
        printf("8. User Management\n");
        printf("0. Logout\n");

        printf("=========================================================\n");
        printf("Enter Choice : ");

        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                Donor_Management_Menu();
                break;

            case 2:
                Donation_Management_Menu();
                break;

            case 3:
                Inventory_Management_Menu();
                break;

            case 4:
                Request_Management_Menu();
                break;

            case 5:
                Distribution_Management_Menu();
                break;

            case 6:
                Blood_Availability_Menu();
                break;

            case 7:
                Reports_Menu();
                break;

            case 8:
                User_Management_Menu();
                break;

            case 0:
                printf("Logged Out Successfully\n");
                break;

            default:
                printf("Invalid Choice\n");
        }

    } while(choice != 0);
}

/* ========================================================= */
/* DONOR MANAGEMENT                                          */
/* ========================================================= */

void Donor_Management_Menu()
{
    int choice;
    int ID;

    do
    {
        printf("\n=========================================================\n");
        printf("                 DONOR MANAGEMENT\n");
        printf("=========================================================\n");

        printf("1. Add Donor\n");
        printf("2. View Donors\n");
        printf("3. Search Donor\n");
        printf("4. Update Donor\n");
        printf("5. Delete Donor\n");
        printf("0. Back\n");

        printf("Enter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                Add_Donor();
                break;

            case 2:
                View_Donors();
                break;

            case 3:
                Search_Donor();
                break;

            case 4:
                printf("Enter Donor ID : ");
                scanf("%d",&ID);
                Update_Donor_Details(ID);
                break;

            case 5:
                printf("Enter Donor ID : ");
                scanf("%d",&ID);
                Delete_Donor(ID);
                break;
        }

    } while(choice != 0);
}

/* ========================================================= */
/* DONATION MANAGEMENT                                       */
/* ========================================================= */

void Donation_Management_Menu()
{
    int choice;

    do
    {
        printf("\n1. Record Donation");
        printf("\n2. View Donations");
        printf("\n3. Search Donations By Donor ID");
        printf("\n4. Search Donations By Date");
        printf("\n0. Back");

        printf("\n\nEnter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                Record_Donation();
                break;

            case 2:
                View_Donation();
                break;

            case 3:
            {
                int Donor_ID;

                printf("Enter Donor ID : ");
                scanf("%d",&Donor_ID);

                Search_Donation_By_DonorID(Donor_ID);
                break;
            }

            case 4:
            {
                Date date;

                printf("Day   : ");
                scanf("%d",&date.day);

                printf("Month : ");
                scanf("%d",&date.month);

                printf("Year  : ");
                scanf("%d",&date.year);

                Search_Donation_By_Date(date);
                break;
            }
        }

    } while(choice != 0);
}

/* ========================================================= */
/* INVENTORY MANAGEMENT                                      */
/* ========================================================= */

void Inventory_Management_Menu()
{
    int choice;

    do
    {
        printf("\n1. Add Stock");
        printf("\n2. Reserve Blood");
        printf("\n3. Issue Blood");
        printf("\n4. View Inventory");
        printf("\n5. Low Stock Report");
        printf("\n6. Out Of Stock Report");
        printf("\n0. Back");

        printf("\n\nEnter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1: Add_Stock(); break;
            case 2: Reserve_Blood(); break;
            case 3: Issue_Blood(); break;
            case 4: View_Inventory(); break;
            case 5: Low_Stock_Report(); break;
            case 6: Out_Of_Stock_Report(); break;
        }

    } while(choice != 0);
}

/* ========================================================= */
/* REQUEST MANAGEMENT                                        */
/* ========================================================= */

void Request_Management_Menu()
{
    int choice;
    int Request_ID;

    do
    {
        printf("\n1. View Requests");
        printf("\n2. Approve Request");
        printf("\n3. Reject Request");
        printf("\n4. View Pending Requests");
        printf("\n0. Back");

        printf("\n\nEnter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                View_Requests();
                break;

            case 2:
                printf("Enter Request ID : ");
                scanf("%d",&Request_ID);
                Approve_Request(Request_ID);
                break;

            case 3:
                printf("Enter Request ID : ");
                scanf("%d",&Request_ID);
                Reject_Request(Request_ID);
                break;

            case 4:
                View_Pending_Requests();
                break;
        }

    } while(choice != 0);
}

/* ========================================================= */
/* DISTRIBUTION MANAGEMENT                                   */
/* ========================================================= */

void Distribution_Management_Menu()
{
    int choice;
    int Request_ID;

    do
    {
        printf("\n1. Issue Blood To Request");
        printf("\n2. View Distribution History");
        printf("\n3. View Transaction History");
        printf("\n0. Back");

        printf("\n\nEnter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                printf("Enter Request ID : ");
                scanf("%d",&Request_ID);

                Issue_Blood_To_Request(Request_ID);
                break;

            case 2:
                View_Distribution_History();
                break;

            case 3:
                View_Transaction_History();
                break;
        }

    } while(choice != 0);
}

/* ========================================================= */
/* BLOOD AVAILABILITY                                        */
/* ========================================================= */

void Blood_Availability_Menu()
{
    char Blood_Type[5];

    printf("Enter Blood Type : ");
    scanf("%4s",Blood_Type);

    Blood_Inventory b =
        Search_Blood_Group(Blood_Type);

    if(b.Available_Units == -1)
        printf("Blood Type Not Found\n");
    else
        Print_Inventory(b);
}

/* ========================================================= */
/* REPORTS                                                   */
/* ========================================================= */

void Reports_Menu()
{
    int choice;

    do
    {
        printf("\n1. Donor Report");
        printf("\n2. Donation Report");
        printf("\n3. Inventory Report");
        printf("\n4. Request Report");
        printf("\n5. Distribution Report");
        printf("\n6. Low Stock Report");
        printf("\n7. Out Of Stock Report");
        printf("\n8. System Summary");
        printf("\n0. Back");

        printf("\n\nEnter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                Donor_Report();
                break;

            case 2:
                Donation_Report();
                break;

            case 3:
                Inventory_Report();
                break;

            case 4:
                Request_Report();
                break;

            case 5:
                Distribution_Report();
                break;

            case 6:
                Low_Stock_Report_Display();
                break;

            case 7:
                Out_Of_Stock_Report_Display();
                break;

            case 8:
                System_Summary_Report();
                break;
        }

    } while(choice != 0);
}

/* ========================================================= */
/* USER MANAGEMENT                                           */
/* ========================================================= */

void User_Management_Menu()
{
    int choice;

    do
    {
        printf("\n1. View Login Details");
        printf("\n2. Change Admin Password");
        printf("\n0. Back");

        printf("\n\nEnter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                View_Login_Details();
                break;

            case 2:
                Change_Admin_Password();
                break;
        }

    } while(choice != 0);
}