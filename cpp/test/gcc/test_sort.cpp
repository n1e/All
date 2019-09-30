#include "quik_sort.h"
#include "merge_sort.h"
#include <iostream>
using namespace std;
int main()
{
    int arr[] =  {234, 345345, 22, 234234545, -1, 0, 123123, 234234,655,7};
    quick_sort(arr, 0, sizeof(arr)/sizeof(int) - 1);


    std::cout << "quick sort:"<<std::endl;
    for (int i = 0 ; i < sizeof(arr)/sizeof(int) ; i++)
        std::cout << arr[i] << ", ";
    std::cout << std::endl;


    int arr2[] = {234, 345345, 22, 234234545, -1, 0, 123123, 234234,655,7};
    merge_sort(arr2, 0, sizeof(arr2)/sizeof(int) - 1);

    std::cout << "merge sort:"<<std::endl;
    for (int i = 0 ; i < sizeof(arr2)/sizeof(int) ; i++)
        std::cout << arr2[i] << ", ";
    std::cout << std::endl;
}