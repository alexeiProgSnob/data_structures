#include "list.h"
#include "listInternal.h"
#include <stdlib.h>/* for malloc */

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

static List_Result PushToList(List *_list, void *_item, Node *_leftNode);
static List *InisalizeData(List *_list);

List *ListCreate() {
    List *lPtr;

    lPtr = (List *)malloc(sizeof(List));
    if (lPtr == NULL) {
        return NULL;
    }

    return InisalizeData(lPtr);
}

static List *InisalizeData(List *_list) {
    _list->m_tail.m_prev = &_list->m_head;
    _list->m_tail.m_next = &_list->m_tail;
    _list->m_head.m_next = &_list->m_tail;
    _list->m_head.m_prev = &_list->m_head;
    return _list;
}

void ListDestroy(List **_plist, void (*_elementDestroy)(void *_item)) {
    void *item;
    if (_plist == NULL || *_plist == NULL) {
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

List_Result ListPushHead(List *_list, void *_item) {
    return PushToList(_list, _item, &_list->m_head);
}

List_Result ListPopHead(List *_list, void **_pItem) {
    Node *tempRemove;
    if (_list == NULL || _pItem == NULL) {
        return LIST_UNINITIALIZED_ERROR;
    }

    if (_list->m_head.m_next == _list->m_tail.m_next) {
        return LIST_UNDERFLOW;
    }

    tempRemove = (_list->m_head).m_next;
    *_pItem = tempRemove->m_item;

    PopNode(&(_list->m_head), tempRemove->m_next);

    free(tempRemove);
    return LIST_SUCCESS;
}

List_Result ListPushTail(List *_list, void *_item) {
    return PushToList(_list, _item, _list->m_tail.m_prev);
}

List_Result ListPopTail(List *_list, void **_pItem) {
    Node *tempRemove;
    if (_list == NULL || _pItem == NULL) {
        return LIST_UNINITIALIZED_ERROR;
    }

    if (_list->m_head.m_next == _list->m_tail.m_next) {
        return LIST_UNDERFLOW;
    }

    tempRemove = (_list->m_tail).m_prev;
    *_pItem = tempRemove->m_item;

    PopNode(tempRemove->m_prev, &(_list->m_tail));

    free(tempRemove);
    return LIST_SUCCESS;
}

size_t ListSize(const List *_list) {
    Node *runOnList = (_list->m_head).m_next;
    size_t count = 0;
    if (_list == NULL) {
        return 0;
    }
    while (runOnList != &_list->m_tail) {
        ++count;
        runOnList = runOnList->m_next;
    }
    return count;
}

static List_Result PushToList(List *_list, void *_item, Node *_leftNode) {
    Node *newNode;

    if (_list == NULL) {
        return LIST_UNINITIALIZED_ERROR;
    }

    if (_item == NULL) {
        return LIST_NULL_ELEMENT_ERROR;
    }

    newNode = NodeCreate(_item);
    if (newNode == NULL) {
        return LIST_ALLOCATION_ERROR;
    }

    newNode->m_item = _item;
    PushNode(_leftNode, newNode);

    return LIST_SUCCESS;
}

Node *NodeCreate(void *_item) {
    Node *newNode;
    newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
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
