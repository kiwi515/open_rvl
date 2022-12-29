#ifndef RVL_SDK_OS_PPC_EABI_INIT_H
#define RVL_SDK_OS_PPC_EABI_INIT_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

extern u8 _db_stack_addr[];
extern u8 _db_stack_end[];

extern u8 __ArenaLo[];
extern u8 __ArenaHi[];

extern u8 _f_init[];
extern u8 _e_init[];

extern u8 _stack_addr[];
extern u8 _stack_end[];

#ifdef __cplusplus
}
#endif
#endif
