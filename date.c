typedef struct Date {
    int day;
    int month;
    int year;
} Date;

Date CreateDate(const int day, const int month, const int year) {
    Date ret;
    ret.day = day;
    ret.month = month;
    ret.year = year;
    return ret;
}

void CopyDate(Date* date, const Date data) {
    (*date).day = data.day;
    (*date).month = data.month;
    (*date).year = data.year;
}
