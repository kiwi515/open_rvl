#ifndef RVL_SDK_TRK___MEM_H
#define RVL_SDK_TRK___MEM_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

__declspec(section ".init") void* memcpy(void*, const void*, size_t);
__declspec(section ".init") void __fill_mem(void*, int, size_t);
__declspec(section ".init") void* memset(void*, int, size_t);

#ifdef __cplusplus
}
#endif
#endif
