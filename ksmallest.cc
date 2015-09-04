/* ksmallest.cc -- tonychen [@] finenet.com.tw

   总时间限制: 10000ms, 单个测试点时间限制: 5000ms, 内存限制: 3000kB

   描述
    小张需要从一批数量庞大的正整数中挑选出第k小的数，因为数据量太庞大，挑选
    起来很费劲，希望你能编程帮他进行挑选。

   输入
    第一行第一个是数据的个数 n (10 <= n <= 10^6)，第二个是需要挑选出的数据的
    序号 k (1 <= k <= 10^5)，n 和 k 以空格分隔；
    第二行以后是 n 个数据 T (1 <= T <= 10^9)，数据之间以空格或者换行符分隔。

   输出
    第 k 小数(如果有相同大小的也进行排序，例如对 1,2,3,8,8，第 4 小的为 8，
    第 5 小的也为 8)。

   样例输入
    10 5
    1  3  8 20 2
    9 10 12  8 9

   样例输出
    8

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

#define CPP_IOSTREAM 1
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
typedef priority_queue<int, vint, less<int> > MaxPQ;
typedef priority_queue<int, vint, greater<int> > MinPQ;

const int MAX_NUM = 1000000;

static int debug;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#if defined(CPP_IOSTREAM)
  #define input(x)      cin >> x
  #define output(x)     cout << x << endl
#else
  #define input(x)      scanf("%d", &x)
  #define output(x)     printf("%d\n", x)
#endif

static int find_kth_smallest(int n, int k)
{
#if defined(CPP_IOSTREAM)
    if (debug) {
        cout << "--> find k-th smallest: " << k << endl;
    }
#endif

    MaxPQ pq;
    for (int i = 0; i < k; i++) {
        int t;
        input(t);
        pq.push(t);
    }

    int maxT = pq.top();
    for (int i = k; i < n; i++) {
        int t;
        input(t);
        if (t < maxT) {
            pq.pop();
            pq.push(t);
            maxT = pq.top();
        }
    }

    return maxT;
}

static int find_kth_largest(int n, int k)
{
#if defined(CPP_IOSTREAM)
    if (debug) {
        cout << "--> find k-th largest: " << k << endl;
    }
#endif

    MinPQ pq;
    for (int i = 0; i < k; i++) {
        int t;
        input(t);
        pq.push(t);
    }

    int minT = pq.top();
    for (int i = k; i < n; i++) {
        int t;
        input(t);
        if (t > minT) {
            pq.pop();
            pq.push(t);
            minT = pq.top();
        }
    }

    return minT;
}

static int find_smallest(int n, bool isSmallest)
{
#if defined(CPP_IOSTREAM)
    if (debug) {
        cout << "--> find "
            << (isSmallest ? "smallest" : "largest")
            << endl;
    }
#endif

    int result;
    input(result);

    for (int i = 1; i < n; i++) {
        int t;
        input(t);
        if (isSmallest) {
            if (t < result) result = t;
        } else {
            if (t > result) result = t;
        }
    }

    return result;
}

static void run(int max_num)
{
    int n, k;

    input(n);
    assert(2 <= n && n <= max_num);

    input(k);
    assert(1 <= k && k <= n);

    int answer;
    if (k == 1) {
        answer = find_smallest(n, true);
    } else if (k == n) {
        answer = find_smallest(n, false);
    } else if (k <= (n+1) / 2) {
        answer = find_kth_smallest(n, k);
    } else {
        answer = find_kth_largest(n, n-k+1);
    }
    output(answer);
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
