#include <RVLFaceLib.h>

void RFLSetIconDrawDoneCallback(RFLIconDrawDoneCallback callback) {
    RFLiGetManager()->iconDrawCb = callback;
}
