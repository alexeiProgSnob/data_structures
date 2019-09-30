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

#include <stddef.h> /*< size_t >*/

typedef struct Heap Heap;

typedef enum _Heap_Result {
	HEAP_SUCCESS = 0,
	HEAP_UNINITIALIZED_ERROR, 		/**< Uninitialized map error 	*/
	HEAP_KEY_NULL_ERROR, 			/**< Key was null 				*/
	HEAP_ALLOCATION_ERROR 			/**< Allocation error 	 		*/
} Heap_Result;

typedef enum _Heap_Type {
    HEAP_TYPE_ENUM_START,
    HEAP_TYPE_MIN,
    HEAP_TYPE_MAX,
    HEAP_TYPE_ENUM_END
} Heap_Type;

typedef enum _Heap_Data_Compare_Result {
    HEAP_COMPARE_ENUM_START,
    HEAP_COMPARE_SMALLER,
    HEAP_COMPARE_BIGGER,
    HEAP_COMPARE_EQUAL,
    HEAP_COMPARE_ENUM_END
} Heap_Data_Compare_Result;

/* For example
Heap_Data_Compare_Result Int_Heap(const void* _a, const void* _b) {
    const int* a = (const int*)_a;
    const int* b = (const int*)_b;
    if (a > b) {
        return HEAP_COMPARE_BIGGER;
    }

    return HEAP_COMPARE_SMALLER;
}
*/
typedef Heap_Data_Compare_Result (*HeapDataCompareFunc)(const void*, const void*);

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
 * @params[in] _elementDestroy : A function pointer to be used to destroy all elements in the vector
 *             or a null if no such destroy is required
 * @return void
 */
void HeapDestroy(Heap** _heap, void (*_elementDestroy)(void* _item));

/**
 * @brief insert new data into heap
 * @param[in] _heap - Heap.
 * @params[in] _data - pointer to the data
 * @return HEAP_SUCCESS, and other on error
 */
Heap_Result HeapInsert(Heap* _heap, void* _data);
#endif /* __HEAP_H__ */
