#include "syscall.h"

long syscall(long num, long a0, long a1, long a2) {
    register long ret asm("a0");
    register long _a0 asm("a0") = a0;
    register long _a1 asm("a1") = a1;
    register long _a2 asm("a2") = a2;
    register long _num asm("a7") = num;

    asm volatile(
        "ecall"
        : "=r"(ret)
        : "r"(_a0), "r"(_a1), "r"(_a2), "r"(_num)
        : "memory"
    );
    return ret;
}

void exit(int code) {
    syscall(SYS_EXIT, code, 0, 0);
    while(1); // Should not return
}

int read(int fd, char* buf, int n) {
    return syscall(SYS_READ, fd, (long)buf, n);
}

int write(int fd, const char* buf, int n) {
    return syscall(SYS_WRITE, fd, (long)buf, n);
}

int yield(void) {
    return syscall(SYS_YIELD, 0, 0, 0);
}

int getpid(void) {
    return syscall(SYS_GETPID, 0, 0, 0);
}

int fork(void) {
    return syscall(SYS_FORK, 0, 0, 0);
}

int exec(char* path, char** argv) {
    return syscall(SYS_EXEC, (long)path, (long)argv, 0);
}

int wait(int* status) {
    return syscall(SYS_WAIT, (long)status, 0, 0);
}
