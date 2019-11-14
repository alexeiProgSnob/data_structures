#include "binary_tree.h"
#include <stdlib.h> /*< malloc >*/

typedef struct TreeNode TreeNode;

struct TreeNode {
    TreeNode* mLeftNode;
    TreeNode* mRightNode;
    void* mData;
    size_t mLevel;
};

typedef struct _Sentinel {
    TreeNode* mRoot;
} Sentinel;

struct BTree {
    Sentinel mSentinel;
    size_t mTreeHight;
    size_t mNumOfItems;
    CompareFunc mCompareFunc;
};

typedef void (*NodeTravelTask)(TreeNode* _node);
typedef struct _TravelTasks {
    void* mContext;
    BTreeElementAction mTaskOnItem;
    NodeTravelTask mTaskOnNode;
} TravelTasks;

static TreeNode* _CreateAndInitNewNode(void* _data);
static void _RecursiveInsert(BTree* _tree, TreeNode* _root, TreeNode* _newNode);
static void _OperateOnNodeAndData(TreeNode* _node, TravelTasks* _tasks);

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

    newTree->mSentinel.mRoot = NULL;
    newTree->mTreeHight = 0;
    newTree->mCompareFunc = _compareFunc;
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
    tasks.mTaskOnItem = DeleteDataFromNodes;
    tasks.mTaskOnNode = _FreeNode;
    tasks.mContext = (void*)&funcOnItem;
    if (NULL == _pTree || NULL == *_pTree) {
        return;
    }

    if (NULL != (*_pTree)->mSentinel.mRoot) {
        _PostorderTravel((*_pTree)->mSentinel.mRoot, &tasks);
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
    
    if (NULL == _tree->mSentinel.mRoot) {
        _tree->mSentinel.mRoot = newNodeToInsert;
    } else {
        _RecursiveInsert(_tree, _tree->mSentinel.mRoot, newNodeToInsert);
    }
    ++_tree->mNumOfItems;
    return DS_SUCCESS;
}

aps_ds_error BTreeRemoveData(BTree* _tree, void** _pData) {
    if (NULL == _tree) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (NULL == _tree->mSentinel.mRoot) {
        return DS_UNINITIALIZED_ITEM_ERROR;
    }

    return DS_SUCCESS;
}

aps_ds_error BTreeGetItem(BTree* _tree, void** _pData) {
    if (NULL == _tree || NULL == _pData) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (NULL == _tree->mSentinel.mRoot) {
        return DS_UNINITIALIZED_ITEM_ERROR;
    }
    
    return DS_SUCCESS;
}

ssize_t BTreeGetNumberOfItems(BTree* _tree) {
    if (NULL == _tree) {
        return -1;
    }

    return _tree->mNumOfItems; 
}

aps_ds_error BTreeForEach(BTree* _tree, TravelType _travelType, BTreeElementAction _action, void* _context) {
    TravelTasks tasks;
    if (NULL == _tree || NULL == _action) {
        return DS_UNINITIALIZED_ERROR;
    }

    if (NULL == (_tree->mSentinel).mRoot) {
        return DS_UNINITIALIZED_ITEM_ERROR;
    }

    tasks.mContext = _context;
    tasks.mTaskOnItem = _action;
    tasks.mTaskOnNode = NULL;
    switch (_travelType) {
        case IN_ORDER:      _InorderTravel((_tree->mSentinel).mRoot, &tasks); break;
        case POST_ORDER:    _PostorderTravel((_tree->mSentinel).mRoot, &tasks); break;
        case PRE_ORDER:     _PreorderTravel((_tree->mSentinel).mRoot, &tasks); break;
        default:
            return DS_INVALID_PARAM_ERROR;
    }

    return DS_SUCCESS;
}

static void _RecursiveInsert(BTree* _tree, TreeNode* _root, TreeNode* _newNode) {
    Compare_Result comapreResult = _tree->mCompareFunc(_newNode->mData, _root->mData);
    _newNode->mLevel += 1;
    switch (comapreResult) {
        case EQUAL:
        case BIGGER:
            if (NULL == _root->mRightNode) {
                _root->mRightNode = _newNode;
                if (_newNode->mLevel > _tree->mTreeHight) {
                    _tree->mTreeHight = _newNode->mLevel;
                }
                return;
            }
            _RecursiveInsert(_tree, _root->mRightNode, _newNode);
            break;
        case SMALLER:
            if (NULL == _root->mLeftNode) {
                _root->mLeftNode = _newNode;
                if (_newNode->mLevel > _tree->mTreeHight) {
                    _tree->mTreeHight = _newNode->mLevel;
                }
                return;
            }
            _RecursiveInsert(_tree, _root->mLeftNode, _newNode);
            break;
    }

}

static TreeNode* _CreateAndInitNewNode(void* _data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (NULL == newNode) {
        return NULL;
    }

    newNode->mData = _data;
    newNode->mLevel = 0;
    newNode->mLeftNode = NULL;
    newNode->mRightNode = NULL;

    return newNode;
}

static void _OperateOnNodeAndData(TreeNode* _node, TravelTasks* _tasks) {
    if (NULL == _node || NULL == _tasks) {
        return;
    }

    if (NULL != _tasks->mTaskOnItem) {
        _tasks->mTaskOnItem(_node->mData, _tasks->mContext);
    }

    if (NULL != _tasks->mTaskOnNode) {
        _tasks->mTaskOnNode(_node);
    }
}

/*< (Left, Root, Right) >*/
static void _InorderTravel(TreeNode* _root, TravelTasks* _tasks) {
    if (NULL == _root->mLeftNode && NULL == _root->mRightNode) {
        _OperateOnNodeAndData(_root, _tasks);
        return;
    }

    if (NULL != _root->mLeftNode) {
        _InorderTravel(_root->mLeftNode, _tasks);
    }

    _OperateOnNodeAndData(_root, _tasks);

    if (NULL != _root->mRightNode) {
        _InorderTravel(_root->mRightNode, _tasks);
    }
}

/*< (Left, Right, Root) >*/
static void _PostorderTravel(TreeNode* _root, TravelTasks* _tasks) {
    if (NULL == _root->mLeftNode && NULL == _root->mRightNode) {
        _OperateOnNodeAndData(_root, _tasks);
        return;
    }

    if (NULL != _root->mLeftNode) {
        _PostorderTravel(_root->mLeftNode, _tasks);
    }

    if (NULL != _root->mRightNode) {
        _PostorderTravel(_root->mRightNode, _tasks);
    }
    _OperateOnNodeAndData(_root, _tasks);
}


static void _PreorderTravel(TreeNode* _root, TravelTasks* _tasks) {
        if (NULL == _root->mLeftNode && NULL == _root->mRightNode) {
        _OperateOnNodeAndData(_root, _tasks);
        return;
    }

    _OperateOnNodeAndData(_root, _tasks);
    if (NULL != _root->mLeftNode) {
        _PreorderTravel(_root->mLeftNode, _tasks);
    }

    if (NULL != _root->mRightNode) {
        _PreorderTravel(_root->mRightNode, _tasks);
    }
}