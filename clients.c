#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"
#include "input.h"

typedef struct Client {
    char name[COL_SIZE];
    char ci[COL_SIZE];
    char address[COL_SIZE];
    char cellphone[COL_SIZE];
} Client;

typedef struct ClientNode {
    Client data;
    struct ClientNode* next;
} ClientNode;

Client CreateClient(const char name[], const char ci[], const char address[], const char cellphone[]) {
    Client ret;
    strcpy(ret.name, name);
    strcpy(ret.ci, ci);
    strcpy(ret.address, address);
    strcpy(ret.cellphone, cellphone);
    return ret;
}

void CreateClientList(ClientNode** list, const Client data) {
    if (*list != NULL) { return; }
    (*list) = MALLOC_CLI;
    (*list)->data = data;
    (*list)->next = NULL;
}

ClientNode* LookForClient(ClientNode* list, const char name[], const char ci[], const char address[], const char cellphone[]) {
    ClientNode* aux = list;
    ClientNode* loop = list;
    while (loop) {
        if (name != NULL && strcmp(loop->data.name, name)) { loop = loop->next; continue; }
        if (ci != NULL && strcmp(loop->data.ci, ci)) { loop = loop->next; continue; }
        if (address != NULL && strcmp(loop->data.address, address)) { loop = loop->next; continue; }
        if (cellphone != NULL && strcmp(loop->data.cellphone, cellphone)) { loop = loop->next; continue; }
        if (aux == loop) { return list; }
        while (aux->next != loop) { aux = aux->next; } return aux->next;
    } return NULL;
}

void PushClient(ClientNode** list, const Client data) {
    if (*list == NULL) { CreateClientList(list, data); return; } 
    if (LookForClient(*list, NULL, data.ci, ZERO, ZERO) != NULL) { return; }
    ClientNode* head = MALLOC_CLI;
    head->data = data;
    head->next = *list;
    *list = head;
}

void AppendClient(ClientNode** list, const Client data) {
    if (*list == NULL) { CreateClientList(list, data); return; }
    if (LookForClient(*list, NULL, data.ci, ZERO, ZERO) != NULL) { return; }
    ClientNode* loop = *list;
    while(loop->next) { loop = loop->next; }
    loop->next = MALLOC_CLI;
    loop->next->data = data;
    loop->next->next = NULL;
}

void InsertClient(ClientNode** list, const Client data, int position) {
    if (*list == NULL) { CreateClientList(list, data); return; }
    if (LookForClient(*list, NULL, data.ci, ZERO, ZERO) != NULL) { return; }
    if (position == ZERO) { PushClient(list, data); return; }
    if (position == LAST) { AppendClient(list, data); return; }
    ClientNode* loop = *list; position--;
    while (loop->next != NULL && position-- > 0) { loop = loop->next; }
    ClientNode* aux = loop->next;
    loop->next = MALLOC_CLI; loop->next->next = aux;
    loop->next->data = data;
}

ClientNode* PreviousClient(ClientNode *list, ClientNode *client) {
  if (list == NULL) { return NULL; }
  if (list == client) { return NULL; }
  while (list->next != NULL) {
    if (list->next == client) { return list; }
    list = list->next;
  } return NULL;
};

ClientNode* LookForClients(ClientNode* list, const char name[], const char ci[], const char address[], const char cellphone[]) {
    ClientNode* loop = list;
    ClientNode* ret = NULL;
    while (loop) {
        if (name != NULL && strcmp(loop->data.name, name)) { loop = loop->next; continue; }
        if (ci != NULL && strcmp(loop->data.ci, ci)) { loop = loop->next; continue; }
        if (address != NULL && strcmp(loop->data.address, address)) { loop = loop->next; continue; }
        if (cellphone != NULL && strcmp(loop->data.cellphone, cellphone)) { loop = loop->next; continue; }
        AppendClient(&ret, loop->data); loop = loop->next;
    } return ret;
}

