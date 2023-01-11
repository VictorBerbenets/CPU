#include "asm.h"

const int labels_commands_size = 200;
const char labels_commands[labels_commands_size] = "call jmp jb jbe ja jae je jne";


size_t getFileSize (const char *file) { 
    
    ASSERT(file != nullptr);

    struct stat buf = {};
    if (stat(file, &buf)) {

        fprintf(stderr, "\n\033[0;31mERROR\033[0m!!!\nFile <%s>: in function <%s>,  in line '%d' - error in function \"stat\"\n\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return ERROR_IN_READING_FILE;
    }
    return buf.st_size;
}


void FillBuffer (buffer* asm_commands) {

    FILE* test_asm = fopen("test.asm", "r");

    ASSERT(test_asm != nullptr);

    asm_commands->buf_size = getFileSize("test.asm");
    asm_commands->buf = (char*) calloc(asm_commands->buf_size, sizeof(char));

    ASSERT(asm_commands->buf != nullptr);
    
    size_t size_from_fread;
    if ((size_from_fread = fread(asm_commands->buf, sizeof(char), asm_commands->buf_size, test_asm)) != (asm_commands->buf_size)) {

        ASSERT(!feof(test_asm));
    }

    GetNumbersOfStrings (asm_commands);

    char check_on_clossing_file = fclose(test_asm);
    ASSERT (check_on_clossing_file == 0);
}

token*  FillStruct (buffer* asm_commands) {

    ASSERT(asm_commands->buf != nullptr);

    size_t number_of_commands = CountComands (asm_commands);

    token* tokens = (token*) calloc(number_of_commands, sizeof(token));
    ASSERT(tokens != nullptr);

    tokens[0].string = strtok(asm_commands->buf, separates_symbols);

    int tokens_number = 0;
    while(tokens[tokens_number].string) {
        tokens_number++;
        tokens[tokens_number].string = strtok (NULL, separates_symbols);   
    }
    asm_commands->numbers_of_strings = tokens_number;
    return tokens;
}

size_t CountComands (buffer* asm_commands) {

    ASSERT(asm_commands != nullptr);

    Replacement(asm_commands);   // '\0' -> '\n'

    char* pt_buf = asm_commands->buf;
    size_t number_of_commands = 0;
    char in_word = 0;

    for (size_t counter = 0; counter < asm_commands->buf_size; counter++, pt_buf++) {

        if (isspace(*pt_buf)) {

            in_word = 0;
            while (!isspace(*pt_buf)) {
                pt_buf++;
            }         
        }
        if (!isspace(*pt_buf) && in_word == 0) {

            in_word = 1; // inside
            number_of_commands ++;
        }
    }
    return number_of_commands;
}

void GetNumbersOfStrings (buffer* asm_commands) {

    ASSERT(asm_commands->buf != nullptr);

    char* buf_pointer = asm_commands->buf;
    asm_commands->numbers_of_strings = 0;

    for (size_t current_symbol = 0; current_symbol < asm_commands->buf_size; current_symbol++, buf_pointer++) {

        if (*buf_pointer == '\n') {

            asm_commands->numbers_of_strings++;
        }
    }
    asm_commands->numbers_of_strings++; // последняя строка не посчиталась
}

void CheckingForCorrectData (buffer* asm_commands, int* count_errors) {

    char** pts = nullptr;
    Constructor(asm_commands, &pts);

    for (size_t string_number = 0; string_number < asm_commands->numbers_of_strings; string_number++) { //string_number < asm_commands->numbers_of_strings
        ValidationOfInputData(pts[string_number], string_number, count_errors);
    }
    free(pts);

}

void ValidationOfInputData(char* string, int line, int* count_errors) {
    ASSERT(string != nullptr);
    if (string == nullptr) {
        return ;
    }

    char* pt      = string;
    char data[15] = {};
    int res       = 0;
    line++;

    if ((res = Check_First_Command(&pt, data, line, string, count_errors)) == ERROR) {
        return ;
    }
    switch (res) {

        case asm_push:      Check_Push(&pt, line, string, count_errors);                                break;
        case asm_pop:       Check_Pop_Reg(&pt, line, string, count_errors);                             break;     
        case asm_jmp:       Check_Jmp(&pt, line, string, count_errors);                                 break;
        case asm_jbe:       Check_Jmp(&pt, line, string, count_errors);                                 break;
        case asm_jae:       Check_Jmp(&pt, line, string, count_errors);                                 break;
        case asm_jne:       Check_Jmp(&pt, line, string, count_errors);                                 break;
        case asm_je:        Check_Jmp(&pt, line, string, count_errors);                                 break;
        case asm_ja:        Check_Jmp(&pt, line, string, count_errors);                                 break;
        case asm_jb:        Check_Jmp(&pt, line, string, count_errors);                                 break;
        case asm_call:      Check_call(&pt, line, string, count_errors);                                break;
        case asm_label:     Check_label(&pt, line, string, count_errors);                               break;
        case UNCORRECTDATA: PrintErrorForCommand(line, string, 0, count_errors, 0, 1, INVALID_COMMAND); break;
        default:            Check_Command_Without_Argument(&pt, line, string, count_errors);            
    }
}

int IsCorrectData(char* command) {

    ASSERT(command != nullptr);

    #define CMP(cmd) (!strcmp(#cmd, command))

    #define CMD(cmd_name) if(CMP(cmd_name)) {   \
                      return  asm_##cmd_name;   \
                      } else                    \

    #include "cmds.h"

    #undef CMP
    #undef CMD
    
    return UNCORRECTDATA;
}

