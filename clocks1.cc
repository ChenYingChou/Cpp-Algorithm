/********************************************************************************************************
2814:拨钟问题

http://poj.grids.cn/practice/2814

时间限制:
1000ms
内存限制:
65536kB

描述
有9个时钟，排成一个3*3的矩阵。

现在需要用最少的移动，将9个时钟的指针都拨到12点的位置。共允许有9种不同的移动。如右表所示，每个移动会将若干个时钟的指针沿顺时针方向拨动90度。

移动 影响的时钟
ABDE
ABC
BCEF
ADG
BDEFH
CFI
DEGH
GHI
EFHI
(图 2)
输入
从标准输入设备读入9个整数，表示各时钟指针的起始位置。0=12点、1=3点、2=6点、3=9点。
输出
输出一个最短的移动序列，使得9个时钟的指针都指向12点。按照移动的序号大小，输出结果。
样例输入
3 0 
2 2 
1 2 

样例输出
5 8 9 
********************************************************************************************************/

/********************************************************************************************************
解题思路：

假设时钟指针位置对应的值为clock_time，那么顺时针旋转90°就是clock_time = (clock_time+1)%4
这一组时针就用一个数组表示。9种操作对应一个二维数组。

这一题实质类似熄灯问题和画家问题。其共通点在于：
操作对环境的改变是无序的，每个操作都会影响到周围的状态。
同时每一种操作都有周期性限制，也即最多需要几次操作，多于这个次数产生循环。

熄灯问题中，每个灯最多熄灯一次，因为灯只有两种状态，
并且循环。而这里，有4种循环的状态，因此每个移动操作顶多使用3次。

我们对移动方法1,2,3进行枚举，每种方法无非实施0-3次，也即一共4^3=64种情况。
这些情况之间并非没有关系。

例如，我们确定了1,2,3的情况数，那么得到一个灯A,B,C的状态，而只有移动4能够改变A，
移动5能够改变B，移动6能够改变C，那么移动4-6的次数也确定了。

同样，这时只有移动7能够改变D，移动9能够改变F，这时移动7和9的次数也确定了。

最后，时钟A,B,C,D,F都已经到达12点，E,G,H,I还没确定，只剩下移动8能够改变GHI，
所以只要检查E是否已经到达12点以及，GHI的时钟数是否相等就行了。

最后找到一个移动次数最小的情况。

这题也可以用暴力搜索，因为最多有4^9个组合，不会超时。

这题还可以列出一个方程组，九个未知数，通过高斯消元法来解方程组。
********************************************************************************************************/

#include <iostream>
#include <cmath>
#include <cctype>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <list>
#include <stack>
#include <cstring>
//#include <stdlib.h>
//#include <iomanip>

using namespace std;

void operate(unsigned operations[10][10], unsigned clocks2[10], int op_num, int op_count)
{
    for (int i = 1; i <= 9; i++)
    {
        clocks2[i] += operations[op_num][i]*op_count;
        clocks2[i] %= 4;
    }
}

int main()
{
    //保存原始的时钟状态
    unsigned clocks[10] = {0};

    //保存最后最少的移动次数，最多也就是27次
    int min_res = 28, tmp = 0;
    //保存最小次数时候的移动方法
    unsigned min_op[10] = {0};

    //9种操作对应数组
    unsigned operations[10][10] = 
    {
        {0},
        // A  B  C D  E F  G  H  I
        {0, 1, 1, 0, 1, 1, 0, 0, 0, 0},  //op1: ABDE
        {0, 1, 1, 1, 0, 0, 0, 0, 0, 0},  //op2: ABC
        {0, 0, 1, 1, 0, 1, 1, 0, 0, 0},  //op3: BCEF
        {0, 1, 0, 0, 1, 0, 0, 1, 0, 0},  //op4: ADG
        {0, 0, 1, 0, 1, 1, 1, 0, 1, 0},  //op5: BDEFH
        {0, 0, 0, 1, 0, 0, 1, 0, 0, 1},  //op6: CFI
        {0, 0, 0, 0, 1, 1, 0, 1, 1, 0},  //op7: DEGH
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 1},  //op8: GHI
        {0, 0, 0, 0, 0, 1, 1, 0, 1, 1}  //op9: EFHI
    };

    //移动操作改变数据clocks2，而clocks备份源数据
    unsigned clocks2[10] = {0};

    //记录每种移动方法操作次数
    unsigned op[10] = {0};
    
    //保存枚举移动1,2,3的64种执行情况
    unsigned move123[64][3];

    unsigned num, divide_num;
    for (int i = 0; i < 64; i++)
    {
        num = i;
        
        //将10进制i转换为对应的4进制move123[i]
        for (int j = 2; j >= 0; j--)
        {
            divide_num = (unsigned)num/4;
            move123[i][j] = num - divide_num*4;
            num = divide_num;
        }
    }

    for (int i = 1; i <= 9; i++)
        cin >> clocks[i];

    //枚举移动1,2,3的64种执行次数，计算出移动4-9的移动次数，判断是否满足最后条件，记录总次数
    for (int i = 0; i < 64; i++)
    {
        memcpy(clocks2, clocks, sizeof(unsigned)*10);
        memset(op, 0, sizeof(unsigned)*10);

        for (int j = 1; j <=3; j++)
        {
            op[j] = move123[i][3-j];
            operate(operations, clocks2, j, op[j]);
        }

        //移动操作4,5,6的次数分别由时钟A,B,C的状态决定
        for (int j = 4; j <=6; j++)
        {
            op[j] = (4-clocks2[j-3])%4;
            operate(operations, clocks2, j, op[j]);
        }

        //移动操作7,9的次数分别由时钟D,F的状态决定
        op[7] = (4-clocks2[4])%4;
        operate(operations, clocks2, 7, op[7]);
        op[9] = (4-clocks2[6])%4;
        operate(operations, clocks2, 9, op[9]);

        //判断E是否为0，GHI是否相等
        if (clocks2[5] == 0 && clocks2[7] == clocks2[8] && clocks2[8] == clocks2[9])
        {
            op[8] = (4-clocks2[7])%4;
            tmp = 0;
            for (int j = 1; j <= 9; j++)
                tmp += op[j];
            if (tmp < min_res)
            {
                min_res = tmp;
                memcpy(min_op, op, sizeof(unsigned)*10);
            }
        }
    }

    //非零项排序
    vector<unsigned> res;
    for (int i = 1; i <= 9; i++)
    {
        if (min_op[i] != 0)
        {
            res.push_back(i);
            min_op[i]--;
            i--;
        }
    }

    sort(res.begin(), res.end());

    for (vector<unsigned>::iterator it = res.begin(); it != res.end(); it++)
            cout << *it << ' ';

    cout << "\n";

    return 0;
}