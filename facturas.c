#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "articles.h"
#include "vendors.h"

#ifndef _DATE_H
#define _DATE_H
#include "date.h"
#endif

#include "clients.h"
#include "definitions.h"
#include "input.h"

#ifndef NULL_DATE
#define NULL_DATE CreateDate(0, 0, 0)
#endif 

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
    float Total;
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

void PrintSingleRenglon(Renglon list) {
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Renglon:                                     ||"); NL;
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Article: %34s ||", list.art->data.name); NL;
    TAB; printf("|| Vendedor: %33s ||", list.ven->data.name); NL;
    TAB; printf("|| Cantidad: %33d ||", list.Ammount); NL;
    TAB; printf("|| Descuento: %32f ||", list.Sale); NL;
    TAB; printf("|| Subtotal: %33f ||", list.SubTotal); NL;
    TAB; printf("=================================================="); NL;
}

void PrintRenglonList(RenglonNode* list) {
    if (list == NULL) { printf("List is NULL\n"); }
    while (list) { 
        PrintSingleRenglon(list->data);
        list = list->next;
    }
}

void PrintSingleFactura(Factura data) {
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Factura:                                     ||"); NL;
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Cliente: %34s ||", data.Client->data.name); NL;
    TAB; printf("|| Numero: %35d ||", data.Number); NL;
    TAB; printf("|| Total: %36f ||", data.Total); NL;
    TAB; printf("=================================================="); NL;
    PrintRenglonList(data.RenglonList);
}

RenglonNode* PreviousRenglon(RenglonNode *list, RenglonNode *data) {
  if (list == NULL) { return NULL; }
  if (list == data) { return NULL; }
  while (list->next != NULL) {
    if (list->next == data) { return list; }
    list = list->next;
  } return NULL;
};

FacturaNode* PreviousFactura(FacturaNode *list, FacturaNode *data) {
  if (list == NULL) { return NULL; }
  if (list == data) { return NULL; }
  while (list->next != NULL) {
    if (list->next == data) { return list; }
    list = list->next;
  } return NULL;
};

void PrintFacturaList(FacturaNode *list) {
    char tmp[4];
    if (list == NULL) { return; }
    FacturaNode* nodo_inicial = list;
    FacturaNode* aux = list;

    while (list != NULL) {
        CLEAR;
        PrintSingleFactura(list->data);
        printf("\n");
        if (PreviousFactura(nodo_inicial, list) != NULL) { TAB; printf("[P] Factura Previa\n"); }
        if (list->next != NULL) { TAB; printf("[S] Factura Siguiente\n"); }
        TAB; printf("[T] Terminar Visualizacion\n");
        InputString(tmp, "%2s");
        switch (tmp[0]) {
            case 'T':
            case 't':
            return;
            break;
        case 'S':
        case 's':
            if (list->next == NULL) { break; }
            list = list->next;
            break;
        case 'P':
        case 'p':
            aux = PreviousFactura(nodo_inicial, list);
            if (aux != NULL) { list = aux; }
            break;
        }
    }
};

FacturaNode* GetFacturaFromPosition(FacturaNode** list, int position) {
    if (*list == NULL) { return NULL; }
    if ((*list)->next == NULL || position == 0) { return *list; }
    FacturaNode* aux = *list;
    if (position == LAST) { while (aux->next != NULL) { aux = aux->next; } return aux; }
    while (aux->next != NULL && position-- > 0) { aux = aux->next; }
    return aux;
}

RenglonNode* GetRenglonFromPosition(RenglonNode** list, int position) {
    if (*list == NULL) { return NULL; }
    if ((*list)->next == NULL || position == 0) { return *list; }
    RenglonNode* aux = *list;
    if (position == LAST) { while (aux->next != NULL) { aux = aux->next; } return aux; }
    while (aux->next != NULL && position-- > 0) { aux = aux->next; }
    return aux;
}

