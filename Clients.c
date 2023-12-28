#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_CLI (struct ClientNode *)malloc(sizeof(struct ClientNode))

#ifndef ZERO
#define ZERO 0
#endif

#ifndef LAST
#define LAST -1
#endif

#ifndef COL_SIZE
#define COL_SIZE 30
#endif

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
    // TODO: Add validation for data
    // Like string size, numbers being positive
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
    // NULL a string to skip
    // ZERO a value to skip
    ClientNode* aux = list;
    ClientNode* loop = list;
    while (loop) {
        if (name != NULL && strcmp(loop->data.name, name)) { loop = loop->next; continue; }
        if (ci != NULL && strcmp(loop->data.ci, ci)) { loop = loop->next; continue; }
        if (address != NULL && strcmp(loop->data.address, address)) { loop = loop->next; continue; }
        if (cellphone != NULL && strcmp(loop->data.cellphone, cellphone)) { loop = loop->next; continue; }
        if (aux == loop) { return list; }                                   // If aux is loop the first item is the item we're looking for
        while (aux->next != loop) { aux = aux->next; } return aux->next;    // Else we have to find the pointer to it
    } return NULL;
}

void PushClient(ClientNode** list, const Client data) {
    if (*list == NULL) { CreateClientList(list, data); return; }               // If list is NULL create it 
    if (LookForClient(*list, NULL, data.ci, ZERO, ZERO) != NULL) { return; }   // If ci already exists don't add it
    ClientNode* head = MALLOC_CLI;
    head->data = data;
    head->next = *list;
    *list = head;
}

void AppendClient(ClientNode** list, const Client data) {
    if (*list == NULL) { CreateClientList(list, data); return; }               // If list is NULL create it 
    if (LookForClient(*list, NULL, data.ci, ZERO, ZERO) != NULL) { return; }   // If ci already exists don't add it
    ClientNode* loop = *list;
    while(loop->next) { loop = loop->next; }
    loop->next = MALLOC_CLI;
    loop->next->data = data;
    loop->next->next = NULL;
}

void InsertClient(ClientNode** list, const Client data, int position) {
    if (*list == NULL) { CreateClientList(list, data); return; }               // If list is NULL create it 
    if (LookForClient(*list, NULL, data.ci, ZERO, ZERO) != NULL) { return; }   // If ci already exists don't add it
    if (position == ZERO) { PushClient(list, data); return; }                  // If position is ZERO push
    if (position == LAST) { AppendClient(list, data); return; }                // If position is LAST append
    ClientNode* loop = *list; position--;
    while (loop->next != NULL && position-- > 0) { loop = loop->next; }
    ClientNode* aux = loop->next;
    loop->next = MALLOC_CLI; loop->next->next = aux;
    loop->next->data = data;
}

ClientNode* LookForClients(ClientNode* list, const char name[], const char ci[], const char address[], const char cellphone[]) {
    // NULL a string to skip
    // ZERO a value to skip
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

void PrintSingleClient(const ClientNode* list) {
    if (list == NULL) { printf("Client is NULL\n"); }
    printf("Client:\n");
    printf("Name: %s\nCI: %s\nAddress: %s\nCellphone: %s\n", list->data.name, list->data.ci, list->data.address, list->data.cellphone);
}

void PrintClientList(ClientNode* list) {
    if (list == NULL) { printf("List is NULL\n"); }
    while (list) { 
        PrintSingleClient(list);
        list = list->next;
    }
}

void ReadFileClient(ClientNode** list, const char dir[]) {
    FILE *f; f = fopen(dir, "r");
    if (f == NULL) { return; }           // If file is NULL return
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
    if (f == NULL) { fclose(f); return; }           // If file is NULL return
    if (list == NULL) { fprintf(f, ""); return; }   // If list is NULL return 
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
