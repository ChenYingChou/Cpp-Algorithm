/* joseph.cc -- tonychen [@] finenet.com.tw

   描述
    有n只猴子，按顺时针方向围成一圈选大王（编号从1到n），从第1号开始报数，一直
    数到m，数到m的猴子退出圈外，剩下的猴子再接着从1开始报数。就这样，直到圈内
    只剩下一只猴子时，这个猴子就是猴王，编程求输入n，m后，输出最后猴王的编号。

   输入
    输入包含两个整数，第一个是n，第二个是m (0 < m,n <=300)。

   输出
    输出包含一行，即最后猴王的编号。

   样例输入
    12 4

   样例输出
    1

 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<int> vint;

const unsigned int MAX_NUM = 300;

static int debug;

//---------------------------------------------------------------------------

/*
// 編號從0開始，如果編號從1開始結果要加1
static int josephus(int n, int k) { //非遞回版本
    int s = 0;
    for (int i = 2; i <= n; i++)
        s = (s + k) % i;
    return s + 1;
}

static int josephus_recursion(int n, int k) { //遞回版本
    return (n > 1 ? (josephus_recursion(n - 1, k) + k) % n : 0) + 1;
}
*/

static int loop_remove(int n, int m)
{
    if (n <= 1) return 1;

    vint next(n);
    for (int i = 1; i < n; i++) {
        next[i-1] = i;
    }

    int prev = n-1;
    int curr = next[prev] = 0;
    for (; n > 1; n--) {
        for (int j = 1, k = (m-1) % n; j <= k; j++) {
            curr = next[prev=curr];
            assert(curr >= 0);
        }

        // remove curr
        if (debug) cout << "Remove #" << (curr+1) << endl;
        int last = curr;
        next[prev] = curr = next[curr];
        next[last] = -1;
    }

    return curr+1;  // return 1-base
}

//---------------------------------------------------------------------------

static void run(unsigned int max_num)
{
    int n, m;
    while (cin >> n >> m) {
        assert(n > 0 && n <= max_num && m > 0 && m <= max_num);
        int answer = loop_remove(n, m);
        if (debug) cout << '(' << n << ',' << m << ") --> ";
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
