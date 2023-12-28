#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Articles.c"
#include "Vendors.c"
#include "Clients.c"

ArticleNode* ArticleList = NULL;
VendorNode* VendorList = NULL;
ClientNode* ClientList = NULL;

int main(void) {
    ReadFileArticle(&ArticleList, "Articulos.txt");
    ReadFileVendor(&VendorList, "Vendedores.txt");
    ReadFileClient(&ClientList, "Clientes.txt");
    PrintClientList(ClientList);
    return 0;
}
