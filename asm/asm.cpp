#include "asm.h"

const int Labels_size   = 512;  
int Labels[Labels_size] = {};

int main() {

    memset(Labels, -1, sizeof(int) * Labels_size);

    buffer asm_commands = {} ;
    FillBuffer(&asm_commands);

    int count_errors = 0;
    CheckingForCorrectData (&asm_commands, &count_errors);

    token* tokens = FillStruct (&asm_commands); // tokens

    ReplacingArgumentsInJmp(tokens, &asm_commands, Labels);

    if (count_errors) {
        return ERROR_IN_FILE;
    }

    char* command                      = nullptr;
    size_t number_of_elements_to_fread = asm_commands.numbers_of_strings;
    Data* test_bin_commands            = (Data*) calloc(number_of_elements_to_fread, sizeof(Data));
    ASSERT(test_bin_commands != nullptr, return MEMORY_ALLOCATION_ERROR);

    int value               = 0;
    int command_number      = 0;
    size_t test_bin_number  = 0;

    #define CMD(cmd) if(!strcmp(#cmd, command)){       \
        value = asm_##cmd;                             \
        test_bin_commands[test_bin_number] = value;    \
        } else                                         

    for ( ; (command = tokens[command_number].string) != nullptr; command_number++, test_bin_number++) { 

        #include "cmds.h"
        if (strstr(register_arguments, command)) { MarkRegisterCommand(&test_bin_commands, &test_bin_number, &number_of_elements_to_fread, command);}
        else if (!strcmp("L", command))          { command_number++;  test_bin_number--;}
        else { 
            MarkNotRegisterCommand(&test_bin_commands, &test_bin_number, &number_of_elements_to_fread);
            test_bin_commands [test_bin_number] = atof(command); 
        }
    }

    test_bin_commands = (Data*) realloc(test_bin_commands, sizeof(Data) * (test_bin_number));
    ASSERT(test_bin_commands != nullptr, return MEMORY_ALLOCATION_ERROR);

    #undef CMD

    FILE* replacement = fopen("test.bin", "wb");
    ASSERT(replacement != nullptr, return ERROR_IN_OPEN_FILE);

    size_t check_fwrite = fwrite(test_bin_commands, sizeof(Data), test_bin_number, replacement);
    ASSERT(check_fwrite == test_bin_number, return ERROR_IN_FWRITE);

    char check_file_close = fclose(replacement);
    ASSERT(check_file_close == 0, return ERROR_IN_CLOSE_FILE);

    Destructor(&asm_commands, &tokens, &test_bin_commands);

    return 0;
}




