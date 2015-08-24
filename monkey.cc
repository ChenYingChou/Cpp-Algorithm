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

   參考: http://www.pep.com.cn/czsx/jszx/czsxtbjxzy/czsxdzkb_1_1_1/czsxdzkb7s_1_1_1_1_1/201203/t20120309_1106196.htm

   (此例為5隻猴子)

   解法一：设这一堆桃子至少有x个，由于每次平均分成五堆后都多一个，因此借给
    它们4个，于是连同这4个桃子，一共有(x+4)个桃子．

    假定这五子猴子分别拿走了(包括它们各自所吃掉的1个)a、b、c、d、e个桃子. 于是
        a = 4^0(x+4)/5^1 =   1(x+4)/5
        b = 4^1(x+4)/5^2 =   4(x+4)/25
        c = 4^2(x+4)/5^3 =  16(x+4)/125
        d = 4^3(x+4)/5^4 =  64(x+4)/625
        e = 4^4(x+4)/5^5 = 256(x+4)/3125
    而e为整数(a~d亦同)，且256与3125互质，因此x+4应是3125的倍数，于是x+4=3125k，
    其中k为自然数．

    显然，当 k=1 时，x=3121．即这五只猴子至少摘了 3121 个桃子．

   解法二：设第五只猴子拿走了x只桃子,那么
        第5只猴子取前的桃子是            (5x+1)
        第4只猴子取前的桃子是        [5/4(5x+1)+1]
        第3只猴子取前有        {5/4[5/4(5x+1)+1]+1}
        第2只猴子取前有     5/4{5/4[5/4(5x+1)+1]+1}+1
        第1只猴子取前有 5/4{5/4{5/4[5/4(5x+1)+1]+1}+1}+1 = 12x+8+53(x+1)/256

    设 x+1=256k，则 x=256k－1,于是这堆桃子一共有 12(256k-1)+8+53k=3125k-4．

    显然，当 k=1 时，桃子数最少，因此，这五只猴子至少摘了 3121 个桃子．

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

static bool monkey(int n, unsigned int peachs)
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

    // 使用窮舉法(暴力法)
    for (unsigned int i = n+1; i < max_num; i++) {
        if (debug) cout << "monkey:" << n << ", peachs:" << i << endl;
        if (monkey(n, i)) return i;
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
