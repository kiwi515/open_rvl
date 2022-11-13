#include "mem_list.h"

void MEMInitList(MEMList* list, u16 offset) {
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->offset = offset;
}

static void SetFirstObject_(MEMList* list, void* object) {
    MEMNode* node = MEMGetObjectNode(list, object);
    node->next = NULL;
    node->prev = NULL;

    list->head = object;
    list->tail = object;
    list->length++;
}

void MEMAppendListObject(MEMList* list, void* object) {
    MEMNode* node;
    MEMNode* tail;

    if (list->head == NULL) {
        SetFirstObject_(list, object);
    } else {
        node = MEMGetObjectNode(list, object);
        node->prev = list->tail;
        node->next = NULL;

        tail = MEMGetObjectNode(list, list->tail);
        tail->next = object;

        list->tail = object;
        list->length++;
    }
}

void MEMRemoveListObject(MEMList* list, void* object) {
    MEMNode* node = MEMGetObjectNode(list, object);

    if (node->prev == NULL) {
        list->head = node->next;
    } else {
        MEMGetObjectNode(list, node->prev)->next = node->next;
    }

    if (node->next == NULL) {
        list->tail = node->prev;
    } else {
        MEMGetObjectNode(list, node->next)->prev = node->prev;
    }

    node->prev = NULL;
    node->next = NULL;

    list->length--;
}

void* MEMGetNextListObject(MEMList* list, void* object) {
    if (object == NULL) {
        return list->head;
    }

    return MEMGetObjectNode(list, object)->next;
}
