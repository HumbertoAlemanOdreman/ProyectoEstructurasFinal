#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"
#include "input.h"

typedef struct Article {
    char code[COL_SIZE];
    char name[COL_SIZE];
    float price;
    int ammount;
} Article;

typedef struct ArticleNode {
    Article data;
    struct ArticleNode* next;
} ArticleNode;

Article CreateArticle(const char code[], const char name[], const float price, const int ammount) {
    Article ret;
    strcpy(ret.code, code);
    strcpy(ret.name, name);
    ret.price = price;
    ret.ammount = ammount;
    return ret;
}

void CreateArticleList(ArticleNode** list, const Article data) {
    if (*list != NULL) { return; }
    (*list) = MALLOC_ART;
    (*list)->data = data;
    (*list)->next = NULL;
}

ArticleNode* LookForArticle(ArticleNode* list, const char code[], const char name[], const float price, const int ammount) {
    ArticleNode* aux = list;
    ArticleNode* loop = list;
    while (loop) {
        if (code != NULL && strcmp(loop->data.code, code)) { loop = loop->next; continue; }
        if (name != NULL && strcmp(loop->data.name, name)) { loop = loop->next; continue; }
        if (price != ZERO && loop->data.price != price) { loop = loop->next; continue; }
        if (ammount != ZERO && loop->data.ammount != ammount) { loop = loop->next; continue; }
        if (aux == loop) { return list; }
        while (aux->next != loop) { aux = aux->next; } return aux->next;
    } return NULL;
}

void PushArticle(ArticleNode** list, const Article data) {
    if (*list == NULL) { CreateArticleList(list, data); return; } 
    if (LookForArticle(*list, data.code, NULL, ZERO, ZERO) != NULL) { return; }
    ArticleNode* head = MALLOC_ART;
    head->data = data;
    head->next = *list;
    *list = head;
}

void AppendArticle(ArticleNode** list, const Article data) {
    if (*list == NULL) { CreateArticleList(list, data); return; } 
    if (LookForArticle(*list, data.code, NULL, ZERO, ZERO) != NULL) { return; }
    ArticleNode* loop = *list;
    while(loop->next) { loop = loop->next; }
    loop->next = MALLOC_ART;
    loop->next->data = data;
    loop->next->next = NULL;
}

void InsertArticle(ArticleNode** list, const Article data, int position) {
    if (*list == NULL) { CreateArticleList(list, data); return; }
    if (LookForArticle(*list, data.code, NULL, ZERO, ZERO) != NULL) { return; }
    if (position == ZERO) { PushArticle(list, data); return; }
    if (position == LAST) { AppendArticle(list, data); return; }
    ArticleNode* loop = *list; position--;
    while (loop->next != NULL && position-- > 0) { loop = loop->next; }
    ArticleNode* aux = loop->next;
    loop->next = MALLOC_ART; loop->next->next = aux;
    loop->next->data = data;
}

ArticleNode* PreviousArticle(ArticleNode *list, ArticleNode *article) {
  if (list == NULL) { return NULL; }
  if (list == article) { return NULL; }
  while (list->next != NULL) {
    if (list->next == article) { return list; }
    list = list->next;
  } return NULL;
};

ArticleNode* LookForArticles(ArticleNode* list, const char code[], const char name[], const float price, const int ammount) {
    ArticleNode* loop = list;
    ArticleNode* ret = NULL;
    while (loop) {
        if (code != NULL && strcmp(loop->data.code, code)) { loop = loop->next; continue; }
        if (name != NULL && strcmp(loop->data.name, name)) { loop = loop->next; continue; }
        if (price != ZERO && loop->data.price != price) { loop = loop->next; continue; }
        if (ammount != ZERO && loop->data.ammount != ammount) { loop = loop->next; continue; }
        AppendArticle(&ret, loop->data); loop = loop->next;
    } return ret;
}

void PrintSingleArticle(Article list) {
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Nombre: %35s ||", list.name); NL;
    TAB; printf("=================================================="); NL;
    TAB; printf("|| Codigo: %35s ||", list.code); NL;
    TAB; printf("|| Precio: %35f ||", list.price); NL;
    TAB; printf("|| Cantidad: %33d ||", list.ammount); NL;
    TAB; printf("=================================================="); NL;
}

void PrintArticleList(ArticleNode *list) {
    char tmp[4];
    if (list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); }
    ArticleNode* nodo_inicial = list;
    ArticleNode* aux = list;

    while (list != NULL) {
        CLEAR;
        PrintSingleArticle(list->data);
        printf("\n");
        if (PreviousArticle(nodo_inicial, list) != NULL) { TAB; printf("[P] Articulo Previo\n"); }
        if (list->next != NULL) { TAB; printf("[S] Articulo Siguiente\n"); }
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
            aux = PreviousArticle(nodo_inicial, list);
            if (aux != NULL) { list = aux; }
            break;
        }
    }
};

