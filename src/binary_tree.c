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


static TreeNode* _CreateAndInitNewNode(void* _data);
static void _RecursiveInsert(BTree* _tree, TreeNode* _root, TreeNode* _newNode);

typedef void (*DataTrevalTask)(void* _item, void* _context);
typedef void (*NodeTrevalTask)(TreeNode* _node);

typedef struct _TrevalTasks {
    DataTrevalTask mTaskOnItem;
    NodeTrevalTask mTaskOnNode;
} TrevalTasks;

static void _OperateOnNodeAndData(TreeNode* _node, TrevalTasks* _tasks);

/*< (Left, Root, Right) >*/
static void _InorderTreval(TreeNode* _root, TrevalTasks* _tasks);

/*< (Root, Left, Right)  >*/
static void _PreorderTreval(TreeNode* _root, TrevalTasks* _tasks);

/*< (Left, Right, Root) >*/
static void _PostorderTreval(TreeNode* _root, TrevalTasks* _tasks);


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

 
void BTreeDestroy(BTree** _pTree, void (*_elementDestroy)(void* _item)) {
    TrevalTasks tasks;
    tasks.mTaskOnItem = _elementDestroy;
    tasks.mTaskOnNode = _FreeNode;
    if (NULL == _pTree || NULL == *_pTree) {
        return;
    }

    if (NULL != (*_pTree)->mSentinel.mRoot) {
        _PostorderTreval((*_pTree)->mSentinel.mRoot, &tasks);
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

ssize_t BTreeGetNumberOfItems(BTree* _tree) {
    if (NULL == _tree) {
        return -1;
    }

    return _tree->mNumOfItems; 
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

static void _OperateOnNodeAndData(TreeNode* _node, TrevalTasks* _tasks) {
    if (NULL == _node || NULL == _tasks) {
        return;
    }

    if (NULL != _tasks->mTaskOnItem) {
        _tasks->mTaskOnItem(_node->mData);
    }

    if (NULL != _tasks->mTaskOnNode) {
        _tasks->mTaskOnNode(_node);
    }
}

/*< (Left, Root, Right) >*/
static void _InorderTreval(TreeNode* _root, TrevalTasks* _tasks) {
    if (NULL == _root->mLeftNode && NULL == _root->mRightNode) {
        _OperateOnNodeAndData(_root, _tasks);
        return;
    }

    if (NULL != _root->mLeftNode) {
        _InorderTreval(_root->mLeftNode, _tasks);
    }

    _OperateOnNodeAndData(_root, _tasks);

    if (NULL != _root->mRightNode) {
        _InorderTreval(_root->mRightNode, _tasks);
    }
}

/*< (Left, Right, Root) >*/
static void _PostorderTreval(TreeNode* _root, TrevalTasks* _tasks) {
    if (NULL == _root->mLeftNode && NULL == _root->mRightNode) {
        _OperateOnNodeAndData(_root, _tasks);
        return;
    }

    if (NULL != _root->mLeftNode) {
        _PostorderTreval(_root->mLeftNode, _tasks);
    }

    if (NULL != _root->mRightNode) {
        _PostorderTreval(_root->mRightNode, _tasks);
    }
    _OperateOnNodeAndData(_root, _tasks);
}


static void _PreorderTreval(TreeNode* _root, TrevalTasks* _tasks) {
        if (NULL == _root->mLeftNode && NULL == _root->mRightNode) {
        _OperateOnNodeAndData(_root, _tasks);
        return;
    }

    _OperateOnNodeAndData(_root, _tasks);
    if (NULL != _root->mLeftNode) {
        _PreorderTreval(_root->mLeftNode, _tasks);
    }

    if (NULL != _root->mRightNode) {
        _PreorderTreval(_root->mRightNode, _tasks);
    }
}