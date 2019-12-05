#ifndef __AVL_H__
#define __AVL_H__

/** 
 *  @file avl.h
 *  @brief
 *	
 *  @details  
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @update Alexei Radashkovsky (alexeirada@gmail.com)
 *  @bug No known bugs.
 */

#include "data_structure_defenitions.h"

typedef struct AVL AVL;

AVL* AVLCreate(CompareFunc _compareFunc);

void AVLDestroy(AVL** _tree, ElementDestroy _elementDestroy, ElementDestroy _keyDestroy);

aps_ds_error AVLInsert(AVL* _tree, const void* _key, void* _data);

aps_ds_error AVLRemove(AVL* _tree, const void* _key, void** _pData);

aps_ds_error AVLForEach(const AVL* _tree, TravelType _travelType, ElementAction _action, void* _context);

#endif /* __AVL_H__ */