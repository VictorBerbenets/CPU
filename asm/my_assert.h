

#define ASSERT(expression, ...)                                                          \
        if (!(expression)) {                                                             \
        fprintf(stderr, "in file <%s>: \033[0;31merror\033[0m in line '%d'\n"            \
                "the exspression \"%s\" is false\n\n", __FILE__, __LINE__, #expression); \
        __VA_ARGS__;                                                                     \
    }                                                                                    


