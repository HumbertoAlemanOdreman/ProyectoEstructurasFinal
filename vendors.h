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

Vendor CreateVendor(const char name[], const char ci[], const Date date, const float commission);
void CreateVendorList(VendorNode** list, const Vendor data);
VendorNode* LookForVendor(VendorNode* list, const char name[], const char ci[], const Date date, const float commission);
void PushVendor(VendorNode** list, const Vendor data);
void AppendVendor(VendorNode** list, const Vendor data);
void InsertVendor(VendorNode** list, const Vendor data, int position);
VendorNode* PreviousVendor(VendorNode *list, VendorNode *vendor);
VendorNode* LookForVendors(VendorNode* list, const char name[], const char ci[], const Date date, const float commission);
void PrintSingleVendor(Vendor list);
void PrintVendorList(struct VendorNode* list);
VendorNode* SelectVendorFromList(struct VendorNode* list);
void ReadFileVendor(VendorNode** list, const char dir[]);
void SaveFileVendor(VendorNode* list, const char dir[]);
int GetPositionVendor(VendorNode* list, VendorNode* data);
VendorNode* GetVendorFromPosition(VendorNode** list, int position);
void RemovePositionVendor(VendorNode** list, int position);
Vendor InputCreateVendor();
VendorNode* SearchVendor(VendorNode* list, int single);
void ModifyVendor(VendorNode* globalList, VendorNode* list);
VendorNode* MenuVendorSelection(VendorNode** list);
void MenuVendor(VendorNode** list);
