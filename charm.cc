/* charm.cc -- tonychen@finenet.com.tw

   描述
    Bessie has gone to the mall's jewelry store and spies a charm bracelet.
    Of course, she'd like to fill it with the best charms possible from the
    N(1 ≤ N ≤ 3,402) available charms. Each charm i in the supplied list
    has a weight Wi(1 ≤ Wi≤ 400), a desirability factor Di(1 ≤ Di≤ 100),
    and can be used at most once. Bessie can only support a charm bracelet
    whose weight is no more than M(1 ≤ M≤ 12,880).

    Given that weight limit as a constraint and a list of the charms with
    their weights and desirability rating, deduce the maximum possible sum
    of ratings.

   输入
    Line 1: Two space-separated integers: N and M
    Lines 2..N+1: Line i+1 describes charm i with two space-separated
            integers: Wi and Di

   输出
    Line 1: A single integer that is the greatest sum of charm desirabilities
    that can be achieved given the weight constraints

   样例输入
    4 6
    1 4
    2 6
    3 12
    2 7

   样例输出
    23

 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <assert.h>

using namespace std;

typedef vector<int> vint;
typedef unsigned long long ULL;

static vint Wi;
static vint Di;
static vint Order;
static int debug;
static ULL counter;

static int knap(int n, int D, int M)
{
    if (n >= Order.size()) return D;
    if (debug) counter++;

    int idx = Order[n];
    if (Wi[idx] > M) return D;  // 往後只會有更大的Wi[], 因此也不用往下計算

    return max(knap(n+1, D, M), knap(n+1, D+Di[idx], M-Wi[idx]));
}

// 依重量由小到大排序
static bool cmpWi(int i, int j)
{
    return Wi[i] < Wi[j];
}

int main(int argc, char *argv[])
{
    if (argc > 1 && strncmp(argv[1], "-d", 2) == 0) debug = strlen(&argv[1][1]);

    int N, M;
    while (!(cin >> N >> M).eof() && N > 0 && M > 0) {
        Wi.resize(N, 0);
        Di.resize(N, 0);
        for (int i = 0; i < N; i++) {
            cin >> Wi[i] >> Di[i];
        }

        Order.resize(N, 0);
        for (int i = 0; i < N; i++) Order[i] = i;
        sort(Order.begin(), Order.end(), cmpWi);

        if (debug) counter = 0;
        int answer = knap(0, 0, M);

        if (debug) cout << "--> counter = " << counter << endl;
        cout << answer << endl;
    }

    Wi.clear();
    Di.clear();
    Order.clear();
    return 0;
}

