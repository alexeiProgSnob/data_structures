#include "sorts.h"
#include <stdlib.h> /*< malloc  >*/
#include <stdint.h> /*< uint8_t >*/

struct BubbleSort {
    CompareFunc mCompareOperation;
    SwapFunc    mSwapOperation;
};

BubbleSort* CreateBubbleSort(CompareFunc _compareOperation, SwapFunc _swapOperation) {
    BubbleSort* sortData = NULL;
    if (NULL == _compareOperation || NULL == _swapOperation) {
        return NULL;
    }

    sortData = (BubbleSort*)malloc(sizeof(BubbleSort));
    if (NULL == sortData) {
        return NULL;
    }

    sortData->mCompareOperation = _compareOperation;
    sortData->mSwapOperation = _swapOperation;
    return  sortData;
}

Sort_Result BubbleSortOnData(
    BubbleSort* _bubbleSortData, 
    void*       _arrayOfDataToSort, 
    size_t      _dataSizeInByte, 
    size_t      _arrayDataToSortInByte) {
    
    size_t i = 0;
    size_t j = 0;
    int isSwapOccur = 0;
    uint8_t* arrayRunner = (uint8_t*)_arrayOfDataToSort;

    /* TODO: better name */
    size_t maxIters = _arrayDataToSortInByte - _dataSizeInByte;
    if (NULL == _bubbleSortData     || 
        NULL == _arrayOfDataToSort  || 
        0 == _dataSizeInByte        ||
        0 == _arrayDataToSortInByte ||
        _arrayDataToSortInByte < _dataSizeInByte) {
        return SORT_UNINITIALIZED_ERROR;
    }

    for (i = 0; i < maxIters ; i += _dataSizeInByte) {
        for (j = 0; j < (maxIters - i) ; j += _dataSizeInByte) {
            if (_bubbleSortData->mCompareOperation(arrayRunner + j, arrayRunner + j + _dataSizeInByte) == BIGGER) {
                _bubbleSortData->mSwapOperation(arrayRunner + j, arrayRunner + j + _dataSizeInByte); 
                isSwapOccur = -1;
            }
        }
        
        if (0 == isSwapOccur) {
            return SORT_SUCCESS;
        }
    }

    return SORT_SUCCESS;
}

void DestroyBubbleSort(BubbleSort** _bubbleSortData) {
    if (NULL == _bubbleSortData || NULL == *_bubbleSortData) {
        return;
    }

    free(*_bubbleSortData);
    *_bubbleSortData = NULL;
}