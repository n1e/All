#include <iostream>
using namespace std;
#define MAX 1024
//#define max(num1, num2)\
//    num1 >= num2 ? num1 : num2

int max(int num1, int num2)
{
    return num1 >= num2 ? num1 : num2;
}

void find_path(int **res, int *w, int weight, int num)
{
    int i = num;
    int j = weight;
    if (res[i][j] == 0)
        return;
    else if (res[i][j] == res[i - 1][j])
        find_path(res, w, i - 1, j);
    else
    {
        cout << i <<",";
        find_path(res, w, i - 1, j - w[i]);
    }
    
}
int main()
{
    int weight = 0, num = 0;
    int w[MAX] = {0};
    int v[MAX] = {0};


    cout << "输入最大重量和物品件数:";
    cin >> weight >> num;

    int **res = new int*[weight];

    for (int i = 0 ; i < num ; i++)
    {
        cout << "输入第" << i << "件物品重量和价值: ";
        cin >> w[i] >> v[i];
    }

     cout << "输出重量和价值" << endl;
    for (int i = 0 ; i < num ; i++)
    {
        cout << w[i] << "," << v[i] << endl;
    }

    for (int i = 0 ; i < num ; i++)
    {
        for (int j = 0 ; j < weight ; j++ )
        {
            res[i][j] = (i == 0 || j== 0) ? 
                                            0 : ( w[i] > j ? 
                                                                res[i - 1][j] : max(res[i - 1][j], res[i - 1][j - w[i]] + v[i]));
            cout << "i : " << i << ", j : " << j << ", res[i][j] : " << res[i][j] << endl;
        }
    }

    cout << "输出价值列表" << endl;
    for (int i = 0 ; i < num ; i++)
    {
        for (int j = 0 ; j < weight ; j++ )
        {
            cout << res[i][j] << ",";
        }
        cout << endl;
    }
    
    //find_path(res, w, weight, num);
    delete[] res;
    return 0;
}    