void ReadFileArticle(ArticleNode** list, const char dir[]) {
    FILE *f; f = fopen(dir, "r");
    if (f == NULL) { return; }
    Article aux;
    while (1) {
        if (fscanf(f, "%s", aux.code) == EOF) { break; }
        if (fscanf(f, "%s", aux.name) == EOF) { break; }
        if (fscanf(f, "%f", &aux.price) == EOF) { break; }
        if (fscanf(f, "%d", &aux.ammount) == EOF) { break; }
        AppendArticle(list, aux);
    } fclose(f);
}

void SaveFileArticle(ArticleNode* list, const char dir[]) {
    FILE *f; f = fopen(dir, "w");
    if (f == NULL) { fclose(f); return; }
    if (list == NULL) { fprintf(f, ""); return; } 
    ArticleNode* aux = list;
    while (aux) {
        fprintf(f, "%s ", aux->data.code);
        fprintf(f, "%s ", aux->data.name);
        fprintf(f, "%2f ", aux->data.price);
        fprintf(f, "%d", aux->data.ammount);
        fprintf(f, "\n");
        aux = aux->next;
    } fclose(f);
}

int GetPositionArticle(ArticleNode* list, ArticleNode* data) {
    if (list == NULL || data == NULL || list == data) { return 0; }
    int position = 0;
    while(list->next != NULL) {
        if (list->next == data) { return position; }
        list = list->next; position++;
    } return -1;
}

ArticleNode* GetArticleFromPosition(ArticleNode** list, int position) {
    if (*list == NULL) { return NULL; }
    if ((*list)->next == NULL || position == 0) { return *list; }
    ArticleNode *aux = *list;
    if (position == LAST) { while (aux->next != NULL) { aux = aux->next; } return aux; }
    while (aux->next != NULL && position-- > 0) { aux = aux->next; }
    return aux;
}

void RemovePositionArticle(ArticleNode** list, int position) {
    if (*list == NULL) { return; }
    if ((*list)->next == NULL) { 
        free(*list);
        *list = NULL;
        return;
    }

    ArticleNode *aux = *list;
    ArticleNode *aux_2 = MALLOC_ART;

    if (position == ZERO) {
        aux = (*list)->next;
        free(*list);
        *list = aux;
        return;
    }
    
    if (position == LAST) {
        while (aux->next != NULL) { aux = aux->next; }
        aux_2 = aux->next;
        aux = PreviousArticle(*list, aux);
        free(aux->next);
        aux->next = aux_2;
        return;
    }

    while (aux->next != NULL && position-- > 0) { aux = aux->next; }
    aux_2 = aux->next;
    aux = PreviousArticle(*list, aux);
    free(aux->next);
    aux->next = aux_2;
    return;
}

Article InputCreateArticle() {
    Article ret; strcpy(ret.code, ""); strcpy(ret.name, ""); ret.price = 0; ret.ammount = 0;
    char input[256];

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleArticle(ret); NL;
        TAB; printf("Ingrese el nombre del Articulo: "); InputString(input, "%29s");
    } strcpy(ret.name, input);

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleArticle(ret); NL;
        TAB; printf("Ingrese el codigo del Articulo: "); InputString(input, "%29s");
    } strcpy(ret.code, input);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleArticle(ret); NL;
        TAB; printf("Ingrese el precio del Articulo: "); InputString(input, "%29s");
    } ret.price = atof(&input[0]);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleArticle(ret); NL;
        TAB; printf("Ingrese la cantidad de stock del Articulo: "); InputString(input, "%29s");
    } ret.ammount = atoi(input);

    return ret;
}

