#include <iostream>
//#define _HEAP_SORT_HEAP_
void swap(int &num_1, int &num_2)
{
    int temp = num_1;
    num_1 = num_2;
    num_2 = temp;
}

void max_heapify(int *arr, int pos, int nLen)
{
    int child_left = pos*2 + 1;
    int child_right = child_left + 1;

    if (arr == NULL || child_left >= nLen) return;

    int bigger_child = child_right < nLen ? ( arr[child_left] > arr[child_right] ? child_left : child_right) : child_left;
    if (arr[bigger_child] > arr[pos])
    {
        swap(arr[bigger_child], arr[pos]);
        max_heapify(arr, bigger_child, nLen);
    }
}

void init_heap(int *arr, int nLen)
{
    int first_no_leaf = (nLen - 2) / 2;
    int i = first_no_leaf;
    while (i >= 0)
    {
        max_heapify(arr, i, nLen);
        i--;
    }
}

void sort(int *arr, int nLen)
{
    for (int i = nLen - 1; i > 0; i-- )
    {
        swap(arr[0], arr[i]);
#ifdef _HEAP_SORT_HEAP_
        std::cout << "in loop : ";
         for (int i = 0 ; i < nLen ; i++)
            std::cout << arr[i] << "->";
        std::cout << std::endl;
#endif
        max_heapify(arr, 0, i);
    }
}

void heap_sort(int *arr, int start, int end)
{
    int nLen = end - start + 1;
    
    if (nLen > 1)
    {
        init_heap(arr, nLen);
#ifdef _HEAP_SORT_HEAP_
        for (int i = 0 ; i < nLen ; i++)
            std::cout << arr[i] << "->";
        std::cout << std::endl;
#endif
        sort(arr, nLen);
    }
}

int main()
{
    int arr[] = {5,-133,0,2,3,1,5467,64564,7,5,4,10,34,100};
    heap_sort(arr, 0, sizeof(arr)/sizeof(int) - 1);

    for (int i = 0 ; i < sizeof(arr)/sizeof(int) ; i++)
        std::cout << arr[i] << "->";
}