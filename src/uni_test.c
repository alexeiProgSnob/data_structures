#include "uni_test.h"
#include "sorts.h"
#include "vector.h"
#include "heap.h"
#include "hash.h"
#include "list.h"
#include "circular_queue.h"
#include "circular_safe_queue.h"
#include "stack.h"
#include "binary_tree.h"
#include <stdio.h>

Compare_Result CompareSizeTPointers(const void* _generalTypeA, const void* _generalTypeB) {
    size_t** typeA = (size_t**) _generalTypeA;
    size_t** typeB = (size_t**) _generalTypeB;
    if (**typeA > **typeB) {
        return BIGGER;
    }

    return SMALLER;
}


void SwapToSizeTPointers(void* _generalTypeA, void* _generalTypeB) {
    size_t** typeA = (size_t**) _generalTypeA;
    size_t** typeB = (size_t**) _generalTypeB;
    size_t temp = **typeA;
    **typeA = **typeB;
    **typeB = temp; 
}

int CompareTwoArraysP(size_t** arrA, size_t* arrB, size_t sizeOfArrays) {
    size_t i = 0;
    for (; i < sizeOfArrays ; ++i) {
        if (*(arrA[i]) != arrB[i]) {
            return -1;
        }
    }

    return 0;
}

int CompareTwoArrays(size_t* arrA, size_t* arrB, size_t sizeOfArrays) {
    size_t i = 0;
    for (; i < sizeOfArrays ; ++i) {
        if (arrA[i] != arrB[i]) {
            return -1;
        }
    }

    return 0;
}

Compare_Result CompareSizeT(const void* _generalTypeA, const void* _generalTypeB) {
    size_t* typeA = (size_t*) _generalTypeA;
    size_t* typeB = (size_t*) _generalTypeB;
    if (*typeA > *typeB) {
        return BIGGER;
    }

    if (*typeA == *typeB) {
        return EQUAL;
    }

    return SMALLER;
}


Compare_Result CompareSizeTHeap(const void* _generalTypeA, const void* _generalTypeB) {
    const size_t* typeA = (const size_t*) _generalTypeA;
    const size_t* typeB = (const size_t*) _generalTypeB;
    if (*typeA > *typeB) {
        return BIGGER;
    }

    return SMALLER;
}

void SwapToSizeT(void* _generalTypeA, void* _generalTypeB) {
    size_t* typeA = (size_t*) _generalTypeA;
    size_t* typeB = (size_t*) _generalTypeB;
    size_t temp = *typeA;
    *typeA = *typeB;
    *typeB = temp; 
}


Compare_Result Int_Heap(const void* _a, const void* _b) {
    const int* a = (const int*)_a;
    const int* b = (const int*)_b;
    if (a > b) {
        return BIGGER;
    }

    return SMALLER;
}

UNIT(basic_valid_size_t_pointer_bubble_sort_test)
    size_t one = 1;
    size_t two = 2;
    size_t three = 3;
    size_t four = 4;
    size_t arrFinalResult[4] = {1,2,3,4};
    size_t arrSize = 4;
    size_t* arr[4];
    arr[0] = &two;
    arr[1] = &one;
    arr[2] = &four;
    arr[3] = &three;
    ASSERT_THAT(SORT_SUCCESS == BubbleSortOnData((void*)&arr, sizeof(size_t*), sizeof(arr), CompareSizeTPointers, SwapToSizeTPointers));
    ASSERT_THAT(0 == CompareTwoArraysP(arr, arrFinalResult, arrSize));
END_UNIT

UNIT(basic_valid_size_t_bubble_sort_test)
    size_t arrFinalResult[4] = {1,2,3,4};
    size_t arr[4] = {2,1,4,3};
    size_t arrSize = 4;
    ASSERT_THAT(SORT_SUCCESS == BubbleSortOnData((void*)arr, sizeof(size_t*), sizeof(arr), CompareSizeT, SwapToSizeT));
    ASSERT_THAT(0 == CompareTwoArrays(arr, arrFinalResult, arrSize));
END_UNIT

