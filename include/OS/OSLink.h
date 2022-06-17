#ifndef RVL_SDK_OS_LINK_H
#define RVL_SDK_OS_LINK_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void* OS_MODULE_FIRST : 0x800030C8;
void* OS_MODULE_LAST : 0x800030CC;
void* OS_MODULE_NAME_TABLE : 0x800030D0;

void __OSModuleInit(void);

#ifdef __cplusplus
}
#endif
#endif
