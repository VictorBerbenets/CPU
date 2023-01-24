#include "asm.h"
#include "labels.h"


#define Grey   "\033[0m"
#define Red    "\033[0;31m"
#define White  "\033[1m"
#define Blue   "\033[0;36m"
#define Under  "\033[4m"

void Check_Push(char** pt, int line, char* string, int* count_errors ) {
    Data argument  = 0;
    int value     = 0;
    int symb      = 0;

    char* push_reg = (char*) calloc(len_registr + 1, sizeof(char));

    if ((value = sscanf(*pt, "%lg%n", &argument, &symb))) {

        *pt += symb;
        if (value == -1) {

            fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d': \"%s\" - no command for \"push\".\n\n", line, string);
            *count_errors += MISSING_ARGUMENT;
        }
        while (**pt != '\0') {

            if (**pt != ' ') {
                fprintf(stderr,"\n\033[0;31merror\033[0m %s:%d : \"%s\" -Invalid input\n" // make func
                "\"%s\" is invalid symbols.\n\n","test.asm", line, string, *pt);
                *count_errors += INVALID_SYMBOLS;
                break;
            }
        (*pt)++;
        }
        free(push_reg);
    }

    else if ((Strlen(*pt) == len_registr)) {
        int number = 0;
        
        while (**pt != '\0') {
            if (**pt != ' ') {
                push_reg[number] = **pt;
                number++;
            }
        (*pt)++;
        }

        if (strstr(register_arguments, push_reg) == nullptr) {      

            fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input\n"
            "\"%s\" is invalid symbols\n\n", line, string, *pt);
            *count_errors += INVALID_SYMBOLS;
        }

        free(push_reg);
    }
    
    else {
        
        fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input\n"
        "\"%s\" is invalid data - argument \"push\" must be a number.\n\n", line, string, *pt);
        *count_errors += INVALID_ARGUMENT;
        free(push_reg);
    }

}

void Check_Pop_Reg(char** pt, int line, char* string, int* count_errors ) {

    char* pop_reg = (char*) calloc(len_registr + 1, sizeof(char));

    if ((Strlen(*pt) == len_registr)) {
        int number = 0;
        while (**pt != '\0') {
            if (**pt != ' ') {
                pop_reg[number] = **pt;
                number++;
            }
        (*pt)++;
        }

        if (strstr(register_arguments, pop_reg) == nullptr) {
            fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input\n"
            "\"%s\" is invalid symbols\n\n", line, string, *pt);
            *count_errors += INVALID_SYMBOLS;
        }
        free(pop_reg);
    }
    else if (!Strlen(*pt)) {
        
        free(pop_reg);
        return ;
    }
    else {

        fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input\n"
        "\"%s\" is invalid data - argument \"pop\" must be register(ax, bx, cx, dx)\n\n", line, string, *pt);
        *count_errors += INVALID_ARGUMENT;
        free(pop_reg);
    }
}

    
void Check_Jmp(char** pt, int line, char* string, int* count_errors) {

    int value    = 0;
    int argument = 0;
    int symb     = 0;

    while (**pt == ' ') {
        (*pt)++;
    }

    if (**pt != ':') {
        fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input.\n"
        "\"%s\" is invalid data - argument \"jmp\" must be like: \": 'value'\".\n\n", line, string, *pt);
        *count_errors += INVALID_DATA;
        return ;
    }
    (*pt)++;

    if ((value = sscanf(*pt, "%d%n", &argument, &symb))) {
        
        *pt += symb;

        if (value == -1) {
            fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d': \"%s\" - no command for \"jmp\".\n\n", line, string);
            *count_errors += MISSING_ARGUMENT;
            return ;
        }
        else
            while (**pt != '\0') {
                
                if (**pt != ' ') {
                    fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input.\n"
                    "\"%s\" is invalid symbol(s).\n\n", line, string, *pt);
                    *count_errors += INVALID_SYMBOLS;
                    break;
                }
                (*pt)++;
            }
    }
    else {
        fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input.\n"
        "\"%s\" is invalid data - argument \"jmp\" must be a number.\n\n", line, string, *pt);
        *count_errors += INVALID_ARGUMENT;
    }
}