UNIT(Allocate_Vector)
    Vector* newVector = VectorCreate(10, 5);
    ASSERT_THAT(NULL != newVector);
    VectorDestroy(&newVector, NULL);
    ASSERT_THAT(NULL == newVector); 
END_UNIT

UNIT(Append_To_Vector_Elements_Expect_No_Crash)
    size_t arr[] = {2,1,4,3};
    size_t i = 0;
    Vector* newVector = VectorCreate(10, 5);
    aps_ds_error resultChecker = DS_UNINITIALIZED_ERROR;
    ASSERT_THAT(NULL != newVector);
    ASSERT_THAT(VectorCapacity(newVector) == 10);
    for (i = 0; i < sizeof(arr)/sizeof(size_t) ; ++i) {
        resultChecker = VectorAppend(newVector, arr + i);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
    }

    ASSERT_THAT(VectorSize(newVector) == sizeof(arr)/sizeof(size_t));
    VectorDestroy(&newVector, NULL);
    ASSERT_THAT(NULL == newVector); 
END_UNIT

UNIT(Append_To_Vector_Elements_Expect_No_Crash_And_Then_Get_All)
    size_t arr[] = {2,1,4,3};
    size_t i = 0;
    size_t numOfElements = sizeof(arr) / sizeof(size_t);
    size_t* value = NULL;
    Vector* newVector = VectorCreate(10, 5);
    aps_ds_error resultChecker = DS_UNINITIALIZED_ERROR;
    ASSERT_THAT(NULL != newVector);
    for (i = 0; i < numOfElements; ++i) {
        resultChecker = VectorAppend(newVector, arr + i);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
    }

    for (i = 0; i < numOfElements; ++i) {
        resultChecker = VectorGet(newVector, i, (void**)&value);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
        ASSERT_THAT(*value == arr[i]);
    }

    for (i = numOfElements; i > 0; --i) {
        resultChecker = VectorRemove(newVector, (void**)&value);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
        ASSERT_THAT(*value == arr[i - 1]);
    }
    
    VectorDestroy(&newVector, NULL);
    ASSERT_THAT(NULL == newVector);
END_UNIT

UNIT(Vector_RemoveFrom)
    size_t arr[] = {2,1,4,3};
    size_t i = 0;
    size_t numOfElements = sizeof(arr) / sizeof(size_t);
    size_t* value = NULL;
    Vector* newVector = VectorCreate(10, 5);
    aps_ds_error resultChecker = DS_UNINITIALIZED_ERROR;
    ASSERT_THAT(NULL != newVector);
    for (i = 0; i < numOfElements; ++i) {
        resultChecker = VectorAppend(newVector, arr + i);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
    }

    resultChecker = VectorRemoveFrom(newVector, 1, (void**)&value);
    ASSERT_THAT(DS_SUCCESS == resultChecker);
    ASSERT_THAT(*value == arr[1]);
    
    resultChecker = VectorRemoveFrom(newVector, 1, (void**)&value);
    ASSERT_THAT(DS_SUCCESS == resultChecker);
    ASSERT_THAT(*value == arr[2]);

    resultChecker = VectorRemoveFrom(newVector, 1, (void**)&value);
    ASSERT_THAT(DS_SUCCESS == resultChecker);
    ASSERT_THAT(*value == arr[3]);

    resultChecker = VectorRemoveFrom(newVector, 1, (void**)&value);
    ASSERT_THAT(DS_OUT_OF_BOUNDS_ERROR == resultChecker);

    resultChecker = VectorRemoveFrom(newVector, 0, (void**)&value);
    ASSERT_THAT(DS_SUCCESS == resultChecker);
    ASSERT_THAT(*value == arr[0]);

    VectorDestroy(&newVector, NULL);
    ASSERT_THAT(NULL == newVector);
END_UNIT

UNIT(Allocate_Heap)
    Heap* newHeap = HeapCreate(10, HEAP_TYPE_MIN, Int_Heap);
    ASSERT_THAT(NULL != newHeap);
    HeapDestroy(&newHeap, NULL);
    ASSERT_THAT(NULL == newHeap); 
END_UNIT

