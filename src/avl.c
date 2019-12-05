#include "avl.h"
#include "tree_internal.h"

struct AVL {
    Sentinel m_sentinel;
    size_t m_numOfItems;
    CompareFunc m_compareFunc;
};

AVL* AVLCreate(CompareFunc _compareFunc) {
    AVL* newTree = NULL;
    if (NULL == _compareFunc) {
        return NULL;
    }

    newTree = (AVL*)malloc(sizeof(AVL));
    if (NULL == newTree) {
        return NULL;
    }

    newTree->m_sentinel.m_root = NULL;
    newTree->m_numOfItems = 0;
    newTree->m_compareFunc = _compareFunc;
    return newTree;
}

void AVLDestroy(AVL** _pTree, ElementDestroy _elementDestroy) {
    TravelTasks tasks;
    FuncOnItem funcOnItem;
    funcOnItem.mDeleteFunc = _elementDestroy;
    tasks.m_taskOnItem = DeleteDataFromNodes;
    tasks.m_taskOnNode = _FreeNode;
    tasks.m_context = (void*)&funcOnItem;
    if (NULL == _pTree || NULL == *_pTree) {
        return;
    }

    if (NULL != (*_pTree)->m_sentinel.m_root) {
        _PostorderTravel((*_pTree)->m_sentinel.m_root, &tasks);
    }

    free(*_pTree);
    *_pTree = NULL;
}

aps_ds_error AVLInsert(AVL* _tree, const void* _key, void* _data) {
    TreeNode* newNodeToInsert = NULL;
    if (NULL == _tree || NULL == _data || NULL == _key) {
        return DS_UNINITIALIZED_ERROR;
    }

    newNodeToInsert = _CreateAndInitNewNode(_key, _data);
    if (NULL == newNodeToInsert) {
        return DS_ALLOCATION_ERROR;
    }
    
    if (NULL == _tree->m_sentinel.m_root) {
        _tree->m_sentinel.m_root = newNodeToInsert;
        ++_tree->m_numOfItems;
    } else {
        return _RecursiveInsert(_tree, _tree->m_sentinel.m_root, newNodeToInsert);
    }

    return DS_SUCCESS;
}

aps_ds_error AVLRemove(AVL* _tree, const void* _key, void** _pData);

aps_ds_error AVLForEach(const AVL* _tree, TravelType _travelType, ElementAction _action, void* _context);