FacturaNode* LookForFacturas(FacturaNode* list, ClientNode* client) {
    FacturaNode* loop = list;
    FacturaNode* ret = NULL;
    while (loop) {
        if (strcmp(loop->data.Client->data.name, client->data.name)) { loop = loop->next; continue; }
        if (strcmp(loop->data.Client->data.ci, client->data.ci)) { loop = loop->next; continue; }
        if (strcmp(loop->data.Client->data.address, client->data.address)) { loop = loop->next; continue; }
        if (strcmp(loop->data.Client->data.cellphone, client->data.cellphone)) { loop = loop->next; continue; }
        PushFactura(&ret, loop->data); loop = loop->next;
    } return ret;
}

RenglonNode* LookForRenglons(RenglonNode* list, ArticleNode* article, VendorNode* vendor) {
    RenglonNode* loop = list;
    RenglonNode* ret = NULL;
    while (loop) {
        if (article != NULL && strcmp(article->data.code, loop->data.art->data.code)) { loop = loop->next; continue; }
        if (vendor != NULL && strcmp(vendor->data.ci, loop->data.ven->data.ci)) { loop = loop->next; continue; }
        PushRenglon(&ret, loop->data); loop = loop->next;
    } return ret;
}

RenglonNode* SelectRenglonFromList(RenglonNode *list) {
    char tmp[4];
    if (list == NULL) { return NULL; }
    RenglonNode* nodo_inicial = list;
    RenglonNode* aux = list;

    while (list != NULL) {
        CLEAR;
        PrintSingleRenglon(list->data);
        printf("\n");
        if (PreviousRenglon(nodo_inicial, list) != NULL) { TAB; printf("[P] Renglon Previo\n"); }
        if (list->next != NULL) { TAB; printf("[S] Renglon Siguiente\n"); }
        TAB; printf("[T] Seleccionar\n");
        InputString(tmp, "%2s");
        switch (tmp[0]) {
            case 'T':
            case 't':
            return list;
            break;
        case 'S':
        case 's':
            if (list->next == NULL) { break; }
            list = list->next;
            break;
        case 'P':
        case 'p':
            aux = PreviousRenglon(nodo_inicial, list);
            if (aux != NULL) { list = aux; }
            break;
        }
    } return NULL;
};

FacturaNode* SelectFacturaFromList(FacturaNode *list) {
    char tmp[4];
    if (list == NULL) { return NULL; }
    FacturaNode* nodo_inicial = list;
    FacturaNode* aux = list;

    while (list != NULL) {
        CLEAR;
        PrintSingleFactura(list->data);
        printf("\n");
        if (PreviousFactura(nodo_inicial, list) != NULL) { TAB; printf("[P] Factura Previa\n"); }
        if (list->next != NULL) { TAB; printf("[S] Factura Siguiente\n"); }
        TAB; printf("[T] Seleccionar\n");
        InputString(tmp, "%2s");
        switch (tmp[0]) {
            case 'T':
            case 't':
            return list;
            break;
        case 'S':
        case 's':
            if (list->next == NULL) { break; }
            list = list->next;
            break;
        case 'P':
        case 'p':
            aux = PreviousFactura(nodo_inicial, list);
            if (aux != NULL) { list = aux; }
            break;
        }
    } return NULL;
};

