#ifndef RVL_SDK_RFL_MODEL_H
#define RVL_SDK_RFL_MODEL_H
#include "RFL_System.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLCoordinateData {
    u32 WORD_0x0;
    u32 WORD_0x4;
    u32 WORD_0x8;
    u32 WORD_0xC;
} RFLCoordinateData;

extern RFLCoordinateData coordinateData;

static inline void RFLiSetCoordinateData(const RFLCoordinateData* data) {
    coordinateData = *data;
}

#ifdef __cplusplus
}
#endif
#endif
