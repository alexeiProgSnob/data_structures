#ifndef __SAFE_SQUEUE_H__
#define __SAFE_SQUEUE_H__


/**
 *  @file safe_queue.h
 *  @brief safe queue implementation
 *
 *  @details
 *
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 8/20/17
 *  @bug no bugs known.
 */


#include <stddef.h> /*< size_t >*/

typedef struct SQueue SQueue;

typedef enum SQueue_Result
{
    SQUEUE_SUCCESS = 0xDF98,
    SQUEUE_UNITIALIZED_ERROR = 1,
    SQUEUE_OVERFLOW_ERROR,
    SQUEUE_ELEMENT_UNITIALIZED_ERROR,
	SQUEUE_ELEMENT_NOT_FOUND_ERROR
}SQueue_Result;

typedef int	(*SQueueElementAction)(void* _element, void* _context);
typedef void (*ElementDestroy)(void* _element);

 /**
  * @brief Create a new SQueue with given initialize size.
  * @param[in] _initSize - initial capacity, number of elements that can be stored initially.
  * @return SQueue* - on success / NULL on fail
  */
SQueue* SQueue_Create(size_t _initSize);

/**
 * @brief : free SQueue.
 * @details : free SQueue if _desFunc is NULL.
 *
 * @param[in] _squeue - safe queue.
 * @param[in] _desFunc - a pointer to a function to destroy elements.
 * 						 If destruction function do not provided user have to
 * 						 delete all elements by him self.
 */
void SQueue_Destroy(SQueue** _squeue,ElementDestroy _desFunc);


/** 
 * @brief : Inserts a new element to tail of safe queue .
 *
 * @param[in] _squeue - safe queue.
 * @param[in] _element - a new element to insert.
 *
 * @return[success] : SQUEUE_SUCCESS
 * @return[failure] : SQUEUE_UNINITIALIZED_ERROR
 * @return[failure] : SQUEUE_ELEMENT_UNINITIALIZED_ERROR
 */
SQueue_Result SQueue_Insert(SQueue* _squeue, void* _element);


/** 
 * @brief : Removes an element from the head of the safe queue.
 *
 * @param[in] _squeue 			- safe queue.
 * @param[out] _returnElement 	- return the element that removed.
 * 								  if _returnElement NULL just remove from queue
 * @return[success] : SQUEUE_SUCCESS
 * @return[failure] : SQUEUE_UNINITIALIZED_ERROR
 */
SQueue_Result SQueue_Remove(SQueue* _squeue,void** _returnElement);

/** 
 * @brief : check if SQueue is empty
 *
 * @param[in] _squeue - safe queue.
 *
 * @return[success] : 1 if empty
 * @return[success] : 0 if not empty
 */
int SQueue_IsEmpty(const SQueue* _squeue);


/** 
 * @brief : run on all elements and preform SQueueElementAction function.
 *
 * @param[in]  _squeue - safe queue.
 * @param[in] _action - function to preform on safe queue elements
 * @param[in] _context - context for action function.
 */
SQueue_Result SQueue_ForEach(const SQueue* _squeue,SQueueElementAction _action, void* _context);


#endif /* __SAFE_SQUEUE_H__ */
