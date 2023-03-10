#ifndef ASM
#define ASM

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>

#include "colors.h"
#include "my_assert.h"
#include "registers.h"


typedef double Data;

const int shift_by_8_bytes                = 8;

const int len_registr                     = 2;

const char register_arguments[]           = "ax bx cx dx ex fx";

const char separates_symbols[]            = " '\n''\t'':'";

const char skip_simbols[]                 = " '\t'':'";

const int Ram_size                        = 1024;

const int bite_for_register               = 1;

const int Push_reg                        = 69;

const int Push_number                     = 0;

const int Push_or_Pop_ram                 = -1;

const int Push_or_Pop_ram_reg             = -2;           

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

typedef struct {

    const char* label_name;
    int lable_number;
} Label;

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
    INVALID_RAM                =  0xFEED2,
    INVALID_RAM_VALUE          =  0xFEED5,
    INVALID_TEXT1              =  0xDED1,
    INVALID_TEXT2              =  0xDED2,
    INVALID_TEXT3              =  0xDED3,              
    UNCORRECT_DATA             =  0XDED02,
    ERROR_IN_FWRITE            =  0XDED03,
    ERROR_IN_OPEN_FILE         =  0XDED04,
    ERROR_IN_CLOSE_FILE        =  0XDED05,
    ERROR_IN_READING_FILE      =  0XDED06,
    MEMORY_ALLOCATION_ERROR    =  0XDED07,
    ERROR                      =  0XDED08,
} ;

enum Commands{

#define CMD(cmd, ...) asm_##cmd,

#include "cmds.h"

#undef CMD

} ;


size_t getFileSize (const char *file);

size_t CountComands (buffer* asm_commands); 

token* FillStruct (buffer* asm_commands);

void FillToks(size_t* tokens_number, size_t number_of_commands, token** tokens, char* string);

void ReplacingArgumentsInJmp (token* toks, buffer* asm_commands, int* labels);

void CheckingForCorrectData (buffer* asm_commands, int* count_errors, const char* file_name) ;

void StringsDeveloper (buffer* asm_commands, char*** pts);

void Destructor (buffer* asm_commands, token** toks, char** test_bin_commands);

void ValidationOfInputData (char* string, int line, int* count_errors, const char* file_name);

void FillBuffer (buffer* asm_commands, const char* file_name);

void SkipComments(char** current_symbol_ptr, size_t* symbols_in_one_string, size_t* symbols, size_t buf_size);

void Check_Command_Without_Argument(char** pt, int line, char* string, int* count_errors, const char* file_name);

int Check_First_Command(char** pt, char* data, int line, char* string, int* count_errors, const char* file_name);

void Check_jmp(char** pt, int line, char* string, int* count_errors, const char* file_name);

void Check_push(char** pt, int line, char* string, int* count_errors, const char* file_name);

void Check_pop(char** pt, int line, char* string, int* count_errors, const char* file_name);

void Print_Incorrect_Data(char* data, int line, char* string, int* count_errors);

void Check_label(char** pt, int line, char* string, int* count_errors, const char* file_name);

void Check_call (char** pt, int line, char* string, int* count_errors, const char* file_name);

void PrintErrorForCommand(const char* func, const char* file_name, int _line_, int line, char* string, int* count_errors,\
int numbers_of_errors, ...);

void MarkRegisterCommand(char** test_bin_commands, size_t* test_bin_number, size_t* size, char* command);

void MarkNotRegisterCommand(char** test_bin_commands, size_t* test_bin_number, size_t* size, char* command);

void GetTextInArray (char** test_bin_commands, size_t* test_bin_number, char* text_string, size_t* size);

void Check_ram_command(char*** current_token, int line, char* string, int* count_errors, const char* file_name);

void Check_db (char** pt, int line, char* string, int* count_errors, const char* file_name);

void GetNumbersOfStrings (buffer* asm_commands);

void SkipFirstSpacesInText(char** string);

int IsLabelCommand(const char* command);

void Replacement (buffer* asm_commands);

int IsCorrectData(char* command);

void SkipSpaces(char*** pointer);

int GiveRegistor(char* command);

int Is_Label(char* string);

int Strlen(char* string);

void foo(int line);



#endif


// pop [12]
// push [12]
// video