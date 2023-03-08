#ifndef RVL_SDK_TRK_PRINTF_H
#define RVL_SDK_TRK_PRINTF_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

int printf(const char*, ...);
int vprintf(const char*, va_list);
int snprintf(char*, size_t, const char*, ...);
int vsnprintf(char*, size_t, const char*, va_list);

#ifdef __cplusplus
}
#endif
#endif
