#include "heap.h"
#include "ds_assert.h"
#include "vector.h"
#include <stdlib.h> /*< malloc >*/

struct Heap {
  Vector *m_vector;
  Heap_Type m_heapType;
  HeapDataCompareFunc m_compareFunc;
};

static aps_ds_error
_FindPlaceToInsert(Heap *_heap, void *_data, size_t _lastPlace,
                   Heap_Data_Compare_Result _expectedCompareResult);

/**
 * @brief Create a new heap with given size.
 * @param[in] _heapSize - Expected max capacity.
 * @param[in] _heapType - Heap type if store max or min
 * @param[in] _comapreFunc - compare function for heap data
 * @return newly created heap or null on failure
 */
Heap *HeapCreate(size_t _heapSize, Heap_Type _heapType,
                 HeapDataCompareFunc _comapreFunc) {
  Heap *newHeap = NULL;
  Vector *newHeapVector = NULL;
  if (0 == _heapSize || NULL == _comapreFunc) {
    return NULL;
  }

  if (_heapType >= HEAP_TYPE_ENUM_END || _heapType <= HEAP_TYPE_ENUM_START) {
    return NULL;
  }

  newHeap = (Heap *)malloc(sizeof(Heap));
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
 * @params[in] _elementDestroy : A function pointer to be used to destroy all
 * elements in the vector or a null if no such destroy is required
 * @return void
 */
void HeapDestroy(Heap **_heap, void (*_elementDestroy)(void *_item)) {
  if (NULL == _heap || NULL == *_heap) {
    return;
  }

  VectorDestroy(&(*_heap)->m_vector, _elementDestroy);
  free(*_heap);
  *_heap = NULL;
}

/**
 * @brief insert new data into heap
 * @param[in] _heap - Heap.
 * @params[in] _data - pointer to the data
 * @return DS_SUCCESS, and other on error
 */
aps_ds_error HeapPush(Heap *_heap, void *_data) {
  Heap_Data_Compare_Result expectedCopareResult;
  if (NULL == _heap) {
    return DS_UNINITIALIZED_ERROR;
  }

  if (NULL == _data) {
    return DS_UNINITIALIZED_ITEM_ERROR;
  }

  expectedCopareResult = _heap->m_heapType == HEAP_TYPE_MAX
                             ? HEAP_COMPARE_BIGGER
                             : HEAP_COMPARE_SMALLER;
  /* TODO: fix error hendeling */
  if (_FindPlaceToInsert(_heap, _data, VectorSize(_heap->m_vector),
                         expectedCopareResult) != DS_SUCCESS) {
    return DS_GENERAL_ERROR;
  }
  return DS_SUCCESS;
}

/**
 * @brief Remove elemnt from the top
 * @param[in] _heap - Heap.
 * @param[out]_pValue - pointer where to store the pointer to the value
 * @return DS_SUCCESS, other error on failure
 */
aps_ds_error HeapPop(Heap *_heap, void **_pValue) {
  aps_ds_error vectorResult = DS_UNINITIALIZED_ERROR;
  void *lastElement = NULL;
  if (NULL == _heap || NULL == _pValue) {
    return DS_UNINITIALIZED_ERROR;
  }

  if (0 == HeapSize(_heap)) {
    return DS_OUT_OF_BOUNDS_ERROR;
  }

  vectorResult = VectorRemove(_heap->m_vector, &lastElement);
  DS_ASSERT_EQUAL(vectorResult, DS_SUCCESS);

  vectorResult = VectorSet(_heap->m_vector, 0, lastElement, _pValue);
  if (vectorResult != DS_SUCCESS) {
    VectorAppend(_heap->m_vector, lastElement);
    return vectorResult;
  }

  return DS_SUCCESS;
}

/**
 * @brief Get the top value in heap do not remove the value
 * @param[in] _heap - Heap.
 * @return the address to the value on success, NULL on failure
 */
const void *HeapGetTopValue(const Heap *_heap) {
  void *value = NULL;
  if (NULL == _heap) {
    return NULL;
  }

  if (0 == VectorSize(_heap->m_vector)) {
    return NULL;
  }

  if (DS_SUCCESS != VectorGet(_heap->m_vector, 0, &value)) {
    return NULL;
  }

  return value;
}

/**
 * @brief Get the number of elements that inserted into heap
 * @param[in] _heap - Heap.
 * @return the size, -1 on failure
 */
ssize_t HeapSize(const Heap *_heap) {
  if (NULL == _heap) {
    return -1;
  }
  return VectorSize(_heap->m_vector);
}

static aps_ds_error
_ReplaceAfterRemove(Heap *_heap, size_t _place,
                    Heap_Data_Compare_Result _expectedCompareResult) {
  void *childA = NULL;
  void *childB = NULL;
  size_t childPlaceA = (_place + 1) * 2;
  size_t childPlaceB = (_place + 1) * 2 - 1;
}

static aps_ds_error
_FindPlaceToInsert(Heap *_heap, void *_data, size_t _lastPlace,
                   Heap_Data_Compare_Result _expectedCompareResult) {
  void *comapreData = NULL;
  aps_ds_error vectorResult = DS_OUT_OF_BOUNDS_ERROR;
  size_t parentPlace = 0;
  size_t vectorSize = VectorSize(_heap->m_vector);
  if (0 == _lastPlace) {
    if (0 == vectorSize) {
      return VectorAppend(_heap->m_vector, _data);
    }
    return VectorSet(_heap->m_vector, _lastPlace, _data, NULL);
  }

  parentPlace = (_lastPlace % 2) == 0 ? _lastPlace / 2 - 1 : _lastPlace / 2;
  vectorResult = VectorGet(_heap->m_vector, parentPlace, &comapreData);
  if (vectorResult != DS_SUCCESS) {
    return vectorResult;
  }

  if (_expectedCompareResult == _heap->m_compareFunc(_data, comapreData)) {
    if (_lastPlace == vectorSize) {
      VectorAppend(_heap->m_vector, comapreData);
    } else {
      VectorSet(_heap->m_vector, _lastPlace, comapreData, NULL);
    }
    return _FindPlaceToInsert(_heap, _data, parentPlace,
                              _expectedCompareResult);
  }

  if (_lastPlace == vectorSize) {
    return VectorAppend(_heap->m_vector, _data);
  } else {
    return VectorSet(_heap->m_vector, _lastPlace, _data, NULL);
  }
}