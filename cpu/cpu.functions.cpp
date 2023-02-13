#include "CPU.h"


long getFileSize(const char *file) {
    
    assert(file != NULL);

    struct stat buf = {};

    if (stat(file, &buf)) {

        fprintf(stderr, "" Red "error" Grey "!!!\nFile <%s>: in function <%s>,  in line '%d' - error in function \"stat\"\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return -1;
    }

    return buf.st_size;
}


const int Max_text_size = 100;

void in(stack* stack_cpu, int* number) {
    
    Data push_value = 0;
    char ch = 0;
    char arr[Max_text_size];

    if (scanf("%lg", &push_value) && ((ch = getchar()) == '\n' || ch == ' ')) {
        StackPush(stack_cpu, push_value);
    }
    else {
        arr[0] = ch;
        int number_of_char = (ch == 0) ? 0:1;
        for ( ; (ch = getchar()) != '\n' && number_of_char < Max_text_size; number_of_char++) {

            arr[number_of_char] = ch;
        }
        arr[number_of_char] = '\0';

        printf("\n*********************************************************************\n");
        if (number_of_char == Max_text_size) { printf("Input size is too long. It must be < '%d' symbols\n", Max_text_size); }
        
        printf("" Red "error!!!" Grey "\n%s\" - invalid symbols\n", arr);
        printf("Please try again!\n");
        printf("***********************************************************************\n");
        
        (*number) --;
    }
}

// DSL.h - definding special language (какие команды повторяется задефайнить)
void push(CPU* my_cpu, int* number) {

    (*number)++;

    if (my_cpu->data[*number] == Push_reg) {
        (*number)++;
        Push_register();
    }
    else if (my_cpu->data[*number] == Push_or_Pop_ram) {
        (*number)++;
        Push_ram_number();
        (*number) += sizeof(int) - 1;
    }
    else if (my_cpu->data[*number] == Push_or_Pop_ram_reg) {
        (*number)++;
        Push_ram_reg();
        (*number) += sizeof(char) - 1;
        _StackDump(&my_cpu->stack_cpu);
    }
    else {
        (*number)++;
        Push_number();
        (*number) += sizeof(Data) - 1;
    }

}

void pop(CPU* my_cpu, int* number) {

    (*number)++;
    if (my_cpu->data[*number] == Pop_reg) { 

        (*number)++;
        Pop_to_register();
    }
    else if (my_cpu->data[*number] == Push_or_Pop_ram) {

        (*number)++;
        Pop_to_ram_number();
        (*number) += sizeof(int) - 1;
    }
    else if (my_cpu->data[*number] == Push_or_Pop_ram_reg) {

        (*number)++;
        Pop_to_ram_reg();
        (*number) += sizeof(char) - 1;
    }
    else {

        (*number)++;
        Pop();
    }

}

void sqrt(stack* stack_cpu) {

    StackPush(stack_cpu, sqrt(StackPop(stack_cpu)));
}

void jmp(CPU* my_cpu, int* number) {

    (*number)++;
    *number = Jmp_command_address();
}


void jae(CPU* my_cpu, int* number) {

    (*number)++;
    Stack_pop_value1();
    Stack_pop_value2();

    if (stack_value1 >= stack_value2) {

        *number = Jmp_command_address();
    }
    else {
        (*number) += sizeof(int) - 1;
    }
}

void ja(CPU* my_cpu, int* number) {

    (*number)++;
    Stack_pop_value1();
    Stack_pop_value2();

    if (stack_value1 > stack_value2) {

        *number = Jmp_command_address();
    }
    else {
        (*number) += sizeof(int) - 1;
    }
    
}

void jbe(CPU* my_cpu, int* number) {

    (*number)++;
    Stack_pop_value1();
    Stack_pop_value2();

    if (stack_value1 <= stack_value2) {

        *number = Jmp_command_address();
    }
    else {
        (*number) += sizeof(int) - 1;
    }
    
}

void jb(CPU* my_cpu, int* number) {

    (*number)++;
    Stack_pop_value1();
    Stack_pop_value2();

    if (stack_value1 < stack_value2) {

        *number = Jmp_command_address();
    }
    else {
        (*number) += sizeof(int) - 1;
    }
}

void jne(CPU* my_cpu, int* number) {

    (*number)++;
    Stack_pop_value1();
    Stack_pop_value2();

    if (!is_equal(stack_value1, stack_value2)) {

        *number = Jmp_command_address();
    }
    else {
        (*number) += sizeof(int) - 1;
    }
}

void je(CPU* my_cpu, int* number) {

    (*number)++;
    Stack_pop_value1();
    Stack_pop_value2();

    if (is_equal(stack_value1, stack_value2)) {

        *number = Jmp_command_address();
    }

    else {
        (*number) += sizeof(int) - 1;
    }
}

void call(CPU* my_cpu, int* number, stack* addresses_for_call) {

    StackPush(addresses_for_call, *number + sizeof(int));
    (*number)++;
    *number = Jmp_command_address();

}

void ret(stack* addresses_for_call, int* number) {

    *number = (int) StackPop(addresses_for_call);
}

void add(stack* st) {

    ASSERT(st->size > 1, fprintf(stderr, "Size of stack < 2, we can't get two numbers to add them\n"); exit(EXIT_FAILURE));

    StackPush(st, StackPop(st) + StackPop(st));
}

void mul(stack* st) {

    ASSERT(st->size > 1, fprintf(stderr, "Size of stack < 2, we can't get two numbers to mul them\n"); exit(EXIT_FAILURE));

    StackPush(st, StackPop(st) * StackPop(st));
}

void div(stack* st) {

    ASSERT(st->size > 1, fprintf(stderr, "Size of stack < 2, we can't get two numbers to div them\n"); exit(EXIT_FAILURE));
    
    Data stack_value1 = StackPop(st);
    Data stack_value2 = StackPop(st);

    Data delim = stack_value1 / stack_value2;

    StackPush(st, delim);
}

void sum(stack* st) {
    
    ASSERT(st->size > 1, fprintf(stderr, "Size of stack < 2, we can't get two numbers to sub them\n"); exit(EXIT_FAILURE));

    StackPush(st, StackPop(st) + StackPop(st));
}

void sub(stack* st) {
    
    ASSERT(st->size > 1, fprintf(stderr, "Size of stack < 2, we can't get two numbers to sub them\n"); exit(EXIT_FAILURE));

    Data value1 = StackPop(st);
    Data value2 = StackPop(st);

    Data diff = value1 - value2;

    StackPush(st, diff);
}

void out(stack* st) {
    
    if (st->size > 0){

        FILE* log_txt = fopen("log.txt", "a");

        Data stack_value_out = st->data[st->size - 1];

        StackPop(st);

        fprintf(log_txt, "command <out> deleted '%lg' from stack\n", stack_value_out);

        fclose(log_txt);
    }
    else
        printf("Stack is empty\n");
}

void hlt(int* number) {

    *number = Stop_programm - 1;
    printf("------------end of commands------------\n");
}

void print(CPU* my_cpu) {

    Data last_number_in_stack = StackPop(&my_cpu->stack_cpu);
    printf("%.2lf", last_number_in_stack);
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

void db (CPU* my_cpu, int* number) {

    (*number)++;
    while(my_cpu->data[(*number)] != '^') {

        if (my_cpu->data[(*number)] == '\\') {
            (*number)++;
            if (my_cpu->data[(*number)] == 'n') {
                printf("\n");
            }
            else if (my_cpu->data[(*number)] == 't') {
                printf("\t");
            }
            else if (my_cpu->data[(*number)] == 'v') {
                printf("\v");
            }
        }
        else {
            printf("%c", my_cpu->data[(*number)]);
        }

        (*number)++;
    }
}

const int video_size = 32;

void video(CPU* my_cpu) {

    printf("\n\t\t\t" Blinking "RAM_DUMP" Grey "\n");
    printf("" Blue "------------------------------------------------------------------\n|" Grey "");
    for (int ram_number = 0; ram_number < Ram_size; ram_number++) {

        if (my_cpu -> ram[ram_number] != 0) {
            printf("" Red "* " Grey "");
        } 
        else {
            printf(". ");
        }
        if ((ram_number + 1) % video_size == 0) {
            printf("" Blue "|\n|" Grey "");
        }
    }
    printf("" Blue "----------------------------------------------------------------|\n\n" Grey "");

}

void Destructor(CPU* my_cpu) {

    free(my_cpu->data);
}


void meow() {

    printf("\nmeeeow (Poltorashka forever!!!)\n\n");
}