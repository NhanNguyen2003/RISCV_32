#include "syscall.h"
#include "ulib.h"

// Helper to print integers (since we lack full printf)
void print_int(int n) {
    char buf[16];
    itoa(n, buf);
    print(buf);
}

// 1. exitwait: Tests if exit status is correctly passed to wait()
void exitwait(char *s) {
    int i, pid;
    print("\n--- Test: "); print(s); print(" ---\n");

    for(i = 0; i < 50; i++) {
        pid = fork();
        if(pid < 0){
            print(s); print(": fork failed\n");
            exit(1);
        }
        if(pid){
            // Parent
            int xstate;
            int wpid = wait(&xstate);
            if(wpid != pid){
                print(s); print(": wait wrong pid\n");
                exit(1);
            }
            if(i != xstate) {
                print(s); print(": wait wrong exit status. Expected "); 
                print_int(i); print(" got "); print_int(xstate); print("\n");
                exit(1);
            }
        } else {
            // Child
            exit(i);
        }
    }
    print(s); print(": OK\n");
}

// 2. twochildren: Tests waiting for multiple children
void twochildren(char *s) {
    print("\n--- Test: "); print(s); print(" ---\n");
    
    for(int i = 0; i < 50; i++) {
        int pid1 = fork();
        if(pid1 < 0){
            print(s); print(": fork failed\n");
            exit(1);
        }
        if(pid1 == 0){
            exit(0);
        } else {
            int pid2 = fork();
            if(pid2 < 0){
                print(s); print(": fork failed\n");
                exit(1);
            }
            if(pid2 == 0){
                exit(0);
            } else {
                wait(0);
                wait(0);
            }
        }
    }
    print(s); print(": OK\n");
}

// 3. forkfork: Concurrent forks to test scheduler/locking
void forkfork(char *s) {
    print("\n--- Test: "); print(s); print(" ---\n");
    int N = 2;
  
    for(int i = 0; i < N; i++){
        int pid = fork();
        if(pid < 0){
            print(s); print(": fork failed\n");
            exit(1);
        }
        if(pid == 0){
            for(int j = 0; j < 20; j++){
                int pid1 = fork();
                if(pid1 < 0){
                    exit(1);
                }
                if(pid1 == 0){
                    exit(0);
                }
                wait(0);
            }
            exit(0);
        }
    }

    int xstatus;
    for(int i = 0; i < N; i++){
        wait(&xstatus);
        if(xstatus != 0) {
            print(s); print(": fork in child failed\n");
            exit(1);
        }
    }
    print(s); print(": OK\n");
}

// 4. reparent2: Tests if grandchildren are correctly reparented to init
void reparent2(char *s) {
    print("\n--- Test: "); print(s); print(" ---\n");
    
    for(int i = 0; i < 50; i++){
        int pid1 = fork();
        if(pid1 < 0){
            print("fork failed\n");
            exit(1);
        }
        if(pid1 == 0){
            // Child creates grandchildren then exits immediately.
            // Grandchildren should be reparented to init.
            int p2 = fork();
            if(p2 == 0) exit(0);
            int p3 = fork();
            if(p3 == 0) exit(0);
            exit(0);
        }
        wait(0);
    }
    print(s); print(": OK\n");
}

int main() {
    print("Starting process tests...\n");
    
    exitwait("exitwait");
    twochildren("twochildren");
    forkfork("forkfork");
    reparent2("reparent2");
    
    print("\nALL TESTS PASSED\n");
    exit(0);
    return 0;
}
