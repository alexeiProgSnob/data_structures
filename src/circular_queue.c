/**
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 08/20/17
 *  @bug no bugs known.
 */

#include "circular_queue.h"
#include <stdlib.h> /*< malloc >*/

struct CQueue {
    void** m_elements; /* <Array of elements that saved in CQueue> */
    size_t m_size;     /* <size of CQueue> */
    size_t m_head;     /*< Index of head in m_elements >*/
    size_t m_tail;     /* <Index of tail in m_elements> */
    size_t m_nItems;   /* <Number of items in CQueue> */
};

CQueue* CQueueCreate(size_t _initSize) {
    CQueue* pQue;
    void** mPQue;
    if (0 == _initSize) {
        return NULL;
    }

    pQue = (CQueue*)malloc(sizeof(CQueue));
    if (NULL == pQue) {
        return NULL;
    }

    mPQue = (void**)malloc(_initSize * sizeof(void*));
    if (NULL == mPQue) {
        free(pQue);
        return NULL;
    }

    pQue->m_elements = mPQue;
    pQue->m_head = 0;
    pQue->m_size = _initSize;
    pQue->m_nItems = 0;
    pQue->m_tail = 0;
    return pQue;
}

void CQueueDestroy(CQueue** _queue, void (*_elementDestroy)(void* _item)) {
    size_t idx;
    if (_queue != NULL && *_queue != NULL) {
        if (_elementDestroy != NULL) {
            for (idx = 0; idx < (*_queue)->m_nItems; ++idx) {
                _elementDestroy((*_queue)->m_elements[idx]);
            }
        }
        free((*_queue)->m_elements);
        free(*_queue);
        *_queue = NULL;
    }
    return;
}

aps_ds_error CQueueInsert(CQueue* _queue, void* _element) {
    if (NULL == _queue) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (NULL == _element) {
        return DS_UNINITIALIZED_ITEM_ERROR;
    }

    if (_queue->m_nItems == _queue->m_size) {
        return DS_OVERFLOW_ERROR;
    }

    _queue->m_tail = (_queue->m_tail + 1) % _queue->m_size;
    _queue->m_elements[_queue->m_tail] = _element;
    ++_queue->m_nItems;
    return DS_SUCCESS;
}

aps_ds_error CQueueRemove(CQueue* _queue, void** _returnElement) {
    if (NULL == _queue) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (0 == _queue->m_nItems) {
        return DS_EMPTY_ERROR;
    }

    _queue->m_head = (_queue->m_head + 1) % _queue->m_size;
    if (NULL == _returnElement) {
        *_returnElement = *(_queue->m_elements + _queue->m_head);
    }
    --(_queue->m_nItems);
    return DS_SUCCESS;
}

int CQueueIsEmpty(const CQueue* _queue) {
    if (NULL == _queue) {
        return 1;
    }

    if (0 == _queue->m_nItems) {
        return 1;
    }
    return 0;
}

aps_ds_error CQueueForEach(const CQueue* _queue, CQueueElementAction _action,
                           void* _context) {
    size_t i;
    if (NULL == _queue || NULL == _action) {
        return DS_UNINITIALIZED_ERROR;
    }

    i = (_queue->m_head) - 1;
    while (i != _queue->m_tail) {
        i = (i + 1) % _queue->m_size;
        _action(_queue->m_elements[i], _context);
    }

    return DS_SUCCESS;
}
