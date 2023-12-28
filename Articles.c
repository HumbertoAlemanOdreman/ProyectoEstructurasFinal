#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_ART (struct ArticleNode *)malloc(sizeof(struct ArticleNode))

#ifndef ZERO
#define ZERO 0
#endif

#ifndef LAST
#define LAST -1
#endif

#ifndef COL_SIZE
#define COL_SIZE 30
#endif

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
    // TODO: Add validation for data
    // Like string size, numbers being positive
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

ArticleNode* LookForArticle(ArticleNode* list, const char code[], const char name[COL_SIZE], const float price, const int ammount) {
    // NULL a string to skip
    // ZERO a value to skip
    ArticleNode* aux = list;
    ArticleNode* loop = list;
    while (loop) {
        if (code != NULL && strcmp(loop->data.code, code)) { loop = loop->next; continue; }
        if (name != NULL && strcmp(loop->data.name, name)) { loop = loop->next; continue; }
        if (price != ZERO && loop->data.price != price) { loop = loop->next; continue; }
        if (ammount != ZERO && loop->data.ammount != ammount) { loop = loop->next; continue; }
        if (aux == loop) { return list; }                                   // If aux is loop the first item is the item we're looking for
        while (aux->next != loop) { aux = aux->next; } return aux->next;    // Else we have to find the pointer to it
    } return NULL;
}

void PushArticle(ArticleNode** list, const Article data) {
    if (*list == NULL) { CreateArticleList(list, data); return; }               // If list is NULL create it 
    if (LookForArticle(*list, data.code, NULL, ZERO, ZERO) != NULL) { return; } // If code already exists don't add it
    ArticleNode* head = MALLOC_ART;
    head->data = data;
    head->next = *list;
    *list = head;
}

void AppendArticle(ArticleNode** list, const Article data) {
    if (*list == NULL) { CreateArticleList(list, data); return; }               // If list is NULL create it 
    if (LookForArticle(*list, data.code, NULL, ZERO, ZERO) != NULL) { return; } // If code already exists don't add it
    ArticleNode* loop = *list;
    while(loop->next) { loop = loop->next; }
    loop->next = MALLOC_ART;
    loop->next->data = data;
    loop->next->next = NULL;
}

void InsertArticle(ArticleNode** list, const Article data, int position) {
    if (*list == NULL) { CreateArticleList(list, data); return; }               // If list is NULL create it 
    if (LookForArticle(*list, data.code, NULL, ZERO, ZERO) != NULL) { return; } // If code already exists don't add it
    if (position == ZERO) { PushArticle(list, data); return; }                  // If position is ZERO push
    if (position == LAST) { AppendArticle(list, data); return; }                // If position is LAST append
    ArticleNode* loop = *list; position--;
    while (loop->next != NULL && position-- > 0) { loop = loop->next; }
    ArticleNode* aux = loop->next;
    loop->next = MALLOC_ART; loop->next->next = aux;
    loop->next->data = data;
}

ArticleNode* LookForArticles(ArticleNode* list, const char code[], const char name[], const float price, const int ammount) {
    // NULL a string to skip
    // ZERO a value to skip
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

void PrintSingleArticle(ArticleNode* list) {
    if (list == NULL) { printf("Article is NULL\n"); }
    printf("Article:\n");
    printf("code: %s\nname: %s\nprice: %f\nammount: %d\n", list->data.code, list->data.name, list->data.price, list->data.ammount);
}

void PrintArticleList(ArticleNode* list) {
    if (list == NULL) { printf("List is NULL\n"); }
    while (list) { 
        PrintSingleArticle(list);
        list = list->next;
    }
}

void ReadFileArticle(ArticleNode** list, const char dir[]) {
    FILE *f; f = fopen(dir, "r");
    if (f == NULL) { return; }           // If file is NULL return
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
    if (f == NULL) { fclose(f); return; }           // If file is NULL return
    if (list == NULL) { fprintf(f, ""); return; }   // If list is NULL return 
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
