#include "syscall.h"
#include "ulib.h"

int main() {
    print("INIT: Starting System...\n");
    
    // We keep the shell path ready
    char sh_path[] = "sh";
    char* args[2];
    args[0] = sh_path;
    args[1] = 0;

    while(1) {
        // 1. Fork the shell (The Console)
        print("INIT: Forking shell...\n");
        int shell_pid = fork();

        if (shell_pid < 0) {
            print("INIT: Fork failed! Retrying...\n");
            continue;
        }

        if (shell_pid == 0) {
            // --- Child Process (Shell) ---
            exec(sh_path, args);
            print("INIT: Exec failed\n");
            exit(1);
        } 
        else {
            // --- Parent Process (Init) ---
            // The "Reaper Loop"
            // We sit here handling ANY child exit. We only break this loop
            // to restart the shell if the *shell* specifically dies.
            while(1) {
                int status;
                // wait() blocks until ANY child exits
                int zombie_pid = wait(&status);

                if (zombie_pid == shell_pid) {
                    // The Shell specifically has died. 
                    // Break the inner loop to restart it.
                    print("INIT: Shell exited. Restarting.\n");
                    break; 
                }
                else if (zombie_pid > 0) {
                    // An adopted orphan has died.
                    // We successfully reaped it (wait returned its PID).
                    // We print a debug message and continue waiting.
                    print("INIT: Reaped orphan PID: ");
                    char buf[16];
                    itoa(zombie_pid, buf);
                    puts(buf);
                    // LOOP CONTINUES -> waiting for next zombie or shell
                }
                else {
                    // wait() returned -1 (Error or no children left??)
                    // This shouldn't happen if shell is alive.
                    // To be safe, we might yield or break to restart shell.
                     yield();
                }
            }
        }
    }
    return 0;
}