UNIT(Append_To_Heap_Min_Elements_Expect_No_Crash)
    size_t arr[] = {2,1,4,3};
    size_t i = 0;
    Heap* newData = HeapCreate(10, HEAP_TYPE_MIN, CompareSizeTHeap);
    aps_ds_error resultChecker = DS_SUCCESS;
    const size_t* data = NULL;
    ASSERT_THAT(NULL != newData);
    ASSERT_THAT(0 == HeapSize(newData));
    for (i = 0; i < sizeof(arr)/sizeof(size_t) ; ++i) {
        resultChecker = HeapPush(newData, arr + i);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
    }
    ASSERT_THAT(sizeof(arr)/sizeof(size_t) == HeapSize(newData));
    data = (const size_t*)HeapGetTopValue(newData);
    ASSERT_THAT(NULL != data);
    ASSERT_THAT(1 == *data);
    HeapDestroy(&newData, NULL);
    ASSERT_THAT(NULL == newData); 
END_UNIT

UNIT(Append_To_Heap_Max_Elements_Expect_No_Crash)
    size_t arr[] = {2,1,4,3};
    size_t i = 0;
    Heap* newData = HeapCreate(10, HEAP_TYPE_MAX, CompareSizeTHeap);
    aps_ds_error resultChecker = DS_SUCCESS;
    const size_t* data = NULL;
    ASSERT_THAT(NULL != newData);
    ASSERT_THAT(0 == HeapSize(newData));
    for (i = 0; i < sizeof(arr)/sizeof(size_t) ; ++i) {
        resultChecker = HeapPush(newData, arr + i);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
    }
    ASSERT_THAT(sizeof(arr)/sizeof(size_t) == HeapSize(newData));
    data = (const size_t*)HeapGetTopValue(newData);
    ASSERT_THAT(NULL != data);
    ASSERT_THAT(4 == *data);
    HeapDestroy(&newData, NULL);
    ASSERT_THAT(NULL == newData); 
END_UNIT

UNIT(Append_To_Heap_Min_Elements_And_Pop)
    size_t arr[] = {2,1,4,3};
    size_t arrRes[] = {1,2,3,4};
    size_t i = 0;
    Heap* newData = HeapCreate(10, HEAP_TYPE_MIN, CompareSizeTHeap);
    aps_ds_error resultChecker = DS_SUCCESS;
    size_t* data = NULL;
    ASSERT_THAT(NULL != newData);
    ASSERT_THAT(0 == HeapSize(newData));
    for (i = 0; i < sizeof(arr)/sizeof(size_t) ; ++i) {
        resultChecker = HeapPush(newData, arr + i);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
    }
    ASSERT_THAT(sizeof(arr)/sizeof(size_t) == HeapSize(newData));
    for (i = 0; i < sizeof(arr)/sizeof(size_t) ; ++i) {
        resultChecker = HeapPop(newData, (void**)&data);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
        ASSERT_THAT(NULL != data);
        ASSERT_THAT(arrRes[i] == *data);
        data = NULL;
    }
    HeapDestroy(&newData, NULL);
    ASSERT_THAT(NULL == newData); 
END_UNIT

UNIT(Append_To_Heap_Max_Elements_And_Pop)
    size_t arr[] = {2,1,4,3};
    size_t arrRes[] = {4,3,2,1};
    size_t i = 0;
    Heap* newData = HeapCreate(10, HEAP_TYPE_MAX, CompareSizeTHeap);
    aps_ds_error resultChecker = DS_SUCCESS;
    size_t* data = NULL;
    ASSERT_THAT(NULL != newData);
    ASSERT_THAT(0 == HeapSize(newData));
    for (i = 0; i < sizeof(arr)/sizeof(size_t) ; ++i) {
        resultChecker = HeapPush(newData, arr + i);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
    }
    ASSERT_THAT(sizeof(arr)/sizeof(size_t) == HeapSize(newData));
    for (i = 0; i < sizeof(arr)/sizeof(size_t) ; ++i) {
        resultChecker = HeapPop(newData, (void**)&data);
        ASSERT_THAT(DS_SUCCESS == resultChecker);
        ASSERT_THAT(NULL != data);
        ASSERT_THAT(arrRes[i] == *data);
        data = NULL;
    }
    HeapDestroy(&newData, NULL);
    ASSERT_THAT(NULL == newData); 
