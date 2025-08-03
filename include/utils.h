#ifndef UTILS_H
#define UTILS_H
char *read_file(char *filename, char *regex);
char *findstr(const char *src, const char *srch, const char *delim);
char *trim(char *str);
int isregex(const char ch, char *regex, int size);
#endif