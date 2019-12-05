#ifndef __TREE_INTERNAL_H__
#define __TREE_INTERNAL_H__

#include "data_structure_defenitions.h" /*< ElementAction >*/
#include <stddef.h> /*< size_t >*/
#include <stdlib.h> /*< free >*/

typedef struct TreeNode TreeNode;

struct TreeNode {
    TreeNode* m_leftNode;
    TreeNode* m_rightNode;
    const void* m_key;
    void* m_data;
    size_t m_level;
};

typedef struct _Sentinel {
    TreeNode* m_root;
} Sentinel;

void _FreeNode(TreeNode* _node) {
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

typedef void (*NodeTravelTask)(TreeNode* _node);
typedef void (*ActionOnKey)(void* _key);

typedef struct _TravelTasks {
    void* m_context;
    ElementAction m_taskOnItem;
    ActionOnKey m_taskOnKey;
    NodeTravelTask m_taskOnNode;
} TravelTasks;

void _OperateOnNodeAndData(TreeNode* _node, TravelTasks* _tasks) {
    if (NULL == _node || NULL == _tasks) {
        return;
    }

    if (NULL != _tasks->m_taskOnItem) {
        _tasks->m_taskOnItem(_node->m_data, _tasks->m_context);
    }

    if (NULL != _tasks->m_taskOnKey) {
        _tasks->m_taskOnKey((void*)_node->m_key);
    }
    
    if (NULL != _tasks->m_taskOnNode) {
        _tasks->m_taskOnNode(_node);
    }
}

/*< (Left, Root, Right) >*/
void _InorderTravel(TreeNode* _root, TravelTasks* _tasks) {
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
void _PostorderTravel(TreeNode* _root, TravelTasks* _tasks) {
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


void _PreorderTravel(TreeNode* _root, TravelTasks* _tasks) {
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
#endif /* __TREE_INTERNAL_H__ */