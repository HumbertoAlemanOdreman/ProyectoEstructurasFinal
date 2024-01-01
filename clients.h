#include "definitions.h"

#ifndef _CLIENTS_C
#define _CLIENTS_C
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
#endif

Client CreateClient(const char name[], const char ci[], const char address[], const char cellphone[]);
void CreateClientList(ClientNode** list, const Client data);
ClientNode* LookForClient(ClientNode* list, const char name[], const char ci[], const char address[], const char cellphone[]);
void PushClient(ClientNode** list, const Client data);
void AppendClient(ClientNode** list, const Client data);
void InsertClient(ClientNode** list, const Client data, int position);
ClientNode* PreviousClient(ClientNode *list, ClientNode *client);
ClientNode* LookForClients(ClientNode* list, const char name[], const char ci[], const char address[], const char cellphone[]);
void PrintSingleClient(Client list);
void PrintClientList(ClientNode *list);
ClientNode* SelectClientFromList(ClientNode *list);
void ReadFileClient(ClientNode** list, const char dir[]);
void SaveFileClient(ClientNode* list, const char dir[]);
int GetPositionClient(ClientNode* list, ClientNode* data);
ClientNode* GetClientFromPosition(ClientNode** list, int position);
void RemovePositionClient(ClientNode** list, int position);
Client InputCreateClient();
ClientNode* SearchClient(ClientNode* list, int single);
void ModifyClient(ClientNode* globalList, ClientNode* list);
ClientNode* MenuClientSelection(ClientNode** list);
void MenuClient(ClientNode** list);
