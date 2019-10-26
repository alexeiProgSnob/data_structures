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
typedef Compare_Result (*TreeCompareFunc)(const void*, const void*);

BTree* BTreeCreate(TreeCompareFunc _compareFunc);

void BTreeDestroy(BTree** _pTree, void (*_elementDestroy)(void* _item));

#endif /* __BINARY_TREE_H__ */