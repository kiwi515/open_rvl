#ifndef RVL_SDK_RFL_DATABASE_H
#define RVL_SDK_RFL_DATABASE_H
#include "RFL_System.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void RFLLoadDBCallback(void);

void RFLiInitDatabase(MEMiHeapHead*);

RFLError RFLiBootLoadDatabaseAsync(RFLLoadDBCallback);

#ifdef __cplusplus
}
#endif
#endif
