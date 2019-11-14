/**
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 08/20/17
 *  @bug no bugs known.
 */

#include "circular_safe_queue.h"
#include <pthread.h>   /*< mutex >*/
#include <semaphore.h> /*< semaphore >*/
#include <stdlib.h>    /*< malloc >*/

struct CSQueue {
    void** m_elements; /* <Array of elements that saved in CSQueue> */

    size_t m_size;   /* <size of CSQueue> */
    size_t m_head;   /* <Index of head in m_elements> */
    size_t m_tail;   /* <Index of tail in m_elements> */
    size_t m_nItems; /* <Number of items in CSQueue> */

    sem_t m_full;            /* <samaphore condition for full queue> */
    sem_t m_empty;           /* <samaphore condition for empty queue> */
    pthread_mutex_t m_mutex; /* <mutex for critical section> */
};

typedef struct Destroy {
    ElementDestroy m_func; /* <Destroy function use ForEach function for destroy
                              all element> */
} Destroy;

static CSQueue* _InitCSQueue(CSQueue* _squeue, size_t _initSize);
static CSQueue* _InitSemMutex(CSQueue* _squeue, size_t _initSize);
static int _MyActionDestroy(void* _element, void* _context);
static void* _GetElement(CSQueue* _squeue);
static void _PutElement(CSQueue* _squeue, void* _element);

/**
 * @brief Create a new Squeue with given initialize size.
 * @param[in] _initSize - initial capacity, number of elements that can be
 * stored initially.
 * @return CSQueue* - on success / NULL on fail
 */
CSQueue* CSQueueCreate(size_t _initSize) {
    CSQueue* pQue;
    if (_initSize == 0) {
        return NULL;
    }

    pQue = (CSQueue*)malloc(sizeof(CSQueue));
    if (pQue == NULL) {
        return NULL;
    }

    pQue->m_elements = (void**)malloc(_initSize * sizeof(void*));
    if (pQue->m_elements == NULL) {
        free(pQue);
        return NULL;
    }

    return _InitCSQueue(pQue, _initSize);
}

/**
 * @brief : free CSQueue.
 * @details : free CSQueue if _elementDestroy is NULL.
 *
 * @param[in] _squeue - safe queue.
 * @param[in] _elementDestroy - a pointer to a function to destroy elements.
 */
void CSQueueDestroy(CSQueue** _squeue, ElementDestroy _desFunc) {
    Destroy des;
    if (_squeue != NULL && *_squeue != NULL) {
        if (_desFunc != NULL) {
            des.m_func = _desFunc;
            CSQueueForEach(*_squeue, _MyActionDestroy, &des);
        }
        pthread_mutex_destroy(&((*_squeue)->m_mutex));
        sem_destroy(&((*_squeue)->m_full));
        sem_destroy(&((*_squeue)->m_empty));

        free((*_squeue)->m_elements);
        free(*_squeue);
        *_squeue = NULL;
    }
    return;
}

/**
 * @brief : Inserts a new element to tail of safe queue .
 *
 * @param[in] _squeue - safe queue.
 * @param[in] _element - a new element to insert.
 *
 * @return[success] : DS_SUCCESS
 * @return[failure] : SQUEUE_UNINITIALIZED_ERROR
 * @return[failure] : SQUEUE_ELEMENT_UNINITIALIZED_ERROR
 */
aps_ds_error CSQueueInsert(CSQueue* _squeue, void* _element) {
    if (_squeue == NULL) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (_element == NULL) {
        return DS_UNINITIALIZED_ITEM_ERROR;
    }

    _PutElement(_squeue, _element);
    return DS_SUCCESS;
}

/**
 * @brief : Removes an element from the head of the safe queue.
 *
 * @param[in] _squeue - safe queue.
 *
 * @return[success] : a pointer to the element.
 * @return[failure] : NULL if _squeue is NULL
 */
aps_ds_error CSQueueRemove(CSQueue* _squeue, void** _returnElement) {
    if (_squeue == NULL || _returnElement == NULL) {
        return DS_UNINITIALIZED_ERROR;
    }

    *_returnElement = _GetElement(_squeue);
    return DS_SUCCESS;
}

