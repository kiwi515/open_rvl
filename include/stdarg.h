#ifndef STDARG_H
#define STDARG_H

typedef struct __va_list_struct {
    char gpr;
    char fpr;
    char* input_arg_area;
    char* reg_save_area;
} va_list[1];

void* __va_arg(va_list, int);

#define va_start(ARG, VA_LIST) ((void)ARG, __builtin_va_info(&VA_LIST))
#define va_end(VA_LIST) ((void)VA_LIST)
#define va_arg(VA_LIST, ARG_TYPE)                                              \
    (*(ARG_TYPE*)) __va_arg(VA_LIST, _var_arg_typeof(ARG_TYPE))

#endif
