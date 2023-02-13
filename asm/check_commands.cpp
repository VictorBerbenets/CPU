#include "asm.h"
#include "labels.h"

char End_text_symb      = '^';


void Check_push(char** current_token, int line, char* string, int* count_errors, const char* file_name) {

    Data argument  = 0;
    int value     = 0;
    int symb      = 0;

    SkipSpaces(&current_token);

    if (**current_token == '[') {

        Check_ram_command(&current_token, line, string, count_errors, file_name);
        return ;
    }

    char* push_reg = (char*) calloc(len_registr + 1, sizeof(char));

    if (sscanf(*current_token, "%lg%n", &argument, &symb) && (char)argument == '[') {
        current_token++;
    }

    if ((value = sscanf(*current_token, "%lg%n", &argument, &symb))) {

        *current_token += symb;
        if (value == -1) {
            PrintErrorForCommand(__PRETTY_FUNCTION__,  file_name, __LINE__, line, string, count_errors, 1, MISSING_ARGUMENT); 
        }
        while (**current_token != '\0') {

            if (**current_token != ' ') {
                PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); 
                break;
            }
        (*current_token)++;
        }
        free(push_reg);
    }

    else if ((Strlen(*current_token) == len_registr)) {
        int number = 0;
        
        while (**current_token != '\0') {
            if (**current_token != ' ') {
                push_reg[number] = **current_token;
                number++;
            }
        (*current_token)++;
        }
        if (strstr(register_arguments, push_reg) == nullptr) {      

            PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); 
        }
        free(push_reg);
    }  
    else {
        
        PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT); 
        free(push_reg);
    }
}

void Check_ram_command(char*** current_token, int line, char* string, int* count_errors, const char* file_name) {

    char* command_data_str       = nullptr;
    int command_data_num         = 0;
    int readed_symbs             = 0;
    const int left_symbs         = 3; // one of registers with len = 2 and ']'

    (**current_token)++;
    // printf("ptr  = %s\n", **current_token);
    // printf("str = %d\n", Strlen(**current_token));
    if (sscanf(**current_token, "%d%n", &command_data_num, &readed_symbs)) {

        (**current_token) += readed_symbs;

        if (command_data_num >= Ram_size || command_data_num < 0) {
            PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_RAM_VALUE); 
        }
        if (***current_token != ']') {
            PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_RAM); 
        }
        else if (Strlen(**current_token) != 1) {
            PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_RAM); 
        }
    }
    else if (Strlen(**current_token) == left_symbs) {

        SkipSpaces(current_token);

        if (***current_token >= 'a' && ***current_token <= 'f') {
            (**current_token)++;
            if (***current_token == 'x' && *(**current_token + 1) == ']') {
                return ;
            }
        }
        PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_RAM); 
    }
    else {
        PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_RAM); 
    } 
    // printf("Strlen(**current_token) != left_symbs = %d\n", Strlen(**current_token) != left_symbs);
    // printf("strstr(register_arguments, **current_token) = %s\n", strstr(register_arguments, **current_token));
    // printf("com = %s\n", command_data_str);
    // printf("ptr after   = %s\n", **current_token);
    // printf("strlen after = %d\n", Strlen(**current_token));
}


// check_pop 
void Check_pop(char** current_token, int line, char* string, int* count_errors, const char* file_name) {

    SkipSpaces(&current_token);

    if (**current_token == '[') {
        Check_ram_command(&current_token, line, string, count_errors, file_name);
        return ;
    }

    char* pop_reg = (char*) calloc(len_registr + 1, sizeof(char));

    if ((Strlen(*current_token) == len_registr)) {
        int number = 0;
        while (**current_token != '\0') {
            if (**current_token != ' ') {
                pop_reg[number] = **current_token;
                number++;
            }
        (*current_token)++;
        }

        if (strstr(register_arguments, pop_reg) == nullptr) {
            PrintErrorForCommand(__PRETTY_FUNCTION__,  file_name, __LINE__, line, string, count_errors, 1, INVALID_DATA); 
        }
        free(pop_reg);
    }
    else if (!Strlen(*current_token)) {
        
        free(pop_reg);
        return ;
    }
    else {
        PrintErrorForCommand(__PRETTY_FUNCTION__,  file_name, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT); 
        free(pop_reg);
    }
}

    
void Check_jmp(char** current_token, int line, char* string, int* count_errors, const char* file_name) {

    int value    = 0;
    int argument = 0;
    int symb     = 0;

    while (**current_token == ' ') {
        (*current_token)++;
    }

    if (**current_token != ':') {
        PrintErrorForCommand(__PRETTY_FUNCTION__,  file_name, __LINE__, line, string, count_errors, 1, INVALID_DATA); 
        return ;
    }
    (*current_token)++;

    if ((value = sscanf(*current_token, "%d%n", &argument, &symb))) {
        
        *current_token += symb;

        if (value == -1) {
            PrintErrorForCommand(__PRETTY_FUNCTION__,  file_name, __LINE__, line, string, count_errors, 1, MISSING_ARGUMENT); 
            return ;
        }
        else
            while (**current_token != '\0') {
                
                if (**current_token != ' ') { 
                    PrintErrorForCommand(__PRETTY_FUNCTION__,  file_name, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); 
                }
                (*current_token)++;
            }
    }
    else { PrintErrorForCommand(__PRETTY_FUNCTION__,  file_name, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT); }
}

