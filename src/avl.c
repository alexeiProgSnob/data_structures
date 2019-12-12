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

static TreeNode* _RightRotate(TreeNode* _node) {
    TreeNode* leftNode = _node->m_leftNode;
    TreeNode* t2 = leftNode->m_rightNode;
    
    /* preform Rotation */
    leftNode->m_rightNode = _node;
    _node->m_leftNode = t2;

    /* TODO: add height calulation */
    return leftNode;
}

static TreeNode* _LeftRotate(TreeNode* _node) {
    TreeNode* rightNode = _node->m_rightNode;
    TreeNode* t2 = rightNode->m_leftNode;
    
    /* preform Rotation */
    rightNode->m_leftNode = _node;
    _node->m_rightNode = t2;

    /* TODO: add height calulation */
    return rightNode;
}

static aps_ds_error _RecursiveInsert(AVL* _tree, TreeNode* _root, TreeNode* _newNode) {
    Compare_Result comapreResult = _tree->m_compareFunc(_newNode->m_key, _root->m_key);
    aps_ds_error recursionRetval = DS_GENERAL_ERROR;
    _newNode->m_level += 1;
    switch (comapreResult) {
        case EQUAL:
            return DS_KEY_EXISTS_ERROR;
        case BIGGER:
            if (NULL == _root->m_rightNode) {
                _root->m_rightNode = _newNode;
                ++_tree->m_numOfItems;
                return DS_SUCCESS;
            }
            recursionRetval = _RecursiveInsert(_tree, _root->m_rightNode, _newNode);
            if (recursionRetval == DS_SUCCESS) {

            }
            return recursionRetval;

        case SMALLER:
            if (NULL == _root->m_leftNode) {
                _root->m_leftNode = _newNode;
                ++_tree->m_numOfItems;
                return DS_SUCCESS;
            }
            return _RecursiveInsert(_tree, _root->m_leftNode, _newNode);
    }
    return DS_GENERAL_ERROR;
}

aps_ds_error AVLInsert(AVL* _tree, const void* _key, void* _data) {
    TreeNode* newNodeToInsert = NULL;
    aps_ds_error returnValue = DS_SUCCESS;
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
        returnValue = _RecursiveInsert(_tree, _tree->m_sentinel.m_root, newNodeToInsert);
        switch (returnValue) {
            case DS_KEY_EXISTS_ERROR: free(newNodeToInsert); break;
            default: break;
        }

    }

    return returnValue;
}

aps_ds_error AVLRemove(AVL* _tree, const void* _key, void** _pData);

aps_ds_error AVLForEach(const AVL* _tree, TravelType _travelType, ElementAction _action, void* _context);