void PrintSingleClient(Client list) {
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Nombre: %35s ||", list.name); NL;
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Cedula: %35s ||", list.ci); NL;
    TAB; printf("|| Telefono: %33s ||", list.cellphone); NL;
    TAB; printf("|| Direccion: %32s ||", list.address); NL;
    TAB; printf("=================================================="); NL;
}

void PrintClientList(ClientNode *list) {
    char tmp[4];
    if (list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); }
    ClientNode* nodo_inicial = list;
    ClientNode* aux = list;

    while (list != NULL) {
        CLEAR;
        PrintSingleClient(list->data);
        printf("\n");
        if (PreviousClient(nodo_inicial, list) != NULL) { TAB; printf("[P] Cliente Previo\n"); }
        if (list->next != NULL) { TAB; printf("[S] Cliente Siguiente\n"); }
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
            aux = PreviousClient(nodo_inicial, list);
            if (aux != NULL) { list = aux; }
            break;
        }
    }
};

ClientNode* SelectClientFromList(ClientNode *list) {
    char tmp[4];
    if (list == NULL) { return NULL; }
    ClientNode* nodo_inicial = list;
    ClientNode* aux = list;

    while (list != NULL) {
        CLEAR;
        PrintSingleClient(list->data);
        printf("\n");
        if (PreviousClient(nodo_inicial, list) != NULL) { TAB; printf("[P] Cliente Previo\n"); }
        if (list->next != NULL) { TAB; printf("[S] Cliente Siguiente\n"); }
        TAB; printf("[T] Seleccionar Elemento\n");
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
            aux = PreviousClient(nodo_inicial, list);
            if (aux != NULL) { list = aux; }
            break;
        }
    } return NULL;
};

void ReadFileClient(ClientNode** list, const char dir[]) {
    FILE *f; f = fopen(dir, "r");
    if (f == NULL) { return; }
    Client aux;
    while (1) {
        if (fscanf(f, "%s", aux.name) == EOF) { break; }
        if (fscanf(f, "%s", aux.ci) == EOF) { break; }
        if (fscanf(f, "%s", aux.address) == EOF) { break; }
        if (fscanf(f, "%s", aux.cellphone) == EOF) { break; }
        AppendClient(list, aux);
    } fclose(f);
}

void SaveFileClient(ClientNode* list, const char dir[]) {
    FILE *f; f = fopen(dir, "w");
    if (f == NULL) { fclose(f); return; }
    if (list == NULL) { fprintf(f, ""); return; } 
    ClientNode* aux = list;
    while (aux) {
        fprintf(f, "%s ", aux->data.name);
        fprintf(f, "%s ", aux->data.ci);
        fprintf(f, "%s ", aux->data.address);
        fprintf(f, "%s", aux->data.cellphone);
        fprintf(f, "\n");
        aux = aux->next;
    } fclose(f);
}

int GetPositionClient(ClientNode* list, ClientNode* data) {
    if (list == NULL || data == NULL || list == data) { return 0; }
    int position = 0;
    while(list->next != NULL) {
        if (list->next == data) { return position; }
        list = list->next; position++;
    } return -1;
}

ClientNode* GetClientFromPosition(ClientNode** list, int position) {
    if (*list == NULL) { return NULL; }
    if ((*list)->next == NULL || position == 0) { return *list; }
    ClientNode *aux = *list;
    if (position == LAST) { while (aux->next != NULL) { aux = aux->next; } return aux; }
    while (aux->next != NULL && position-- > 0) { aux = aux->next; }
    return aux;
}

