#ifndef RVL_SDK_RFL_ICON_H
#define RVL_SDK_RFL_ICON_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef UNKTYPE (*RFLIconDrawDoneCallback)(UNKTYPE);

void RFLSetIconDrawDoneCallback(RFLIconDrawDoneCallback);

#ifdef __cplusplus
}
#endif
#endif