void Check_Command_Without_Argument(char** current_token, int line, char* string, int* count_errors, const char* file_name) {

    while (**current_token != '\0') {
        if (**current_token != ' ') {
            PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); 
            break;
        }
    (*current_token)++;
    }
}

int Check_First_Command(char** current_token, char* data, int line, char* string, int* count_errors, const char* file_name) {

    int symb      = 0;

    if (!sscanf(*current_token, "%s%n", data, &symb) && symb != 0) { 
        PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_COMMAND);
    }

    if (symb == 0) {
        //empty string error
        return ERROR;
    }

    if (Is_Label(string)) {
        return asm_label;    //IsCorrectData((char*) "label")
    }
    
    *current_token += symb;
    return IsCorrectData(data);
}

void Check_call (char** current_token, int line, char* string, int* count_errors, const char* file_name) {

    int symb      = 0;
    int argument  = 0;
    int value     = 0;

    if ((value = sscanf(*current_token, "%d%n", &argument, &symb))) {
        *current_token += symb;
        if (value == -1) { 
            PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, MISSING_ARGUMENT); 
        }
        if (value != -1) {

            if (!(argument >= Min_Label_Value && argument <= Max_Label_Value)) { 
                PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT); 
            }
        }
        while (**current_token != '\0') {

            if (**current_token != ' ') { 
                PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); 
            }
            (*current_token)++;
        }
    }
else { 
    PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT); 
    }
}


void Check_label(char** current_token, int line, char* string, int* count_errors, const char* file_name) {

    int value    = 0;
    int argument = 0;
    int symb     = 0;

    while (**current_token != 'L' && **current_token != '\0') { (*current_token)++; }
    (*current_token)++;
    
    if (**current_token != ' ') {
        PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT_OF_LABEL0);
    }

    if ((value = sscanf(*current_token, "%d%n", &argument, &symb))) {
        *current_token += symb;

        if (value == -1) { 
            PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, MISSING_ARGUMENT); 
        }

        if (value != -1) {
            if (!(argument >= Min_Label_Value && argument <= Max_Label_Value)) { 
                PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT_OF_LABEL1); 
            }
            else if (current_labels[argument] == argument) { 
                PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT_OF_LABEL2);
            }
            else { current_labels[argument] = argument; }
        }
        while (**current_token != '\0') {

            if (**current_token != ' ') { 
                PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_SYMBOLS); break;
            }
            (*current_token)++;
        }
    }
    else  { 
        PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_ARGUMENT_OF_LABEL3);
    }
}

void SkipSpaces(char*** current_token) {

    while (***current_token == ' ') {
        (**current_token)++;
    }
}

