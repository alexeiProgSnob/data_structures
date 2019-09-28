#include "heap.h"
#include "vector.h"
#include <stdlib.h> /*< malloc >*/

struct Heap {
    Vector* m_vector; 
    Heap_Type m_heapType;
    HeapDataCompareFunc m_compareFunc;
};

/** 
 * @brief Create a new heap with given size.
 * @param[in] _heapSize - Expected max capacity.
 * @param[in] _heapType - Heap type if store max or min
 * @param[in] _comapreFunc - compare function for heap data
 * @return newly created heap or null on failure
 */
Heap* HeapCreate(size_t _heapSize, Heap_Type _heapType, HeapDataCompareFunc _comapreFunc) {
    Heap* newHeap = NULL;
    Vector* newHeapVector = NULL;
    if (0 == _heapSize || NULL == _comapreFunc) {
        return NULL;
    }

    if (_heapType >= HEAP_TYPE_ENUM_END || _heapType <= HEAP_TYPE_ENUM_START) {
        return NULL;
    }


    newHeap = (Heap*)malloc(sizeof(Heap));
    if (NULL == newHeap) {
        return NULL;
    }

    newHeapVector = VectorCreate(_heapSize, 10);
    if (NULL == newHeapVector) {
        free(newHeap);
        return NULL;
    }

    newHeap->m_vector = newHeapVector;
    newHeap->m_heapType = _heapType;
    newHeap->m_compareFunc = _comapreFunc;
    return newHeap;
}

/**
 * @brief Dynamically deallocate a previously allocated heap
 * @param[in] _heap - Heap to be deallocated.
 * @params[in] _elementDestroy : A function pointer to be used to destroy all elements in the vector
 *             or a null if no such destroy is required
 * @return void
 */
void HeapDestroy(Heap** _heap, void (*_elementDestroy)(void* _item)) {
    if (NULL == _heap || NULL == *_heap) {
        return;
    }

    VectorDestroy(&(*_heap)->m_vector, _elementDestroy);
    free(*_heap);
    *_heap = NULL;
}