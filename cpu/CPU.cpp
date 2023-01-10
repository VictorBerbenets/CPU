#include "CPU.h"

// const int Registers_size = 6;
// int cpu_registers[Registers_size] = {};
//#include "asm.h"
    //1) Сделать массив меток размером 512(например) забитый -1 (обозначение неправильной метки)
    //2) Компиляция в два прохода ОДИНАКОВЫХ
    //3) в проце сделать push_reg - взять значение из стека и положить в переданный в аргументе регистр, pop_reg - взять значение из стека и положить в регистр

int main(){

    CPU my_cpu = {};
    StackCtor(&my_cpu.stack_cpu) ;
    _StackDump(&my_cpu.stack_cpu);

    stack addresses_for_call =   {};
    StackCtor(&addresses_for_call) ;

    my_cpu.data_size    = getFileSize("..//asm//test.bin") / sizeof(Data);
    my_cpu.data         = (Data*) calloc(my_cpu.data_size, sizeof(Data));
    ASSERT(my_cpu.data != nullptr, return ERROR_IN_GIVING_MEMORY);

    FILE* TEST_BIN   = fopen("..//asm//test.bin", "rb");
    ASSERT(TEST_BIN != nullptr, return ERROR_IN_OPENNING_FILE);

    size_t is_fread_read_the_data = fread(my_cpu.data, sizeof(Data), my_cpu.data_size, TEST_BIN);
    ASSERT(is_fread_read_the_data == my_cpu.data_size, return ERROR_IN_READING_FILE);

    char is_file_closed = fclose(TEST_BIN);
    ASSERT(!is_file_closed, return ERROR_IN_CLOSSING_FILE);

    for (size_t i = 0; i < my_cpu.data_size; i++) {
        printf("[%zd] = %lg\n", i, my_cpu.data[i]);
    }
    printf("\n");
    
    for (int number = 0 ; number < (int)my_cpu.data_size && number != Stop_programm; number++){ // while 
        _StackCheck(&my_cpu.stack_cpu);

        switch ((int)my_cpu.data[number]) {              
            case cpu_push:       push (&my_cpu,&number)  ;                                          break;
            case cpu_pop:        pop  (&my_cpu, &number) ;                                          break;
            case cpu_in:         in   (&my_cpu.stack_cpu, &number);                                 break;
            case cpu_add:        add  (&my_cpu.stack_cpu);                                          break;
            case cpu_mul:        mul  (&my_cpu.stack_cpu);                                          break;                              
            case cpu_sub:        sub  (&my_cpu.stack_cpu);                                          break;  
            case cpu_sum:        sum  (&my_cpu.stack_cpu);                                          break;                        
            case cpu_out:        out  (&my_cpu.stack_cpu);                                          break;                               
            case cpu_div:        div  (&my_cpu.stack_cpu);                                          break;               
            case cpu_call:       call (&my_cpu, &number, &addresses_for_call);                      break;
            case cpu_sqrt:       sqrt (&my_cpu.stack_cpu);                                          break;
            case cpu_jmp:        jmp  (&my_cpu, &number);                                           break;
            case cpu_jb:         jb   (&my_cpu, &number);                                           break;       
            case cpu_jbe:        jbe  (&my_cpu, &number);                                           break;
            case cpu_ja:         ja   (&my_cpu, &number);                                           break;
            case cpu_jae:        jae  (&my_cpu, &number);                                           break;
            case cpu_je:         je   (&my_cpu, &number);                                           break;
            case cpu_jne:        jne  (&my_cpu, &number);                                           break;
            case cpu_ret:        ret  (&addresses_for_call, &number);                               break;
            case cpu_print:      print(&my_cpu);                                                    break;
            case cpu_hlt:        hlt  (&number);                                                    break;                
            case cpu_text_begin: text (&my_cpu, &number);                                           break;                           
            default:             error(&my_cpu, number);                                            break;
        }
    }

    _StackCheck(&my_cpu.stack_cpu);
    _StackDump (&my_cpu.stack_cpu);

    StackDtor  (&my_cpu.stack_cpu);
    StackDtor  (&addresses_for_call);

    return 0;
 }