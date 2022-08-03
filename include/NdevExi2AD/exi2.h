#ifndef RVL_SDK_NDEV_EXI2AD_EXI2_H
#define RVL_SDK_NDEV_EXI2AD_EXI2_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum { EXI_OP_READ, EXI_OP_WRITE, EXI_OP_OP2 } EXIOperation;

void __DBEXIInit(void);
BOOL __DBEXIReadReg(u32, void*, int);
BOOL __DBEXIWriteReg(u32, const void*, int);
BOOL __DBEXIReadRam(u32, void*, int);
BOOL __DBEXIWriteRam(u32, const void*, int);

#ifdef __cplusplus
}
#endif
#endif
