#include "sorts.h"
#include <stdlib.h> /*< malloc  >*/
#include <stdint.h> /*< uint8_t >*/
#include <string.h> /*< memscpy >*/

static int _CheckSortInputParameters(
    void*       _arrayOfDataToSort, 
    size_t      _dataSizeInByte, 
    size_t      _arrayDataToSortInByte,
    CompareFunc _compareOperation, 
    SwapFunc    _swapOperation,
    Sort_Result* _retval
);


Sort_Result BubbleSortOnData(
    void*       _arrayOfDataToSort, 
    size_t      _dataSizeInByte, 
    size_t      _arrayDataToSortInByte,
    CompareFunc _compareOperation, 
    SwapFunc    _swapOperation) {
    
    size_t i = 0;
    size_t j = 0;
    int isSwapOccur = 0;
    size_t maxIters = 0;
    uint8_t* arrayRunner = (uint8_t*)_arrayOfDataToSort;
    Sort_Result retval;
    int checkerResult = _CheckSortInputParameters(
        _arrayOfDataToSort, 
        _dataSizeInByte, 
        _arrayDataToSortInByte,
        _compareOperation, 
        _swapOperation,
        &retval);
    
    if (checkerResult != 0) {
        return retval;
    }

    maxIters = _arrayDataToSortInByte - _dataSizeInByte;
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

Sort_Result MergeSortOnData(
    void*       _arrayOfDataToSort, 
    size_t      _dataSizeInByte, 
    size_t      _arrayDataToSortInByte,
    CompareFunc _compareOperation, 
    SwapFunc    _swapOperation
) {
    Sort_Result retval;
    int checkerResult = _CheckSortInputParameters(
        _arrayOfDataToSort, 
        _dataSizeInByte, 
        _arrayDataToSortInByte,
        _compareOperation, 
        _swapOperation,
        &retval);
    
    if (checkerResult != 0) {
        return retval;
    }

    return SORT_SUCCESS;
}

static int _CheckSortInputParameters(
    void*       _arrayOfDataToSort, 
    size_t      _dataSizeInByte, 
    size_t      _arrayDataToSortInByte,
    CompareFunc _compareOperation, 
    SwapFunc    _swapOperation,
    Sort_Result* _retval
) {
    if (NULL == _arrayOfDataToSort) {
        *_retval = SORT_UNINITIALIZED_ARRAY_DATA_ERROR;
        return -1;
    }

    if (_dataSizeInByte > _arrayDataToSortInByte) {
        *_retval = SORT_TOO_SMALL_ARRAY_SIZE_ERROR;
        return -1;
    }

    if (_arrayDataToSortInByte % _dataSizeInByte != 0) {
        *_retval =  SORT_ARRAY_SIZE_UNELINED_ERROR;
        return -1;
    }

    return 0;
}