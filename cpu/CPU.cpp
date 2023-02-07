#include "CPU.h"


int main(){

    CPU my_cpu = {};
    StackCtor(&my_cpu.stack_cpu);
    _StackDump(&my_cpu.stack_cpu);

    stack addresses_for_call = {};
    StackCtor(&addresses_for_call);

    my_cpu.data_size    = getFileSize("..//asm//test.bin") / sizeof(char);
    my_cpu.data         = (char*) calloc(my_cpu.data_size + 1, sizeof(char));
    ASSERT(my_cpu.data != nullptr, return ERROR_IN_GIVING_MEMORY);

    FILE* TEST_BIN   = fopen("..//asm//test.bin", "rb");
    ASSERT(TEST_BIN != nullptr, return ERROR_IN_OPENNING_FILE);

    size_t is_fread_read_the_data = fread(my_cpu.data, sizeof(char), my_cpu.data_size, TEST_BIN);
    ASSERT(is_fread_read_the_data == my_cpu.data_size, return ERROR_IN_READING_FILE);

    char is_file_closed = fclose(TEST_BIN);
    ASSERT(!is_file_closed, return ERROR_IN_CLOSSING_FILE);
    

    #define CMD(name, body) case cpu_##name: body; break; 

    for (int number = 0 ; number < (int)my_cpu.data_size && number != Stop_programm; number++){

        _StackCheck(&my_cpu.stack_cpu);
        switch ((int)my_cpu.data[number]) {    
                      
            #include "..//asm//cmds.h"
            default: error(&my_cpu, number);
        }
    }

    #undef CMD
    
    _StackCheck(&my_cpu.stack_cpu);
    _StackDump (&my_cpu.stack_cpu);

    StackDtor  (&my_cpu.stack_cpu);
    StackDtor  (&addresses_for_call);

    Destructor(&my_cpu);

    return 0;
 }