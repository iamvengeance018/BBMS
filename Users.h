#pragma once

#define ADMIN_FILE "Admin.dat"

typedef struct Admin
{
    char Username[50];
    char Password[50];
} Admin;

/* Setup */

void Initialize_Admin(void);

/* Login */

int Admin_Login(void);

/* Password */

void Change_Admin_Password(void);

/* Information */

void View_Login_Details(void);