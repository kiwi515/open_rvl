#include "OSNet.h"
#include "OS.h"
#include "OSError.h"

#include <NWC24/NWC24Schedule.h>
#include <NWC24/NWC24System.h>
#include <NWC24/NWC24Time.h>

void __OSInitNet(void) {
    int error;

    OSIOSRev rev;
    __OSGetIOSRev(&rev);

    if (rev.verMajor <= 4 || rev.verMajor == 9) {
        return;
    }

    error = NWC24iPrepareShutdown();
    if (error != 0) {
        if (error < 0) {
            OSReport("Failed to register network shutdown function. %d\n",
                     error);
        }

        error = NWC24SuspendScheduler();
        if (error < 0) {
            OSReport("Failed to suspend the WiiConnect24 scheduler. %d\n",
                     error);
        }
    }

    error = NWC24iSynchronizeRtcCounter();
    if (error != 0) {
        OSReport(
            "Failed to synchronize time with network resource managers. %d\n",
            error);
    }
}

// Unused strings
void __OSNet_c_UNUSED() {
    OSReport("NWC24iPrepareShutdown");
    OSReport("/dev/net/kd/request");
    OSReport("NWC24SuspendScheduler");
    OSReport("NWC24ResumeScheduler");
    OSReport("NWC24iRequestShutdown");
    OSReport("NWC24Shutdown_: Give up!\n");
    OSReport("NWC24iSetRtcCounter_");
    OSReport("/dev/net/kd/time");
}
