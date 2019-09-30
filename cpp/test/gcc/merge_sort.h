#ifndef _MERGE_SORT_
#define _MERGE_SORT_
#include <iostream>

void merge(int *arr, int left, int mid, int right)
{
    int *ptr = new int[right - left + 1];

    int j = 0;
    for (int i = left ; i <= right ; i++)
        ptr[j++] = arr[i];

    int pos_1 = 0;
    int pos_2 = mid - left + 1;

    j = 0;
    //{234, 345345, 22, 234234545, -1, 0, 123123, 234234,655,7};
    while ( j < right - left + 1 )
    {
        if(( pos_1 < mid - left + 1 && pos_2 < right - left + 1 && ptr[pos_1] <= ptr[pos_2] ) || (pos_2 > right - left && pos_1 < mid - left + 1 ))
        {
            arr[left + j++] = ptr[pos_1++];
        }

        if(( pos_1 < mid - left + 1 && pos_2 < right - left + 1 && ptr[pos_2] < ptr[pos_1] ) || (pos_1 > mid - left && pos_2 < right - left + 1))
        {
            arr[left + j++] = ptr[pos_2++];
        }
    }
}

void merge_sort(int *arr, int left, int right)
{
    if (left >= right) return;

    int mid = (left + right) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}
#endif