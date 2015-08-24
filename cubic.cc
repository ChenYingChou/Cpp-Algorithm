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

    用質因數分解, 設法找出 (W,H,D), 使得 (W,H,D) 這三者儘量接近, 即可得到最小面積:
    例如: 100 = 2 x 2 x 5 x 5
        => (1,1,100) (1,2,50), (1,4,25), (1,10,10), (2,2,25), (4,5,5)
        => 取三者最接近者: (4,5,5)
        => 面積: 2 x (4x5 + 4x5 + 5x5) = 2 x 65 = 130

      900 = 2 x 2 x 3 x 3 x 5 x 5 => (3x3,2x5,2x5) => (9,10,10)

    但要從質因子裡組合取出乘積之和最小者也是有點複雜, 反倒是直接以暴力法來得簡單明瞭。

 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

const unsigned int MAX_NUM = 1000;

static int debug;

//---------------------------------------------------------------------------

static int find_min_area(int n)
{
    int min_area = INT_MAX;
    for (int w = pow(n, 1.0/3)+1; w > 0; w--) {
        if (n % w != 0) continue;
        for (int h = sqrt(n/w)+1; h >= w; h--) {
            int side = w * h;
            if (n % side != 0) continue;
            int d = n / side;
            int area = side + h*d + d*w;
            if (area < min_area) {
                min_area = area;
                if (debug) {
                    cout << ">>> (" << w
                        << ", " << h
                        << ", " << d
                        << ") = " << 2*area
                        << endl;
                }

            }
        }
    }

    return 2 * min_area;
}

//---------------------------------------------------------------------------

static void run(unsigned int max_num)
{
    unsigned int num;
    cin >> num;
    while (cin >> num) {
        int area = find_min_area(num);
        if (debug) cout << num << " --> ";
        cout << area << endl;
    }
}

int main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    unsigned int max_num = MAX_NUM;
    {
        int n = 1;
        while (n < argc && argv[n][0] == '-') {
            if (strncmp(argv[n], "-d", 2) == 0) {
                debug += strlen(&argv[n][1]);
            } else if (strncmp(argv[n], "-m", 2) == 0) {
                char *p = &argv[n][2];
                while (*p && !isdigit(*p)) p++;
                if (isdigit(*p)) {
                    max_num = atoi(p);
                    if (max_num < 10) max_num = MAX_NUM;
                }
            } else if (strncmp(argv[n], "-i:", 3) == 0) {
                ifstream in(&argv[n][3]);
                //streambuf *cinbuf = cin.rdbuf(); //save old buf
                cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
                run(max_num);
                //cin.rdbuf(cinbuf);
                return 0;
            }
            n++;
        }
    }

    run(max_num);

    return 0;
}
