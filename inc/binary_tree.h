#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

/** 
 *  @file binary_tree.h
 *  @brief
 *	
 *  @details  
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @update Alexei Radashkovsky (alexeirada@gmail.com)
 *  @bug No known bugs.
 */

#include "data_structure_defenitions.h"
#include <unistd.h> /*< ssize_t >*/

typedef struct BTree BTree;


/* For example
Compare_Result Int(const void* _a, const void* _b) {
    const int* a = (const int*)_a;
    const int* b = (const int*)_b;
    if (a > b) {
        return BIGGER;
    }
    
    if (a == b) {
        return EQUAL;
    }
    
    return SMALLER;
}
*/

/** 
 * @brief Create a new binary search tree.
 * @param[in] _comapreFunc - compare function for tree data
 * @return newly created BTree or NULL on failure
 */
BTree* BTreeCreate(CompareFunc _compareFunc);

/** 
 * @brief Destroy the tree
 * @param[in/out]   _pTree          - Pointer to the pointer tree after free it will be NULL
 * @param[in]       _elementDestroy - Destroy function for for the data if NULL the data will not be freed
 * @return
 */
void BTreeDestroy(BTree** _pTree, ElementDestroy _elementDestroy, ElementDestroy _keyDestroy);

/** 
 * @brief Insert new data to binary search tree.
 * @param[in] _tree - binary search tree
 * @param[in] _data - data that will be inserted. The data must contain a key.
 * @return[success] DS_SUCCESS 
 */
aps_ds_error BTreeInsert(BTree* _tree, const void* _key, void* _data);

/** 
 * @brief Remove data from binary search tree.
 * @param[in] _tree         - binary search tree
 * @param[in] _keyToFind    - key to remove from tree.
 * @param[out]_pData        - pointer to get the data that was removed.It could be NULL.
 * @return[success] DS_SUCCESS 
 */
aps_ds_error BTreeRemove(BTree* _tree, const void* _keyToFind, void** _pData);

/** 
 * @brief Get praticular data from binary search tree.
 * @param[in] _tree         - binary search tree
 * @param[in] _keyToFind    - key to get from tree.
 * @param[out]_pData        - pointer to get the data that will be returned.
 * @return[success] DS_SUCCESS 
 */
aps_ds_error BTreeGetItem(const BTree* _tree, const void* _keyToFind, void** _pData);

/**
 * @brief Iterate over all elements in the tree.
 * @details The user provided _action function will be called for each element.
 * @param[in] _tree     - binary search tree
 * @param[in] _action   - User provided function pointer to be invoked for each element
 * @param[in] _context  - User provided context, will be sent to _action
 * @returns[success] DS_SUCCESS
 */
aps_ds_error BTreeForEach(const BTree* _tree, TravelType _travelType, ElementAction _action, void* _context);

/** 
 * @brief Get The minimal key data.
 * @param[in] _tree - binary search tree
 * @returns[success] Pointer to the data
 * @returns[failure] NULL
 */
void* BTreeGetMin(const BTree* _tree);

/** 
 * @brief Get The maximal key data.
 * @param[in] _tree - binary search tree
 * @returns[success] Pointer to the data
 * @returns[failure] NULL
 */
void* BTreeGetMax(const BTree* _tree);

/** 
 * @brief Get The root data.
 * @param[in] _tree - binary search tree
 * @returns[success] Pointer to the data
 * @returns[failure] NULL
 */
void* BTreeGetRoot(const BTree* _tree);

/** 
 * @brief Get height of the tree
 * @param[in] _tree - binary search tree
 * @returns[success] The height of the tree
 * @returns[failure] -1
 */
ssize_t BTreeGetTreeHeight(BTree* _tree);

/** 
 * @brief Get number items in a tree
 * @param[in] _tree - binary search tree
 * @returns[success] number items in a tree
 * @returns[failure] -1
 */
ssize_t BTreeGetNumberOfItems(const BTree* _tree);

#endif /* __BINARY_TREE_H__ */