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

ArticleNode* SelectArticleFromList(ArticleNode *list) {
    char tmp[4];
    if (list == NULL) { return NULL; }
    ArticleNode* nodo_inicial = list;
    ArticleNode* aux = list;

    while (list != NULL) {
        CLEAR;
        PrintSingleArticle(list->data);
        printf("\n");
        if (PreviousArticle(nodo_inicial, list) != NULL) { TAB; printf("[P] Articulo Previo\n"); }
        if (list->next != NULL) { TAB; printf("[S] Articulo Siguiente\n"); }
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
            aux = PreviousArticle(nodo_inicial, list);
            if (aux != NULL) { list = aux; }
            break;
        }
    } return NULL;
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

void ModifyArticle(ArticleNode* globalList, ArticleNode* list) {
    char input[256];
    while(1) {
        CLEAR;
        PrintSingleArticle(list->data); NL;
        TAB; printf("=================================================="); NL;
        TAB; printf("|| Seleccione el campo a modificar              ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| 1. Modificar el nombre del Articulo          ||"); NL;
        TAB; printf("|| 2. Modificar el codigo del Articulo          ||"); NL;
        TAB; printf("|| 3. Modificar el precio del Articulo          ||"); NL;
        TAB; printf("|| 4. Modificar el stock del Articulo           ||"); NL;
        TAB; printf("||                                              ||"); NL;
        TAB; printf("|| 0. Concluir las modificaciones               ||"); NL;
        TAB; printf("=================================================="); NL;
        NL;
        TAB; printf("Seleccion: "); InputString(input, "%10s");
        switch(input[0]) {
            case '0': return;
            case '1':
                strcpy(input, "");
                while (!strcmp(input, "")) {
                    CLEAR;
                    PrintSingleArticle(list->data); NL;
                    TAB; printf("Ingrese el nuevo nombre del Articulo (NULL para cancelar): "); InputString(input, "%29s");
                } strcpy(list->data.name, (strcmp(input, "NULL") ? input : list->data.name));
                break;
            case '2':
                strcpy(input, "");
                while (!strcmp(input, "")) {
                    CLEAR;
                    PrintSingleArticle(list->data); NL;
                    TAB; printf("Ingrese el nuevo codigo del Articulo (NULL para cancelar): "); InputString(input, "%29s");
                }
                if (LookForArticle(globalList, NULL, input, ZERO, ZERO != NULL)) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| Ese codigo ya se encuentra en la lista       ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| ENTER para continuar                         ||"); NL;
                    TAB; printf("=================================================="); NL; 
                    NL; getchar();
                    break;
                } strcpy(list->data.code, (strcmp(input, "NULL") ? input : list->data.code));
            case '3':
                strcpy(input, "");
                while (atoi(input) <= 0) {
                    CLEAR;
                    PrintSingleArticle(list->data); NL;
                    TAB; printf("Ingrese el nuevo precio del Articulo (ZERO para cancelar): "); InputString(input, "%29s");
                    if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
                }
                if (atoi(input) < 0) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| El precio no puede ser menor a cero          ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| ENTER para continuar                         ||"); NL;
                    TAB; printf("=================================================="); NL; 
                    NL; getchar();
                    break;
                } list->data.price = strcmp(input, "0") ? atoi(input) : list->data.price;
                break;
            case '4':
                strcpy(input, "");
                while (atoi(input) <= 0) {
                    CLEAR;
                    PrintSingleArticle(list->data); NL;
                    TAB; printf("Ingrese la nuevo cantidad de stock del Articulo (ZERO para omitir): "); InputString(input, "%29s");
                    if (!strcmp(input, "ZERO")) { strcpy(input, "0"); break; }
                }
                if (atoi(input) < 0) {
                    CLEAR;
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| El stock no puede ser menor a cero           ||"); NL;
                    TAB; printf("||                                              ||"); NL;
                    TAB; printf("|| ENTER para continuar                         ||"); NL;
                    TAB; printf("=================================================="); NL; 
                    NL; getchar();
                    break;
                } list->data.ammount = strcmp(input, "0") ? atoi(input) : list->data.ammount;
                break;
        }
    }
}

ArticleNode* MenuArticleSelection(ArticleNode** list) {
    char input[256];
    ArticleNode* aux_node;
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
                TAB; printf("|| Ingrese la posicion del Articulo             ||"); NL;
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
                if (!strcmp(input, "Inicio")) { return GetArticleFromPosition(list, ZERO); }
                if (!strcmp(input, "Final")) { return GetArticleFromPosition(list, LAST); }
                if (atoi(input) > 0) { return GetArticleFromPosition(list, atoi(input)); }
            }
        }
        if (input[0] == '2') {
            aux_node =  SearchArticle(*list, 1);
            if (aux_node == NULL) { NL; TAB; printf("No se ha encontrado el elemento"); NL; getchar(); continue; }
            return aux_node;
        }
        if (input[0] == '3') {
            aux_node = SelectArticleFromList(*list);
            if (aux_node == NULL) { NL; TAB; printf("No se ha encontrado el elemento"); NL; getchar(); continue; }
            return aux_node;
        }
    } return NULL;
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
                    TAB; printf("=================================================="); NL;
                    TAB; printf("|| Ingrese la posicion del Articulo             ||"); NL;
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

                InsertArticle(list, aux, posicion);
                SaveFileArticle(*list, "Articulos.txt");
                break;
            case '2':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                aux_node = MenuArticleSelection(list);
                if (aux_node == NULL) { break; }
                while (1) {
                    CLEAR;
                    PrintSingleArticle(aux_node->data);
                    NL;
                    TAB; printf("1. Eliminar de la lista"); NL;
                    TAB; printf("0. No eliminar de la lista"); NL;
                    NL;
                    TAB; printf("Seleccion: "); InputString(input, "%2s");
                    if (input[0] == '0' || input[0] == '1') { break; }
                } if (input[0] == '0') { break; }
                RemovePositionArticle(list, GetPositionArticle(*list, aux_node));
                SaveFileArticle(*list, "Articulos.txt");
                break;
            case '3':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                aux_node = MenuArticleSelection(list);
                if (aux_node == NULL) { break; }
                while (1) {
                    CLEAR;
                    PrintSingleArticle(aux_node->data);
                    NL;
                    TAB; printf("1. Modificar"); NL;
                    TAB; printf("0. No Modificar"); NL;
                    NL;
                    TAB; printf("Seleccion: "); InputString(input, "%2s");
                    if (input[0] == '0' || input[0] == '1') { break; }
                } if (input[0] == '0') { break; }
                ModifyArticle(*list, aux_node);
                SaveFileArticle(*list, "Articulos.txt");
                break;
            case '4':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                aux_node = SearchArticle(*list, 0);
                if (aux_node == NULL) { TAB; printf("No se ha encontrado ningun elemento con esas caracteristicas"); NL; getchar(); }
                PrintArticleList(aux_node);
                break;
            case '5':
                if (*list == NULL) { TAB; printf("Lista esta vacia\n"); getchar(); break; }
                PrintArticleList(*list);
                break;
        }
    }
}
