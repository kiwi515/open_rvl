#include "RFL_Icon.h"
#include <revolution/RFL/RFL_System.h>

void RFLSetIconDrawDoneCallback(RFLIconDrawDoneCallback callback) {
    RFLiGetManager()->iconDrawDone = callback;
}
