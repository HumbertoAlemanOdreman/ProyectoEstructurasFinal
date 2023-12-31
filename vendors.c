#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"
#include "input.h"
#include "date.h"

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

Vendor CreateVendor(const char name[], const char ci[], const Date date, const float commission) {
    Vendor ret;
    strcpy(ret.name, name);
    strcpy(ret.ci, ci);
    CopyDate(&ret.date, date);
    ret.commission = commission;
    ret.totalEarned = 0.0f;
    return ret;
}

void CreateVendorList(VendorNode** list, const Vendor data) {
    if (*list != NULL) { return; }
    (*list) = MALLOC_VEN;
    (*list)->data = data;
    (*list)->next = NULL;
}

VendorNode* LookForVendor(VendorNode* list, const char name[], const char ci[], const Date date, const float commission) {
    VendorNode* aux = list;
    VendorNode* loop = list;
    while (loop) {
        if (name != NULL && strcmp(loop->data.name, name)) { loop = loop->next; continue; }
        if (ci != NULL && strcmp(loop->data.ci, ci)) { loop = loop->next; continue; }
        if (date.day != ZERO && loop->data.date.day != date.day) { loop = loop->next; continue; }
        if (date.month != ZERO && loop->data.date.month != date.month) { loop = loop->next; continue; }
        if (date.year != ZERO && loop->data.date.year != date.year) { loop = loop->next; continue; }
        if (commission != ZERO && loop->data.commission != commission) { loop = loop->next; continue; }
        if (aux == loop) { return list; }
        while (aux->next != loop) { aux = aux->next; } return aux->next;
    } return NULL;
}

void PushVendor(VendorNode** list, const Vendor data) {
    if (*list == NULL) { CreateVendorList(list, data); return; }
    if (LookForVendor(*list, NULL, data.ci, NULL_DATE, ZERO) != NULL) { return; }
    VendorNode* head = MALLOC_VEN;
    head->data = data;
    head->next = *list;
    *list = head;
}

void AppendVendor(VendorNode** list, const Vendor data) {
    if (*list == NULL) { CreateVendorList(list, data); return; } 
    if (LookForVendor(*list, NULL, data.ci, NULL_DATE, ZERO) != NULL) { return; }
    VendorNode* loop = *list;
    while(loop->next) { loop = loop->next; }
    loop->next = MALLOC_VEN;
    loop->next->data = data;
    loop->next->next = NULL;
}

void InsertVendor(VendorNode** list, const Vendor data, int position) {
    if (*list == NULL) { CreateVendorList(list, data); return; }
    if (LookForVendor(*list, NULL, data.ci, NULL_DATE, ZERO) != NULL) { return; }
    if (position == ZERO) { PushVendor(list, data); return; }
    if (position == LAST) { AppendVendor(list, data); return; }
    VendorNode* loop = *list; position--;
    while (loop->next != NULL && position-- > 0) { loop = loop->next; }
    VendorNode* aux = loop->next;
    loop->next = MALLOC_VEN; loop->next->next = aux;
    loop->next->data = data;
}

VendorNode* PreviousVendor(VendorNode *list, VendorNode *vendor) {
  if (list == NULL) { return NULL; }
  if (list == vendor) { return NULL; }
  while (list->next != NULL) {
    if (list->next == vendor) { return list; }
    list = list->next;
  } return NULL;
}

VendorNode* LookForVendors(VendorNode* list, const char name[], const char ci[], const Date date, const float commission) {
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

void PrintSingleVendor(Vendor list) {
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Nombre: %35s ||", list.name); NL;
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Cedula: %35s ||", list.ci); NL;
    TAB; printf("|| Fecha: %2d/%2d/%4d                           ||", list.date.day, list.date.month, list.date.year); NL;
    TAB; printf("|| Comision: %33f ||", list.commission); NL;
    TAB; printf("|| Ganancias: %32f ||", list.totalEarned); NL;
    TAB; printf("=================================================="); NL;
}

void PrintVendorList(struct VendorNode* list) {
    char tmp[4];
    if (list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); }
    struct VendorNode* nodo_inicial = list;
    struct VendorNode* aux = list;

    while (list != NULL) {
        CLEAR;
        PrintSingleVendor(list->data);
        printf("\n");
        if (PreviousVendor(nodo_inicial, list) != NULL) { printf("  [P] Vendedor Previo\n"); }
        if (list->next != NULL) { printf("  [S] Vendedor Siguiente\n"); }
        printf("  [T] Terminar Visualizacion\n");
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
            aux = PreviousVendor(nodo_inicial, list);
            if (aux != NULL) { list = aux; }
            break;
        }
    }
};

