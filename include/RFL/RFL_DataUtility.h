#ifndef RVL_SDK_RFL_DATA_UTILITY_H
#define RVL_SDK_RFL_DATA_UTILITY_H
#include "RFL_MiddleDatabase.h"
#include "RFL_System.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLCharInfo {
    u16 SHORT_0x0;
} RFLCharInfo;

RFLResult RFLiPickupCharInfo(RFLCharInfo*, RFLDataSource, RFLMiddleDB*, u16);

#ifdef __cplusplus
}
#endif
#endif
