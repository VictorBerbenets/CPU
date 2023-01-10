#ifndef ASM
#define ASM

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>


#include "my_assert.h"
#include "registers.h"

typedef double Data;

const int shift_by_8_bytes = 8;

const int len_registr                     = 2;

const char register_arguments[]           = "ax bx cx dx ex fx";

const char separates_symbols[]            = " '\n''\t'':'";

const char skip_simbols[]                 = " '\t'':'";

const int bite_for_register               = 1;

const int Push_reg                        = 777;

const int Pop_reg                         = 0;

struct buffer {

    char* buf;
    size_t numbers_of_strings;
    size_t buf_size;
} ;

struct token {

    char* string;
    int is_memory_allocated;
    int value;
    int cmd;
} ;

enum Labels {

    LABEL            = 0xFCD001,
    LABEL_SIZE       = 5,
    MEMORY_ALLOCATED = 0x777
} ;

enum ASM_errors {

    INVALID_DATA               = (1 << 0),
    INVALID_COMMAND            = (1 << 1),
    INVALID_SYMBOLS            = (1 << 2),
    INVALID_ARGUMENT           = (1 << 3),
    MISSING_ARGUMENT           = (1 << 4),
    INVALID_ARGUMENT_OF_LABEL0 = (1 << 5),
    INVALID_ARGUMENT_OF_LABEL1 = (1 << 6),
    INVALID_ARGUMENT_OF_LABEL2 = (1 << 7),
    INVALID_ARGUMENT_OF_LABEL3 = (1 << 8),
    ERROR_IN_FILE              = -0xFAAACE1,
    CORRECTDATA                =  0xDEDFEED1,
    UNCORRECTDATA              =  0XDED02,
    ERROR_IN_FWRITE            =  0XDED03,
    ERROR_IN_OPEN_FILE         =  0XDED04,
    ERROR_IN_CLOSE_FILE        =  0XDED05,
    ERROR_IN_READING_FILE      =  0XDED06,
    MEMORY_ALLOCATION_ERROR    =  0XDED07,
    ERROR                      =  0XDED08,
} ;

enum Commands{

#define CMD(cmd) asm_##cmd,

#include "cmds.h"

#undef CMD

} ;

size_t getFileSize (const char *file);

size_t CountComands (buffer* asm_commands); 

token* FillStruct (buffer* asm_commands);

void ReplacingArgumentsInJmp (token* toks, buffer* asm_commands, int* labels);

void CheckingForCorrectData (buffer* asm_commands, int* count_errors) ;

void Constructor (buffer* asm_commands, char*** pts);

void Destructor (buffer* asm_commands, token** toks, Data** test_bin_commands);

void Replacement (buffer* asm_commands);

void ValidationOfInputData (char* string, int line, int* count_errors);

int IsCorrectData(char* command);

void GetNumbersOfStrings (buffer* asm_commands);

void FillBuffer (buffer* asm_commands);

int Strlen(char* string);

int GiveRegistor(char* command);

int Is_Label(char* string);

int IsLabelCommand(const char* command);

void SkipComments(char** is_eof, size_t* symbols_in_one_string, size_t* symbols);

void Check_Command_Without_Argument(char** pt, int line, char* string, int* count_errors);

int Check_First_Command(char** pt, char* data, int line, char* string, int* count_errors);

void Check_Jmp(char** pt, int line, char* string, int* count_errors);

void Check_Push(char** pt, int line, char* string, int* count_errors);

void Check_Push_Register(char** pt, int line, char* string, int* count_errors );

void Check_Pop_Reg(char** pt, int line, char* string, int* count_errors);

void Print_Incorrect_Data(char* data, int line, char* string, int* count_errors);

void Check_label(char** pt, int line, char* string, int* count_errors);

void Check_call (char** pt, int line, char* string, int* count_errors);

void PrintErrorForCommand(int line, char* string, char** pt, int* count_errors, int argument, int numbers_of_errors, ...);

void MarkRegisterCommand(Data** test_bin_commands, size_t* test_bin_number, size_t* number_of_elements_to_fread, char* command);

void MarkNotRegisterCommand(Data** test_bin_commands, size_t* test_bin_number, size_t* number_of_elements_to_fread);

#endif


// pop [12]
// push [12]
// video