void ReadFileVendor(VendorNode** list, const char dir[]) {
    FILE *f; f = fopen(dir, "r");
    if (f == NULL) { return; }
    Vendor aux;
    while (1) {
        if (fscanf(f, "%s", aux.name) == EOF) { break; }
        if (fscanf(f, "%s", aux.ci) == EOF) { break; }
        if (fscanf(f, "%d/%d/%d", &aux.date.day, &aux.date.month, &aux.date.year) == EOF) { break; }
        if (fscanf(f, "%f", &aux.commission) == EOF) { break; }
        if (fscanf(f, "%f", &aux.totalEarned) == EOF) { break; }
        AppendVendor(list, aux);
    } fclose(f);
}

void SaveFileVendor(VendorNode* list, const char dir[]) {
    FILE *f; f = fopen(dir, "w");
    if (f == NULL) { fclose(f); return; }
    if (list == NULL) { fprintf(f, ""); return; }
    VendorNode* aux = list;
    while (aux) {
        fprintf(f, "%s ", aux->data.name);
        fprintf(f, "%s ", aux->data.ci);
        fprintf(f, "%d/%d/%d ", aux->data.date.day, aux->data.date.month, aux->data.date.year);
        fprintf(f, "%f ", aux->data.commission);
        fprintf(f, "%f", aux->data.totalEarned);
        fprintf(f, "\n");
        aux = aux->next;
    } fclose(f);
}

int GetPositionVendor(VendorNode* list, VendorNode* data) {
    if (list == NULL || data == NULL || list == data) { return 0; }
    int position = 0;
    while(list->next != NULL) {
        if (list->next == data) { return position; }
        list = list->next; position++;
    } return -1;
}

VendorNode* GetVendorFromPosition(VendorNode** list, int position) {
    if (*list == NULL) { return NULL; }
    if ((*list)->next == NULL || position == 0) { return *list; }
    VendorNode *aux = *list;
    if (position == LAST) { while (aux->next != NULL) { aux = aux->next; } return aux; }
    while (aux->next != NULL && position-- > 0) { aux = aux->next; }
    return aux->next;
}

void RemovePositionVendor(VendorNode** list, int position) {
    if (*list == NULL) { return; }
    if ((*list)->next == NULL) { 
        free(*list);
        *list = NULL;
        return;
    }

    VendorNode *aux = *list;
    VendorNode *aux_2 = MALLOC_VEN;

    if (position == ZERO) {
        aux = (*list)->next;
        free(*list);
        *list = aux;
        return;
    }
    
    if (position == LAST) {
        while (aux->next != NULL) { aux = aux->next; }
        aux_2 = aux->next;
        aux = PreviousVendor(*list, aux);
        free(aux->next);
        aux->next = aux_2;
        return;
    }

    while (aux->next != NULL && position-- > 0) { aux = aux->next; }
    aux_2 = aux->next;
    aux = PreviousVendor(*list, aux);
    free(aux->next);
    aux->next = aux_2;
    return;
}