void Constructor(buffer* asm_commands, char*** pts) {

    ASSERT(asm_commands->buf != nullptr); 
    
    *pts = (char**) calloc(asm_commands->numbers_of_strings, sizeof(char*));
    ASSERT(*pts != nullptr); 

    char* is_eof            = asm_commands->buf;
    char* buf_pointer       = asm_commands->buf;
    size_t number_of_string = 0;
    size_t symbols          = 0;
    for (size_t symbols_in_one_string = 0; symbols <= asm_commands->buf_size && *is_eof != EOF ; is_eof++, symbols ++) { 

        if (*is_eof != '\n' && *is_eof != '\0') {
            if (*is_eof == '#') { SkipComments(&is_eof, &symbols_in_one_string, &symbols);}
            else                { symbols_in_one_string++;             continue; }
        }   
        *is_eof                    = '\0';
        *(*pts + number_of_string) = buf_pointer;
        number_of_string++;
        buf_pointer          += symbols_in_one_string + 1;   
        symbols_in_one_string = 0;
     
    }
}

void SkipComments(char** is_eof, size_t* symbols_in_one_string, size_t* symbols) {

    while (**is_eof != '\n' && **is_eof != EOF) {
        **is_eof = '\0';
        (*is_eof)++;
        (*symbols_in_one_string)++;
        (*symbols)++;
    }
}

void Destructor(buffer* asm_commands, token** tokens, Data** test_bin_commands) {

    token* ptr = *tokens;
    for (size_t number = 0; number < asm_commands->numbers_of_strings; number++) {
        if (ptr[number].is_memory_allocated == MEMORY_ALLOCATED) {
            free(ptr[number].string);
        }
    }
    free(ptr);

    free(asm_commands->buf);
    asm_commands->buf_size           = 0;
    asm_commands->numbers_of_strings = 0;

    free(*test_bin_commands);

}

void Replacement(buffer* asm_commands) {

    ASSERT(asm_commands->buf != nullptr);

    char* buf_pt = asm_commands->buf;

    for (size_t current_number = 0; current_number < asm_commands->buf_size; current_number++, buf_pt++) {

        if (*buf_pt == '\0') {

            *buf_pt = '\n';
        }
    }
}

int GiveRegistor(char* command) {

    ASSERT(command != nullptr);

    if      (!strcmp(command, "ax"))  return ax;
    else if (!strcmp(command, "bx"))  return bx;
    else if (!strcmp(command, "cx"))  return cx;
    else if (!strcmp(command, "dx"))  return dx;
    else if (!strcmp(command, "ex"))  return ex;
    else if (!strcmp(command, "fx"))  return fx;
    else     fprintf(stderr, "\033[0;31merror\033[0m: invalid register  \"%s\"\n", command);
    
    return ERROR;
}


 void ReplacingArgumentsInJmp(token* tokens, buffer* asm_commands, int* Labels) {

    ASSERT(tokens != nullptr);
    ASSERT(asm_commands != nullptr);
    char arr[] = "push pop";
    int labels_value   = 0;
    for (size_t number = 0; number < asm_commands->numbers_of_strings; number++, labels_value++) {
        if (number >= 2 && strstr(arr, tokens[number - 2].string)) {
                labels_value++;
        }
        if (!strcmp("L", tokens[number].string)) {
            number++;
            labels_value --;
            Labels[atoi(tokens[number].string)] = labels_value + 1;
        }
    }

    for (size_t number = 0; number < asm_commands->numbers_of_strings; number++) {
        
        if (IsLabelCommand(tokens[number].string)) {
            number++;

            char* string = (char*) calloc(LABEL_SIZE, sizeof(char));
            sprintf(string, "%d", Labels[atoi(tokens[number].string)]);
            tokens[number].string              = string;
            tokens[number].is_memory_allocated = MEMORY_ALLOCATED;
        }
    }
 }

void MarkRegisterCommand(Data** test_bin_commands, size_t* test_bin_number, size_t* number_of_elements_to_fread, char* command) {

    int reg = GiveRegistor(command);
    *((*test_bin_commands) + *test_bin_number) = Push_reg;

    (*test_bin_number)++;
    (*number_of_elements_to_fread)++;
    *test_bin_commands = (Data*) realloc(*test_bin_commands, (*number_of_elements_to_fread) * sizeof(Data));
    
    *((*test_bin_commands) + *test_bin_number) = reg;
}

void MarkNotRegisterCommand(Data** test_bin_commands, size_t* test_bin_number, size_t* number_of_elements_to_fread) {

    if (*((*test_bin_commands) + *test_bin_number - 1) == asm_push || *((*test_bin_commands) + *test_bin_number - 1) == asm_pop) {
        *((*test_bin_commands) + *test_bin_number) = 0;
        (*test_bin_number)++;
        (*number_of_elements_to_fread)++;
        *test_bin_commands = (Data*) realloc(*test_bin_commands, ++(*number_of_elements_to_fread)*sizeof(Data));
    }

}

int IsLabelCommand(const char* command) {

    ASSERT(command != nullptr);

    if (strstr(labels_commands, command) != nullptr) {

        return 1;
    }
    return 0;
}