RenglonNode* MenuRenglonSelection(RenglonNode** list, ArticleNode** articleList, VendorNode** vendorList) {
    char input[256];
    RenglonNode* aux_node;
    VendorNode* aux_vendor;
    ArticleNode* aux_article;
    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Seleccione una Modalidad                     ||"); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| 1. Seleccionar por posicion en la lista      ||"); NL;
        TAB; printf("|| 2. Seleccionar por vendedor                  ||"); NL;
        TAB; printf("|| 3. Seleccionar por articulo                  ||"); NL;
        TAB; printf("|| 4. Seleccionar con interfaz grafica          ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| 0. Regresar al Menu Principal                ||"); NL;
        TAB; printf("=================================================="); NL;
        NL;
        TAB; printf("Seleccion: "); InputString(input, "%2s");
        if (input[0] == '0') { return NULL; }
        if (input[0] == '1') {
            while (1) {
                CLEAR;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| Ingrese la posicion del Renglon              ||"); NL;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| 'Inicio' para seleccionar la primera         ||"); NL;
                TAB; printf("|| 'Final' para seleccionar la ultima           ||"); NL;
                TAB; printf("|| 'Numero' Para seleccionar la X posicion      ||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| 0. Seleccionar de otra manera                ||"); NL;
                TAB; printf("=================================================="); NL;
                NL;
                TAB; printf("Posicion: "); InputString(input, "%10s");
                if (input[0] == '0') { break; }
                if (!strcmp(input, "Inicio")) { return GetRenglonFromPosition(list, ZERO); }
                if (!strcmp(input, "Final")) { return GetRenglonFromPosition(list, LAST); }
                if (atoi(input) > 0) { return GetRenglonFromPosition(list, atoi(input)); }
            }
        }
        if (input[0] == '2') {
            aux_vendor = MenuVendorSelection(vendorList);
            aux_node = LookForRenglons(*list, NULL, *vendorList);
            if (aux_node == NULL) {
                CLEAR;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| El renglon no se encontro en la lista        ||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| ENTER para continuar                         ||"); NL;
                TAB; printf("=================================================="); NL; 
                NL; getchar();
                continue;
            } return aux_node;
        }
        if (input[0] == '3') {
            aux_article = MenuArticleSelection(articleList);
            aux_node = LookForRenglons(*list, *articleList, NULL);
            if (aux_node == NULL) {
                CLEAR;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| El renglon no se encontro en la lista        ||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| ENTER para continuar                         ||"); NL;
                TAB; printf("=================================================="); NL; 
                NL; getchar();
                continue;
            } return aux_node;
        }
        if (input[0] == '4') {
            aux_node = SelectRenglonFromList(*list);
            if (aux_node == NULL) {
                CLEAR;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| El renglon no se encontro en la lista        ||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| ENTER para continuar                         ||"); NL;
                TAB; printf("=================================================="); NL; 
                NL; getchar();
                continue;
            } return aux_node;
        }

    } return NULL;
}

FacturaNode* MenuFacturaSelection(FacturaNode** list, ClientNode** clientList) {
    char input[256];
    FacturaNode* aux_node;
    ClientNode* aux_client;
    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Seleccione una Modalidad                     ||"); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| 1. Seleccionar por posicion en la lista      ||"); NL;
        TAB; printf("|| 2. Seleccionar por cliente                   ||"); NL;
        TAB; printf("|| 3. Seleccionar con interfaz grafica          ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| 0. Regresar al Menu Principal                ||"); NL;
        TAB; printf("=================================================="); NL;
        NL;
        TAB; printf("Seleccion: "); InputString(input, "%2s");
        if (input[0] == '0') { return NULL; }
        if (input[0] == '1') {
            while (1) {
                CLEAR;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| Ingrese la posicion de la Factura            ||"); NL;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| 'Inicio' para seleccionar la primera         ||"); NL;
                TAB; printf("|| 'Final' para seleccionar la ultima           ||"); NL;
                TAB; printf("|| 'Numero' Para seleccionar la X posicion      ||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| 0. Seleccionar de otra manera                ||"); NL;
                TAB; printf("=================================================="); NL;
                NL;
                TAB; printf("Posicion: "); InputString(input, "%10s");
                if (input[0] == '0') { break; }
                if (!strcmp(input, "Inicio")) { return GetFacturaFromPosition(list, ZERO); }
                if (!strcmp(input, "Final")) { return GetFacturaFromPosition(list, LAST); }
                if (atoi(input) > 0) { return GetFacturaFromPosition(list, atoi(input)); }
            }
        }
        if (input[0] == '2') {
            aux_client = MenuClientSelection(clientList);
            aux_node = LookForFacturas(*list, *clientList);
            if (aux_node == NULL) {
                CLEAR;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| La factura no se encontro en la lista        ||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| ENTER para continuar                         ||"); NL;
                TAB; printf("=================================================="); NL; 
                NL; getchar();
                continue;
            } return SelectFacturaFromList(aux_node);
        }
        if (input[0] == '3') {
            aux_node = SelectFacturaFromList(*list);
            if (aux_node == NULL) {
                CLEAR;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| La factura no se encontro en la lista        ||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| ENTER para continuar                         ||"); NL;
                TAB; printf("=================================================="); NL; 
                NL; getchar();
                continue;
            } return aux_node;
        }
    } return NULL;
}

