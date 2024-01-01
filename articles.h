#ifndef COL_SIZE
#define COL_SIZE 30
#endif

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

void MenuArticle(ArticleNode** list);
ArticleNode* MenuArticleSelection(ArticleNode** list);
ArticleNode* SearchArticle(ArticleNode* list, int single);
void ReadFileArticle(ArticleNode**, const char dir[]);
void SaveFileArticle(ArticleNode*, const char dir[]);
