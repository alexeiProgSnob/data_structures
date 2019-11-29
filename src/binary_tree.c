#include "binary_tree.h"
#include <stdlib.h> /*< malloc >*/
#include <stdint.h> /*< int8_t >*/
typedef struct TreeNode TreeNode;

struct TreeNode {
    TreeNode* m_leftNode;
    TreeNode* m_rightNode;
    void* m_data;
    size_t m_level;
};

typedef struct _Sentinel {
    TreeNode* m_root;
} Sentinel;

struct BTree {
    Sentinel m_sentinel;
    size_t m_treeHight;
    size_t m_numOfItems;
    int8_t m_isRemoveCalled;
    CompareFunc m_compareFunc;
};

typedef void (*NodeTravelTask)(TreeNode* _node);
typedef struct _TravelTasks {
    void* m_context;
    BTreeElementAction m_taskOnItem;
    NodeTravelTask m_taskOnNode;
} TravelTasks;

static TreeNode* _CreateAndInitNewNode(void* _data);
static aps_ds_error _RecursiveInsert(BTree* _tree, TreeNode* _root, TreeNode* _newNode);
static void _OperateOnNodeAndData(TreeNode* _node, TravelTasks* _tasks);
static size_t _TreeHeight(TreeNode* _root);
static TreeNode* _MaxValueNode(TreeNode* _root);
static TreeNode* _MinValueNode(TreeNode* _root);
static TreeNode* _FindNode(TreeNode* _root, const void* _keyToFind, CompareFunc _compare);
static TreeNode* _RecursiveDeletion(TreeNode* _root, const void* _keyToFind, void** _pData, CompareFunc _compare);

/*< (Left, Root, Right) >*/
static void _InorderTravel(TreeNode* _root, TravelTasks* _tasks);

/*< (Root, Left, Right)  >*/
static void _PreorderTravel(TreeNode* _root, TravelTasks* _tasks);

/*< (Left, Right, Root) >*/
static void _PostorderTravel(TreeNode* _root, TravelTasks* _tasks);


BTree* BTreeCreate(CompareFunc _compareFunc) {
    BTree* newTree = NULL;
    if (NULL == _compareFunc) {
        return NULL;
    }

    newTree = (BTree*)malloc(sizeof(BTree));
    if (NULL == newTree) {
        return NULL;
    }

    newTree->m_sentinel.m_root = NULL;
    newTree->m_treeHight = 0;
    newTree->m_compareFunc = _compareFunc;
    newTree->m_isRemoveCalled = 0;
    return newTree;
}

static void _FreeNode(TreeNode* _node) {
    free(_node);
}

typedef struct FuncOnItem {
    ElementDestroy mDeleteFunc;
} FuncOnItem;

void DeleteDataFromNodes(void* _item, void* _context) {
    FuncOnItem* deleteFunc = (FuncOnItem*)_context;
    if (NULL != deleteFunc->mDeleteFunc) {
        deleteFunc->mDeleteFunc(_item);
    }
}

