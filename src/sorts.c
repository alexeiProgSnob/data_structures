#include "sorts.h"
#include <stdlib.h> /*< malloc  >*/
#include <stdint.h> /*< uint8_t >*/


Sort_Result BubbleSortOnData(
    void*       _arrayOfDataToSort, 
    size_t      _dataSizeInByte, 
    size_t      _arrayDataToSortInByte,
    CompareFunc _compareOperation, 
    SwapFunc    _swapOperation) {
    
    size_t i = 0;
    size_t j = 0;
    int isSwapOccur = 0;
    uint8_t* arrayRunner = (uint8_t*)_arrayOfDataToSort;

    /* TODO: better name */
    size_t maxIters = _arrayDataToSortInByte - _dataSizeInByte;
    if (NULL == _compareOperation   ||
        NULL == _swapOperation      ||
        NULL == _arrayOfDataToSort  || 
        0 == _dataSizeInByte        ||
        0 == _arrayDataToSortInByte ||
        _arrayDataToSortInByte < _dataSizeInByte) {
        return SORT_UNINITIALIZED_ERROR;
    }


    for (i = 0; i < maxIters ; i += _dataSizeInByte) {
        for (j = 0; j < (maxIters - i) ; j += _dataSizeInByte) {
            if (_compareOperation(arrayRunner + j, arrayRunner + j + _dataSizeInByte) == BIGGER) {
                _swapOperation(arrayRunner + j, arrayRunner + j + _dataSizeInByte); 
                isSwapOccur = -1;
            }
        }
        
        if (0 == isSwapOccur) {
            return SORT_SUCCESS;
        }
    }

    return SORT_SUCCESS;
}

struct MergeSort {
    size_t      mDataSizeInByte;
    CompareFunc mCompareOperation;
    SwapFunc    mSwapOperation;
};



static void _RecutionMerge(
    void*       _arrayOfDataToSort, 
    size_t      _arrayDataToSortInByte,
    struct MergeSort* _sortData) {
    uint8_t* byteData = (uint8_t*)_arrayOfDataToSort;
    size_t numOfElemetns = _arrayDataToSortInByte / _sortData->mDataSizeInByte;
    size_t nextNumOfBytes = (numOfElemetns / 2) * _sortData->mDataSizeInByte;
    uint8_t* leftBuffer = NULL;
    uint8_t* rightBuffer = NULL;
    if (numOfElemetns == 2) {
        if (_sortData->mCompareOperation(byteData, byteData + _sortData->mDataSizeInByte) == BIGGER) {
            _sortData->mSwapOperation(byteData, byteData + _sortData->mDataSizeInByte);
        }
        return;
    }

    if (numOfElemetns < 2) {
        return;
    }
    
    leftBuffer = (uint8_t*)malloc(nextNumOfBytes);
    rightBuffer = (uint8_t*)malloc(_arrayDataToSortInByte - nextNumOfBytes);
    memscpy(leftBuffer, nextNumOfBytes, byteData, nextNumOfBytes);
    memscpy(rightBuffer, _arrayDataToSortInByte - nextNumOfBytes, byteData + nextNumOfBytes, _arrayDataToSortInByte - nextNumOfBytes);
    _RecutionMerge(leftBuffer, nextNumOfBytes, _sortData);
    _RecutionMerge(rightBuffer, _arrayDataToSortInByte - nextNumOfBytes, _sortData);

} 

Sort_Result MergeSortOnData(
    void*       _arrayOfDataToSort, 
    size_t      _dataSizeInByte, 
    size_t      _arrayDataToSortInByte,
    CompareFunc _compareOperation, 
    SwapFunc    _swapOperation) {
    
    struct MergeSort sortData = {_dataSizeInByte, _compareOperation, _swapOperation};

    if (NULL == _compareOperation   ||
        NULL == _swapOperation      ||
        NULL == _arrayOfDataToSort  || 
        0 == _dataSizeInByte        ||
        0 == _arrayDataToSortInByte ||
        _arrayDataToSortInByte < _dataSizeInByte) {
        return SORT_UNINITIALIZED_ERROR;
    }

    return SORT_SUCCESS;
}