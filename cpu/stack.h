#ifndef Stack_program
#define Stack_program

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>



#define _StackDump(st) StackDump(st, __LINE__, __PRETTY_FUNCTION__, #st)
#define _StackCheck(st) StackCheck(st, __LINE__, __PRETTY_FUNCTION__)

typedef double Data;

typedef struct{

    Data *data;
    size_t size; // сколько реально лежит элементов в стеке
    size_t capacity; // Сколько может вместить участок памяти, который я выделил
    char stack_is_damaged;

} stack;


enum{
    ElemIsPoison    = 234567,
    NoPoison        = 345678,
    Pop_Is_Ok       = 456789,
    StackNotEmpty   = 0x25,
    StackIsEmpty    = 0x26,
    StackNotFull    = 0x1,
    StackFull       = 0x2,
    POISON          = 0xDEADBABE,
    MinusSize       = 3,
    MinSize         = 2,
    MaxStayedSize   = 5,
    PUSH            = 111,
    POP             = 222
};

enum ErrorCodes{
    NULL_STACK          = 0x325235,
    NULL_PTR            = (1 << 0),
    INVALID_SIZE        = (1 << 1),
    INVALID_CAPACITY    = (1 << 2),
    ElemOfStackIsPoison = (1 << 3)
    

};

void StackDump (stack* st, int line, const char func[], const char* stack_name);

void StackCtor (stack* st);

void StackDtor (stack* st);

void StackPush (stack* st, Data value);

void StackCheck (stack* st, int line, const char func[]);

Data StackPop (stack* st);

void StackResize(stack* st, int Push_or_Pop);

void StackRecalloc (int Push_or_POP, stack* st, int new_size);

int IsStackEmpty (stack* st);

int IsStackFull (stack* st);

void DoPoison (stack* st);

int IsDataValid(stack* st);

void PrintError (int ErrCode, int line, const char func[]);

#endif