// Values
#ifndef ZERO
#define ZERO 0
#endif 

#ifndef LAST
#define LAST -1
#endif 

#ifndef COL_SIZE
#define COL_SIZE 30
#endif 

#ifndef NULL_DATE
#define NULL_DATE CreateDate(0, 0, 0)
#endif 

// Functions
#ifndef NL
#define NL printf("\n")
#endif 

#ifndef TAB
#define TAB printf("    ");
#endif 

#ifndef CLR_BUF
#define CLR_BUF while(getchar() != '\n') {}
#endif 

#ifdef _WIN32
#define CLEAR system("cls");
#endif 

#ifndef _WIN32
#define CLEAR system("clear");
#endif

// Malloc
#ifndef MALLOC_ART
#define MALLOC_ART (struct ArticleNode *)malloc(sizeof(struct ArticleNode))
#endif 

#ifndef MALLOC_VEN
#define MALLOC_VEN (struct VendorNode *)malloc(sizeof(struct VendorNode))
#endif 

#ifndef MALLOC_CLI
#define MALLOC_CLI (struct ClientNode *)malloc(sizeof(struct ClientNode))
#endif 

#ifndef MALLOC_REN
#define MALLOC_REN (struct RenglonNode *)malloc(sizeof(struct RenglonNode))
#endif 

#ifndef MALLOC_FAC
#define MALLOC_FAC (struct FacturaNode *)malloc(sizeof(struct FacturaNode))
#endif 
