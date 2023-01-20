#ifndef RVL_SDK_DSP_TASK_H
#define RVL_SDK_DSP_TASK_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;

typedef enum {
    DSP_TASK_ACTIVE = (1 << 0),
} DSPTaskFlag;

typedef enum {
    DSP_TASK_STATE_INIT,
    DSP_TASK_STATE_1,
    DSP_TASK_STATE_2,
    DSP_TASK_STATE_REMOVED,
} DSPTaskState;

// TO-DO
typedef struct DSPTask {
    u32 state; // at 0x0
    u32 WORD_0x4;
    u32 flags;          // at 0x8
    void* iramMmemAddr; // at 0xC
    u32 iramMmemLen;    // at 0x10
    void* iramDspAddr;  // at 0x14
    u32 iramDspLen;     // at 0x18
    u32 dramMmemLen;    // at 0x1C
    char UNK_0x20[0x24 - 0x20];
    u16 startVector;          // at 0x24
    u16 resumeVector;         // at 0x26
    UNKTYPE* initCallback;    // at 0x28
    UNKTYPE* resumeCallback;  // at 0x2C
    UNKTYPE* doneCallback;    // at 0x30
    UNKTYPE* requestCallback; // at 0x34
    struct DSPTask* next;     // at 0x38
    struct DSPTask* prev;     // at 0x3C
} DSPTask;

extern BOOL __DSP_rude_task_pending;
extern DSPTask* __DSP_rude_task;
extern DSPTask* __DSP_tmp_task;
extern DSPTask* __DSP_last_task;
extern DSPTask* __DSP_first_task;
extern DSPTask* __DSP_curr_task;

void __DSPHandler(s16, struct OSContext*);

void __DSP_insert_task(DSPTask*);
void __DSP_boot_task(DSPTask*);

#ifdef __cplusplus
}
#endif
#endif
