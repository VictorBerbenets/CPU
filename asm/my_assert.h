#ifndef My_assert
#define My_assert

#define ASSERT(expression, ...)                                                                                         \
        if (!(expression)) {                                                                                             \
        fprintf(stderr, ""White"%s:%d:"Grey"In function "White" '%s': "Grey" \n", __FILE__, __LINE__, __FUNCTION__);      \
        fprintf(stderr, ""White"%s:%d:"Red"error:"Grey"Assert expression is false\n\t| "Blue" \t%s "Grey" "                \
        "\n\t|\n", __FILE__, __LINE__, #expression);                                                                        \
        __VA_ARGS__;                                                                                                         \
    }                                                                                                                                                                                   

#endif