void MenuModificacionFactura(FacturaNode** list, ArticleNode** articleList, VendorNode** vendorList, ClientNode** clientList) {
    char input[256];
    FacturaNode* aux_node;
    RenglonNode* aux_renglon;
    ClientNode* aux_client;
    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Seleccione el campo a modificar              ||"); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| 1. Cambiar de cliente (Debe estar registrado)||"); NL;
        TAB; printf("|| 2. Cambiar un Renglon                        ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| 0. Regresar al Menu Principal                ||"); NL;
        TAB; printf("=================================================="); NL;
        NL;
        TAB; printf("Seleccion: "); InputString(input, "%2s");
        if (input[0] == '0') { return; }
        if (input[0] == '1') {
            aux_client = MenuClientSelection(clientList);
            if (aux_node == NULL) {
                CLEAR;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| El cliente no se encontro en la lista        ||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| ENTER para continuar                         ||"); NL;
                TAB; printf("=================================================="); NL; 
                NL; getchar();
                continue;
            } (*list)->data.Client = aux_client;
            return;
        }
        if (input[0] == '2') {
            aux_renglon = MenuRenglonSelection(&(*list)->data.RenglonList, articleList, vendorList);
            while (1) {
                CLEAR;
                PrintSingleRenglon(aux_renglon->data); NL;
                TAB; printf("=================================================="); NL;
                TAB; printf("|| Seleccione el campo a modificar              ||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| 1. Modificar informacion del Articulo        ||"); NL;
                TAB; printf("|| 2. Modificar informacion del Vendedor        ||"); NL;
                TAB; printf("|| 3. Modificar el descuento recibido           ||"); NL;
                TAB; printf("|| 4. Modificar la cantidad comprada            ||"); NL;
                TAB; printf("||                                              ||"); NL;
                TAB; printf("|| 0. Regresar al Menu de Modificacion          ||"); NL;
                TAB; printf("=================================================="); NL;
                NL;
                TAB; printf("Seleccion: "); InputString(input, "%2s");
                switch (input[0]) {
                    case '0': return;
                    case '1':
                        ModifyArticle(*articleList, aux_renglon->data.art);
                        break;
                    case '2':
                        ModifyVendor(*vendorList, aux_renglon->data.ven);
                        break;
                    case '3':
                        strcpy(input, "");
                        while (atoi(input) <= 0) {
                            CLEAR;
                            PrintSingleRenglon(aux_renglon->data);
                            TAB; printf("Ingrese el nuevo descuento del Renglon (ZERO para cancelar): "); InputString(input, "%29s");
                            if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
                        }
                        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
                        if (atoi(input) < 0 || atoi(input) > 100) {
                            CLEAR;
                            TAB; printf("=================================================="); NL;
                            TAB; printf("|| El descuento de la compra debe estar entre   ||"); NL;
                            TAB; printf("|| 0 y 100                                      ||"); NL;
                            TAB; printf("||                                              ||"); NL;
                            TAB; printf("|| ENTER para continuar                         ||"); NL;
                            TAB; printf("=================================================="); NL; 
                            NL; getchar();
                            break;
                        }
                        aux_renglon->data.SubTotal = (aux_renglon->data.SubTotal / aux_renglon->data.Sale) * atof(input);
                        aux_renglon->data.Sale = atof(input);
                        break;
                    case '4':
                        strcpy(input, "");
                        while (atoi(input) <= 0) {
                            CLEAR;
                            PrintSingleRenglon(aux_renglon->data);
                            TAB; printf("Ingrese la nueva cantidad de Items Comprados (ZERO para cancelar): "); InputString(input, "%29s");
                            if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
                        }
                        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
                        if (atoi(input) < 0) {
                            CLEAR;
                            TAB; printf("=================================================="); NL;
                            TAB; printf("|| La cantidad de Items no puede ser menor a 0  ||"); NL;
                            TAB; printf("||                                              ||"); NL;
                            TAB; printf("|| ENTER para continuar                         ||"); NL;
                            TAB; printf("=================================================="); NL; 
                            NL; getchar();
                            break;
                        }
                        aux_renglon->data.SubTotal = (aux_renglon->data.SubTotal / aux_renglon->data.Ammount) * atof(input);
                        aux_renglon->data.Ammount = atof(input);
                        break;
                }
            }
        }
    } return;
}

