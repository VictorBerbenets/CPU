# CC=g++

# CFLAGS=-Wall -Wextra 

# SRCS1=asm/asm.cpp asm/asm.functions.cpp asm/check_commands.cpp 
# SRCS2=cpu/CPU.cpp cpu/cpu.functions.cpp cpu/Functions.stack.cpp

# OBJ1=$(SRCS1:.cpp=.o)
# OBJ2=$(SRCS2:.cpp=.o)


# all:
# 	make ASM
# 	make CPU

# ASM: $(OBJ1)
# 	$(CC) $(CFLAGS) $(OBJ1) -o ASM

# CPU: $(OBJ2)
# 	$(CC) $(CFLAGS) $(OBJ2) -o CPU

# *.o: *.cpp
# 	$(CC) $(CFLAGS) *.cpp -c


# clean:
# 	rm -rf *.o CPU ASM
# 	rm -rf */*.o 

# .PHONY: clean
CC = g++

CFLAGS =-c -Wall -Wextra 

all : CPU

CPU : cpu.functions.o  CPU.o Functions.stack.o
	$(CC) cpu.functions.o CPU.o Functions.stack.o -o CPU

cpu.functions.o : cpu.functions.cpp
	$(CC) $(CFLAGS) $^
	
CPU.o : CPU.cpp
	$(CC) $(CFLAGS) $^

Functions.stack.o: Functions.stack.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf *.o CPU