#ifndef SORTS_H_
#define SORTS_H_


/**
 *  @file sorts.h
 *  @brief
 *  @details
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 08/20/19
 *  @bug no bugs known.
 */
#include "data_structure_defenitions.h"
#include <stddef.h> /*< size_t >*/

typedef enum Sort_Result {
    SORT_SUCCESS,
    SORT_UNINITIALIZED_ERROR,
    SORT_TOO_SMALL_ARRAY_SIZE_ERROR,
    SORT_ARRAY_SIZE_UNELINED_ERROR,
    SORT_UNINITIALIZED_ARRAY_DATA_ERROR

} Sort_Result;

typedef void (*SwapFunc) (void*, void*);
/* For example
Compare_Result CompareSizeT(void* _generalTypeA, void* _generalTypeB) {
    size_t* typeA = (size_t*) _generalTypeA;
    size_t* typeB = (size_t*) _generalTypeB;
    if (*typeA > *typeB) {
        return BIGGER;
    }

    return SMALLER;
}

void SwapToSizeT(void* _generalTypeA, void* _generalTypeB) {
    size_t* typeA = (size_t*) _generalTypeA;
    size_t* typeB = (size_t*) _generalTypeB;
    size_t temp = *typeA;
    *typeA = *typeB;
    *typeB = temp; 
}
*/


Sort_Result MergeSortOnData(
    void*       _arrayOfDataToSort, 
    size_t      _dataSizeInByte, 
    size_t      _arrayDataToSortInByte,
    CompareFunc _compareOperation, 
    SwapFunc    _swapOperation);

Sort_Result BubbleSortOnData(
    void*       _arrayOfDataToSort, 
    size_t      _dataSizeInByte, 
    size_t      _arrayDataToSortInByte,
    CompareFunc _compareOperation, 
    SwapFunc    _swapOperation);
#endif /*< SORTS_H_ >*/