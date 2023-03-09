#include <RVLFaceLibrary.h>

void RFLSetIconDrawDoneCallback(RFLIconDrawDoneCallback callback) {
    RFLiGetManager()->iconDrawDone = callback;
}
