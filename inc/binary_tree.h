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

#include "data_structure_error.h"
#include <unistd.h> /*< ssize_t >*/

typedef struct BTree BTree;
typedef void (*BTreeElementAction)(void* _item, void* _context);

typedef enum TravelType {
    IN_ORDER,
    POST_ORDER,
    PRE_ORDER
} TravelType;

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
 * @brief Create a new heap with given size.
 * @param[in] _comapreFunc - compare function for heap data
 * @return newly created heap or null on failure
 */
BTree* BTreeCreate(CompareFunc _compareFunc);

void BTreeDestroy(BTree** _pTree, ElementDestroy _elementDestroy);

aps_ds_error BTreeInsert(BTree* _tree, void* _data);

aps_ds_error BTreeRemove(BTree* _tree, void* _keyToFind, void** _pData);

aps_ds_error BTreeGetItem(BTree* _tree, void* _keyToFind, void** _pData);

ssize_t BTreeGetNumberOfItems(BTree* _tree);

void* BTreeGetMin(BTree* _tree);

void* BTreeGetMax(BTree* _tree);

ssize_t BTreeGetTreeHeight(BTree* _tree);

aps_ds_error BTreeForEach(BTree* _tree, TravelType _travelType, BTreeElementAction _action, void* _context);
#endif /* __BINARY_TREE_H__ */