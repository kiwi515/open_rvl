#include "RFL_Icon.h"
#include "RFL_System.h"

void RFLSetIconDrawDoneCallback(RFLIconDrawDoneCallback callback) {
    RFLiGetManager()->iconDrawDone = callback;
}
