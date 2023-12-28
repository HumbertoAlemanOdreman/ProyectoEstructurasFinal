#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_VEN (struct VendorNode *)malloc(sizeof(struct VendorNode))

#ifndef ZERO
#define ZERO 0
#endif

#ifndef LAST
#define LAST -1
#endif

#ifndef COL_SIZE
#define COL_SIZE 30
#endif

#define NULL_DATE CreateDate(0, 0, 0)

typedef struct Date {
    int day;
    int month;
    int year;
} Date;

typedef struct Vendor {
    char name[COL_SIZE];
    char ci[COL_SIZE];
    Date date;
    int commission;
} Vendor;

typedef struct VendorNode {
    Vendor data;
    struct VendorNode* next;
} VendorNode;

Date CreateDate(const int day, const int month, const int year) {
    // Add validation for date
    Date ret;
    ret.day = day;
    ret.month = month;
    ret.year = year;
    return ret;
}

void CopyDate(Date* date, const Date data) {
    (*date).day = data.day;
    (*date).month = data.month;
    (*date).year = data.year;
}

Vendor CreateVendor(const char name[], const char ci[], const Date date, const int commission) {
    // TODO: Add validation for shit
    Vendor ret;
    strcpy(ret.name, name);
    strcpy(ret.ci, ci);
    CopyDate(&ret.date, date);
    ret.commission = commission;
    return ret;
}

void CreateVendorList(VendorNode** list, const Vendor data) {
    if (*list != NULL) { return; }
    (*list) = MALLOC_VEN;
    (*list)->data = data;
    (*list)->next = NULL;
}

VendorNode* LookForVendor(VendorNode* list, const char name[], const char ci[], const Date date, const int commission) {
    // NULL a string to skip
    // ZERO a value to skip
    VendorNode* aux = list;
    VendorNode* loop = list;
    while (loop) {
        if (name != NULL && strcmp(loop->data.name, name)) { loop = loop->next; continue; }
        if (ci != NULL && strcmp(loop->data.ci, ci)) { loop = loop->next; continue; }
        if (date.day != ZERO && loop->data.date.day != date.day) { loop = loop->next; continue; }
        if (date.month != ZERO && loop->data.date.month != date.month) { loop = loop->next; continue; }
        if (date.year != ZERO && loop->data.date.year != date.year) { loop = loop->next; continue; }
        if (commission != ZERO && loop->data.commission != commission) { loop = loop->next; continue; }
        if (aux == loop) { return list; }                                   // If aux is loop the first item is the item we're looking for
        while (aux->next != loop) { aux = aux->next; } return aux->next;    // Else we have to find the pointer to it
    } return NULL;
}

void PushVendor(VendorNode** list, const Vendor data) {
    if (*list == NULL) { CreateVendorList(list, data); return; }               // If list is NULL create it 
    if (LookForVendor(*list, NULL, data.ci, NULL_DATE, ZERO) != NULL) { return; }   // If ci already exists don't add it
    VendorNode* head = MALLOC_VEN;
    head->data = data;
    head->next = *list;
    *list = head;
}

void AppendVendor(VendorNode** list, const Vendor data) {
    if (*list == NULL) { CreateVendorList(list, data); return; }               // If list is NULL create it 
    if (LookForVendor(*list, NULL, data.ci, NULL_DATE, ZERO) != NULL) { return; }   // If ci already exists don't add it
    VendorNode* loop = *list;
    while(loop->next) { loop = loop->next; }
    loop->next = MALLOC_VEN;
    loop->next->data = data;
    loop->next->next = NULL;
}

void InsertVendor(VendorNode** list, const Vendor data, int position) {
    if (*list == NULL) { CreateVendorList(list, data); return; }               // If list is NULL create it 
    if (LookForVendor(*list, NULL, data.ci, NULL_DATE, ZERO) != NULL) { return; }   // If ci already exists don't add it
    if (position == ZERO) { PushVendor(list, data); return; }                  // If position is ZERO push
    if (position == LAST) { AppendVendor(list, data); return; }                // If position is LAST append
    VendorNode* loop = *list; position--;
    while (loop->next != NULL && position-- > 0) { loop = loop->next; }
    VendorNode* aux = loop->next;
    loop->next = MALLOC_VEN; loop->next->next = aux;
    loop->next->data = data;
}

VendorNode* LookForVendors(VendorNode* list, const char name[], const char ci[], const Date date, const int commission) {
    // NULL a string to skip
    // ZERO a value to skip
    VendorNode* loop = list;
    VendorNode* ret = NULL;
    while (loop) {
        if (name != NULL && strcmp(loop->data.name, name)) { loop = loop->next; continue; }
        if (ci != NULL && strcmp(loop->data.ci, ci)) { loop = loop->next; continue; }
        if (date.day != ZERO && loop->data.date.day != date.day) { loop = loop->next; continue; }
        if (date.month != ZERO && loop->data.date.month != date.month) { loop = loop->next; continue; }
        if (date.year != ZERO && loop->data.date.year != date.year) { loop = loop->next; continue; }
        if (commission != ZERO && loop->data.commission != commission) { loop = loop->next; continue; }
        AppendVendor(&ret, loop->data); loop = loop->next;
    } return ret;
}

void PrintSingleVendor(const VendorNode* list) {
    if (list == NULL) { printf("Vendor is NULL\n"); }
    printf("Vendor:\n");
    printf("name: %s\nCI: %s\ndate: %d/%d/%d\ncommission: %d\n", list->data.name, list->data.name, list->data.date.day, list->data.date.month, list->data.date.year, list->data.commission);
}

void PrintVendorList(VendorNode* list) {
    if (list == NULL) { printf("List is NULL\n"); }
    while (list) { 
        PrintSingleVendor(list);
        list = list->next;
    }
}

void ReadFileVendor(VendorNode** list, const char dir[]) {
    FILE *f; f = fopen(dir, "r");
    if (f == NULL) { return; }           // If file is NULL return
    Vendor aux;
    while (1) {
        if (fscanf(f, "%s", aux.name) == EOF) { break; }
        if (fscanf(f, "%s", aux.ci) == EOF) { break; }
        if (fscanf(f, "%d/%d/%d", &aux.date.day, &aux.date.month, &aux.date.year) == EOF) { break; }
        if (fscanf(f, "%d", &aux.commission) == EOF) { break; }
        AppendVendor(list, aux);
    } fclose(f);
}

void SaveFileVendor(VendorNode* list, const char dir[]) {
    FILE *f; f = fopen(dir, "w");
    if (f == NULL) { fclose(f); return; }           // If file is NULL return
    if (list == NULL) { fprintf(f, ""); return; }   // If list is NULL return 
    VendorNode* aux = list;
    while (aux) {
        fprintf(f, "%s ", aux->data.name);
        fprintf(f, "%s ", aux->data.ci);
        fprintf(f, "%d/%d/%d ", aux->data.date.day, aux->data.date.month, aux->data.date.year);
        fprintf(f, "%d", aux->data.commission);
        fprintf(f, "\n");
        aux = aux->next;
    } fclose(f);
}
