#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "articles.h"
#include "vendors.h"
#include "clients.h"
#include "definitions.h"

typedef struct Renglon {
    ArticleNode* art;
    VendorNode* ven;
    float Sale;
    float SubTotal;
    int Ammount;
} Renglon;

typedef struct RenglonNode {
    Renglon data;
    struct RenglonNode* next;
} RenglonNode;

void CreateRenglonList(RenglonNode** list, const Renglon data) {
    if (*list != NULL) { return; }
    (*list) = MALLOC_REN;
    (*list)->data = data;
    (*list)->next = NULL;
}

void PushRenglon(RenglonNode** list, const Renglon data) {
    if (*list == NULL) { CreateRenglonList(list, data); return; }               // If list is NULL create it 
    RenglonNode* head = MALLOC_REN;
    head->data = data;
    head->next = *list;
    *list = head;
}

typedef struct Factura {
    int Number;
    RenglonNode* RenglonList;
    ClientNode* Client;
    int Total;
} Factura;

typedef struct FacturaNode {
    Factura data;
    struct FacturaNode* next;
} FacturaNode;

int GetMaxNum(FacturaNode* list) {
    if (list == NULL) { return 0; }
    int max = -1;
    while (list != NULL) {
        if (list->data.Number > max) { max = list->data.Number; }
        list = list->next;
    } return ++max;
}

void CreateFacturaList(FacturaNode** list, ClientNode* client) {
    if (*list != NULL) { return; }
    (*list) = MALLOC_FAC;
    (*list)->data.Total = 0;
    (*list)->data.Number = 0;
    (*list)->data.Client = client;
    (*list)->data.RenglonList = NULL;
    (*list)->next = NULL;
}

void PushFactura(FacturaNode** list, const Factura data) {
    if (*list == NULL) { CreateFacturaList(list, data.Client); return; }               // If list is NULL create it 
    FacturaNode* head = MALLOC_FAC;
    head->data = data;
    head->next = *list;
    *list = head;
}


// This functions generates a Renglon to be later added to a list to then be added to the Factura
void BuyItem(FacturaNode** list, ArticleNode* article, VendorNode* vendor, const int ammount, const int sale) {
    Renglon ret;

    // We test if there's enough items to buy, if there aren't don't buy
    if (ammount > article->data.ammount) { return; }
    article->data.ammount -= ammount;

    // Make the sale
    ret.Sale = sale;
    ret.Ammount = ammount;
    ret.SubTotal = ret.Ammount * article->data.price * ret.Sale;
    vendor->data.totalEarned += ret.SubTotal * (vendor->data.commission / 100.0f);

    // Add metadata to the sale
    if (article == NULL) { return; } ret.art = article;
    if (vendor == NULL) { return; } ret.ven = vendor;

    // Add renglon to renglon list
    PushRenglon(&(*list)->data.RenglonList, ret);
    (*list)->data.Total += ret.SubTotal;
}

void PrintSingleRenglon(RenglonNode* list) {
    if (list == NULL) { printf("Renglon is NULL\n"); }
    printf("Renglon:\n");
    printf("Article: %s\n", list->data.art->data.name);
    printf("Vendor: %s\n", list->data.ven->data.name);
    printf("Ammount: %d\n", list->data.Ammount);
    printf("Sale: %f\n", list->data.Sale);
    printf("Subtotal: %f\n", list->data.SubTotal);

}

void PrintRenglonList(RenglonNode* list) {
    if (list == NULL) { printf("List is NULL\n"); }
    while (list) { 
        PrintSingleRenglon(list);
        list = list->next;
    }
}

void PrintSingleFactura(FacturaNode* list) {
    if (list == NULL) { printf("Article is NULL\n"); }
    printf("Factura:\n");
    printf("Client: %s\n", list->data.Client->data.name);
    printf("Number: %d\n", list->data.Number);
    printf("Total: %d\n", list->data.Total);
    printf("\n");
    PrintRenglonList(list->data.RenglonList);
}

void PrintFacturaList(FacturaNode* list) {
    if (list == NULL) { printf("List is NULL\n"); }
    while (list) { 
        PrintSingleFactura(list);
        list = list->next;
    }
}
