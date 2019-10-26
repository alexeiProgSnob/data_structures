#include "stack.h"
#include "list.h"
#include <stdlib.h>/*< malloc >*/


struct Stack {
    List* mList;
};


Stack* StackCreate() {
    Stack* newStack = (Stack*)malloc(sizeof(Stack));
    if (NULL == newStack) {
        return NULL;
    }

    newStack->mList = ListCreate();
    if (NULL == newStack->mList) {
        free(newStack);
        return NULL;
    }

    return newStack;
}


aps_ds_error StackPop(Stack* _stack, void** _pValue) {
    if (NULL == _stack || NULL == _pValue) {
        return DS_UNINITIALIZED_ERROR;
    }

    return ListPopHead(_stack->mList, _pValue);
}


aps_ds_error StackPush(Stack* _stack, void* _data) {
    if (NULL == _stack) {
        return DS_UNINITIALIZED_ERROR;
    }

    return ListPushHead(_stack->mList, _data);
}



ssize_t StackSize(Stack* _stack) {
    if (NULL == _stack) {
        return -1;
    }

    return ListSize(_stack->mList);
}
