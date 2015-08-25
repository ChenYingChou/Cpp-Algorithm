/* cubic.cc

   描述
    Minecraft是一个几乎无所不能的沙盒游戏，玩家可以利用游戏内的各种资源进行
    创造，搭建自己的世界。

    在Minecraft中，基本的建筑元素是边长为1个单位的立方体，Tony想用N个这种小
    立方体搭建一个长方体，并用他珍藏已久的贴纸对其进行装饰。如果一张贴纸可以
    贴满小立方体的一个面。那么，他需要用掉多少张贴纸呢？

   输入
    输入的第一行是测试样例的数目C。剩下的C行，每行有一个整数N，表示小明所拥
    有的小立方体的个数。N不会超过1000。

   输出
    你需要输出每个测试样例中，小明最少用掉的贴纸有多少张。

   样例输入
    5
    9
    10
    26
    27
    100

   样例输出
    30
    34
    82
    54
    130

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    以因數分解, 設法找出(W,H,D), 使得(W,H,D)這三者儘量接近, 即可得到最小面積。
    例如: 100 = 2 x 2 x 5 x 5
        => (1,1,100) (1,2,50), (1,4,25), (1,10,10), (2,2,25), (4,5,5)
        => 取三者最接近者: (4,5,5)
        => 面積: 2 x (4x5 + 4x5 + 5x5) = 2 x 65 = 130

      900 = 2 x 2 x 3 x 3 x 5 x 5 => (3x3,2x5,2x5) => (9,10,10)

    要從質因子裡組合取出最小乘積之和也有點複雜, 以暴力法反而來得簡單明瞭。

    --> 立方體最小面積邊長為 N^(1/3), 只要找到第一個最近接 N^(1/3) 的因子組合
        即是答案, 不需要再往下逐一嘗試。

 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef unsigned long long ULL;

static int debug;

//---------------------------------------------------------------------------

static ULL find_min_area(ULL n)
{
    if (n == 0) return 0;

    for (ULL w = (ULL)(pow((double)n, 1.0/3)+0.5); w > 0; w--) {
        if (n % w != 0) continue;
        for (ULL h = (ULL)(sqrt((double)n/w)+0.5); h >= w; h--) {
            ULL side = w * h;
            if (n % side != 0) continue;
            ULL d = n / side;
            ULL area = 2 * (side + h*d + d*w);
            if (debug) {
                cout << ">>> (" << w
                    << ", " << h
                    << ", " << d
                    << ") = " << area
                    << endl;
            }
            return area;
        }
    }

    assert(0);
    return 0;   // 無解?
}

//---------------------------------------------------------------------------

static void run()
{
    ULL num;
    cin >> num;
    while (cin >> num) {
        ULL area = find_min_area(num);
        if (debug) cout << num << " --> ";
        cout << area << endl;
    }
}

int main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    {
        int n = 1;
        while (n < argc && argv[n][0] == '-') {
            if (strncmp(argv[n], "-d", 2) == 0) {
                debug += strlen(&argv[n][1]);
            } else if (strncmp(argv[n], "-i:", 3) == 0) {
                ifstream in(&argv[n][3]);
                //streambuf *cinbuf = cin.rdbuf(); //save old buf
                cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
                run();
                //cin.rdbuf(cinbuf);
                return 0;
            }
            n++;
        }
    }

    run();

    return 0;
}
