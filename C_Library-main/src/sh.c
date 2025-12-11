#include "syscall.h"
#include "ulib.h"

#define MAX_ARGS 10
#define BUF_SIZE 64

// Read a line into buf with basic echo
void readline(char* buf, int max) {
    int i = 0;
    while (i < max - 1) {
        char c;
        if (read(STDIN, &c, 1) < 1) break; // Read failed or EOF
        
        // Handle Enter/Return
        if (c == '\r' || c == '\n') {
            write(STDOUT, "\n", 1);
            break;
        }
        
        // Handle Backspace (Optional, if your terminal sends 127 or 8)
        if (c == 127 || c == 8) {
            if (i > 0) {
                // Visual backspace hack: move back, print space, move back
                write(STDOUT, "\b \b", 3);
                i--;
            }
            continue;
        }

        write(STDOUT, &c, 1); // Echo character back to user
        buf[i++] = c;
    }
    buf[i] = 0; // Null terminate string
}

int main() {
    char buf[BUF_SIZE];
    char* argv[MAX_ARGS + 1]; // +1 for the NULL terminator

    print("\n--- RISC-V Shell ---\n");

    while(1) {
        print("$ ");
        memset(buf, 0, BUF_SIZE);
        readline(buf, BUF_SIZE);

        if (buf[0] == 0) continue; // Empty line

        // --- FIXED TOKENIZER ---
        // Old tokenizer failed on leading spaces. This version skips them correctly.
        int argc = 0;
        char* ptr = buf;

        while(*ptr && argc < MAX_ARGS) {
            // 1. Skip leading spaces (and tabs if supported)
            while(*ptr == ' ') ptr++;
            
            // If we hit the end of the string, stop
            if (*ptr == 0) break;

            // 2. We found the start of a token
            argv[argc++] = ptr;

            // 3. Scan for the end of this token (find next space or null)
            while(*ptr && *ptr != ' ') ptr++;

            // 4. Terminate the token
            if (*ptr) {
                *ptr = 0; // Replace space with Null Terminator
                ptr++;    // Move to next character
            }
        }
        argv[argc] = 0; // Standard Exec requires NULL at end of argv array

        // If no arguments found (user just typed spaces), restart loop
        if (argc == 0) continue;

        // --- Internal Commands ---
        if (strcmp(argv[0], "exit") == 0) {
            exit(0);
        }

        // --- Fork and Exec ---
        int pid = fork();

        if (pid < 0) {
            print("sh: fork failed\n");
        } 
        else if (pid == 0) {
            // Child Process
            exec(argv[0], argv);
            
            // If exec returns, it failed
            print("sh: command not found: ");
            puts(argv[0]);
            exit(1);
        } 
        else {
            // Parent Process (Shell)
            
            // --- CRITICAL FIX ---
            // OLD: wait(0); -> Passed NULL pointer to Kernel (Crash!)
            // NEW: Pass a valid stack address
            int status;
            wait(&status);
            
            // Optional: You could check 'status' here if you wanted
        }
    }
    return 0;
}
