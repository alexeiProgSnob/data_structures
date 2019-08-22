#ifndef SORTS_H_
#define SORTS_H_

typedef enum Sort_Result {
    SORT_SUCCESS,
    SORT_UNINITIALIZED_ERROR
} Sort_Result;

typedef struct MergeSort MergeSort;

MergeSort* CreateMergeSort();
void MergeSortOnData(MergeSort* _mergeSortData, void** _dataToSort);

typedef struct BubbleSort BubbleSort;
BubbleSort* CreateBubbleSort();
void BubbleSortOnData(BubbleSort* _bubbleSortData, void** _dataToSort);
#endif /*< SORTS_H_ >*/