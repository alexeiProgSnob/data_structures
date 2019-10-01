#ifndef __DATA_STRUCTURE_ERROR_H_
#define __DATA_STRUCTURE_ERROR_H_

typedef enum _aps_ds_error {
	DS_SUCCESS,
	DS_UNINITIALIZED_ERROR,				/*< Uninitialized vector error >*/
	DS_OUT_OF_BOUNDS_ERROR,		        /*< Trying to get value in out of bounds >*/
	DS_UNINITIALIZED_ITEM_ERROR,		/*< Uninitialized item error >*/
	DS_REALLOCATION_ERROR,				/*< realloc error on grow/shrink >*/
	DS_OVERFLOW_ERROR,					/*< There is no space to insert items and no reallocte >*/
	DS_UNDERFLOW_ERROR,					/*< There is no items in vector >*/
    DS_GENERAL_ERROR
} aps_ds_error;

#endif /* __DATA_STRUCTURE_ERROR_H_ */