void RemovePositionClient(ClientNode** list, int position) {
    if (*list == NULL) { return; }
    if ((*list)->next == NULL) { 
        free(*list);
        *list = NULL;
        return;
    }

    ClientNode *aux = *list;
    ClientNode *aux_2 = MALLOC_CLI;

    if (position == ZERO) {
        aux = (*list)->next;
        free(*list);
        *list = aux;
        return;
    }
    
    if (position == LAST) {
        while (aux->next != NULL) { aux = aux->next; }
        aux_2 = aux->next;
        aux = PreviousClient(*list, aux);
        free(aux->next);
        aux->next = aux_2;
        return;
    }

    while (aux->next != NULL && position-- > 0) { aux = aux->next; }
    aux_2 = aux->next;
    aux = PreviousClient(*list, aux);
    free(aux->next);
    aux->next = aux_2;
    return;
}

Client InputCreateClient() {
    Client ret; strcpy(ret.name, ""); strcpy(ret.ci, ""); strcpy(ret.address, ""); strcpy(ret.cellphone, "");
    char input[256];

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(ret); NL;
        TAB; printf("Ingrese el nombre del Cliente: "); InputString(input, "%29s");
    } strcpy(ret.name, input);

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(ret); NL;
        TAB; printf("Ingrese la Cedula de Identidad del Cliente: "); InputString(input, "%29s");
    } strcpy(ret.ci, input);

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(ret); NL;
        TAB; printf("Ingrese el Numero de Telefono del Cliente: "); InputString(input, "%29s");
    } strcpy(ret.address, input);

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(ret); NL;
        TAB; printf("Ingrese la Direccion del Cliente: "); InputString(input, "%29s");
    } strcpy(ret.cellphone, input);

    return ret;
}

