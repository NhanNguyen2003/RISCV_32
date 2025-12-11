#include "syscall.h"
#include "ulib.h"

// A large number to burn CPU cycles
// Adjust this if your emulator is too fast or too slow
#define DELAY_LOOP 50000 

void worker(int id) {
    int i;
    volatile int j; // volatile prevents compiler optimization
    char buf[16];

    for (i = 0; i < 5; i++) {
        // Print "Task [ID]: [Iteration]\n"
        print("Task ");
        itoa(id, buf);
        print(buf);
        print(": ");
        itoa(i, buf);
        puts(buf);

        // Busy wait to simulate CPU work
        for (j = 0; j < DELAY_LOOP; j++) {
            asm volatile("nop");
        }
    }
    exit(0);
}

int main() {
    print("\n--- Multi-tasking Test ---\n");
    print("Parent: Forking children...\n");

    int i;
    int pids[3];

    for (i = 0; i < 3; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            print("Fork failed!\n");
            exit(1);
        }

        if (pids[i] == 0) {
            // Child process execution
            // IDs will be 1, 2, 3
            worker(i + 1); 
        }
    }

    print("Parent: Waiting for children...\n");

    // Wait for all 3 children
    for (i = 0; i < 3; i++) {
        int status;
        int child_pid = wait(&status);
        
        print("Parent: Child ");
        char buf[16];
        itoa(child_pid, buf);
        print(buf);
        print(" finished.\n");
    }

    print("--- Test Complete ---\n");
    return 0;
}
