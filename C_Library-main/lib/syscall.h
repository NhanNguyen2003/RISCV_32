#ifndef SYSCALL_H
#define SYSCALL_H

// Syscall Numbers (Must match SystemCallHandler.java)
#define SYS_READ    63
#define SYS_WRITE   64
#define SYS_EXIT    93
#define SYS_YIELD   124
#define SYS_GETPID  172
#define SYS_FORK    220
#define SYS_EXEC    221
#define SYS_WAIT    260

#define STDIN  0
#define STDOUT 1

// System Call Prototypes
void exit(int code);
int read(int fd, char* buf, int n);
int write(int fd, const char* buf, int n);
int yield(void);
int getpid(void);
int fork(void);
int exec(char* path, char** argv);
int wait(int* status);

#endif
