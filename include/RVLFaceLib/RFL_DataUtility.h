#ifndef RVL_FACE_LIBRARY_DATA_UTILITY_H
#define RVL_FACE_LIBRARY_DATA_UTILITY_H
#include <RVLFaceLib/RFL_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct RFLCharInfo;
typedef struct RFLMiddleDB;

RFLErrcode RFLiPickupCharInfo(struct RFLCharInfo*, RFLDataSource,
                              struct RFLMiddleDB*, u16);

#ifdef __cplusplus
}
#endif
#endif
