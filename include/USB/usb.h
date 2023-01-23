#ifndef RVL_SDK_USB_H
#define RVL_SDK_USB_H
#include <IPC.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*USBCallback)(IPCResult result, void* arg);

IPCResult IUSB_OpenLib(void);
IPCResult IUSB_CloseLib(void);
IPCResult IUSB_OpenDeviceIds(const char* module, u16 vid, u16 pid,
                             IPCResult* resultOut);
IPCResult IUSB_CloseDeviceAsync(s32 fd, USBCallback callback,
                                void* callbackArg);

#ifdef __cplusplus
}
#endif
#endif
