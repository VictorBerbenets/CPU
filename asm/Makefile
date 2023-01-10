CC = g++

CFLAGS =-c -g -Wall -Wextra 

all : ASM

ASM : asm.functions.o  asm.o check_commands.o
	$(CC) asm.functions.o asm.o check_commands.o -o ASM

asm.functions.o : asm.functions.cpp
	$(CC) $(CFLAGS) $^
	
asm.o : asm.cpp
	$(CC) $(CFLAGS) $^

check_commands.o : check_commands.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf *.o ASM