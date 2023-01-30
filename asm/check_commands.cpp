#include "asm.h"
#include "labels.h"

char End_text_symb      = '^';


void Check_Push(char** pt, int line, char* string, int* count_errors ) {
    Data argument  = 0;
    int value     = 0;
    int symb      = 0;

    char* push_reg = (char*) calloc(len_registr + 1, sizeof(char));

    if ((value = sscanf(*pt, "%lg%n", &argument, &symb))) {

        *pt += symb;
        if (value == -1) {
            PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, MISSING_ARGUMENT); 
        }
        while (**pt != '\0') {

            if (**pt != ' ') {
                PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); 
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

            PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); 
        }
        free(push_reg);
    }  
    else {
        
        PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT); 
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
            PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_DATA); 
        }
        free(pop_reg);
    }
    else if (!Strlen(*pt)) {
        
        free(pop_reg);
        return ;
    }
    else {
        PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT); 
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
        PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_DATA); 
        return ;
    }
    (*pt)++;

    if ((value = sscanf(*pt, "%d%n", &argument, &symb))) {
        
        *pt += symb;

        if (value == -1) {
            PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, MISSING_ARGUMENT); 
            return ;
        }
        else
            while (**pt != '\0') {
                
                if (**pt != ' ') { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); }
                (*pt)++;
            }
    }
    else { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT); }
}

void Check_Command_Without_Argument(char** pt, int line, char* string, int* count_errors ) {

    while (**pt != '\0') {
        if (**pt != ' ') {
            PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); 
            break;
        }
    (*pt)++;
    }
}

int Check_First_Command(char** pt, char* data, int line, char* string, int* count_errors) {

    int symb      = 0;

    if (!sscanf(*pt, "%s%n", data, &symb) && symb != 0) { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_COMMAND);}

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
        if (value == -1) { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, MISSING_ARGUMENT); }
        if (value != -1) {

            if (!(argument >= Min_Label_Value && argument <= Max_Label_Value)) { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT); }
        }
        while (**pt != '\0') {

            if (**pt != ' ') { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); }
            (*pt)++;
        }
    }
else { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT); }
}


void Check_label(char** pt, int line, char* string, int* count_errors ) {

    int value    = 0;
    int argument = 0;
    int symb     = 0;

    while (**pt != 'L' && **pt != '\0') (*pt)++; 
    (*pt)++;
    
    if (**pt != ' ') { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT_OF_LABEL0); }

    if ((value = sscanf(*pt, "%d%n", &argument, &symb))) {
        *pt += symb;

        if (value == -1) {PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, MISSING_ARGUMENT); }

        if (value != -1) {
            if (!(argument >= Min_Label_Value && argument <= Max_Label_Value)) { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT_OF_LABEL1); }
            else if (current_labels[argument] == argument)                     { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT_OF_LABEL2); }
            else { current_labels[argument] = argument; }
        }
        while (**pt != '\0') {

            if (**pt != ' ') { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); break; }
            (*pt)++;
        }
    }
    else  { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT_OF_LABEL3); }
}

void SkipSpaces(char*** pt) {

    while (***pt == ' ') {
        (**pt)++;
    }
}

