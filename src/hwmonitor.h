#ifndef UTILS
#define UTILS
char *read_file(char *filename);
char *findstr(const char *src, const char *srch, const char *delim);
#endif

#ifndef CPU
#define CPU
void getTotalMem();
#endif