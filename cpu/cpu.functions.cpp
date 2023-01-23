#include "CPU.h"

long getFileSize(const char *file) {
    
    assert(file != NULL);

    struct stat buf = {};

    if (stat(file, &buf)) {

        fprintf(stderr, "\033[0;31mERROR\033[0m!!!\nFile <%s>: in function <%s>,  in line '%d' - error in function \"stat\"\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return -1;
    }

    return buf.st_size;
}

void in(stack* stack_cpu, int* number) {
    
    Data push_value = 0;
    char ch = 0;
    char arr[100];

    if (scanf("%lg", &push_value) && ((ch = getchar()) == '\n' || ch == ' ')) {
        StackPush(stack_cpu, push_value);
    }
    else {
        arr[0] = ch;
        int number_of_char = (ch == 0) ? 0:1;
        for ( ; (ch = getchar()) != '\n'; number_of_char++) {
            arr[number_of_char] = ch;
        }
        arr[number_of_char] = '\0';

        printf("\n*********************************************************************\n");
        printf("\033[0;31mERROR\033[0m!!!\n\"%s\" - invalid symbols\n", arr);
        printf("Please try again!\n");
        printf("*********************************************************************\n");
        
        (*number) --;
    }
}

void push(CPU* my_cpu, int* number) {

    (*number)++;
    _StackDump(&my_cpu->stack_cpu);
    // printf("number = %d\n", *number);
        // printf("my_cpu->data[*number] = %d\n", my_cpu->data[*number]);

    if (my_cpu->data[*number] == Push_reg) {
        // printf("my_cpu->data[*number] = %d\n", my_cpu->data[*number]);
        (*number)++;
        StackPush(&my_cpu->stack_cpu, my_cpu->cpu_registers[(int)my_cpu->data[*number]]);
        // printf("my_cpu->cpu_registers[my_cpu->data[%d]] = <%lg>\n", *number, my_cpu->cpu_registers[my_cpu->data[*number]]);
    }
    else {
        (*number)++;
        StackPush(&my_cpu->stack_cpu, *(Data*)(my_cpu->data + *number));
        // printf("*(Data*)(my_cpu->data + *number) = %lg\n", *(Data*)(my_cpu->data + *number));
        (*number) += sizeof(Data) - 1;
    }
    _StackDump(&my_cpu->stack_cpu);

}

void pop(CPU* my_cpu, int* number) {
    _StackDump(&my_cpu->stack_cpu);
    (*number)++;
    // printf("Number in pop2 = %d\n", *number);
    // printf("my_cpu->data[%d] = %d\n", *number, my_cpu->data[*number]);
    if (my_cpu->data[*number] == Pop_reg) { 
        (*number)++;
        Data x = StackPop(&my_cpu->stack_cpu);
        my_cpu->cpu_registers[(int) my_cpu->data[*number]] = x;
    }
    else {
        (*number)++;
        StackPop(&my_cpu->stack_cpu);
    }
    _StackDump(&my_cpu->stack_cpu);

}

void sqrt(stack* stack_cpu) {

    StackPush(stack_cpu, sqrt(StackPop(stack_cpu)));
}

void jmp(CPU* my_cpu, int* number) {

    (*number)++;

    // *number = my_cpu->data[*number] - 1;
    *number = *((int*)(my_cpu->data + *number)) - 1;
}

void jae(CPU* my_cpu, int* number) {

    (*number)++;
    Data stack_value1 = StackPop(&my_cpu->stack_cpu);
    Data stack_value2 = StackPop(&my_cpu->stack_cpu);

    if (stack_value1 >= stack_value2) {
        
        *number = *((int*)(my_cpu->data + *number)) - 1;
    }
    else {
        (*number) += sizeof(int) - 1;
    }
}

void ja(CPU* my_cpu, int* number) {

    (*number)++;
    Data stack_value1 = StackPop(&my_cpu->stack_cpu);
    Data stack_value2 = StackPop(&my_cpu->stack_cpu);

    if (stack_value1 > stack_value2) {

        *number = *((int*)(my_cpu->data + *number)) - 1;
    }
    else {
        (*number) += sizeof(int) - 1;
    }
    
}

void jbe(CPU* my_cpu, int* number) {

    (*number)++;
    Data stack_value1 = StackPop(&my_cpu->stack_cpu);
    Data stack_value2 = StackPop(&my_cpu->stack_cpu);

    if (stack_value1 <= stack_value2) {

        *number = *((int*)(my_cpu->data + *number)) - 1;
    }
    else {
        (*number) += sizeof(int) - 1;
    }
    
}

void jb(CPU* my_cpu, int* number) {

    (*number)++;
    Data stack_value1 = StackPop(&my_cpu->stack_cpu);
    Data stack_value2 = StackPop(&my_cpu->stack_cpu);
    if (stack_value1 < stack_value2) {

        *number = *((int*)(my_cpu->data + *number)) - 1;
    }
    else {
        (*number) += sizeof(int) - 1;
    }
}

void jne(CPU* my_cpu, int* number) {

    (*number)++;
    Data stack_value1 = StackPop(&my_cpu->stack_cpu);
    Data stack_value2 = StackPop(&my_cpu->stack_cpu);

    if (!is_equal(stack_value1, stack_value2)) {

        *number = *((int*)(my_cpu->data + *number)) - 1;
    }
    else {
        (*number) += sizeof(int) - 1;
    }
}

void je(CPU* my_cpu, int* number) {

    (*number)++;
    Data stack_value1 = StackPop(&my_cpu->stack_cpu);
    Data stack_value2 = StackPop(&my_cpu->stack_cpu);

    if (is_equal(stack_value1, stack_value2)) {

        *number = *((int*)(my_cpu->data + *number)) - 1;
    }

    else {
        (*number) += sizeof(int) - 1;
    }
}

void call(CPU* my_cpu, int* number, stack* addresses_for_call) {
    printf("Number call before = %d\n", *number);
    printf("Number call after  = %d\n", *number + sizeof(int));
    StackPush(addresses_for_call, *number + sizeof(int));
    (*number)++;
    // *number = my_cpu->data[*number] - 1;
    *number = *((int*)(my_cpu->data + *number)) - 1;

}

void ret(stack* addresses_for_call, int* number) {

    *number = (int) StackPop(addresses_for_call);
}

void add(stack* st) {

    if (st->size > 1) {

        Data sum = StackPop(st) + StackPop(st);
        StackPush(st, sum);
    }
    else {

        printf("Size of stack < 2, we can't get two numbers to plus them\n");
    }
}

void mul(stack* st) {

    if (st->size > 1) {

        Data multiplication = StackPop(st) * StackPop(st);
        StackPush(st, multiplication);
    }
    else {

        printf("Size of stack < 2, we can't get two numbers to mul them\n");
    }
}

void div(stack* st) {
    
    if (st->size > 1) {
        Data value1 = StackPop(st);
        Data value2 = StackPop(st);
        Data delim = value1 / value2;
        StackPush(st, delim);
    }
    else {

        printf("Size of stack < 2, we can't get two numbers to div them\n");
    }
}

void sum(stack* st) {
    
    if (st->size > 1) {

        Data value1 = StackPop(st);
        Data value2 = StackPop(st);

        Data diff = value1 + value2;

        StackPush(st, diff);
    }
    else {

        printf("Size of stack < 2, we can't get two numbers to sub them\n");
    }
}

void sub(stack* st) {
    
    if (st->size > 1) {

        Data value1 = StackPop(st);
        Data value2 = StackPop(st);

        Data diff = value1 - value2;

        StackPush(st, diff);
    }
    else {

        printf("Size of stack < 2, we can't get two numbers to sub them\n");
    }
}

void out(stack* st) {
    
    if (st->size > 0){

        FILE* log_txt = fopen("log.txt", "a");

        Data stack_value_out = st->data[st->size - 1];

        StackPop(st);

        fprintf(log_txt, "command <out> deleted '%lg' from stack\n", stack_value_out);
    }
    else
        printf("Stack is empty\n");
}

void hlt(int* number) {

    *number = Stop_programm - 1;
    printf("------------end of commands------------\n");
}

void print(CPU* my_cpu) {
    printf("WHERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRAERRRRRRRRRRRRRRRRRRRRRRYOUUUUUUUUUUUUU\n");
    _StackDump(&my_cpu->stack_cpu);
    Data last_number_in_stack = StackPop(&my_cpu->stack_cpu);
    printf("\n%.2lf\n", last_number_in_stack);
    StackPush(&my_cpu->stack_cpu, last_number_in_stack);
}

void error(CPU* my_cpu, int number) {

    printf("invalid command: %d\n", my_cpu->data[number]);
}

int is_equal(Data number1, Data number2) {
    
    Data delta = number1 - number2;

    if (delta < 0) {
        delta *= (-1);
    }
    
    return delta < Epsilon;
}

void text(CPU* my_cpu, int* number) {

    (*number)++;
    int number_of_symbols = 0;
    while ((int)my_cpu->data[*number] != cpu_text_end) {

        push(my_cpu, number);
        (*number) ++;
        number_of_symbols++;
    }
    for (int current_symbol = 0; current_symbol < number_of_symbols; current_symbol++) {
        printf("%c", (int)StackPop(&my_cpu->stack_cpu));
    }

}

void Destructor(CPU* my_cpu) {

    free(my_cpu->data);
}


void meow() {

    printf("\nmeeeow\n\n");
}