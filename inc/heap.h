#ifndef __HEAP_H__
#define __HEAP_H__

/** 
 *  @file heap.h
 *  @brief
 *	
 *  @details  
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @update Alexei Radashkovsky (alexeirada@gmail.com)
 *  @bug No known bugs.
 */
#include "data_structure_defenitions.h"
#include <stddef.h> /*< size_t >*/
#include <unistd.h> /*< ssize_t >*/

typedef struct Heap Heap;

typedef enum _Heap_Type {
    HEAP_TYPE_ENUM_START,
    HEAP_TYPE_MIN,
    HEAP_TYPE_MAX,
    HEAP_TYPE_ENUM_END
} Heap_Type;

/* For example
Compare_Result Int_Heap(const void* _a, const void* _b) {
    const int* a = (const int*)_a;
    const int* b = (const int*)_b;
    if (a > b) {
        return BIGGER;
    }

    return SMALLER;
}
*/
typedef Compare_Result (*HeapDataCompareFunc)(const void*, const void*);

/** 
 * @brief Create a new heap with given size.
 * @param[in] _heapSize - Expected max capacity.
 * @param[in] _heapType - Heap type if store max or min
 * @param[in] _comapreFunc - compare function for heap data
 * @return newly created heap or null on failure
 */
Heap* HeapCreate(size_t _heapSize, Heap_Type _heapType, HeapDataCompareFunc _comapreFunc);

/**
 * @brief Dynamically deallocate a previously allocated heap
 * @param[in] _heap - Heap to be deallocated.
 * @param[in] _elementDestroy : A function pointer to be used to destroy all elements in the vector
 *             or a null if no such destroy is required
 * @return void
 */
void HeapDestroy(Heap** _heap, void (*_elementDestroy)(void* _item));

/**
 * @brief insert new data into heap
 * @param[in] _heap - Heap.
 * @param[in] _data - pointer to the data
 * @return HEAP_SUCCESS, and other on error
 */
aps_ds_error HeapPush(Heap* _heap, void* _data);

/**
 * @brief Get the top value in heap do not remove the value
 * @param[in] _heap - Heap.
 * @return the address to the value on success, NULL on failure
 */
const void* HeapGetTopValue(const Heap* _heap);

/**
 * @brief Remove element from the top
 * @param[in] _heap - Heap.
 * @param[out]_pValue - pointer where to store the pointer to the value 
 * @return HEAP_SUCCESS, other error on failure
 */
aps_ds_error HeapPop(Heap* _heap, void** _pValue);

/**
 * @brief Get the number of elements that inserted into heap
 * @param[in] _heap - Heap.
 * @return the size, -1 on failure
 */
ssize_t HeapSize(const Heap* _heap);
#endif /* __HEAP_H__ */
