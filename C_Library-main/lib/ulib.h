#ifndef ULIB_H
#define ULIB_H
void print(const char* str);
void puts(const char* str);
int strcmp(const char* s1, const char* s2);
int atoi(const char* str);
void itoa(int num, char* buf);
void memset(void* dst, int c, int n);
#endif
