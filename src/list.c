#include "list.h"
#include "listInternal.h"
#include <stdlib.h>/*< malloc >*/

/**
 *  @file DoubleLindedList.c
 *  @brief Create a Generic Double Linked List data type
 *
 *  @details Implemented as double linked list using head and tail sentinels.
 *  Nodes are allocated from heap an store pointer to user proveded element.
 *
 *  @author by Alexei Radashkovsky on 02/11/16
 *  @bug No known bugs.
 */

static aps_ds_error _PushToList(List* _list, void* _item, Node* _leftNode);
static List* _InisalizeData(List* _list);

List* ListCreate() {
    List* lPtr;

    lPtr = (List* )malloc(sizeof(List));
    if (NULL == lPtr) {
        return NULL;
    }

    return _InisalizeData(lPtr);
}

static List* _InisalizeData(List* _list) {
    _list->m_tail.m_prev = &_list->m_head;
    _list->m_tail.m_next = &_list->m_tail;
    _list->m_head.m_next = &_list->m_tail;
    _list->m_head.m_prev = &_list->m_head;
    return _list;
}

void ListDestroy(List** _plist, void (*_elementDestroy)(void* _item)) {
    void* item;
    if (NULL == _plist || NULL == *_plist) {
        return;
    }

    while (((*_plist)->m_head).m_next != ((*_plist)->m_tail).m_next) {
        ListPopHead((*_plist), &item);
        if (_elementDestroy != NULL) {
            (*_elementDestroy)(item);
        }
    }
    free(*_plist);
    *_plist = NULL;
}

aps_ds_error ListPushHead(List* _list, void* _item) {
    return _PushToList(_list, _item, &_list->m_head);
}

aps_ds_error ListPopHead(List* _list, void* *_pItem) {
    Node *tempRemove;
    if (NULL == _list || NULL == _pItem) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (_list->m_head.m_next == _list->m_tail.m_next) {
        return DS_UNDERFLOW_ERROR;
    }

    tempRemove = (_list->m_head).m_next;
    *_pItem = tempRemove->m_item;

    PopNode(&(_list->m_head), tempRemove->m_next);

    free(tempRemove);
    return DS_SUCCESS;
}

aps_ds_error ListPushTail(List* _list, void* _item) {
    return _PushToList(_list, _item, _list->m_tail.m_prev);
}

aps_ds_error ListPopTail(List* _list, void** _pItem) {
    Node *tempRemove;
    if (NULL == _list || NULL == _pItem) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (_list->m_head.m_next == _list->m_tail.m_next) {
        return DS_UNDERFLOW_ERROR;
    }

    tempRemove = (_list->m_tail).m_prev;
    *_pItem = tempRemove->m_item;

    PopNode(tempRemove->m_prev, &(_list->m_tail));

    free(tempRemove);
    return DS_SUCCESS;
}

size_t ListSize(const List* _list) {
    Node *runOnList = (_list->m_head).m_next;
    size_t count = 0;
    if (NULL == _list) {
        return 0;
    }

    while (runOnList != &_list->m_tail) {
        ++count;
        runOnList = runOnList->m_next;
    }
    return count;
}

static aps_ds_error _PushToList(List* _list, void* _item, Node *_leftNode) {
    Node *newNode;

    if (NULL == _list) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (NULL == _item) {
        return DS_UNINITIALIZED_ITEM_ERROR;
    }

    newNode = NodeCreate(_item);
    if (NULL == newNode) {
        return DS_ALLOCATION_ERROR;
    }

    newNode->m_item = _item;
    PushNode(_leftNode, newNode);

    return DS_SUCCESS;
}

Node *NodeCreate(void* _item) {
    Node *newNode;
    newNode = (Node*)malloc(sizeof(Node));
    if (NULL == newNode) {
        return NULL;
    }
    newNode->m_item = _item;
    return newNode;
}

void PushNode(Node *_leftNode, Node *_newNode) {
    _newNode->m_next = _leftNode->m_next;
    _newNode->m_prev = _leftNode;
    (_leftNode->m_next)->m_prev = _newNode;
    _leftNode->m_next = _newNode;
}

void PopNode(Node *_leftNode, Node *_rightNode) {
    _leftNode->m_next = _rightNode;
    _rightNode->m_prev = _leftNode;
}