int GetPositionFactura(FacturaNode* list, FacturaNode* data) {
    if (list == NULL || data == NULL || list == data) { return 0; }
    int position = 0;
    while(list->next != NULL) {
        if (list->next == data) { return position; }
        list = list->next; position++;
    } return -1;
}

void RemovePositionRenglon(RenglonNode** list, int position) {
    if (*list == NULL) { return; }
    if ((*list)->next == NULL) { 
        free(*list);
        *list = NULL;
        return;
    }

    RenglonNode *aux = *list;
    RenglonNode *aux_2 = MALLOC_REN;

    if (position == ZERO) {
        aux = (*list)->next;
        free(*list);
        *list = aux;
        return;
    }
    
    if (position == LAST) {
        while (aux->next != NULL) { aux = aux->next; }
        aux_2 = aux->next;
        aux = PreviousRenglon(*list, aux);
        free(aux->next);
        aux->next = aux_2;
        return;
    }

    while (aux->next != NULL && position-- > 0) { aux = aux->next; }
    aux_2 = aux->next;
    aux = PreviousRenglon(*list, aux);
    free(aux->next);
    aux->next = aux_2;
    return;
}

void RemovePositionFactura(FacturaNode** list, int position) {
    if (*list == NULL) { return; }
    if ((*list)->next == NULL) { 
        free(*list);
        *list = NULL;
        return;
    }

    FacturaNode *aux = *list;
    FacturaNode *aux_2 = MALLOC_FAC;

    if (position == ZERO) {
        aux = (*list)->next;
        free(*list);
        *list = aux;
        return;
    }
    
    if (position == LAST) {
        while (aux->next != NULL) { aux = aux->next; }
        aux_2 = aux->next;
        aux = PreviousFactura(*list, aux);
        free(aux->next);
        aux->next = aux_2;
        return;
    }

    while (aux->next != NULL && position-- > 0) { aux = aux->next; }
    aux_2 = aux->next;
    aux = PreviousFactura(*list, aux);
    while(aux->next->data.RenglonList != NULL) { RemovePositionRenglon(&aux->next->data.RenglonList, ZERO); }
    free(aux->next);
    aux->next = aux_2;
    return;
}

void SaveFileFactura(FacturaNode* list, const char dir[]) {
    FILE *f; f = fopen(dir, "w");
    if (f == NULL) { fclose(f); return; }
    if (list == NULL) { fprintf(f, ""); return; } 
    FacturaNode* aux = list;
    RenglonNode* renglon_aux;
    while (aux) {
        fprintf(f, "%s ", aux->data.Client->data.ci);
        fprintf(f, "%d ", aux->data.Number);
        fprintf(f, "%f ", aux->data.Total);
        renglon_aux = aux->data.RenglonList;
        while (renglon_aux) {                                   // Loop para los renglones
            fprintf(f, "%s ", renglon_aux->data.art->data.code); // Save article code
            fprintf(f, "%s ", renglon_aux->data.ven->data.ci);   // Save vendor CI
            fprintf(f, "%f ", renglon_aux->data.Sale);           // Save discount from sale
            fprintf(f, "%d ", renglon_aux->data.Ammount);        // Save ammount of product bought
            fprintf(f, "%f ", renglon_aux->data.SubTotal);        // Save subtotal of renglon
            renglon_aux = renglon_aux->next;
        } fprintf(f, "ENDOFFACTURA\n");
        aux = aux->next;
    } fclose(f);
}