END_UNIT

UNIT(Allocate_Queue)
    CQueue* newDataS = CQueueCreate(10);
    ASSERT_THAT(NULL != newDataS);
    CQueueDestroy(&newDataS, NULL);
    ASSERT_THAT(NULL == newDataS); 
END_UNIT

UNIT(Allocate_SafeQueue)
    CSQueue* newDataS = CSQueueCreate(10);
    ASSERT_THAT(NULL != newDataS);
    CSQueueDestroy(&newDataS, NULL);
    ASSERT_THAT(NULL == newDataS); 
END_UNIT


UNIT(Allocate_List)
    List* newDataS = ListCreate();
    ASSERT_THAT(NULL != newDataS);
    ListDestroy(&newDataS, NULL);
    ASSERT_THAT(NULL == newDataS); 
END_UNIT

UNIT(Allocate_Stack)
    Stack* newDataS = StackCreate();
    ASSERT_THAT(NULL != newDataS);
    StackDestroy(&newDataS, NULL);
    ASSERT_THAT(NULL == newDataS); 
END_UNIT

UNIT(Allocate_BTree)
    BTree* newDataS = BTreeCreate(CompareSizeTPointers);
    ASSERT_THAT(NULL != newDataS);
    BTreeDestroy(&newDataS, NULL, NULL);
    ASSERT_THAT(NULL == newDataS); 
END_UNIT

/*
Compare_Result CompareSizeT(const void* _generalTypeA, const void* _generalTypeB)
*/
typedef struct ArrIndexSize {
    size_t* m_arr;
    size_t m_indx;
    size_t m_size;
} ArrIndexSize;

void PrintSizeTAndAddToArrayForValidate(void* _item, void* _context) {
    ArrIndexSize* dataToFill = (ArrIndexSize*)_context;
    printf("%lu ", *((size_t*)(_item)));
    if (dataToFill->m_indx < dataToFill->m_size) {
        (dataToFill->m_arr)[dataToFill->m_indx] = *((size_t*)(_item));
        ++dataToFill->m_indx;
    }
}

