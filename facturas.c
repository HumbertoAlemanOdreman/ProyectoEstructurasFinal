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

void CreateFacturaList(FacturaNode** list, Factura data) {
    if (*list != NULL) { return; }
    (*list) = MALLOC_FAC;
    (*list)->data = data;
    (*list)->next = NULL;
}

void PushFactura(FacturaNode** list, Factura data) {
    if (*list == NULL) { CreateFacturaList(list, data); return; }               // If list is NULL create it 
    FacturaNode* head = MALLOC_FAC;
    head->data = data;
    head->next = *list;
    *list = head;
}

void BuyItem(Factura* fac, ArticleNode** articleList, VendorNode** vendorList) {
    Renglon ret;
    char input[256];
    int ammount; int sale;
    ArticleNode* article; VendorNode* vendor;

    CLEAR;
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Seleccione un articulo para comprar          ||"); NL;
    TAB; printf("||                                              ||"); NL;
    TAB; printf("|| ENTER para continuar                         ||"); NL;
    TAB; printf("=================================================="); NL; 
    NL; getchar(); article = MenuArticleSelection(articleList);
    if (article->data.ammount == 0) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| No se encuentra disponible ese articulo      ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| ENTER para continuar                         ||"); NL;
        TAB; printf("=================================================="); NL; 
        NL; getchar();
        return;
    }
    if (article == NULL) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Fallo o se cancelo la seleccion del articulo ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| ENTER para continuar                         ||"); NL;
        TAB; printf("=================================================="); NL; 
        NL; getchar();
        return;
    } ret.art = article;

    CLEAR;
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Seleccione un vendedor para comprar          ||"); NL;
    TAB; printf("||                                              ||"); NL;
    TAB; printf("|| ENTER para continuar                         ||"); NL;
    TAB; printf("=================================================="); NL; 
    NL; getchar(); vendor = MenuVendorSelection(vendorList);
    if (vendor == NULL) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Fallo o se cancelo la seleccion del vendedor ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| ENTER para continuar                         ||"); NL;
        TAB; printf("=================================================="); NL; 
        NL; getchar();
        return;
    } ret.ven = vendor;

    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Ingrese Articulos a comprar (Maximo %d)      ||", article->data.ammount); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| Presione 0 para no cancelar la compra        ||"); NL;
        TAB; printf("=================================================="); NL; 
        NL;
        TAB; printf("Cantidad: "); InputString(input, "%10s");
        if (input[0] == '0') { return; }
        ammount = atoi(input);
        break;
    } 
    if (ammount > article->data.ammount) { return; }
    article->data.ammount -= ammount;

    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Ingrese el Descuento que recibio             ||"); NL;
        TAB; printf("=================================================="); NL; 
        NL;
        TAB; printf("Descuento: "); InputString(input, "%10s");
        sale = atoi(input);
        break;
    } 
    // Make the sale
    ret.Sale = sale;
    ret.Ammount = ammount;
    ret.SubTotal = ret.Ammount * article->data.price * ((100 - ret.Sale) / 100);
    vendor->data.totalEarned += ret.SubTotal * (vendor->data.commission / 100.0f);

    // Add renglon to renglon list
    PushRenglon(&(*fac).RenglonList, ret);
    (*fac).Total += ret.SubTotal;
}

void PrintSingleRenglon(RenglonNode* list) {
    if (list == NULL) { printf("Renglon is NULL\n"); }
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Renglon:                                     ||"); NL;
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Article: %34s ||", list->data.art->data.name); NL;
    TAB; printf("|| Vendedor: %33s ||", list->data.ven->data.name); NL;
    TAB; printf("|| Cantidad: %33d ||", list->data.Ammount); NL;
    TAB; printf("|| Descuento: %32f ||", list->data.Sale); NL;
    TAB; printf("|| Subtotal: %33f ||", list->data.SubTotal); NL;
    TAB; printf("=================================================="); NL;
}

void PrintRenglonList(RenglonNode* list) {
    if (list == NULL) { printf("List is NULL\n"); }
    while (list) { 
        PrintSingleRenglon(list);
        list = list->next;
    }
}

void PrintSingleFactura(Factura data) {
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Factura:                                     ||"); NL;
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Cliente: %34s ||", data.Client->data.name); NL;
    TAB; printf("|| Numero: %35d ||", data.Number); NL;
    TAB; printf("|| Total: %36d ||", data.Total); NL;
    TAB; printf("=================================================="); NL;
    PrintRenglonList(data.RenglonList);
}

void PrintFacturaList(FacturaNode* list) {
    if (list == NULL) { printf("List is NULL\n"); }
    while (list) { 
        PrintSingleFactura(list->data);
        list = list->next;
    }
}

void MenuFactura(FacturaNode** list, ArticleNode** articleList, VendorNode** vendorList, ClientNode** clientList) {
    char input[256];
    int posicion;
    Factura aux; 
    ClientNode* cliAux;
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
                CLEAR;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| Seleccione el cliente que realizara la compra||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| ENTER para continuar                         ||"); NL;
                TAB; printf("=================================================="); NL; 
                NL; getchar(); cliAux = MenuClientSelection(clientList);
                if (cliAux == NULL) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| Fallo o se cancelo la seleccion del cliente  ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| ENTER para continuar                         ||"); NL;
                    TAB; printf("=================================================="); NL; 
                    NL; getchar();
                    break;
                } 
                aux.Client = cliAux;
                aux.RenglonList = NULL;
                aux.Number = GetMaxNum(*list);
                aux.Total = 0;
                while (1) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| Desea comprar un Articulo?                   ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| 1. Comprar nuevo Articulo                    ||"); NL;
                    TAB; printf("|| 0. Terminar la Factura                       ||"); NL   ;
                    TAB; printf("=================================================="); NL;
                    NL;
                    TAB; printf("Seleccion: "); InputString(input, "%3s");
                    if (input[0] == '1') { 
                        BuyItem(&aux, articleList, vendorList);
                    }
                    if (input[0] == '0') { break; }
                }
                SaveFileArticle(*articleList, "Articulos.txt");
                SaveFileVendor(*vendorList, "Vendedores.txt");
                PushFactura(list, aux);
                PrintSingleFactura(aux); getchar();
                break;
            case '5':
                PrintFacturaList(*list);
                getchar();
                break;
        }
    }
}