Vendor InputCreateVendor() {
    Vendor ret; strcpy(ret.name, ""); strcpy(ret.ci, ""); ret.date = NULL_DATE; ret.commission = 0; ret.totalEarned = 0;
    char input[256];

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleVendor(ret); NL;
        TAB; printf("Ingrese el nombre del Vendedor: "); InputString(input, "%29s");
    } strcpy(ret.name, input);

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleVendor(ret); NL;
        TAB; printf("Ingrese la Cedula de Identidad del Vendedor: "); InputString(input, "%29s");
    } strcpy(ret.ci, input);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(ret); NL;
        TAB; printf("Ingrese el dia en que se unio en Vendedor: "); InputString(input, "%29s");
    } ret.date.day = atoi(&input[0]);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(ret); NL;
        TAB; printf("Ingrese el mes en que se unio en Vendedor: "); InputString(input, "%29s");
    } ret.date.month = atoi(&input[0]);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(ret);
        TAB; printf("Ingrese el anio en que se unio en Vendedor: "); InputString(input, "%29s");
    } ret.date.year = atoi(&input[0]);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(ret); NL;
        TAB; printf("Ingrese la comision del Vendedor: "); InputString(input, "%29s");
    } ret.commission = atoi(input);

    return ret;
}

VendorNode* SearchVendor(VendorNode* list, int single) {
    Vendor ret; strcpy(ret.name, ""); strcpy(ret.ci, ""); ret.date = NULL_DATE; ret.commission = 0; ret.totalEarned = 0;
    char input[256];

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleVendor(ret);
        TAB; printf("Ingrese el nombre del Vendedor (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(ret.name, input);

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleVendor(ret);
        TAB; printf("Ingrese la Cedula de Identidad del Vendedor (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(ret.ci, input);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(ret);
        TAB; printf("Ingrese el dia en que se unio en Vendedor (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } ret.date.day = atoi(&input[0]);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(ret);
        TAB; printf("Ingrese el mes en que se unio en Vendedor (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } ret.date.day = atoi(&input[0]);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(ret);
        TAB; printf("Ingrese el anio en que se unio en Vendedor (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } ret.date.day = atoi(&input[0]);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(ret);
        TAB; printf("Ingrese la comision del Vendedor (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } ret.commission = atoi(input);

    if (single) {
        return LookForVendor(list, strcmp(ret.name, "NULL") ? ret.name : NULL, strcmp(ret.ci, "NULL") ? ret.name : NULL, ret.date, ret.commission);
    } else {
        return LookForVendors(list, strcmp(ret.name, "NULL") ? ret.name : NULL, strcmp(ret.ci, "NULL") ? ret.name : NULL, ret.date, ret.commission);
    }
}


void ModifyVendor(VendorNode* list) {
    char input[256];

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleVendor(list->data);
        TAB; printf("Ingrese el nuevo nombre del Vendedor (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(list->data.name, (strcmp(input, "NULL") ? input : list->data.name));

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleVendor(list->data);
        TAB; printf("Ingrese la nueva Cedula de Identidad del Vendedor (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(list->data.ci, (strcmp(input, "NULL") ? input : list->data.ci));

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(list->data);
        TAB; printf("Ingrese el nuevo dia de ingreso del Vendedor (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } list->data.date.day = strcmp(input, "0") ? atoi(input) : list->data.date.day;

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(list->data);
        TAB; printf("Ingrese el nuevo mes de ingreso del Vendedor (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } list->data.date.month = strcmp(input, "0") ? atoi(input) : list->data.date.month;

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(list->data);
        TAB; printf("Ingrese el nuevo year de ingreso del Vendedor (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } list->data.date.year = strcmp(input, "0") ? atoi(input) : list->data.date.year;

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleVendor(list->data);
        TAB; printf("Ingrese la nueva comision del Vendedor (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } list->data.commission = strcmp(input, "0") ? atoi(input) : list->data.commission;
}

void MenuVendor(VendorNode** list) {
    char input[256];
    int posicion;
    Vendor aux;
    VendorNode* aux_node;
    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Menu Manejo Vendedores                       ||"); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| 1. Agregar un Vendedor                       ||"); NL;
        TAB; printf("|| 2. Remover un Vendedor                       ||"); NL;
        TAB; printf("|| 3. Modificar un Vendedor                     ||"); NL;
        TAB; printf("|| 4. Buscar Vendedores en la lista             ||"); NL;
        TAB; printf("|| 5. Leer lista actual                         ||"); NL;
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
                aux = InputCreateVendor();
                if (LookForVendor(*list, aux.ci, NULL, NULL_DATE, ZERO) != NULL) {
                    TAB; printf("El Vendedor ya se encuentra dentro de la lista"); NL;
                    break;
                }

                while (1) {
                    CLEAR;
                    PrintSingleVendor(aux); NL;
                    TAB; printf("Ingrese la posicion donde agregar el Vendedor: "); NL;
                    TAB; printf("'Inicio' para seleccionar la primera posicion"); NL;
                    TAB; printf("'Final' para seleccionar la ultima posicion"); NL; NL;
                    TAB; printf("Posicion: "); InputString(input, "%10s");
                    if (atoi(input) > 0) { posicion = atoi(input); break; }
                    if (!strcmp(input, "Inicio")) { posicion =  ZERO; break; }
                    if (!strcmp(input, "Final")) { posicion = LAST; break; }
                }

                InsertVendor(list, aux, posicion);
                SaveFileVendor(*list, "Vendedores.txt");
                break;
            case '2':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                while (1) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| Menu Remover Vendedor                        ||"); NL;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| 1. Remover por Posicion                      ||"); NL;
                    TAB; printf("|| 2. Remover por Campo                         ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| 0. Regresar al Menu de Vendedores            ||"); NL;
                    TAB; printf("=================================================="); NL;
                    NL;
                    TAB; printf("Seleccion: "); InputString(input, "%2s");
                    if (atoi(input) == 0) { break; }
                    if (atoi(input) == 1) {
                        while (1) {
                            CLEAR;
                            TAB; printf("Ingrese la posicion donde Eliminar el Vendedor: "); NL;
                            TAB; printf("'Inicio' para seleccionar la primera posicion"); NL;
                            TAB; printf("'Final' para seleccionar la ultima posicion"); NL; NL;
                            TAB; printf("Posicion: "); InputString(input, "%10s");
                            if (atoi(input) > 0) { posicion = atoi(input); break; }
                            if (!strcmp(input, "Inicio")) { posicion =  ZERO; break; }
                            if (!strcmp(input, "Final")) { posicion = LAST; break; }
                        }
                        while (1) {
                            CLEAR;
                            PrintSingleVendor(GetVendorFromPosition(list, posicion)->data);
                            NL;
                            TAB; printf("Desea Eliminar este elemento?: "); NL;
                            TAB; printf("1. Si"); NL;
                            TAB; printf("0. No"); NL; NL;
                            TAB; printf("Seleccion: "); InputString(input, "%10s");
                            if (atoi(input) == 1) { RemovePositionVendor(list, posicion); break; }
                            if (atoi(input) == 0) { break; }
                        }
                        break;
                    }
                    if (atoi(input) == 2) {
                        aux_node = SearchVendor(*list, 1);
                        if (aux_node == NULL) { break; }
                        RemovePositionVendor(list, GetPositionVendor(*list, aux_node));
                        break;
                    }
                }
                SaveFileVendor(*list, "Vendedores.txt");
                break;
            case '3':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                while (1) {
                    CLEAR;
                    TAB; printf("Ingrese la posicion a modificar el Vendedor: "); NL;
                    TAB; printf("'Inicio' para seleccionar la primera posicion"); NL;
                    TAB; printf("'Final' para seleccionar la ultima posicion"); NL; NL;
                    TAB; printf("Posicion: "); InputString(input, "%10s");
                    if (atoi(input) > 0) { posicion = atoi(input); break; }
                    if (!strcmp(input, "Inicio")) { posicion =  ZERO; break; }
                    if (!strcmp(input, "Final")) { posicion = LAST; break; }
                }
                ModifyVendor(GetVendorFromPosition(list, posicion));
                SaveFileVendor(*list, "Vendedores.txt");
                break;
            case '4':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                PrintVendorList(SearchVendor(*list, 0));
                break;
            case '5':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                PrintVendorList(*list);
                break;
        }
    }
}

