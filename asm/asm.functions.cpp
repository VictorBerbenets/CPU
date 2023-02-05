#include "asm.h"

const char Labels_commands[] = "call jmp jb jbe ja jae je jne";
const char Text_command[]    = "db";


size_t getFileSize (const char *file) { 
    
    ASSERT(file != nullptr, exit(ERROR_IN_READING_FILE));

    struct stat buf = {};
    if (stat(file, &buf)) {

        fprintf(stderr, "\n" Red "error" Grey "!!!\nFile <%s>: in function <%s>,  in line '%d' - error in function \"stat\"\n\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return ERROR_IN_READING_FILE;
    }
    return buf.st_size;
}


void FillBuffer (buffer* asm_commands, const char* file_name) {

    FILE* program_file = fopen(file_name, "r");

    ASSERT(program_file != nullptr, exit(ERROR_IN_OPEN_FILE));

    asm_commands->buf_size = getFileSize(file_name);
    asm_commands->buf = (char*) calloc(asm_commands->buf_size + 1, sizeof(char));

    ASSERT(asm_commands->buf != nullptr);
    
    size_t size_from_fread;
    if ((size_from_fread = fread(asm_commands->buf, sizeof(char), asm_commands->buf_size, program_file)) != (asm_commands->buf_size)) {

        ASSERT(!feof(program_file));
    }

    GetNumbersOfStrings (asm_commands);

    char check_on_clossing_file = fclose(program_file);
    ASSERT (check_on_clossing_file == 0);
}

token*  FillStruct (buffer* asm_commands) {

    ASSERT(asm_commands->buf != nullptr);

    size_t number_of_commands = CountComands (asm_commands);
    token* tokens = (token*) calloc(number_of_commands, sizeof(token));
    ASSERT(tokens != nullptr);

    tokens[0].string = strtok(asm_commands->buf, separates_symbols);

    char* string      = nullptr;
    size_t tokens_number = 1;

    FillToks(&tokens_number, number_of_commands, &tokens, string);

    asm_commands->numbers_of_strings = tokens_number;

    return tokens;
}

void FillToks(size_t* tokens_number, size_t number_of_commands, token** tokens, char* string) {

    while(*tokens_number != number_of_commands) {

        if (!strcmp((*tokens)[*tokens_number - 1].string, Text_command)) {
            string = strtok(nullptr, "\n");

            if (string != nullptr) {
            
            SkipFirstSpacesInText(&string);
            (*tokens)[*tokens_number].string = string;
            (*tokens_number)++;
            }
            else { break; }
        }
        else {
        
            string = strtok (nullptr, separates_symbols);
            if (string != nullptr) {

                (*tokens)[*tokens_number].string = string;
                (*tokens_number)++;
            }
            else { break; }
        }
    }
}

void SkipFirstSpacesInText(char** string) {

    while (**string != '\"') {
        (*string)++;
    }

    char* string_ptr = *string;
    while (*string_ptr != '^') {
        string_ptr ++;
    }

    string_ptr += 2;
    *string_ptr = '\0';
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

void CheckingForCorrectData (buffer* asm_commands, int* count_errors, const char* file_name) {

    char** pts = nullptr;
    Constructor(asm_commands, &pts);

    for (size_t string_number = 0; string_number < asm_commands->numbers_of_strings; string_number++) { 
        ValidationOfInputData(pts[string_number], string_number, count_errors, file_name);
    }
    free(pts);
}

const int Data_size = 15;

void ValidationOfInputData(char* string, int line, int* count_errors, const char* file_name) {

    ASSERT(string != nullptr);

    if (string == nullptr) {
        return ;
    }

    char* current_token  = string; //current_lexema
    char data[Data_size] = {};
    int first_command    = 0;
    line++;

    if ((first_command = Check_First_Command(&current_token, data, line, string, count_errors, file_name)) == ERROR) {
        return ;
    }
    // #define Check_command(first_command)
    switch (first_command) {

        case asm_push:      Check_Push(&current_token, line, string, count_errors, file_name);                              break;
        case asm_pop:       Check_Pop (&current_token, line, string, count_errors, file_name);                              break;     
        case asm_jmp:       Check_Jmp(&current_token, line, string, count_errors, file_name);                               break;
        case asm_jbe:       Check_Jmp(&current_token, line, string, count_errors, file_name);                               break;
        case asm_jae:       Check_Jmp(&current_token, line, string, count_errors, file_name);                               break;
        case asm_jne:       Check_Jmp(&current_token, line, string, count_errors, file_name);                               break;
        case asm_je:        Check_Jmp(&current_token, line, string, count_errors, file_name);                               break;
        case asm_ja:        Check_Jmp(&current_token, line, string, count_errors, file_name);                               break;
        case asm_jb:        Check_Jmp(&current_token, line, string, count_errors, file_name);                               break;
        case asm_call:      Check_call(&current_token, line, string, count_errors, file_name);                              break;
        case asm_out_text:  Check_call(&current_token, line, string, count_errors, file_name);                              break;  
        case asm_label:     Check_label(&current_token, line, string, count_errors, file_name);                             break;
        case asm_db:        Check_db   (&current_token, line, string, count_errors, file_name);                             break;
        case UNCORRECT_DATA:PrintErrorForCommand(__FUNCTION__, file_name, __LINE__, line, string, count_errors, 1, INVALID_COMMAND);   break;
        default:            Check_Command_Without_Argument(&current_token, line, string, count_errors, file_name);            
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
    
    return UNCORRECT_DATA;
}

void Constructor(buffer* asm_commands, char*** pts) { //replase name

    ASSERT(asm_commands->buf != nullptr); 
    
    *pts = (char**) calloc(asm_commands->numbers_of_strings, sizeof(char*));
    ASSERT(*pts != nullptr); 

    char* current_symbol_ptr = asm_commands->buf; //rename current_symbol_ptr
    char* current_string     = asm_commands->buf; //rename current_string
    size_t buf_size          = asm_commands->buf_size;
    size_t number_of_string  = 0;
    size_t symbols           = 0; //readed symb
    
    for (size_t symbols_in_one_string = 0; symbols < buf_size; current_symbol_ptr++, symbols ++) { 

        if (*current_symbol_ptr != '\n' && *current_symbol_ptr != '\0') {

            if (*current_symbol_ptr == '#') { SkipComments(&current_symbol_ptr, &symbols_in_one_string, &symbols, buf_size);}
            else                { symbols_in_one_string++;             continue; }
        }   

        if (*current_symbol_ptr != '\n' && *current_symbol_ptr != '\0') {

            symbols_in_one_string++; continue;
        }   

        *current_symbol_ptr                    = '\0';
        *(*pts + number_of_string) = current_string;
        number_of_string++;
        current_string          += symbols_in_one_string + 1;   
        symbols_in_one_string = 0;
     
    }

    *(current_symbol_ptr) = '\0';
    asm_commands->numbers_of_strings = number_of_string;

}

void SkipComments(char** current_symbol_ptr, size_t* symbols_in_one_string, size_t* symbols, size_t buf_size) {

    while (**current_symbol_ptr != '\n' && *symbols < buf_size) {
        **current_symbol_ptr = '\0';
        (*current_symbol_ptr)++;
        (*symbols_in_one_string)++;
        (*symbols)++;
    }
}

void Destructor(buffer* asm_commands, token** tokens, char** test_bin_commands) {

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
    else     fprintf(stderr, "" Red "error" Grey ": invalid register  \"%s\"\n", command);
    
    return ERROR;
}


 void ReplacingArgumentsInJmp(token* tokens, buffer* asm_commands, int* Labels) {

    ASSERT(tokens != nullptr);
    ASSERT(asm_commands != nullptr);

    char push_or_pop[] = "push pop";
    int labels_value   = 0;

    for (size_t number = 0; number < asm_commands->numbers_of_strings; number++, labels_value++) {
        if (number >= 2 && strstr(push_or_pop, tokens[number - 2].string)) {
            if (!strstr(register_arguments, tokens[number - 1].string) && !strcmp("push",tokens[number - 2].string)) {
                labels_value += sizeof(Data);
            }
            else {
                labels_value ++;
            }
        }
        if (strstr(Labels_commands, tokens[number].string)) {
            labels_value += sizeof(int) - 1;
        }
        if (number >= 1 && !strcmp("db", tokens[number - 1].string)) {

            labels_value += strlen(tokens[number].string) - 3;
        }
        if (!strcmp("L", tokens[number].string)) {
            number++;
            labels_value--;
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

void MarkRegisterCommand( char** test_bin_commands, size_t* test_bin_number, size_t* size, char* command) {

    int reg = GiveRegistor(command);
    *(*test_bin_commands + *test_bin_number) = (char) Push_reg;
    
    (*test_bin_number)++;
    (*size) ++;

    *test_bin_commands = (char*) realloc(*test_bin_commands, *size * sizeof(char));

    *(*test_bin_commands + *test_bin_number) = (char) reg;
}

void MarkNotRegisterCommand(char** test_bin_commands, size_t* test_bin_number, size_t* size, char* command) {

    char* command_ptr = *test_bin_commands + *test_bin_number - 1;
    if (*test_bin_number >= 1 && (*command_ptr == asm_push || *command_ptr == asm_pop)) {

        if (strchr(command, '[')) { // ram command

            command++;
            *(*test_bin_commands + *test_bin_number) = Push_or_Pop_ram;

            *size              += sizeof(int) + 1;
            *test_bin_commands = (char*) realloc(*test_bin_commands, *size * sizeof(char));

            (*test_bin_number) ++;
            *(int*)(*test_bin_commands + *test_bin_number) = atoi(command);
            (*test_bin_number) += sizeof(int) - 1;
        }
        else if (*command_ptr == asm_push) {
            *(*test_bin_commands + *test_bin_number) = Push_number;

            *size              += sizeof(Data) + 1;
            *test_bin_commands = (char*) realloc(*test_bin_commands, *size * sizeof(char));

            (*test_bin_number) ++;
            *(Data*)(*test_bin_commands + *test_bin_number) = atof(command);
            (*test_bin_number) += sizeof(Data) - 1;
        }

    }
    else {

        *size             += sizeof(int) + 1;
        *test_bin_commands = (char*) realloc(*test_bin_commands, *size * sizeof(char));

        *(int*)(*test_bin_commands + *test_bin_number) = atoi(command);
        (*test_bin_number) += sizeof(int) - 1;
    }
}

void GetTextInArray (char ** test_bin_commands, size_t* test_bin_number, char* text_string, size_t* size) {

    char* string_ptr   = text_string;
    *size             += strlen(text_string);

    *test_bin_commands = (char*) realloc(*test_bin_commands, (*size) * sizeof(char));
    string_ptr ++;

    while (*string_ptr != '^') {
        *(*test_bin_commands + *test_bin_number) = *string_ptr;
        string_ptr ++;
        (*test_bin_number) ++;
    }

    *(*test_bin_commands + *test_bin_number) = *(string_ptr);
}

int IsLabelCommand(const char* command) {

    ASSERT(command != nullptr);

    if (strstr(Labels_commands, command) != nullptr) {

        return 1;
    }
    return 0;
}