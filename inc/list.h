#ifndef __LIST_H__
#define __LIST_H__

/** 
 *  @file list.h
 *  @brief Create a Generic Double Linked List data type
 * 
 *  @details Implemented as double linked list using head and tail sentinels.
 *  Nodes are allocated from heap an store pointer to user proveded element.
 *  
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @@update Alexei Radashkovsky on 08/20/17
 *  @bug No known bugs.
 */ 

#include "list_itr.h"
#include "list_operations.h"
#include "data_structure_defenitions.h"
#include <stddef.h>  /*< size_t >*/


/** 
 * @brief Create a list
 * 
 * @returns a pointer to the created list.
 * @retval NULL on failure due to allocation failure
 */
List* ListCreate(void);

/** 
 * @brief Destroy list
 * @details Destroys the list completely 
 *          optionally destroys elements using user provided function
 *
 * @param[in] _pList : A pointer to previously created List returned via ListCreate
 *					   on completion *_pList will be null
 * @param[in] _elementDestroy : A function pointer to be used to destroy elements inserted into the list
 *             or a null if no such destroy is required
 */
void ListDestroy(List** _pList, void (*_elementDestroy)(void* _item));

/** 
 * @brief Add element to head of list
 * @details time complexity: O(1).
 *
 * @param _list A previously created List ADT returned via ListCreate
 * @param _item An item to add to the list
 * @returns error code
 * @retval DS_SUCCESS on success
 * @retval DS_UNINITIALIZED_ERROR if the list is not initialized
 * @retval DS_UNINITIALIZED_ITEM_ERROR  if _item is NULL
 * @retval DS_ALLOCATION_ERROR on memory allocation failure
 */
aps_ds_error ListPushHead(List* _list, void* _item);

/** 
 * @brief Add element to the list's tail
 * @details time complexity O(1).
 *
 * @param _list  A previously created List ADT returned via ListCreate
 * @param _item An item to add to the list
 * @returns error code
 * @retval DS_SUCCESS on success
 * @retval DS_UNINITIALIZED_ERROR if the list is not initialized
 * @retval DS_UNINITIALIZED_ITEM_ERROR  if _item is NULL
 * @retval DS_ALLOCATION_ERROR on memory allocation failure 
 */
aps_ds_error ListPushTail(List* _list, void* _item);

/** @brief Remove element from list's head
 *  @details if successfull, stores a pointer to the removed item in _item
 *  time complexity O(1).
 *
 * @param _list : A previously created List ADT returned via ListCreate
 * @param _pItem: To store a pointer to the removed item in Head
 * @returns item that was removed
 * @retval DS_SUCCESS on success
 * @retval DS_UNDERFLOW_ERROR if the list is empty
 * @retval DS_UNINITIALIZED_ERROR if the list or _pItem is not initialized
 */
aps_ds_error ListPopHead(List* _list, void** _pItem);

/** @brief Remove element from list's tail
 *  @details time complexity O(1).
 *
 * @param _list : A previously created List ADT returned via ListCreate
 * @param _pItem: To store a pointer to the removed item in Tail
 * @returns item that was removed
 * @retval DS_SUCCESS on success
 * @retval DS_UNDERFLOW_ERROR if the list is empty
 * @retval DS_UNINITIALIZED_ERROR if the list or _pItem is not initialized 
 */
aps_ds_error ListPopTail(List* _list, void** _pItem);

/** @brief Get number of elements in the list
 * Average time complexity O(n).
 *
 * @param _list : A previously created List ADT returned via ListCreate
 * @returns number of elements in the list
 */
size_t ListSize(const List* _list);


#endif /* __LIST_H__ */
