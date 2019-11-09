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

/* For example
Compare_Result Int_Heap(const void* _a, const void* _b) {
    const int* a = (const int*)_a;
    const int* b = (const int*)_b;
    if (a > b) {
        return BIGGER;
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

void BTreeDestroy(BTree** _pTree, void (*_elementDestroy)(void* _item));

aps_ds_error BTreeInsert(BTree* _tree, void* _data);

ssize_t BTreeGetNumberOfItems(BTree* _tree);
#endif /* __BINARY_TREE_H__ */