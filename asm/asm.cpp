#include "asm.h"

const int Labels_size          = 512;  
int Labels[Labels_size]        = {};

int main() {

    memset(Labels, -1, sizeof(int) * Labels_size);

    buffer asm_commands = {} ;
    FillBuffer(&asm_commands);

    int count_errors = 0;
    CheckingForCorrectData (&asm_commands, &count_errors);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
    if (count_errors) { free(asm_commands.buf); return ERROR_IN_FILE; } //@ - End programm if error was find in file @//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

    token* tokens = FillStruct (&asm_commands); // tokens
    ReplacingArgumentsInJmp(tokens, &asm_commands, Labels);

    // for (int i = 0; i < asm_commands.numbers_of_strings; i++) {
    //     printf("token[%d] = <%s>\n", i, tokens[i].string);
    // }


    fprintf(stderr, "asm_commands.numbers_of_strings = %zd\n", asm_commands.numbers_of_strings);
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

    for ( ; command_number < asm_commands.numbers_of_strings; command_number++, test_bin_number++) { 
        command = tokens[command_number].string;
        fprintf(stderr, "command[%d] = <%s>\n", command_number, command);
        fprintf(stderr, "test_bin_number = %zd\n", test_bin_number);
        #include "cmds.h"
        if (!strcmp("db", tokens[command_number - 1].string)) {printf("CCCCCCCCOM = <%s>\n", command); GetTextInArray(&test_bin_commands, &test_bin_number, command, &size); }
        else if (strstr(register_arguments, command)) { 
            fprintf(stderr, "Register_Command[%d] = <%s>\n", command_number, command);
            MarkRegisterCommand(&test_bin_commands, &test_bin_number, &size, command);
        }
        else if (!strcmp("L", command)) { 
            command_number++;  test_bin_number--;
        }
        else if (strcmp("push", tokens[command_number - 1].string)) {
            test_bin_commands [test_bin_number] = (char) atoi(command);
        }
        else {
            printf("ESSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");
            MarkNotRegisterCommand(&test_bin_commands, &test_bin_number, &size, command);
        }
    }
    fprintf(stderr, "TTTest_bin_numberRRRR = %zd\n", test_bin_number);
    test_bin_commands = (char*) realloc(test_bin_commands, sizeof(char) * (test_bin_number));
    ASSERT(test_bin_commands != nullptr, return MEMORY_ALLOCATION_ERROR);

    for (int i = 0; i < test_bin_number; i++) {
        if (i >= 2 && test_bin_commands[i - 2] == 1 && test_bin_commands[i - 1] == 0) {
            printf("test_bin_commadns[%d] = <%lg>\n", i, *((Data*)(test_bin_commands + i)));
            i += 7;
        }
        if (test_bin_commands[i] == asm_db) {
            printf("test_bin_commadns[%d] = <%d>\n", i, test_bin_commands[i]);
            i++;
            while (test_bin_commands[i] != '^') {
                printf("test_bin_commadns[%d] = <%c>\n", i, test_bin_commands[i]);
                i++;
            }
        }
        else {
            printf("test_bin_commadns[%d] = <%d>\n", i, test_bin_commands[i]);
        }
    }
    printf("ARR SIZEEE = %zd\n", size);

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




