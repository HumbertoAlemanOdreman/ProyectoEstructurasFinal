#include "definitions.h"

#ifndef _ARTICLES_H
#define _ARTICLES_H
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
#endif

Article CreateArticle(const char code[], const char name[], const float price, const int ammount);
void CreateArticleList(ArticleNode** list, const Article data);
ArticleNode* LookForArticle(ArticleNode* list, const char code[], const char name[], const float price, const int ammount);
void PushArticle(ArticleNode** list, const Article data);
void AppendArticle(ArticleNode** list, const Article data);
void InsertArticle(ArticleNode** list, const Article data, int position);
ArticleNode* PreviousArticle(ArticleNode *list, ArticleNode *article);
ArticleNode* LookForArticles(ArticleNode* list, const char code[], const char name[], const float price, const int ammount);
void PrintSingleArticle(Article list);
void PrintArticleList(ArticleNode *list);
ArticleNode* SelectArticleFromList(ArticleNode *list);
void ReadFileArticle(ArticleNode** list, const char dir[]);
void SaveFileArticle(ArticleNode* list, const char dir[]);
int GetPositionArticle(ArticleNode* list, ArticleNode* data);
ArticleNode* GetArticleFromPosition(ArticleNode** list, int position);
void RemovePositionArticle(ArticleNode** list, int position);
Article InputCreateArticle();
ArticleNode* SearchArticle(ArticleNode* list, int single);
void ModifyArticle(ArticleNode* globalList, ArticleNode* list);
ArticleNode* MenuArticleSelection(ArticleNode** list);
void MenuArticle(ArticleNode** list);
