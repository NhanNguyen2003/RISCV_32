# user/Makefile

CROSS = riscv32-unknown-elf-
CC = $(CROSS)gcc
LD = $(CROSS)ld
OBJCOPY = $(CROSS)objcopy

# Flags for RV32I (No hardware mul/div, include lib folder)
CFLAGS = -march=rv32i -mabi=ilp32 -static -nostdlib -fno-builtin -O2 -Wall -I lib
LDFLAGS = -T linker.ld -static -nostdlib

# Library Objects (Shared by all programs)
LIB_OBJS = lib/start.o lib/syscall.o lib/ulib.o

# Target Programs
PROGS = init sh calc usertests multitest

all: $(PROGS)

# --- Compile Library ---
lib/%.o: lib/%.c
	$(CC) $(CFLAGS) -c $< -o $@

lib/%.o: lib/%.s
	$(CC) $(CFLAGS) -c $< -o $@

# --- Compile Programs ---

# Init (PID 1)
init: src/init.o $(LIB_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o init.elf
	# Optional: Create binary for raw loading if needed
	$(OBJCOPY) -O binary init.elf init.bin

# Shell
sh: src/sh.o $(LIB_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o sh.elf

# Calculator
calc: src/calc.o $(LIB_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o calc.elf

# Testing
usertests: src/usertests.o $(LIB_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o usertests.elf

# Testing multi tasking
multitest: src/multitest.o $(LIB_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o multitest.elf

# Generic rule for src C files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f lib/*.o src/*.o *.elf *.bin
