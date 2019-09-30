#ifndef _QUICK_SORT_
#define _QUICK_SORT_
#include <iostream>

void swap(int &num_1, int &num_2)
{
    int temp = num_1;
    num_1 = num_2;
    num_2 = temp;
}
void quick_sort(int *arr, int nLeft, int nRight)
{
    int guard = arr[nRight];
    int left = nLeft, right = nRight;

    if (left >= right) return; //是大于等于 递归的时候哨兵正好是最大值或最小值的时候下次递归的 left比right大或 right比left小

    //std::cout << "loop : "<< nLeft << "," << nRight << std::endl;

    while (left < right)
    {
        while (left < right && arr[left] <= guard) left++;
        arr[right] = arr[left];

        while (left < right && arr[right] >= guard) right--;
        arr[left] = arr[right];
    }

    // for (int i = 0 ; i < 10 ; i++)
        //std::cout << arr[i] << "->";

    //std::cout << std::endl;

    arr[left] = guard;

    quick_sort(arr, nLeft, left - 1);
    quick_sort(arr, left + 1, nRight);
}

#endif