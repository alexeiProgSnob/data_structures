#include "binary_tree.h"
#include <stdlib.h> /*< malloc >*/


typedef struct _TreeNode {
    TreeNode* mLeftNode;
    TreeNode* mRightNode;
    void* mData;
    size_t mLevel;
} TreeNode;

typedef struct _Sentinel {
    TreeNode* mRoot;
} Sentinel;

struct BTree {
    Sentinel mSentinel;
    size_t mTreeHight;
    CompareFunc mCompareFunc;
};

static TreeNode* _CreateAndInitNewNode(void* _data);
static void _RecursiveInsert(BTree* _tree, TreeNode* _root, TreeNode* _newNode);

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