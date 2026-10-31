#pragma once
#include <pthread.h>

typedef struct Date
{
    int day;
    int month;
    int year;
} Date;

extern pthread_mutex_t inventory_mutex;