void Check_db (char** current_token, int line, char* string, int* count_errors, const char* file_name) {

    char is_end_symb_finded = 0;

    SkipSpaces(&current_token);

    if (**current_token != '\"') { 
        PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_TEXT1); 
    }

    while (**current_token != '\0') {

        if (**current_token == End_text_symb) {
            is_end_symb_finded = 1;
            break;
        }
        (*current_token)++;
    }
    (*current_token)++;

    if (is_end_symb_finded == 0) { 
        PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_TEXT2); 
    }
    else if (**current_token != '"')        { 
        PrintErrorForCommand(__PRETTY_FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_TEXT3); 
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


 void PrintErrorForCommand(const char* func, const char* file_name, int _line_, int line, char* string, int* count_errors, int numbers_of_errors, ...) {

    va_list ptr = {};
    va_start(ptr, numbers_of_errors);

    for (int current_number = 0; current_number < numbers_of_errors; current_number++) {

        fprintf(stderr, "" White "%s:%d:" Grey "In function" White " '%s': " Grey " \n", __FILE__, _line_, func);
        switch(va_arg(ptr, int)) {
            case MISSING_ARGUMENT:             
                fprintf(stderr, "" White " %s:%d: " Red " error:" Grey " Missing argument for command. This command needs argument"
                "(error was in line: " Under " %d " Grey " in the \"%s\" file).\n\t| " Blue " \t%s " Grey " \n\t|\n", __FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_ARGUMENT;
                break;
            case INVALID_SYMBOLS:             
                fprintf(stderr, "" White " %s:%d: " Red " error:" Grey " Invalid input. Invalid symbols(error was in line:" Under " %d "
                "" Grey " in the \"%s\" file).\n\t| " Blue " \t%s " Grey " \n\t|\n", __FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_SYMBOLS;
                break;
            case INVALID_ARGUMENT:             
                fprintf(stderr, "" White  " %s:%d:" Red " error:" Grey " Invalid data. Argument of the command must be a number(error was in line:"
                " " Under "%d" Grey " in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n", __FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_ARGUMENT;
                break;
            case INVALID_DATA:             
                fprintf(stderr, "" White " %s:%d:" Red " error:" Grey "Invalid data. Argument \"jmp\" must be like: \": 'number'\"(error was in "
                " line:" Under "%d" Grey " in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n", __FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_DATA;
                break;
            case INVALID_COMMAND:             
                fprintf(stderr, "" White  "%s:%d:" Red " error:" Grey "Invalid input. Unknown command(error was in line:" Under "%d" Grey " in the "
                "\"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n", __FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_COMMAND;
                break;
            case INVALID_ARGUMENT_OF_LABEL0:
                fprintf(stderr, "" White  "%s:%d:" Red " error:" Grey "Invalid input, after 'L' must be ' '(error was in line:" Under "%d" Grey " "
                "in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n", __FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_ARGUMENT_OF_LABEL0;
                break;
            case INVALID_ARGUMENT_OF_LABEL1:
                fprintf(stderr, "" White  "%s:%d:" Red " error:" Grey "Invalid value of label. Valid values from 1 to 512(error was in line:" Under " "
                "%d" Grey " in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n", __FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_ARGUMENT_OF_LABEL1;
                break;
            case INVALID_ARGUMENT_OF_LABEL2:
                fprintf(stderr, "" White  "%s:%d:" Red " error:" Grey "This label is already used. You can't use equal labels twice in one programm"
                "(error was in line:" Under "%d" Grey " in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n", __FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_ARGUMENT_OF_LABEL2;
                break;
            case INVALID_ARGUMENT_OF_LABEL3:
                fprintf(stderr, "" White "%s:%d:" Red " error:" Grey "Invalid input: It must be label as number(error was in line:" Under "%d" Grey " "
                "in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n", __FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_ARGUMENT_OF_LABEL3;
                break;
            case INVALID_TEXT1: 
                fprintf(stderr, "" White "%s:%d:" Red " error:" Grey "must be '\"' after 'db' as the start of the text(error was in line:" Under "%d"
                "" Grey " in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n", __FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_TEXT1;
                break;
            case INVALID_TEXT2: 
                fprintf(stderr, "" White  "%s:%d:" Red " error:" Grey "must be '%c' in the end of the text before '\"'(error was in line:" Under "%d"
                "" Grey " in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n",__FILE__, _line_, End_text_symb, line, file_name, string);
                *count_errors += INVALID_TEXT2;
                break;
            case INVALID_TEXT3:
                fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "must be '\"' in the end of the text after '%c'(error was in line:" Under "%d"
                "" Grey " in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n",__FILE__, _line_,  End_text_symb, line, file_name, string);
                *count_errors += INVALID_TEXT3;
                break;
            case INVALID_RAM:
                fprintf(stderr, "" White "%s:%d:" Red " error:" Grey "Invalid ram argument. Its must be like '[int]'(error was in line:" Under "%d"
                "" Grey " in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n",__FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_TEXT3;
                break;
            case INVALID_RAM_VALUE:
                fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Invalid ram value. Its must be in range[0, 1023](error was in line:" Under "%d"
                "" Grey " in the \"%s\" file).\n\t|" Blue "\t%s" Grey "\n\t|\n",__FILE__, _line_, line, file_name, string);
                *count_errors += INVALID_TEXT3;
                break;
            default: fprintf(stderr, "Chtoto ne tak\n");
        }
    }
    va_end(ptr);
}

