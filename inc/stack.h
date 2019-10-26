#ifndef __STACK_H__
#define __STACK_H__

/** 
 *  @file stack.h
 *  @brief
 *	
 *  @details  
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @update Alexei Radashkovsky (alexeirada@gmail.com)
 *  @bug No known bugs.
 */

#include "data_structure_error.h"
#include <stddef.h> /*< size_t >*/
#include <unistd.h> /*< ssize_t >*/

typedef struct Stack Stack;

/** 
 * @brief Create a new stack without fixed size.
 * @return newly created stack or null on failure
 */
Stack* StackCreate(void);

/**
 * @brief Remove element from the top
 * @param[in] _stack - Stack.
 * @param[out]_pValue - pointer where to store the pointer to the value 
 * @return DS_SUCCESS, other error on failure
 */
aps_ds_error StackPop(Stack* _stack, void** _pValue);

/**
 * @brief insert new data into heap
 * @param[in] _stack - Stack.
 * @params[in] _data - pointer to the data
 * @return DS_SUCCESS, and other on error
 */
aps_ds_error StackPush(Stack* _stack, void* _data);


/**
 * @brief insert new data into heap
 * @param[in] _stack - Stack.
 * @return number of elements in stack, -1 on error
 */
ssize_t StackSize(Stack* _stack);

#endif