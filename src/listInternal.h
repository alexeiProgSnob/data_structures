#ifndef __LISTINTERNAL_H__
#define __LISTINTERNAL_H__


typedef struct Node
{
    void* m_item;       /* pointer to items */
    struct Node* m_next;/* Pointer to the next node */
    struct Node* m_prev;/* Pointer to the previous node */
    
}Node;

struct List
{
    struct Node m_head;/* The Head of the list */
    struct Node m_tail;/* The Tail of the list */
};

/** 
 * @brief  push the new NOde beofre +leftNode to the list
 * @params _leftNode : left node that in the list
 * @params _newNode  : new node to push to the list
 */
void PushNode(Node* _leftNode,Node* _newNode);

/** 
 * @brief  pop the Node  between the left and right nodes
 * @params _leftNode : left node that in the list
 * @params _rightNode  : right node that in the lsit
 */
void PopNode(Node* _leftNode,Node* _rightNode);

/** 
 * @brief  create new node for the list
 * @params _item : item to insert in the new node
 * @returns  : pointer to the new node
 * @ errors  : NULL if alloction for the node failed
 */
Node* NodeCreate(void* _item);

#endif /* __LISTHELP_H__ */