#ifndef RVL_SDK_EXI_EXI_H
#define RVL_SDK_EXI_EXI_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum { EXI_MEM_READ, EXI_MEM_WRITE, EXI_MEM_OP2 } EXIOperation;

volatile u32 EXI_CD006828 : 0xCD006828;
volatile u32 EXI_CD006834 : 0xCD006834;
volatile u32 EXI_CD006838 : 0xCD006838;

void __DBEXIInit(void);
BOOL __DBEXIReadReg(u32, void*, u32);
BOOL __DBEXIWriteReg(u32, const void*, u32);
BOOL __DBEXIReadRam(u32, void*, u32);
BOOL __DBEXIWriteRam(u32, const void*, u32);

#ifdef __cplusplus
}
#endif
#endif
