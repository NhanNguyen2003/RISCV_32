#include "ulib.h"
#include "syscall.h"

void print(const char* str) {
    int len = 0;
    while(str[len]) len++;
    write(STDOUT, str, len);
}

void puts(const char* str) {
    print(str);
    print("\n");
}

int strcmp(const char* s1, const char* s2) {
    while(*s1 && *s1 == *s2) {
        s1++; s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

// RV32I safe string_to_int (No hardware multiply)
int atoi(const char* str) {
    int res = 0;
    int sign = 1;
    if (*str == '-') { sign = -1; str++; }
    
    while (*str >= '0' && *str <= '9') {
        int d = *str - '0';
        int temp = res;
        // res = res * 10 + d using shifts
        res = (temp << 3) + (temp << 1) + d; 
        str++;
    }
    return sign < 0 ? -res : res;
}

// RV32I safe int_to_string (No hardware divide)
void itoa(int num, char* buf) {
    if (num == 0) { buf[0] = '0'; buf[1] = 0; return; }
    
    int i = 0, neg = 0;
    if (num < 0) { neg = 1; num = -num; }
    
    char temp[16];
    int t_i = 0;
    
    while (num > 0) {
        int q = 0, r = num;
        // Software division by 10
        while (r >= 10) { r -= 10; q++; }
        temp[t_i++] = r + '0';
        num = q;
    }
    if (neg) temp[t_i++] = '-';
    
    while (t_i > 0) buf[i++] = temp[--t_i];
    buf[i] = 0;
}

void memset(void* dst, int c, int n) {
    char* d = (char*)dst;
    while(n-- > 0) *d++ = c;
}
