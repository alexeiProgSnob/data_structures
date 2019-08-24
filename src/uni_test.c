#include "uni_test.h"
#include "sorts.h"
#include <stdio.h>

Compare_Result CompareSizeTPointers(void* _generalTypeA, void* _generalTypeB) {
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

Compare_Result CompareSizeT(void* _generalTypeA, void* _generalTypeB) {
    size_t* typeA = (size_t*) _generalTypeA;
    size_t* typeB = (size_t*) _generalTypeB;
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

UNIT(basic_valid_size_t_pointer_bubble_sort_test)
    size_t one = 1;
    size_t two = 2;
    size_t three = 3;
    size_t four = 4;
    size_t arrFinalResult[4] = {1,2,3,4};
    size_t arrSize = 4;
    BubbleSort* bs = CreateBubbleSort(CompareSizeTPointers, SwapToSizeTPointers);	
    size_t* arr[4];
    arr[0] = &two;
    arr[1] = &one;
    arr[2] = &four;
    arr[3] = &three;
    ASSERT_THAT(NULL != bs); 
    ASSERT_THAT(SORT_SUCCESS == BubbleSortOnData(bs, (void*)&arr, sizeof(size_t*), sizeof(arr)));
    ASSERT_THAT(0 == CompareTwoArraysP(arr, arrFinalResult, arrSize));
    DestroyBubbleSort(&bs);
    ASSERT_THAT(NULL == bs);
END_UNIT

UNIT(basic_valid_size_t_bubble_sort_test)
    size_t arrFinalResult[4] = {1,2,3,4};
    size_t arr[4] = {2,1,4,3};
    size_t arrSize = 4;
    BubbleSort* bs = CreateBubbleSort(CompareSizeT, SwapToSizeT);	
    ASSERT_THAT(NULL != bs); 
    ASSERT_THAT(SORT_SUCCESS == BubbleSortOnData(bs, (void*)arr, sizeof(size_t*), sizeof(arr)));
    ASSERT_THAT(0 == CompareTwoArrays(arr, arrFinalResult, arrSize));
    DestroyBubbleSort(&bs);
    ASSERT_THAT(NULL == bs);
END_UNIT


TEST_SUITE(Test DataSructures)
	TEST(basic_valid_size_t_pointer_bubble_sort_test)
    TEST(basic_valid_size_t_bubble_sort_test)
END_SUITE