void BTreeDestroy(BTree** _pTree, ElementDestroy _elementDestroy) {
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


aps_ds_error BTreeInsert(BTree* _tree, void* _data) {
    TreeNode* newNodeToInsert = NULL;
    if (NULL == _tree || NULL == _data) {
        return DS_UNINITIALIZED_ERROR;
    }

    newNodeToInsert = _CreateAndInitNewNode(_data);
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

aps_ds_error BTreeRemove(BTree* _tree, const void* _keyToFind, void** _pData) {
    void* retData = NULL;
    if (NULL == _tree || NULL == _keyToFind) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (NULL == _tree->m_sentinel.m_root) {
        return DS_UNINITIALIZED_ITEM_ERROR;
    }

    _tree->m_sentinel.m_root = _RecursiveDeletion(_tree->m_sentinel.m_root, _keyToFind, &retData, _tree->m_compareFunc);

    if (NULL == retData) {
        return DS_ELEMENT_NOT_FOUND_ERROR;
    }

    --_tree->m_numOfItems;
    _tree->m_isRemoveCalled = -1;
    if (NULL != _pData) {
        *_pData = retData;
    }
    return DS_SUCCESS;
}

aps_ds_error BTreeGetItem(const BTree* _tree, const void* _keyToFind, void** _pData) {
    TreeNode* foundNode = NULL;
    if (NULL == _tree || NULL == _keyToFind || NULL == _pData) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (NULL == _tree->m_sentinel.m_root) {
        return DS_UNINITIALIZED_ITEM_ERROR;
    }

    foundNode = _FindNode(_tree->m_sentinel.m_root, _keyToFind, _tree->m_compareFunc);

    if (NULL == foundNode) {
        return DS_ELEMENT_NOT_FOUND_ERROR;
    }
    
    *_pData = foundNode->m_data;
    return DS_SUCCESS;
}

aps_ds_error BTreeForEach(const BTree* _tree, TravelType _travelType, BTreeElementAction _action, void* _context) {
    TravelTasks tasks;
    if (NULL == _tree || NULL == _action) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (NULL == (_tree->m_sentinel).m_root) {
        return DS_UNINITIALIZED_ITEM_ERROR;
    }

    tasks.m_context = _context;
    tasks.m_taskOnItem = _action;
    tasks.m_taskOnNode = NULL;
    switch (_travelType) {
        case IN_ORDER:      _InorderTravel((_tree->m_sentinel).m_root, &tasks); break;
        case POST_ORDER:    _PostorderTravel((_tree->m_sentinel).m_root, &tasks); break;
        case PRE_ORDER:     _PreorderTravel((_tree->m_sentinel).m_root, &tasks); break;
        default:
            return DS_INVALID_PARAM_ERROR;
    }

    return DS_SUCCESS;
}

ssize_t BTreeGetNumberOfItems(const BTree* _tree) {
    if (NULL == _tree) {
        return -1;
    }

    return _tree->m_numOfItems; 
}

ssize_t BTreeGetTreeHeight(BTree* _tree) {
    if (NULL == _tree) {
        return -1;
    }

    if (_tree->m_isRemoveCalled != 0) {
        _tree->m_treeHight = _TreeHeight(_tree->m_sentinel.m_root);
        _tree->m_isRemoveCalled = 0;
    }

    return _tree->m_treeHight;
}

static void* _GetMinOrMaxFromTree(const BTree* _tree, TreeNode* (*_staticFunc)(TreeNode*)) {
    TreeNode* node = NULL;
    if (NULL == _tree) {
        return NULL;
    }
    
    node = _staticFunc(_tree->m_sentinel.m_root);
    return node->m_data;
}

void* BTreeGetMin(const BTree* _tree) {
    return _GetMinOrMaxFromTree(_tree, _MinValueNode);
}

void* BTreeGetMax(const BTree* _tree) {
    return _GetMinOrMaxFromTree(_tree, _MaxValueNode);
}

void* BTreeGetRoot(const BTree* _tree) {
    if (NULL == _tree) {
        return NULL;
    }
    return _tree->m_sentinel.m_root;
}

static aps_ds_error _RecursiveInsert(BTree* _tree, TreeNode* _root, TreeNode* _newNode) {
    Compare_Result comapreResult = _tree->m_compareFunc(_newNode->m_data, _root->m_data);
    _newNode->m_level += 1;
    switch (comapreResult) {
        case EQUAL:
            return DS_KEY_EXISTS_ERROR;
        case BIGGER:
            if (NULL == _root->m_rightNode) {
                _root->m_rightNode = _newNode;
                if (_newNode->m_level > _tree->m_treeHight) {
                    _tree->m_treeHight = _newNode->m_level;
                }
                ++_tree->m_numOfItems;
                _tree->m_isRemoveCalled = 0;
                return DS_SUCCESS;
            }
            return _RecursiveInsert(_tree, _root->m_rightNode, _newNode);
        case SMALLER:
            if (NULL == _root->m_leftNode) {
                _root->m_leftNode = _newNode;
                if (_newNode->m_level > _tree->m_treeHight) {
                    _tree->m_treeHight = _newNode->m_level;
                }
                ++_tree->m_numOfItems;
                _tree->m_isRemoveCalled = 0;
                return DS_SUCCESS;
            }
            return _RecursiveInsert(_tree, _root->m_leftNode, _newNode);
    }
    return DS_GENERAL_ERROR;
}

static size_t _TreeHeight(TreeNode* _root) {
    size_t leftHeight = 0;
    size_t rightHeight = 0;
    if (NULL == _root) {
        return 0;
    }
    leftHeight = _TreeHeight(_root->m_leftNode);
    rightHeight = _TreeHeight(_root->m_rightNode);
    
    return 1 + MAX(leftHeight, rightHeight);
}

static TreeNode* _CreateAndInitNewNode(void* _data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (NULL == newNode) {
        return NULL;
    }

    newNode->m_data = _data;
    newNode->m_level = 0;
    newNode->m_leftNode = NULL;
    newNode->m_rightNode = NULL;

    return newNode;
}

static void _OperateOnNodeAndData(TreeNode* _node, TravelTasks* _tasks) {
    if (NULL == _node || NULL == _tasks) {
        return;
    }

    if (NULL != _tasks->m_taskOnItem) {
        _tasks->m_taskOnItem(_node->m_data, _tasks->m_context);
    }

    if (NULL != _tasks->m_taskOnNode) {
        _tasks->m_taskOnNode(_node);
    }
}

static TreeNode* _MaxValueNode(TreeNode* _root) {
    if (NULL == _root->m_rightNode) {
        return _root;
    }

    return _MaxValueNode(_root->m_rightNode);
}

static TreeNode* _MinValueNode(TreeNode* _root) {
    if (NULL == _root->m_leftNode) {
        return _root;
    }

    return _MinValueNode(_root->m_leftNode);
}

static TreeNode* _RecursiveDeletion(TreeNode* _root, const void* _keyToFind, void** _pData, CompareFunc _compare) {
    Compare_Result compareResult = 0xFFFFFF;
    TreeNode* retNode = _root;
    TreeNode* minValueNode = NULL;
    if (NULL == _root) {
        return _root;
    }

    compareResult = _compare(_keyToFind, _root->m_data);
    switch (compareResult) {
        case BIGGER:    
            _root->m_rightNode = _RecursiveDeletion(_root->m_rightNode, _keyToFind, _pData, _compare);
            break;
        case SMALLER: 
            _root->m_leftNode = _RecursiveDeletion(_root->m_leftNode, _keyToFind, _pData, _compare);
            break;
        case EQUAL:
            *_pData = _root->m_data;
            if (NULL == _root->m_leftNode) {
                retNode = _root->m_rightNode;
                free(_root);
            } else if (NULL == _root->m_rightNode) {
                retNode = _root->m_leftNode;
                free(_root);
            } else {
                minValueNode = _MinValueNode(_root->m_rightNode);
                _root->m_rightNode = _RecursiveDeletion(_root->m_rightNode, minValueNode->m_data, &_root->m_data, _compare);
            }
        default: break;
    }

    return retNode;
}

static TreeNode* _FindNode(TreeNode* _root, const void* _keyToFind, CompareFunc _compare) {
    Compare_Result compareResult = 0xFFFFFF;
    if (NULL == _root) {
        return _root;
    }

    compareResult = _compare(_keyToFind, _root->m_data);
    switch (compareResult) {
        case EQUAL:     return _root;
        case BIGGER:    return _FindNode(_root->m_rightNode, _keyToFind, _compare);
        case SMALLER:   return _FindNode(_root->m_leftNode, _keyToFind, _compare);
        default:        break;
    }
    return NULL;
}

/*< (Left, Root, Right) >*/
static void _InorderTravel(TreeNode* _root, TravelTasks* _tasks) {
    if (NULL == _root->m_leftNode && NULL == _root->m_rightNode) {
        _OperateOnNodeAndData(_root, _tasks);
        return;
    }

    if (NULL != _root->m_leftNode) {
        _InorderTravel(_root->m_leftNode, _tasks);
    }

    _OperateOnNodeAndData(_root, _tasks);

    if (NULL != _root->m_rightNode) {
        _InorderTravel(_root->m_rightNode, _tasks);
    }
}

/*< (Left, Right, Root) >*/
static void _PostorderTravel(TreeNode* _root, TravelTasks* _tasks) {
    if (NULL == _root->m_leftNode && NULL == _root->m_rightNode) {
        _OperateOnNodeAndData(_root, _tasks);
        return;
    }

    if (NULL != _root->m_leftNode) {
        _PostorderTravel(_root->m_leftNode, _tasks);
    }

    if (NULL != _root->m_rightNode) {
        _PostorderTravel(_root->m_rightNode, _tasks);
    }
    _OperateOnNodeAndData(_root, _tasks);
}


static void _PreorderTravel(TreeNode* _root, TravelTasks* _tasks) {
    if (NULL == _root->m_leftNode && NULL == _root->m_rightNode) {
        _OperateOnNodeAndData(_root, _tasks);
        return;
    }

    _OperateOnNodeAndData(_root, _tasks);
    if (NULL != _root->m_leftNode) {
        _PreorderTravel(_root->m_leftNode, _tasks);
    }

    if (NULL != _root->m_rightNode) {
        _PreorderTravel(_root->m_rightNode, _tasks);
    }
}