/**
 * @brief : check if CSQueue is empty
 *
 * @param[in] _squeue - safe queue.
 *
 * @return[success] : 1 if empty
 * @return[success] : 0 if not empty
 */
int CSQueueIsEmpty(const CSQueue* _squeue) {
    if (_squeue == NULL) {
        return 1;
    }

    if (_squeue->m_nItems == 0) {
        return 1;
    }

    return 0;
}

/**
 * @brief : run on all elements and preform CSQueueElementAction function.
 *
 * @param[in]  _squeue - safe queue.
 * @param[in] _action - function to preform on safe queue elements
 * @param[in] _context - context for action function.
 */
aps_ds_error CSQueueForEach(const CSQueue* _squeue, CSQueueElementAction _action,
                            void* _context) {
    size_t i;
    size_t end;
    if (_squeue == NULL || _action == NULL) {
        return DS_UNINITIALIZED_ERROR;
    }

    pthread_mutex_lock(&(((CSQueue*)_squeue)->m_mutex));
    i = (_squeue->m_head);
    end = (_squeue->m_tail);
    pthread_mutex_unlock(&(((CSQueue*)_squeue)->m_mutex));

    while (i != end + 1) {
        i = (i + 1) % _squeue->m_size;
        pthread_mutex_lock(&(((CSQueue*)_squeue)->m_mutex));
        _action(_squeue->m_elements[i], _context);
        pthread_mutex_unlock(&(((CSQueue*)_squeue)->m_mutex));
    }

    return DS_SUCCESS;
}

static int _MyActionDestroy(void* _element, void* _context) {
    Destroy* des = _context;
    (des->m_func)(_element);
    return 1;
}

static CSQueue* _InitSemMutex(CSQueue* _squeue, size_t _initSize) {
    int ret;

    ret = pthread_mutex_init(&(_squeue->m_mutex), NULL);
    if (ret != 0) {
        free(_squeue->m_elements);
        free(_squeue);
        return NULL;
    }

    ret = sem_init(&(_squeue->m_full), 0, 0);
    if (ret != 0) {
        pthread_mutex_destroy(&((_squeue)->m_mutex));
        free(_squeue->m_elements);
        free(_squeue);
        return NULL;
    }

    ret = sem_init(&(_squeue->m_empty), 0, _initSize);
    if (ret != 0) {
        pthread_mutex_destroy(&((_squeue)->m_mutex));
        sem_destroy(&((_squeue)->m_full));
        free(_squeue->m_elements);
        free(_squeue);
        return NULL;
    }

    return _squeue;
}

static CSQueue* _InitCSQueue(CSQueue* _squeue, size_t _initSize) {

    _squeue = _InitSemMutex(_squeue, _initSize);
    if (_squeue == NULL) {
        return NULL;
    }

    _squeue->m_size = _initSize;
    _squeue->m_head = 0;
    _squeue->m_nItems = 0;
    _squeue->m_tail = 0;

    return _squeue;
}

static void* _GetElement(CSQueue* _squeue) {
    void* element;

    sem_wait(&(_squeue->m_full));

    pthread_mutex_lock(&(_squeue->m_mutex));
    _squeue->m_head = (_squeue->m_head + 1) % _squeue->m_size;
    element = *(_squeue->m_elements + _squeue->m_head);
    --(_squeue->m_nItems);
    pthread_mutex_unlock(&(_squeue->m_mutex));

    sem_post(&(_squeue->m_empty));

    return element;
}

static void _PutElement(CSQueue* _squeue, void* _element) {
    sem_wait(&(_squeue->m_empty));

    pthread_mutex_lock(&(_squeue->m_mutex));
    _squeue->m_tail = (_squeue->m_tail + 1) % _squeue->m_size;
    _squeue->m_elements[_squeue->m_tail] = _element;
    ++_squeue->m_nItems;
    pthread_mutex_unlock(&(_squeue->m_mutex));

    sem_post(&(_squeue->m_full));
}