UNIT(BTree_Valid_Unit_Test)
    size_t i = 0;
    size_t arr[] = {2,1,4,3};
    size_t inOrder[4] = {0};
    size_t inOrderRes[] = {1,2,3,4};
    size_t postOrder[4] = {0};
    size_t postOrderRes[] = {1,3,4,2};
    size_t preOrder[4] = {0};
    size_t preOrderRes[] = {2,1,4,3};
    size_t invalidElement = 5;
    size_t* element = NULL;
    ArrIndexSize context;
    aps_ds_error resultChecker = DS_SUCCESS;
    BTree* newDataS = BTreeCreate(CompareSizeT);
    ASSERT_THAT(NULL != newDataS);
    for (i = 0 ; i < 4; ++i) {
        resultChecker = BTreeInsert(newDataS, arr + i,arr + i);
        ASSERT_THAT(resultChecker == DS_SUCCESS);
    }

    ASSERT_THAT(BTreeGetNumberOfItems(newDataS) == 4);

    element = BTreeGetMin(newDataS);
    ASSERT_THAT_WITH_MESSAGE("Validate BTreeGetMin", *element == 1);
 
    element = BTreeGetMax(newDataS);
    ASSERT_THAT_WITH_MESSAGE("Validate BTreeGetMax", *element == 4);

    context.m_arr = inOrder;
    context.m_indx = 0;
    context.m_size = 4;
    resultChecker = BTreeForEach(newDataS, IN_ORDER, PrintSizeTAndAddToArrayForValidate, &context);
    ASSERT_THAT_WITH_MESSAGE("BTreeForEach expected to succeed", resultChecker == DS_SUCCESS);
    printf("\n");
    ASSERT_THAT_WITH_MESSAGE("Validate IN_ORDER BTreeForEach", CompareTwoArrays(inOrder, inOrderRes, 4) == 0);

    context.m_arr = postOrder;
    context.m_indx = 0;
    context.m_size = 4;
    resultChecker = BTreeForEach(newDataS, POST_ORDER, PrintSizeTAndAddToArrayForValidate, &context);
    printf("\n");
    ASSERT_THAT_WITH_MESSAGE("BTreeForEach expected to succeed", resultChecker == DS_SUCCESS);
    ASSERT_THAT_WITH_MESSAGE("Validate POST_ORDER BTreeForEach", CompareTwoArrays(postOrder, postOrderRes, 4) == 0);

    context.m_arr = preOrder;
    context.m_indx = 0;
    context.m_size = 4;
    resultChecker = BTreeForEach(newDataS, PRE_ORDER, PrintSizeTAndAddToArrayForValidate, &context);
    printf("\n");
    ASSERT_THAT_WITH_MESSAGE("BTreeForEach expected to succeed", resultChecker == DS_SUCCESS);
    ASSERT_THAT_WITH_MESSAGE("Validate PRE_ORDER BTreeForEach", CompareTwoArrays(preOrder, preOrderRes, 4) == 0);

    for (i = 0 ; i < 4; ++i) {
        resultChecker = BTreeGetItem(newDataS, arr + i, (void**)&element);
        ASSERT_THAT_WITH_MESSAGE("BTreeGetItem expected to succeed", resultChecker == DS_SUCCESS);
        ASSERT_THAT_WITH_MESSAGE("BTreeGetItem element received not as expected", arr[i] == *element);
    }

    resultChecker = BTreeGetItem(newDataS, &invalidElement, (void**)&element);
    ASSERT_THAT_WITH_MESSAGE("BTreeGetItem expected to succeed", resultChecker == DS_ELEMENT_NOT_FOUND_ERROR);

    invalidElement = 2;
    resultChecker = BTreeInsert(newDataS, &invalidElement, &invalidElement);
    ASSERT_THAT_WITH_MESSAGE("BTreeInsert expected to fail to insert", resultChecker == DS_KEY_EXISTS_ERROR);

    printf("-----------remove-----------\n");
    for (i = 0 ; i < 4; ++i) {
        resultChecker = BTreeRemove(newDataS, arr + i, (void**)&element);
        ASSERT_THAT_WITH_MESSAGE("BTreeRemove expected to succeed", resultChecker == DS_SUCCESS);
        ASSERT_THAT_WITH_MESSAGE("BTreeRemove element received not as expected", arr[i] == *element);

        context.m_arr = inOrder;
        context.m_indx = 0;
        context.m_size = 4;
        BTreeForEach(newDataS, IN_ORDER, PrintSizeTAndAddToArrayForValidate, &context);
        printf("\n");
    }

    BTreeDestroy(&newDataS, NULL, NULL);
    ASSERT_THAT(NULL == newDataS); 
END_UNIT


TEST_SUITE(Test DataStructures)
    /* bubble Sort Tests */
	TEST(basic_valid_size_t_pointer_bubble_sort_test)
    TEST(basic_valid_size_t_bubble_sort_test)

    /* Vector Tests */
    TEST(Allocate_Vector)
    TEST(Append_To_Vector_Elements_Expect_No_Crash)
    TEST(Append_To_Vector_Elements_Expect_No_Crash_And_Then_Get_All)
    TEST(Vector_RemoveFrom)

    /* Heap Tests */
    TEST(Allocate_Heap)
    TEST(Append_To_Heap_Min_Elements_Expect_No_Crash)
    TEST(Append_To_Heap_Max_Elements_Expect_No_Crash)
    TEST(Append_To_Heap_Min_Elements_And_Pop)
    TEST(Append_To_Heap_Max_Elements_And_Pop)
    
    /* Queue Tests */
    TEST(Allocate_Queue)

    /* SafeQueue Tests */
    TEST(Allocate_SafeQueue)

    /* List Tests */
    TEST(Allocate_List)

    /* Stack Tests */
    TEST(Allocate_Stack)

    /* Binary Tree Tests */
    TEST(Allocate_BTree)
    TEST(BTree_Valid_Unit_Test)
END_SUITE
