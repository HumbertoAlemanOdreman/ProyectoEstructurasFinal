#include "definitions.h"

#ifndef _VENDORS_H
#define _VENDORS_H
typedef struct Date {
    int day;
    int month;
    int year;
} Date;

typedef struct Vendor {
    char name[COL_SIZE];
    char ci[COL_SIZE];
    Date date;
    float commission;
    float totalEarned;
} Vendor;

typedef struct VendorNode {
    Vendor data;
    struct VendorNode* next;
} VendorNode;
#endif

void MenuVendor(VendorNode** list);
void ReadFileVendor(VendorNode** list, const char dir[]);
void SaveFileVendor(VendorNode* list, const char dir[]);
