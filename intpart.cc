/* intpart.cc

   描述
    将正整数n 表示成一系列正整数之和，n=n1+n2+…+nk, 其中n1>=n2>=…>=nk>=1 ，k>=1 。
    正整数n 的这种表示称为正整数n 的划分。正整数n 的不同的划分个数称为正整数n 的划分数。

   输入
    标准的输入包含若干组测试数据。每组测试数据是一个整数N(0 < N <= 50)。

   输出
    对于每组测试数据，输出N的划分数。

   样例输入
    5

   样例输出
    7

   提示
    5, 4+1, 3+2, 3+1+1, 2+2+1, 2+1+1+1, 1+1+1+1+1

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<string> vstring;
typedef vector<long> vlong;
typedef vector<vlong> long2d;

const int MAX_NUM = 300;

static int debug;
static int number;
static vstring stack;
static long2d state;
//---------------------------------------------------------------------------

static string int2str(int value)
{
    ostringstream os;
    os << value << ' ';
    return os.str();
}

static void output_stack(int n)
{
    for (int i = number; i > n; i--) {
        if (!stack[i].empty()) cout << stack[i];
    }
}

static long dfs(int n, int m)
{
    assert(n > 0);

    if (debug > 1) {
        cout << "n:" << n
            << " m:" << m
            << endl;
    }

    if (n == 1) {       // { 1 }
        if (debug) {
            output_stack(n);
            cout << "1" << endl;
        }
        return 1;       // f(1, m) = 1
    }

    if (m == 1) {       // n x 1 => { 1,1,...,1 }
        if (debug) {
            output_stack(n);
            for (int i = 0; i < n; i++) cout << "1 ";
            cout << endl;
        }
        return 1;       // f(n, 1) = 1
    }

    if (n < m) m = n;

    vlong &sts = state[n];
    if (!debug && sts[m]) return sts[m];

    if (n == m) {
        if (debug) {    // self { n }
            output_stack(n);
            cout << n << endl;
        }
        // Expand dfs(n, m-1) => { (n-1)+1, (n-2)+2, ... }
        return sts[m] = dfs(n, m-1) + 1;
    }

    // n > m: f(n-m, m) + f(n, m-1)

    if (debug) stack[n] = int2str(m);
    sts[m] = dfs(n-m, m) + dfs(n, m-1);
    if (debug) stack[n].clear();
    return sts[m];
}

static long partition(int n)
{
    if (debug) {
        number = n;
        stack.clear();
        stack.resize(n+1);
    }

    int lastSize = state.size();
    if (n >= lastSize) {
        state.resize(max(16, n+1));
        for (int i = max(2,lastSize); i < state.size(); i++) {
            state[i].resize(i+1, 0);
        }
    }

    return dfs(n, n);
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int N;
    while (cin >> N) {
        assert(N >= 0 && N <= max_num);
        long count = partition(N);
        if (debug) cout << "--> ";
        cout << count << endl;
    }
}

int main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);

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