void Check_Command_Without_Argument(char** pt, int line, char* string, int* count_errors ) {

    while (**pt != '\0') {
        if (**pt != ' ') {
            fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input\n"
            "\"%s\" is invalid symbols. This command takes no arguments.\n\n", line, string, *pt);
            *count_errors += INVALID_SYMBOLS;
            break;
        }
    (*pt)++;
    }
}

int Check_First_Command(char** pt, char* data, int line, char* string, int* count_errors) {

    int symb      = 0;

    if (!sscanf(*pt, "%s%n", data, &symb) && symb != 0) {
        fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input\n"
        "\"%s\" is invalid command\n\n", line, string, data);
        *count_errors += INVALID_COMMAND;
    }

    if (symb == 0) {
        return ERROR;
    }

    if (Is_Label(string)) {
        return IsCorrectData((char*) "label");
    }

    *pt += symb;
    return IsCorrectData(data);
}

void Check_call (char** pt, int line, char* string, int* count_errors ) {

    int symb      = 0;
    int argument  = 0;
    int value     = 0;

    if ((value = sscanf(*pt, "%d%n", &argument, &symb))) {
        *pt += symb;
        if (value == -1) {

            fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d': \"%s\" - not label\n\n", line, string);
            *count_errors += MISSING_ARGUMENT;
        }
        if (value != -1) {

            if (!(argument >= Min_Label_Value && argument <= Max_Label_Value)) {
                fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" - Valid values for call from 1 to 512\n", line, string);
                *count_errors += INVALID_ARGUMENT;
            }
        }
        while (**pt != '\0') {

            if (**pt != ' ') {
                fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input\n"
                "\"%s\" is invalid symbols\n\n", line, string, *pt);
                *count_errors += INVALID_SYMBOLS;
                break;
            }
        (*pt)++;
        }
    }
    else {
        fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':   \"%s\" -Invalid input\n"
        "\"%s\" is invalid data - it must be label\n\n", line, string, *pt);
        *count_errors += INVALID_ARGUMENT;
    }
}


void Check_label(char** pt, int line, char* string, int* count_errors ) {

    int value    = 0;
    int argument = 0;
    int symb     = 0;

    while (**pt != 'L' && **pt != '\0') (*pt)++; 
    (*pt)++;
    
    if (**pt != ' ') {
        PrintErrorForCommand(line, string, 0, count_errors, 0, 1, INVALID_ARGUMENT_OF_LABEL0);
    }

    if ((value = sscanf(*pt, "%d%n", &argument, &symb))) {
        *pt += symb;
        if (value == -1) {
            PrintErrorForCommand(line, string, 0, count_errors, 0, 1, MISSING_ARGUMENT);
        }
        if (value != -1) {
            if (!(argument >= Min_Label_Value && argument <= Max_Label_Value)) {
                PrintErrorForCommand(line, string, 0, count_errors, 0, 1, INVALID_ARGUMENT_OF_LABEL1);
            }
            else if (current_labels[argument] == argument) {
                PrintErrorForCommand(line, 0, 0, count_errors, argument, 1, INVALID_ARGUMENT_OF_LABEL2);
            }
            else {  

                current_labels[argument] = argument;
            }
        }
        while (**pt != '\0') {

            if (**pt != ' ') {

                PrintErrorForCommand(line, string, pt, count_errors, 0, 1, INVALID_SYMBOLS);
                break;
            }
        (*pt)++;
        }
    }
    else  PrintErrorForCommand(line, string, pt, count_errors, 0, 1, INVALID_ARGUMENT_OF_LABEL3); 
}

void SkipSpaces(char*** pt) {

    while (***pt == ' ') {
        (**pt)++;
    }
}