void ReadFileFactura(FacturaNode** list, ArticleNode* articleList, VendorNode* vendorList, ClientNode* clientList, const char dir[]) {
    FILE *f; f = fopen(dir, "r");
    if (f == NULL) { fclose(f); return; }
    Factura aux;
    aux.RenglonList = NULL;
    char buffer[256];
    Renglon renglon_aux;
    while (1) {
        if (fscanf(f, "%s", buffer) == EOF) { break; }
        aux.Client = LookForClient(clientList, NULL, buffer, NULL, NULL);
        PrintSingleClient(aux.Client->data); getchar();
        if (fscanf(f, "%d", &aux.Number) == EOF) { break; }
        if (fscanf(f, "%f", &aux.Total) == EOF) { break; }
        aux.RenglonList = NULL;
        while (1) {
            if (fscanf(f, "%s", buffer) == EOF) { break; }
            if (!strcmp(buffer, "ENDOFFACTURA")) {
                if (aux.Client == NULL) { break; }
                PushFactura(list, aux);
                break;
            }
            renglon_aux.art = LookForArticle(articleList, buffer, NULL, ZERO, ZERO);
            if (fscanf(f, "%s", buffer) == EOF) { break; } // Save article code
            renglon_aux.ven = LookForVendor(vendorList, NULL, buffer, NULL_DATE, ZERO);
            if (fscanf(f, "%f", &renglon_aux.Sale) == EOF) { break; }
            if (fscanf(f, "%d", &renglon_aux.Ammount) == EOF) { break; }
            if (fscanf(f, "%f", &renglon_aux.SubTotal) == EOF) { break; }
            if (renglon_aux.art == NULL || renglon_aux.ven == NULL) { continue; }
            PrintSingleRenglon(renglon_aux); getchar();
            PushRenglon(&aux.RenglonList, renglon_aux);
        }
    } fclose(f);
}

void MenuFactura(FacturaNode** list, ArticleNode** articleList, VendorNode** vendorList, ClientNode** clientList) {
    char input[256];
    int posicion;
    Factura aux; FacturaNode* aux_node;
    ClientNode* cliAux;
    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Menu Manejo Facturas                         ||"); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| 1. Crear una Factura                         ||"); NL;
        TAB; printf("|| 2. Eliminar una Factura                      ||"); NL;
        TAB; printf("|| 3. Modificar una Factura                     ||"); NL;
        TAB; printf("|| 4. Leer lista de Facturas                    ||"); NL;
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

                if (aux.RenglonList == NULL) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| El cliente no ha comprado nada               ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| ENTER para continuar                         ||"); NL;
                    TAB; printf("=================================================="); NL; 
                    NL; getchar();
                    break;
                }
                PushFactura(list, aux);
                SaveFileArticle(*articleList, "Articulos.txt");
                SaveFileVendor(*vendorList, "Vendedores.txt");
                SaveFileClient(*clientList, "Clientes.txt");
                SaveFileFactura(*list, "Facturas.txt");
                PrintSingleFactura(aux); getchar();
                break;
            case '2':
                if (*list == NULL) { NL; TAB; printf("La lista esta vacia"); NL; getchar(); break; }
                aux_node = MenuFacturaSelection(list, clientList);
                if (aux_node == NULL) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| Ha fallado la seleccion de la factura        ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| ENTER para continuar                         ||"); NL;
                    TAB; printf("=================================================="); NL; 
                    NL; getchar();
                    break;
                } RemovePositionFactura(list, GetPositionFactura(*list, aux_node));
                SaveFileArticle(*articleList, "Articulos.txt");
                SaveFileVendor(*vendorList, "Vendedores.txt");
                SaveFileClient(*clientList, "Clientes.txt");
                SaveFileFactura(*list, "Facturas.txt");
                break;
            case '3':
                if (*list == NULL) { NL; TAB; printf("La lista esta vacia"); NL; getchar(); break; }
                aux_node = MenuFacturaSelection(list, clientList);
                if (aux_node == NULL) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| Ha fallado la seleccion de la factura        ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| ENTER para continuar                         ||"); NL;
                    TAB; printf("=================================================="); NL; 
                    NL; getchar();
                    break;
                } MenuModificacionFactura(&aux_node, articleList, vendorList, clientList);
                SaveFileArticle(*articleList, "Articulos.txt");
                SaveFileVendor(*vendorList, "Vendedores.txt");
                SaveFileClient(*clientList, "Clientes.txt");
                SaveFileFactura(*list, "Facturas.txt");
                break;
            case '4':
                if (*list == NULL) { NL; TAB; printf("La lista esta vacia"); NL; getchar(); break; }
                PrintFacturaList(*list);
                break;
        }
    }
}