ArticleNode* SearchArticle(ArticleNode* list, int single) {
    Article ret; strcpy(ret.code, ""); strcpy(ret.name, ""); ret.price = 0; ret.ammount = 0;
    char input[256];

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleArticle(ret); NL;
        TAB; printf("Ingrese el nombre del Articulo (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(ret.name, input);
    

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleArticle(ret); NL;
        TAB; printf("Ingrese el codigo del Articulo (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(ret.code, input);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleArticle(ret); NL;
        TAB; printf("Ingrese el precio del Articulo (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } ret.price = atoi(input);

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleArticle(ret); NL;
        TAB; printf("Ingrese la cantidad de stock del Articulo (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } ret.ammount = atoi(input);

    if (single) {
        return LookForArticle(list, strcmp(ret.code, "NULL") ? ret.code : NULL, strcmp(ret.name, "NULL") ? ret.name : NULL, ret.price, ret.ammount);
    } else {
        return LookForArticles(list, strcmp(ret.code, "NULL") ? ret.code : NULL, strcmp(ret.name, "NULL") ? ret.name : NULL, ret.price, ret.ammount);
    }
}

void ModifyArticle(ArticleNode* list) {
    char input[256];

    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleArticle(list->data); NL;
        TAB; printf("Ingrese el nuevo nombre del Articulo (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(list->data.name, (strcmp(input, "NULL") ? input : list->data.name));
    
    strcpy(input, "");
    while (!strcmp(input, "")) {
        CLEAR;
        PrintSingleArticle(list->data); NL;
        TAB; printf("Ingrese el nuevo codigo del Articulo (NULL para omitir): "); InputString(input, "%29s");
    } strcpy(list->data.code, (strcmp(input, "NULL") ? input : list->data.code));

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleArticle(list->data); NL;
        TAB; printf("Ingrese el nuevo precio del Articulo (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } list->data.price = strcmp(input, "0") ? atoi(input) : list->data.price;

    strcpy(input, "");
    while (atoi(input) <= 0) {
        CLEAR;
        PrintSingleArticle(list->data); NL;
        TAB; printf("Ingrese la nuevo cantidad de stock del Articulo (ZERO para omitir): "); InputString(input, "%29s");
        if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
    } list->data.ammount = strcmp(input, "0") ? atoi(input) : list->data.ammount;
}

void MenuArticle(ArticleNode** list) {
    char input[256];
    int posicion;
    Article aux;
    ArticleNode* aux_node;
    while (1) {
        CLEAR;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Menu Manejo Articulos                        ||"); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| 1. Agregar un Articulo                       ||"); NL;
        TAB; printf("|| 2. Remover un Articulo                       ||"); NL;
        TAB; printf("|| 3. Modificar un Articulo                     ||"); NL;
        TAB; printf("|| 4. Buscar Articulos en la lista              ||"); NL;
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
                aux = InputCreateArticle();
                if (LookForArticle(*list, aux.code, NULL, ZERO, ZERO) != NULL) {
                    TAB; printf("El articulo ya se encuentra dentro de la lista"); NL;
                    break;
                }

                while (1) {
                    CLEAR;
                    PrintSingleArticle(aux);
                    NL;
                    TAB; printf("Ingrese la posicion donde agregar el Articulo: "); NL;
                    TAB; printf("'Inicio' para seleccionar la primera posicion"); NL;
                    TAB; printf("'Final' para seleccionar la ultima posicion"); NL; NL;
                    TAB; printf("Posicion: "); InputString(input, "%10s");
                    if (atoi(input) > 0) { posicion = atoi(input); break; }
                    if (!strcmp(input, "Inicio")) { posicion =  ZERO; break; }
                    if (!strcmp(input, "Final")) { posicion = LAST; break; }
                }

                InsertArticle(list, aux, posicion);
                SaveFileArticle(*list, "Articulos.txt");
                break;
            case '2':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                while (1) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| Menu Remover Articulo                        ||"); NL;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| 1. Remover por Posicion                      ||"); NL;
                    TAB; printf("|| 2. Remover por Campo                         ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| 0. Regresar al Menu de Articulos             ||"); NL;
                    TAB; printf("=================================================="); NL;
                    NL;
                    TAB; printf("Seleccion: "); InputString(input, "%2s");
                    if (atoi(input) == 0) { break; }
                    if (atoi(input) == 1) {
                        while (1) {
                            CLEAR;
                            TAB; printf("Ingrese la posicion donde Eliminar el articulo: "); NL;
                            TAB; printf("'Inicio' para seleccionar la primera posicion"); NL;
                            TAB; printf("'Final' para seleccionar la ultima posicion"); NL; NL;
                            TAB; printf("Posicion: "); InputString(input, "%10s");
                            if (atoi(input) > 0) { posicion = atoi(input); break; }
                            if (!strcmp(input, "Inicio")) { posicion =  ZERO; break; }
                            if (!strcmp(input, "Final")) { posicion = LAST; break; }
                        }
                        while (1) {
                            CLEAR;
                            PrintSingleArticle(GetArticleFromPosition(list, posicion)->data); NL;
                            TAB; printf("Desea Eliminar este elemento?: "); NL;
                            TAB; printf("1. Si"); NL;
                            TAB; printf("0. No"); NL; NL;
                            TAB; printf("Seleccion: "); InputString(input, "%10s");
                            if (atoi(input) == 1) { RemovePositionArticle(list, posicion); break; }
                            if (atoi(input) == 0) { break; }
                        }
                        break;
                    }
                    if (atoi(input) == 2) {
                        aux_node = SearchArticle(*list, 1);
                        if (aux_node == NULL) { break; }
                        RemovePositionArticle(list, GetPositionArticle(*list, aux_node));
                        break;
                    }
                }
                SaveFileArticle(*list, "Articulos.txt");
                break;
            case '3':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                while (1) {
                    CLEAR;
                    TAB; printf("Ingrese la posicion a modificar el articulo: "); NL;
                    TAB; printf("'Inicio' para seleccionar la primera posicion"); NL;
                    TAB; printf("'Final' para seleccionar la ultima posicion"); NL; NL;
                    TAB; printf("Posicion: "); InputString(input, "%10s");
                    if (atoi(input) > 0) { posicion = atoi(input); break; }
                    if (!strcmp(input, "Inicio")) { posicion =  ZERO; break; }
                    if (!strcmp(input, "Final")) { posicion = LAST; break; }
                }
                ModifyArticle(GetArticleFromPosition(list, posicion));
                SaveFileArticle(*list, "Articulos.txt");
                break;
            case '4':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                PrintArticleList(SearchArticle(*list, 0));
                break;
            case '5':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                PrintArticleList(*list);
                break;
        }
    }
}
