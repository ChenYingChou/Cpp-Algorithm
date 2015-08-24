/* monkey.cc

   描述
    海滩上有一堆桃子，N只猴子来分。第一只猴子把这堆桃子平均分为N份，多了一个，
    这只猴子把多的一个扔入海中，拿走了一份。第二只猴子接着把剩下的桃子平均分成
    N份，又多了一个，它同样把多的一个扔入海中，拿走了一份。第三、第四、……，
    第N只猴子仍是最终剩下的桃子分成N份，扔掉多了的一个，并拿走一份。

    编写程序，输入猴子的数量N，输出海滩上最少的桃子数，使得每只猴子都可吃到桃子。

   输入
    一组整数，最后一个整数是0，表示输入结束，其余整数均大于1。

   输出
    对输入的每个整数N，输出当猴子数量为N时海滩上最少的桃子数。所输出的每个整数
    占一行。结果保证在int型范围内。

   样例输入
    2 4 0

   样例输出
    7
    253

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    猴子拿走桃子數　　原有桃子數　　留下桃子數
    －－－－－－－　　－－－－－　　－－－－－
    　　　X1　　　　　N X1 + 1　↙　(N-1) X1
    　　　X2　　　　　N X2 + 1　↙　(N-1) X2
    　　　X3　　　　　N X3 + 1　↙　(N-1) X3
    　　 ...　　 　　 ...
    　　　Xn　　　　　N X3 + 1　　　(N-1) Xn

    　　　　　　　　　N X(i+1)+1 = (N-1) X(i)

    比值關係 N:(N-1) => N^N - N + 1

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    參考: http://ilovers.sinaapp.com/article/猴子分桃

    (此例為5隻猴子)

    假设第二个猴子拿了 x2 个桃子，第三个猴子拿了 x3 个，那么有这么个关系：
    4 x2 = 5 x3 + 1 ，这是类似于 4 a = 5 b + 1 的式子。毫无疑问的是 a 、b
    都是整数了。4 a = 5 b + 1 = 4 b + (b + 1)，那么可想而知 b + 1 = 4 k，
    于是有：

        a = 5 k - 1  b = 4 k - 1

    因为 (x1, x2)， (x2, x3)， (x3, x4)，(x4, x5) 均满足类似于 4 a = 5 b + 1
    的式子，也自然满足上面的 a, b 关系。假设对应的 k 分别是 k1  k2  k3  k4，
    根据 x2 = 4 k1 - 1 = 5 k2 - 1 ，可以得出 k1 : k2 = 5 : 4，所以会有 ：

        k1 : k2 = 5 : 4  k2 : k3 = 5 : 4  k3 : k4 = 5 : 4

    k1  k2  k3  k4 均是整数，所以不难找到最小的 k1 是 5×5×5，当然可以加任意
    倍数。那么得出来的桃子总数应该是
    z = 5 x1+1 = 5 (5 k1 - 1) + 1 = 3125 k - 4 ,(k ∈ N)

 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

const unsigned int MAX_NUM = 1 << 31;

static int debug;

//---------------------------------------------------------------------------

static bool money(int n, unsigned int peachs)
{
    for (int i = 0; i < n; i++) {
        peachs--;
        if (peachs <= 0 || peachs % n != 0) return false;
        peachs = peachs / n * (n-1);
    }
    return true;
}

static unsigned long calc(int n, unsigned int max_num)
{
    if (n > 7) {
        if (n > 16) return 0;
        // calculate: v = n^n
        unsigned long v = 1;
        unsigned long exp = n;
        unsigned int x = n;
        while (x > 0) {
            if (x & 1) v *= exp;
            x >>= 1;
            exp *= exp;
        }
        return v - n + 1;   // N^N - N + 1
    }

    for (unsigned int i = n+1; i < max_num; i++) {
        if (debug) cout << "monkey:" << n << ", peachs:" << i << endl;
        if (money(n, i)) return i;
    }
    return 0;
}

static void run(unsigned int max_num)
{
    int n;
    while ((cin >> n) && n > 0) {
        unsigned long answer = calc(n, max_num);
        if (debug) cout << n << " --> ";
        cout << answer << endl;
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