void Check_db (char** pt, int line, char* string, int* count_errors) {
    printf("STRLEN = %d\n", Strlen(*pt));
    SkipSpaces(&pt);

    if (**pt != '\"') {
        fprintf(stderr, ""White"%s:%d:"Grey"In function "White"'%s':"Grey"\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
        fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"must be '\"' after 'db' as the start of the text(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n"Blue"%s"Grey"\n\n",__FILE__, __LINE__, line, string);
        *count_errors += 1;
        return ;
    }
    printf("STRLEN = %d\n", Strlen(*pt));
    if (*(*pt + Strlen(*pt) - 1) != '\"') {
        fprintf(stderr, ""White"%s:%d:"Grey"In function "White"'%s':"Grey"\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
        fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"must be '\"' in the end of the text (error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n"Blue"\t|\t%s"Grey"\n\t|\n",__FILE__, __LINE__, line, string);
        *count_errors += 1;

    if (*(*pt + Strlen(*pt) - 1) != '^') {
        fprintf(stderr, ""White"%s:%d:"Grey"In function "White"'%s':"Grey"\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
        fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"must be '^' in the end of the text before '\"'(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n"Blue"\t|\t%s"Grey"\n\t|\n",__FILE__, __LINE__, line, string);
        *count_errors += 1;
    }
    }
    while (**pt != '^' && **pt != '\0') {
        printf("*pt = <%c>\n", **pt);
        if (**pt == ' ') {
        fprintf(stderr, ""White"%s:%d:"Grey"In function "White"'%s':"Grey"\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
        fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"you can't use symbol ' ', instead of it use '_', please(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n"Blue"\t\t%s"Grey"\n\n",__FILE__, __LINE__, line, string);
            *count_errors += 1;
            return ;
        }
        (*pt) ++;
    }
}

//return number of not space's symbols
int Strlen(char* string) {  

    ASSERT(string != nullptr);

    int counter = 0;
    char* ptr = string;

    while (*(ptr) != '\0') {

        if (*ptr != ' ') {
            counter ++;
        }
        ptr++;
    }
    return counter;
}

int Is_Label(char* string) {

    ASSERT(string != nullptr);

    char* ptr = string;

    while (*ptr == ' ') {
        ptr++;
    }

    if (*ptr == ':') {

        *ptr = 'L';
        return 1;
    }
    return 0;
}


 void PrintErrorForCommand(int line, char* string, char** pt, int* count_errors, int argument, int numbers_of_errors, ...) {

    va_list ptr = {};
    va_start(ptr, numbers_of_errors);

    for (int current_number = 0; current_number < numbers_of_errors; current_number++) {

        switch(va_arg(ptr, int)) {
            case MISSING_ARGUMENT:             
                fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d': \"%s\" - missing argument for command.\n\n", line, string);
                *count_errors += MISSING_ARGUMENT;
                break;
            case INVALID_SYMBOLS:             
                fprintf(stderr,"\n\033[0;31merror\033[0m %s:%d : \"%s\" -Invalid input\n" 
                "\"%s\" is invalid symbols.\n\n","test.asm", line, string, *pt);
                *count_errors += INVALID_SYMBOLS;
                break;
            case INVALID_ARGUMENT:             
                fprintf(stderr, "\n\033[0;31merror" Grey "in line '%d':\"%s\" -Invalid input\n"
                "\"%s\" is invalid data - argument must be a number.\n\n", line, string, *pt);
                *count_errors += INVALID_ARGUMENT;
                break;
            case INVALID_DATA:             
                fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input.\n"
                "\"%s\" is invalid data - argument \"jmp\" must be like: \": 'number'\".\n\n", line, string, *pt);
                *count_errors += INVALID_DATA;
                break;
            case INVALID_COMMAND:             
                fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid input. "
                "Unknown command.\n\n", line, string);
                *count_errors += INVALID_COMMAND;
                break;
            case INVALID_ARGUMENT_OF_LABEL0:
                fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':   \"%s\" -Invalid input, after ':' must be ' '\n"
                "\n\n", line, string);
                *count_errors += INVALID_ARGUMENT_OF_LABEL0;
                break;
            case INVALID_ARGUMENT_OF_LABEL1:
                fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':\"%s\" -Invalid value of label. Valid values (from 1 to 512)\n", line, string);
                *count_errors += INVALID_ARGUMENT_OF_LABEL1;
                break;
            case INVALID_ARGUMENT_OF_LABEL2:
                fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d': \"%d\" - this label is already in using, you can't apply it twice\n", line, argument);
                *count_errors += INVALID_ARGUMENT_OF_LABEL2;
                break;
            case INVALID_ARGUMENT_OF_LABEL3:
                fprintf(stderr, "\n\033[0;31merror\033[0m in line '%d':   \"%s\" -Invalid input\n"
                "\"%s\" is invalid data - it must be label\n\n", line, string, *pt);
                *count_errors += INVALID_ARGUMENT_OF_LABEL3;
                break;
            default: fprintf(stderr, "Chtoto ne tak\n");
        }
    }
    va_end(ptr);
}

