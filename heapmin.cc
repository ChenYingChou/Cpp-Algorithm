/* heapmin.cc -- tony [@] finenet.com.tw

   描述
    定义一个数组，初始化为空。在数组上执行两种操作：
    1、增添1个元素，把1个新的元素放入数组。
    2、输出并删除数组中最小的数。

    使用堆结构实现上述功能的高效算法。

   输入
    第一行输入一个整数t，代表测试数据的组数。
    对于每组测试数据，第一行输入一个整数n，代表操作的次数。
    每次操作首先输入一个整数type。
    当type=1，增添操作，接着输入一个整数u，代表要插入的元素。
    当type=2，输出删除操作，输出并删除数组中最小的元素。
    1<=n<=100000。

   输出
    每次删除操作输出被删除的数字。

   样例输入
    2
    5
    1 1
    1 2
    1 3
    2
    2
    4
    1 5
    1 1
    1 7
    2

   样例输出
    1
    2
    1

   提示
    每组测试数据的复杂度为O(nlgn)的算法才能通过本次，否则会返回TLE(超时)
    需要使用最小堆结构来实现本题的算法

 */

#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

//#define CPP_IOSTREAM 1
#if defined(CPP_IOSTREAM)
  #include <iostream>
#else
  #include <cstdio>
#endif

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<int> vint;
typedef priority_queue<int, vint, greater<int> > pqmin;

const int MAX_NUM = 100000;

static int debug;

//---------------------------------------------------------------------------

#if defined(CPP_IOSTREAM)
  #define input(x)      cin >> x
  #define output(x)     cout << x << endl
#else
  #define input(x)      scanf("%d", &x)
  #define output(x)     printf("%d\n", x)
#endif

static void run(int max_num)
{
    int nCase;
    input(nCase);

    while (--nCase >= 0) {
        pqmin pq;
        int n;
        input(n);

#if defined(CPP_IOSTREAM)
        bool inInput = false;
#endif

        for (int i = 0; i < n; i++) {
            int op;
            input(op);
            assert(1 <= op && op <= 2);

            if (op == 1) {
                int u;
                input(u);
                pq.push(u);
#if defined(CPP_IOSTREAM)
                if (debug) {
                    cout << "-> " << u << ' ';
                    inInput = true;
                }
#endif
            } else {
#if defined(CPP_IOSTREAM)
                if (debug && inInput) {
                    cout << endl;
                    inInput = false;
                }
#endif
                output(pq.top());
                pq.pop();
            }
        }

#if defined(CPP_IOSTREAM)
        if (debug) {
            if (inInput) cout << endl;
            cout << "--> Heap: (";
            while (!pq.empty()) {
                cout << ' ' << pq.top();
                pq.pop();
            }
            cout << " )\n" << endl;
        }
#endif
    }
}

int main (int argc, char *argv[])
{
#if defined(CPP_IOSTREAM)
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
#endif

    int max_num = MAX_NUM;
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
#if defined(CPP_IOSTREAM)
            } else if (strncmp(argv[n], "-i:", 3) == 0) {
                ifstream in(&argv[n][3]);
                //streambuf *cinbuf = cin.rdbuf(); //save old buf
                cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
                run(max_num);
                //cin.rdbuf(cinbuf);
                return 0;
#endif
            }
            n++;
        }
    }

    run(max_num);

    return 0;
}
