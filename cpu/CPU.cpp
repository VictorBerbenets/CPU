#include "CPU.h"


int main(){

    // CPU my_cpu = {};
    // StackCtor(&my_cpu.stack_cpu) ;
    // _StackDump(&my_cpu.stack_cpu);

    // stack addresses_for_call =   {};
    // StackCtor(&addresses_for_call) ;

    // my_cpu.data_size    = getFileSize("..//asm//test.bin") / sizeof(Data);
    // my_cpu.data         = (Data*) calloc(my_cpu.data_size, sizeof(Data));
    // ASSERT(my_cpu.data != nullptr, return ERROR_IN_GIVING_MEMORY);

    // FILE* TEST_BIN   = fopen("..//asm//test.bin", "rb");
    // ASSERT(TEST_BIN != nullptr, return ERROR_IN_OPENNING_FILE);

    // size_t is_fread_read_the_data = fread(my_cpu.data, sizeof(Data), my_cpu.data_size, TEST_BIN);
    // ASSERT(is_fread_read_the_data == my_cpu.data_size, return ERROR_IN_READING_FILE);

    // char is_file_closed = fclose(TEST_BIN);
    // ASSERT(!is_file_closed, return ERROR_IN_CLOSSING_FILE);
    
    // for (int number = 0 ; number < (int)my_cpu.data_size && number != Stop_programm; number++){ // while 
    //     _StackCheck(&my_cpu.stack_cpu);

    //     switch ((int)my_cpu.data[number]) {              
    //         case cpu_push:       push (&my_cpu,&number)  ;                                          break;
    //         case cpu_pop:        pop  (&my_cpu, &number) ;                                          break;
    //         case cpu_in:         in   (&my_cpu.stack_cpu, &number);                                 break;
    //         case cpu_add:        add  (&my_cpu.stack_cpu);                                          break;
    //         case cpu_mul:        mul  (&my_cpu.stack_cpu);                                          break;                              
    //         case cpu_sub:        sub  (&my_cpu.stack_cpu);                                          break;  
    //         case cpu_sum:        sum  (&my_cpu.stack_cpu);                                          break;                        
    //         case cpu_out:        out  (&my_cpu.stack_cpu);                                          break;                               
    //         case cpu_div:        div  (&my_cpu.stack_cpu);                                          break;               
    //         case cpu_call:       call (&my_cpu, &number, &addresses_for_call);                      break;
    //         case cpu_sqrt:       sqrt (&my_cpu.stack_cpu);                                          break;
    //         case cpu_jmp:        jmp  (&my_cpu, &number);                                           break;
    //         case cpu_jb:         jb   (&my_cpu, &number);                                           break;       
    //         case cpu_jbe:        jbe  (&my_cpu, &number);                                           break;
    //         case cpu_ja:         ja   (&my_cpu, &number);                                           break;
    //         case cpu_jae:        jae  (&my_cpu, &number);                                           break;
    //         case cpu_je:         je   (&my_cpu, &number);                                           break;
    //         case cpu_jne:        jne  (&my_cpu, &number);                                           break;
    //         case cpu_ret:        ret  (&addresses_for_call, &number);                               break;
    //         case cpu_print:      print(&my_cpu);                                                    break;
    //         case cpu_hlt:        hlt  (&number);                                                    break;                
    //         case cpu_text_begin: text (&my_cpu, &number);                                           break; 
    //         case cpu_meow:       meow ();                                                           break;                          
    //         default:             error(&my_cpu, number);                                            break;
    //     }
    // }

    // _StackCheck(&my_cpu.stack_cpu);
    // _StackDump (&my_cpu.stack_cpu);

    // StackDtor  (&my_cpu.stack_cpu);
    // StackDtor  (&addresses_for_call);

     CPU my_cpu = {};
    StackCtor(&my_cpu.stack_cpu) ;
    _StackDump(&my_cpu.stack_cpu);

    stack addresses_for_call =   {};
    StackCtor(&addresses_for_call) ;

    my_cpu.data_size    = getFileSize("..//asm//test.bin") / sizeof(char);
    my_cpu.data         = (char*) calloc(my_cpu.data_size, sizeof(char));
    ASSERT(my_cpu.data != nullptr, return ERROR_IN_GIVING_MEMORY);

    FILE* TEST_BIN   = fopen("..//asm//test.bin", "rb");
    ASSERT(TEST_BIN != nullptr, return ERROR_IN_OPENNING_FILE);

    size_t is_fread_read_the_data = fread(my_cpu.data, sizeof(char), my_cpu.data_size, TEST_BIN);
    ASSERT(is_fread_read_the_data == my_cpu.data_size, return ERROR_IN_READING_FILE);

    char is_file_closed = fclose(TEST_BIN);
    ASSERT(!is_file_closed, return ERROR_IN_CLOSSING_FILE);
    
    for (int i = 0; i < my_cpu.data_size; i++) {
        if (i >= 2 && my_cpu.data[i - 2] == 1 && my_cpu.data[i - 1] == 0) {
            printf("my_cpu.data[%d] = <%lg>\n", i, *((Data*)(my_cpu.data + i)));
            i += 7;
        }
        else if (my_cpu.data[i] == cpu_db) {
            printf("my_cpu.data[%d] = <%d>\n", i, my_cpu.data[i]);
            i++;
            while (my_cpu.data[i] != '^') {
                printf("my_cpu.data[%d] = <%c>\n", i, my_cpu.data[i]);
                i++;
            }
        }
        else if (my_cpu.data[i] == cpu_jmp || my_cpu.data[i] == cpu_je || my_cpu.data[i] == cpu_jne || my_cpu.data[i] == cpu_jb || my_cpu.data[i] == cpu_jbe || my_cpu.data[i] == cpu_ja || my_cpu.data[i] == cpu_jae || my_cpu.data[i] == cpu_call ) {
            printf("my_cpu.data[%d] = <%d>\n", i, my_cpu.data[i]);
            i++;
            printf("my_cpu.data[%d] = <%d>\n", i, *((int*)(my_cpu.data + i)));
            i += sizeof(int) - 1;
        }
        else {
            printf("my_cpu.data[%d] = <%d>\n", i, my_cpu.data[i]);
        }
    }
    printf("\n\n\n\n\n\n");
    for (int i = 1; i <= 6; i++) {
        my_cpu.cpu_registers[i] = 0;
    }
    for (int number = 0 ; number < (int)my_cpu.data_size && number != Stop_programm; number++){ // while 
        _StackCheck(&my_cpu.stack_cpu);
        printf("NUMBER = %d\n", number);
        // fprintf(stderr, "(int)my_cpu.data[%d] = %d\n", number, (int)my_cpu.data[number]);
        printf("registers: ax = %lg   bx = %lg   cx = %lg   dx = %lg    ex = %lg    fx = %lg\n", my_cpu.cpu_registers[1], my_cpu.cpu_registers[2], my_cpu.cpu_registers[3], my_cpu.cpu_registers[4], my_cpu.cpu_registers[5], my_cpu.cpu_registers[6]);
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
            case cpu_meow:       meow ();                                                           break;                          
            default:             error(&my_cpu, number);                                            break;
        }
    }

    _StackCheck(&my_cpu.stack_cpu);
    _StackDump (&my_cpu.stack_cpu);

    StackDtor  (&my_cpu.stack_cpu);
    StackDtor  (&addresses_for_call);

    Destructor(&my_cpu);

    return 0;
 }