void Check_db (char** pt, int line, char* string, int* count_errors) {

    char is_end_symb_finded = 0;

    SkipSpaces(&pt);

    if (**pt != '\"') { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_TEXT1); }

    while (**pt != '\0') {

        if (**pt == End_text_symb) {
            is_end_symb_finded = 1;
            break;
        }
        (*pt)++;
    }
    (*pt)++;

    if (is_end_symb_finded == 0) { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_TEXT2); }
    else if (**pt != '"')        { PrintErrorForCommand(__PRETTY_FUNCTION__, __LINE__, line, string, count_errors, 1, INVALID_TEXT3); }
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


 void PrintErrorForCommand(const char* func, int _line_, int line, char* string, int* count_errors, int numbers_of_errors, ...) {

    va_list ptr = {};
    va_start(ptr, numbers_of_errors);

    for (int current_number = 0; current_number < numbers_of_errors; current_number++) {

        fprintf(stderr, " "White" %s:%d: "Grey" In function "White" '%s': "Grey" \n", __FILE__, _line_, func);
        switch(va_arg(ptr, int)) {
            case MISSING_ARGUMENT:             
                fprintf(stderr, " "White" %s:%d: "Red" error: "Grey" Missing argument for command. This command needs argument(error was in line: "Under" %d "Grey" in the \"test_asm\" file).\n\t| "Blue" \t%s "Grey" \n\t|\n", __FILE__, _line_, line, string);
                *count_errors += INVALID_ARGUMENT;
                break;
            case INVALID_SYMBOLS:             
                fprintf(stderr, " "White" %s:%d: "Red" error: "Grey" Invalid input. Invalid symbols(error was in line: "Under" %d "Grey" in the \"test_asm\" file).\n\t| "Blue" \t%s "Grey" \n\t|\n", __FILE__, _line_, line, string);
                *count_errors += INVALID_SYMBOLS;
                break;
            case INVALID_ARGUMENT:             
                fprintf(stderr, " "White" %s:%d:"Red" error: "Grey" Invalid data. Argument of the command must be a number(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n\t|"Blue"\t%s"Grey"\n\t|\n", __FILE__, _line_, line, string);
                *count_errors += INVALID_ARGUMENT;
                break;
            case INVALID_DATA:             
                fprintf(stderr, " "White" %s:%d:"Red" error: "Grey"Invalid data. Argument \"jmp\" must be like: \": 'number'\"(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n\t|"Blue"\t%s"Grey"\n\t|\n", __FILE__, _line_, line, string);
                *count_errors += INVALID_DATA;
                break;
            case INVALID_COMMAND:             
                fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"Invalid input. Unknown command(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n\t|"Blue"\t%s"Grey"\n\t|\n", __FILE__, _line_, line, string);
                *count_errors += INVALID_COMMAND;
                break;
            case INVALID_ARGUMENT_OF_LABEL0:
                fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"Invalid input, after 'L' must be ' '(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n\t|"Blue"\t%s"Grey"\n\t|\n", __FILE__, _line_, line, string);
                *count_errors += INVALID_ARGUMENT_OF_LABEL0;
                break;
            case INVALID_ARGUMENT_OF_LABEL1:
                fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"Invalid value of label. Valid values from 1 to 512(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n\t|"Blue"\t%s"Grey"\n\t|\n", __FILE__, _line_, line, string);
                *count_errors += INVALID_ARGUMENT_OF_LABEL1;
                break;
            case INVALID_ARGUMENT_OF_LABEL2:
                fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"This label is already used. You can't use equal labels twice in one programm(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n\t|"Blue"\t%s"Grey"\n\t|\n", __FILE__, _line_, line, string);
                *count_errors += INVALID_ARGUMENT_OF_LABEL2;
                break;
            case INVALID_ARGUMENT_OF_LABEL3:
                fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"Invalid input: It must be label as number(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n\t|"Blue"\t%s"Grey"\n\t|\n", __FILE__, _line_, line, string);
                *count_errors += INVALID_ARGUMENT_OF_LABEL3;
                break;
            case INVALID_TEXT1: 
                fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"must be '\"' after 'db' as the start of the text(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n\t|"Blue"\t%s"Grey"\n\t|\n", __FILE__, _line_, line, string);
                *count_errors += INVALID_TEXT1;
                break;
            case INVALID_TEXT2: 
                fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"must be '%c' in the end of the text before '\"'(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n\t|"Blue"\t%s"Grey"\n\t|\n",__FILE__, _line_, End_text_symb, line, string);
                *count_errors += INVALID_TEXT2;
                break;
            case INVALID_TEXT3:
                fprintf(stderr, ""White"%s:%d:"Red" error: "Grey"must be '\"' in the end of the text after '%c'(error was in line:"Under"%d"Grey" in the \"test_asm\" file).\n\t|"Blue"\t%s"Grey"\n\t|\n",__FILE__, _line_,  End_text_symb, line, string);
                *count_errors += INVALID_TEXT3;
                break;
            default: fprintf(stderr, "Chtoto ne tak\n");
        }
    }
    va_end(ptr);
}

