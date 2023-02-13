#ifndef CPU_PROGRAMM
#define CPU_PROGRAMM

#include "..//asm//colors.h"
#include "..//asm//my_assert.h"
#include "stack.h"
#include "DSL.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "..//asm//registers.h"

const int Stop_programm         = -1;

const int Push_reg              = 69;
const int Pop_reg               = 69;
const int Push_or_Pop_ram       = -1;
const int Push_or_Pop_ram_reg   = -2;  


const int Ram_size       = 1024;
const int Registers_size = 7;

const Data Epsilon       = 1e-17;

struct CPU { 

    stack stack_cpu;
    size_t data_size;
    Data cpu_registers[Registers_size];
    char*  data;    
    int ram_elements;
    Data ram[Ram_size] = {};
} ;

enum CPU_errors {

    OK                     =       1,
    ERROR_IN_GIVING_MEMORY = 1111111,
    ERROR_IN_READING_FILE  = 2222222,
    ERROR_IN_OPENNING_FILE = 0xDED1 ,
    ERROR_IN_CLOSSING_FILE = 0xDED2 ,
};

#define CMD(cmd, ...) cpu_##cmd,

enum Commands{

#include "..//asm//cmds.h"

};

#undef CMD

long getFileSize(const char *file);

void push(CPU* my_cpu, int* number);

void pop(CPU* my_cpu, int* number);

void in(stack* stack_cpu, int* number);

void jmp(CPU* my_cpu, int* number);

void jae(CPU* my_cpu, int* number);

void ja(CPU* my_cpu, int* number);

void jbe(CPU* my_cpu, int* number);

void jb(CPU* my_cpu, int* number);

void jne(CPU* my_cpu, int* number);

void je(CPU* my_cpu, int* number);

void call(CPU* my_cpu, int* number, stack* addresses_for_call);

void ret(stack* addresses_for_call, int* number);

void add(stack* st);

void sub(stack* st);

void sum(stack* st);

void mul(stack* st);

void out(stack* st);

void div(stack* st);

void print(CPU* my_cpu);

void sqrt(stack* stack_cpu);

void hlt(int* number);

void error(CPU* my_cpu, int number);

int is_equal(Data number1, Data number2); 

void db (CPU* cpu, int* number);

void Destructor(CPU* my_cpu);

void video(CPU* cpu);

void meow();

#endif