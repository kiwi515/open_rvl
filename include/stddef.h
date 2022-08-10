#ifndef STDDEF_H
#define STDDEF_H

#ifndef NULL
#define NULL 0
#endif // NULL

typedef unsigned long size_t;

#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

#define offsetof(ST, M) ((size_t) & (((ST*)0)->M))

#define va_start(ARG, VA_LIST) ((void)ARG, __builtin_va_info(&VA_LIST))
#define va_end(VA_LIST) ((void)VA_LIST)
#define va_arg(VA_LIST, ARG_TYPE)                                              \
    (*(ARG_TYPE*)) __va_arg(VA_LIST, _var_arg_typeof(ARG_TYPE))

#endif