ClientNode* SearchClient(ClientNode* list, int single) {
    Client ret; strcpy(ret.name, ""); strcpy(ret.ci, ""); strcpy(ret.address, ""); strcpy(ret.cellphone, "");
    char input[256];

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(ret); NL;
        TAB; printf("Ingrese el nombre del Cliente (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(ret.name, input);

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(ret); NL;
        TAB; printf("Ingrese la Cedula de Identidad del Cliente (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(ret.ci, input);

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(ret); NL;
        TAB; printf("Ingrese el Numero de Telefono del Cliente (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(ret.address, input);

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(ret); NL;
        TAB; printf("Ingrese la Direccion del Cliente (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(ret.cellphone, input);

    if (single) {
        return LookForClient(list, strcmp(ret.name, "NULL") ? ret.name : NULL, strcmp(ret.ci, "NULL") ? ret.ci : NULL, strcmp(ret.address, "NULL") ? ret.address : NULL, strcmp(ret.cellphone, "NULL") ? ret.cellphone : NULL);
    } else {
        return LookForClients(list, strcmp(ret.name, "NULL") ? ret.name : NULL, strcmp(ret.ci, "NULL") ? ret.ci : NULL, strcmp(ret.address, "NULL") ? ret.address : NULL, strcmp(ret.cellphone, "NULL") ? ret.cellphone : NULL);
    }
}

void ModifyClient(ClientNode* list) {
    char input[256];
    
    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(list->data); NL;
        TAB; printf("Ingrese el nombre del Cliente (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(list->data.name, (strcmp(input, "NULL") ? input : list->data.name));

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(list->data); NL;
        TAB; printf("Ingrese la Cedula de Identidad del Cliente (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(list->data.ci, (strcmp(input, "NULL") ? input : list->data.ci));

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(list->data); NL;
        TAB; printf("Ingrese el Numero de Telefono del Cliente (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(list->data.cellphone, (strcmp(input, "NULL") ? input : list->data.cellphone));

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleClient(list->data); NL;
        TAB; printf("Ingrese la Direccion del Cliente (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(list->data.address, (strcmp(input, "NULL") ? input : list->data.address));
}

ClientNode* MenuClientSelection(ClientNode** list) {
    char input[256];
    ClientNode* aux_node;
    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Seleccione una Modalidad                     ||"); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| 1. Seleccionar por posicion en la lista      ||"); NL;
        TAB; printf("|| 2. Seleccionar por busqueda de campos        ||"); NL;
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
                TAB; printf("|| Ingrese la posicion del Clientes             ||"); NL;
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
                if (!strcmp(input, "Inicio")) { return GetClientFromPosition(list, ZERO); }
                if (!strcmp(input, "Final")) { return GetClientFromPosition(list, LAST); }
                if (atoi(input) > 0) { return GetClientFromPosition(list, atoi(input)); }
            }
        }
        if (input[0] == '2') {
            aux_node =  SearchClient(*list, 1);
            if (aux_node == NULL) { NL; TAB; printf("No se ha encontrado el elemento"); NL; getchar(); continue; }
            return aux_node;
        }
        if (input[0] == '3') {
            aux_node = SelectClientFromList(*list);
            if (aux_node == NULL) { NL; TAB; printf("No se ha encontrado el elemento"); NL; getchar(); continue; }
            return aux_node;
        }
    } return NULL;
}

void MenuClient(ClientNode** list) {
    char input[256];
    int posicion;
    Client aux;
    ClientNode* aux_node;
    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Menu Manejo Cliente                          ||"); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| 1. Agregar un Cliente                        ||"); NL;
        TAB; printf("|| 2. Remover un Cliente                        ||"); NL;
        TAB; printf("|| 3. Modificar un Clientes                     ||"); NL;
        TAB; printf("|| 4. Buscar Clientes en la lista               ||"); NL;
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
                aux = InputCreateClient();
                if (LookForClient(*list, NULL, aux.ci, ZERO, ZERO) != NULL) {
                    TAB; printf("El articulo ya se encuentra dentro de la lista"); NL;
                    break;
                }

                while (1) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| Ingrese la posicion del Cliente              ||"); NL;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| 'Inicio' para seleccionar la primera         ||"); NL;
                    TAB; printf("|| 'Final' para seleccionar la ultima           ||"); NL;
                    TAB; printf("|| 'Numero' Para seleccionar la X posicion      ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| 0. Seleccionar de otra manera                ||"); NL;
                    TAB; printf("=================================================="); NL;
                    NL;
                    TAB; printf("Posicion: "); InputString(input, "%10s");
                    if (atoi(input) > 0) { posicion = atoi(input); break; }
                    if (!strcmp(input, "Inicio")) { posicion =  ZERO; break; }
                    if (!strcmp(input, "Final")) { posicion = LAST; break; }
                }

                InsertClient(list, aux, posicion);
                SaveFileClient(*list, "Clientes.txt");
                break;
            case '2':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                aux_node = MenuClientSelection(list);
                if (aux_node == NULL) { break; }
                while (1) {
                    CLEAR;
                    PrintSingleClient(aux_node->data);
                    NL;
                    TAB; printf("1. Eliminar de la lista"); NL;
                    TAB; printf("0. No eliminar de la lista"); NL;
                    NL;
                    TAB; printf("Seleccion: "); InputString(input, "%2s");
                    if (input[0] == '0' || input[0] == '1') { break; }
                } if (input[0] == '0') { break; }
                RemovePositionClient(list, GetPositionClient(*list, aux_node));
                SaveFileClient(*list, "Clientes.txt");
                break;
            case '3':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                aux_node = MenuClientSelection(list);
                if (aux_node == NULL) { break; }
                while (1) {
                    CLEAR;
                    PrintSingleClient(aux_node->data);
                    NL;
                    TAB; printf("1. Modificar"); NL;
                    TAB; printf("0. No Modificar"); NL;
                    NL;
                    TAB; printf("Seleccion: "); InputString(input, "%2s");
                    if (input[0] == '0' || input[0] == '1') { break; }
                } if (input[0] == '0') { break; }
                ModifyClient(aux_node);
                SaveFileClient(*list, "Clientes.txt");
                break;
            case '4':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                aux_node = SearchClient(*list, 0);
                if (aux_node == NULL) { TAB; printf("No se ha encontrado ningun elemento con esas caracteristicas"); NL; getchar(); }
                PrintClientList(aux_node);
                break;
            case '5':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                PrintClientList(*list);
                break;
        }
    }
}
