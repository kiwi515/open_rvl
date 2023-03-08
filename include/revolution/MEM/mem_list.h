#ifndef RVL_SDK_MEM_LIST_H
#define RVL_SDK_MEM_LIST_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMList {
    void* head; // at 0x0
    void* tail; // at 0x4
    u16 length; // at 0x8
    u16 offset; // at 0xA
} MEMList;

typedef struct MEMNode {
    void* prev; // at 0x0
    void* next; // at 0x4
} MEMNode;

#define MEMGetObjectNode(list, object)                                         \
    ((MEMNode*)((char*)object + list->offset))

void MEMInitList(MEMList*, u16);
void MEMAppendListObject(MEMList*, void*);
void MEMRemoveListObject(MEMList*, void*);
void* MEMGetNextListObject(MEMList*, void*);

#ifdef __cplusplus
}
#endif
#endif
