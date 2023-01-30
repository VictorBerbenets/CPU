#include "asm.h"

const int Labels_size          = 512;  
int Labels[Labels_size]        = {};

int main() {

    memset(Labels, -1, sizeof(int) * Labels_size);

    buffer asm_commands = {} ;
    FillBuffer(&asm_commands);

    int count_errors = 0;
    CheckingForCorrectData (&asm_commands, &count_errors);

    if (count_errors) { free(asm_commands.buf); return ERROR_IN_FILE; } //@ - End programm if error was find in file @//

/////////////////////////////The_Main_Part/////////////////////////////////////////////////////////////////

    token* tokens = FillStruct (&asm_commands); 
    ReplacingArgumentsInJmp(tokens, &asm_commands, Labels);

    char* command                      = nullptr;
    size_t size = asm_commands.numbers_of_strings;
    char* test_bin_commands            = (char*) calloc(size, sizeof(char));
    
    ASSERT(test_bin_commands != nullptr, return MEMORY_ALLOCATION_ERROR);

    int value               = 0;
    int command_number      = 0;
    size_t test_bin_number  = 0;

    #define CMD(cmd) if(!strcmp(#cmd, command)){              \
        value = asm_##cmd;                                    \
        test_bin_commands[test_bin_number] = (char) value;    \
        } else                       

    for ( ; command_number < (int)asm_commands.numbers_of_strings; command_number++, test_bin_number++) { 

        command = tokens[command_number].string;

        #include "cmds.h"
        if (command_number >= 1 && !strcmp("db", tokens[command_number - 1].string)) { 
            GetTextInArray(&test_bin_commands, &test_bin_number, command, &size); 
        }
        else if (strstr(register_arguments, command)) { 
            MarkRegisterCommand(&test_bin_commands, &test_bin_number, &size, command);
        }
        else if (!strcmp("L", command)) { 
            command_number++;  test_bin_number--;
        }
        else {
            MarkNotRegisterCommand(&test_bin_commands, &test_bin_number, &size, command);
        }
    }
    test_bin_commands = (char*) realloc(test_bin_commands, sizeof(char) * (test_bin_number));
    ASSERT(test_bin_commands != nullptr, return MEMORY_ALLOCATION_ERROR);

    #undef CMD

    FILE* replacement = fopen("test.bin", "wb");
    ASSERT(replacement != nullptr, return ERROR_IN_OPEN_FILE);

    size_t check_fwrite = fwrite(test_bin_commands, sizeof(char), test_bin_number, replacement);
    ASSERT(check_fwrite == test_bin_number, return ERROR_IN_FWRITE);
    
    char check_file_close = fclose(replacement);
    ASSERT(check_file_close == 0, return ERROR_IN_CLOSE_FILE);

    Destructor(&asm_commands, &tokens, &test_bin_commands);

    return 0;
}




