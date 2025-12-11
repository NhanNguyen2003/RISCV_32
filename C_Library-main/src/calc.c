#include "syscall.h"
#include "ulib.h"

int main() {
    char buf[32];
    
    print("\n--- Calculator ---\n");
    print("Enter A: ");
    
    // Simple readline implementation for calc
    int i=0; char c;
    while(1) {
        read(STDIN, &c, 1);
        if (c == '\n' || c == '\r') { write(STDOUT, "\n", 1); break; }
        write(STDOUT, &c, 1);
        buf[i++] = c;
    }
    buf[i] = 0;
    int a = atoi(buf);

    print("Enter B: ");
    i=0;
    while(1) {
        read(STDIN, &c, 1);
        if (c == '\n' || c == '\r') { write(STDOUT, "\n", 1); break; }
        write(STDOUT, &c, 1);
        buf[i++] = c;
    }
    buf[i] = 0;
    int b = atoi(buf);

    int sum = a + b;
    char res[16];
    itoa(sum, res);
    
    print("Result: ");
    puts(res);
    
    exit(0);
    return 0;
}
