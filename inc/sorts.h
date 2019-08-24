#ifndef SORTS_H_
#define SORTS_H_


/**
 *  @file sorst.h
 *  @brief
 *  @details
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 08/20/19
 *  @bug no bugs known.
 */

#include <stddef.h> /*< size_t >*/

typedef enum Sort_Result {
    SORT_SUCCESS,
    SORT_UNINITIALIZED_ERROR,
    SORT_UNINITIALIZED_ARRAY_DATA_ERROR

} Sort_Result;


typedef enum Compare_Result {
    BIGGER  = 0xFFF,
    SMALLER = 0xEFD,
    EQUAL = 0xDEF
} Compare_Result;

typedef Compare_Result (*CompareFunc) (void*, void*);
typedef void (*SwapFunc) (void*, void*);
typedef void* (*NextIterationFunc) (void*);

/*
typedef struct MergeSort MergeSort;
MergeSort* CreateMergeSort(CompareFunc _compareOperation);
void MergeSortOnData(MergeSort* _mergeSortData, void** _dataToSort);
void DestroyMergeSort(MergeSort** _mergeSortData);
*/

typedef struct BubbleSort BubbleSort;
BubbleSort* CreateBubbleSort(CompareFunc _compareOperation, SwapFunc _swapOperation);
Sort_Result BubbleSortOnData(
    BubbleSort* _bubbleSortData, 
    void*       _arrayOfDataToSort, 
    size_t      _dataSizeInByte, 
    size_t      _arrayDataToSortInByte);
void DestroyBubbleSort(BubbleSort** _bubbleSortData);
#endif /*< SORTS_H_ >*/