/**
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 08/20/17
 *  @bug no bugs known.
 */


#include "vector.h"
#include <stdlib.h> /*< malloc >*/
struct Vector {
    void** m_items;		  	/*< array of pointers of items 					>*/
    size_t m_originalSize;	/*< Vector original size 						>*/
    size_t m_size;		  	/*< Vector capacity 							>*/
	size_t m_numOfItems;	/*< Number of elemnts 							>*/
    size_t m_blockSize;		/*< block size to reallocate the size of vector >*/
};


static Vector_Result _ReallocateSpace(Vector* _vector, int _inc);
static Vector_Result _CheckIfNeedToReallocateMemory(Vector*_vector);

Vector* VectorCreate(size_t _initialCapacity, size_t _blockSize) {
	Vector *vector;
	void** pm_item;

	if (0 == _initialCapacity && 0 == _blockSize) {
		return NULL;
	}

	vector = (Vector*)malloc(sizeof(Vector));
	if (NULL == vector) {
		return NULL;
	}

	pm_item = (void**)malloc(_initialCapacity * sizeof(void*));
	if (NULL == pm_item) {
		free(vector);
		return NULL;
	}

	vector->m_items = pm_item;
    vector->m_originalSize = _initialCapacity;
    vector->m_size = _initialCapacity;
	vector->m_numOfItems = 0;
    vector->m_blockSize = _blockSize;
    return vector;
}

void VectorDestroy(Vector** _vector, void (*_elementDestroy)(void* _item)) {
	size_t idx;
    if (_vector != NULL && *_vector != NULL) {
        if (_elementDestroy != NULL) {
			for (idx = 0 ; idx < (*_vector)->m_numOfItems ; ++idx) {
				_elementDestroy((*_vector)->m_items[idx]);
			}
	    }
		free((*_vector)->m_items);
		free(*_vector);
    }
	*_vector = NULL;
    return;
}



Vector_Result VectorAppend(Vector* _vector, void* _item) {
	Vector_Result retval;
	if (NULL == _vector) {
        return VECTOR_UNINITIALIZED_ERROR;
    }

	if (NULL == _item) {
		return VECTOR_UNINITIALIZED_ITEM_ERROR;
	}

	if (_vector->m_numOfItems == _vector->m_size) {
		if (0 == _vector->m_blockSize) {
			return VECTOR_OVERFLOW_ERROR;
		}

		retval = _ReallocateSpace(_vector,1);
		if(VECTOR_SUCCESS != retval) {
			return retval;
		}
	}

	*(_vector->m_items+_vector->m_numOfItems) = _item;
	++(_vector->m_numOfItems);

	return VECTOR_SUCCESS;
}

Vector_Result VectorRemove(Vector* _vector, void** _pValue) {
	if (NULL == _vector || NULL == _pValue)
    {
        return VECTOR_UNINITIALIZED_ERROR;
    }

    if (0 == _vector->m_numOfItems) {
        return VECTOR_UNDERFLOW_ERROR;
    }

    --(_vector->m_numOfItems);
	*_pValue = *(_vector->m_items + _vector->m_numOfItems);

    return _CheckIfNeedToReallocateMemory(_vector);
}

Vector_Result VectorRemoveFrom(Vector* _vector, size_t _index, void** _pValue) {
	size_t i = 0;
	if (NULL == _vector || NULL == _pValue) {
		return VECTOR_UNINITIALIZED_ERROR;
	}

	if (_index >= _vector->m_numOfItems) {
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}

	*_pValue = *(_vector->m_items + _index);
	for (i = _index + 1; i < _vector->m_numOfItems; ++i) { 
		*(_vector->m_items + i - 1) = *(_vector->m_items + i);
	}

    --(_vector->m_numOfItems);
	return _CheckIfNeedToReallocateMemory(_vector);
}


Vector_Result VectorGet(const Vector* _vector, size_t _index, void** _pValue) {
	if (NULL == _vector || NULL == _pValue) {
		return VECTOR_UNINITIALIZED_ERROR;
	}

	if (_index >= _vector->m_numOfItems) {
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}

	*_pValue = *(_vector->m_items + _index);
	return VECTOR_SUCCESS;
}

Vector_Result VectorSet(Vector* _vector, size_t _index, void*  _value, void** _prevValue) {
	if (NULL == _vector || NULL == _value) {
		return VECTOR_UNINITIALIZED_ERROR;
	}

	if (_index >= _vector->m_numOfItems) {
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}

	if (_prevValue != NULL) {
		*_prevValue = *(_vector->m_items + _index);
	}

	*(_vector->m_items + _index) = _value;
	return VECTOR_SUCCESS;
}

size_t VectorSize(const Vector* _vector) {
	if (NULL == _vector) {
		return 0;
	}

	return _vector->m_numOfItems;
}

size_t VectorCapacity(const Vector* _vector) {
	if (NULL == _vector) {
		return 0;
	}

	return _vector->m_size;
}

size_t VectorForEach(const Vector* _vector,VectorElementAction _action, void* _context) {
	void* elem;
	size_t i;
	
	if(NULL == _vector || NULL == _action) {
		return 0;
	}

	for(i = 0; i < _vector->m_numOfItems;) {
		elem = _vector->m_items[i];
		if(_action(elem, ++i, _context) == 0) {
			break;
		}
	}
	return i;
}

static Vector_Result _CheckIfNeedToReallocateMemory(Vector*_vector) {
	if ((_vector->m_size - _vector->m_numOfItems)  >= (2 * _vector->m_blockSize)) {
		if ((_vector->m_size - _vector->m_blockSize) >= _vector-> m_originalSize) {
			return _ReallocateSpace(_vector, -1);
		}
	}

	return VECTOR_SUCCESS;
}

static Vector_Result _ReallocateSpace(Vector* _vector, int _inc) {
	void** temp;
	size_t tempSize;

	tempSize=_vector->m_size + _inc * _vector->m_blockSize;
	temp =(void**) realloc(_vector->m_items,tempSize * sizeof(void*));
	if (NULL == temp) {
	   return VECTOR_REALLOCATION_ERROR;
	}

	_vector->m_size = tempSize;
	_vector->m_items = temp;
	return VECTOR_SUCCESS;
}
