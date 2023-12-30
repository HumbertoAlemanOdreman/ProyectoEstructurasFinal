#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "articles.h"
#include "vendors.h"
#include "clients.h"
#include "definitions.h"
#include "input.h"

#include "Facturas.c"

ArticleNode* ArticleList = NULL;
VendorNode* VendorList = NULL;
ClientNode* ClientList = NULL;
FacturaNode* FacturaList = NULL;

// TODO: Make print functions not die when recieving NULL

void MainMenu() {
    while (1) {
        char input[256];
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Menu Principal                               ||"); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| 1. Manejo de Articulos                       ||"); NL;
        TAB; printf("|| 2. Manejo de Vendedores                      ||"); NL;
        TAB; printf("|| 3. Manejo de Clients                         ||"); NL;
        TAB; printf("|| 4. Manejo de Facturas                        ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| 0. Salir del Programa                        ||"); NL;
        TAB; printf("=================================================="); NL;
        NL;
        TAB; printf("Seleccion: "); InputString(input, "%2s");
        switch (input[0]) {
            case '0':
                return;
                break;
            case '1':
                MenuArticle(&ArticleList);
                break;
            case '2':
                MenuVendor(&VendorList);
                break;
            default:
                break;
        }
    }
}

int main(void) {
    ReadFileArticle(&ArticleList, "Articulos.txt");
    ReadFileVendor(&VendorList, "Vendedores.txt");
    ReadFileClient(&ClientList, "Clientes.txt");

    MainMenu();

    SaveFileArticle(ArticleList, "Articulos.txt");
    SaveFileVendor(VendorList, "Vendedores.txt");
    SaveFileClient(ClientList, "Clientes.txt");
    return 0;
}
