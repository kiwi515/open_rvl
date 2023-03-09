#ifndef RVL_FACE_LIBRARY_DATA_UTILITY_H
#define RVL_FACE_LIBRARY_DATA_UTILITY_H
#include <RVLFaceLibrary/RFL_MiddleDatabase.h>
#include <RVLFaceLibrary/RFL_System.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

RFLErrcode RFLiPickupCharInfo(RFLCharInfo*, RFLDataSource, RFLMiddleDB*, u16);

#ifdef __cplusplus
}
#endif
#endif
