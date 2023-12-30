#ifndef _DATE_H
#define _DATE_H
typedef struct Date {
    int day;
    int month;
    int year;
} Date;
#endif

Date CreateDate(const int day, const int month, const int year);
void CopyDate(Date* date, const Date data);
