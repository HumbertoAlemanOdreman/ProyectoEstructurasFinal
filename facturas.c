#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "articles.h"
#include "vendors.h"
#include "clients.h"
#include "definitions.h"
#include "input.h"

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

ClientNode* GetBuyerClient(ClientNode** list) {
    char input[256]; int position;
    Client aux;
    while (1) {
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Que cliente esta comprando?                  ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| 1. Crear un cliente                          ||"); NL;
        TAB; printf("|| 2. Seleccionar cliente por posisicion        ||"); NL;
        TAB; printf("|| 3. Seleccionar cliente por campo             ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| 0. Regresar al menu de Facturas              ||");
        TAB; printf("=================================================="); NL;
        NL;
        TAB; printf("Seleccion: "); InputString(input, "%3s");

        if (input[0] == '0') { return NULL; }
        if (input[0] == '1') {
            aux = InputCreateClient();
            if (LookForClient(*list, NULL, aux.ci, ZERO, ZERO) != NULL) {
                TAB; printf("El Cliente ya se encuentra dentro de la lista"); NL;
                return NULL;
            } AppendClient(list, aux);
        }
        if (input[0] == '2') {
            while (1) {
                CLEAR;
                TAB; printf("Ingrese la posicion donde Eliminar el Cliente: "); NL;
                TAB; printf("'Inicio' para seleccionar la primera posicion"); NL;
                TAB; printf("'Final' para seleccionar la ultima posicion"); NL; NL;
                TAB; printf("Posicion: "); InputString(input, "%10s");
                if (atoi(input) > 0) { position = atoi(input); break; }
                if (!strcmp(input, "Inicio")) { position =  ZERO; break; }
                if (!strcmp(input, "Final")) { position = LAST; break; }
            } return GetClientFromPosition(list, position);
        }
        if (input[0] == '3') { return SearchClient(*list, 1); }
    } return NULL;
}

void MenuArticle(ArticleNode** list) {
    char input[256];
    int posicion;
    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Menu Manejo Articulos                        ||"); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| 1. Crear una Factura                         ||"); NL;
        TAB; printf("|| 2. Eliminar una Factura                      ||"); NL;
        TAB; printf("|| 3. Modificar una Factura                     ||"); NL;
        TAB; printf("|| 4. Buscar Factura en la lista                ||"); NL;
        TAB; printf("|| 5. Leer lista de Facturas                    ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| 0. Regresar al Menu Principal                ||"); NL;
        TAB; printf("=================================================="); NL;
        NL;
        TAB; printf("Seleccion: "); InputString(input, "%2s");
        switch (input[0]) {
            case '0':
                return;
                break;
            case '1':
                break;
        }
    }
}
