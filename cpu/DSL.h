#ifndef DSL_h
#define DSL_h

#define Push_number()         StackPush(&my_cpu->stack_cpu, *(Data*)(my_cpu->data + *number))
#define Push_register()       StackPush(&my_cpu->stack_cpu, my_cpu->cpu_registers[(int)my_cpu->data[*number]])
#define Push_ram_number()     StackPush(&my_cpu->stack_cpu, my_cpu->ram[*(int*)(my_cpu->data + *number)])
#define Pop()                 StackPop(&my_cpu->stack_cpu);
#define Pop_to_register()     my_cpu->cpu_registers[(int) my_cpu->data[*number]] = StackPop(&my_cpu->stack_cpu)
#define Pop_to_ram()          my_cpu->ram [*(int*)(my_cpu->data + *number)]      = StackPop(&my_cpu->stack_cpu)
#define Stack_pop_value1()    Data stack_value1 = StackPop(&my_cpu->stack_cpu)
#define Stack_pop_value2()    Data stack_value2 = StackPop(&my_cpu->stack_cpu)
#define Jmp_command_address() *((int*)(my_cpu->data + *number)) - 1
 
#endif