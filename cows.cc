/* cows.cc: tonychen [@] finenet.com.tw

   描述
    Farmer John has built a new long barn, with N (2 <= N <= 100,000) stalls.
    The stalls are located along a straight line at positions x1,...,xN
    (0 <= xi <= 1,000,000,000).

    His C (2 <= C <= N) cows don't like this barn layout and become aggressive
    towards each other once put into a stall. To prevent the cows from hurting
    each other, FJ want to assign the cows to the stalls, such that the minimum
    distance between any two of them is as large as possible. What is the
    largest minimum distance?

   输入
    Line 1: Two space-separated integers: N and C
    Lines 2..N+1: Line i+1 contains an integer stall location, xi

   输出
    Line 1: One integer: the largest minimum distance

   样例输入
    5 3
    1
    2
    8
    4
    9

   样例输出
    3

   提示
    FJ can put his 3 cows in the stalls at positions 1, 4 and 8, resulting in
    a minimum distance of 3.

    Huge input data,scanf is recommended.

 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <assert.h>

using namespace std;

typedef vector<int> vint;

static int N;       // 閘門數
static int C;       // 牛頭數
static vint x;      // 閘門位置

static int debug;

// dist: 所關牛的閘門之間最少的距離
static bool check(int dist)
{
    if (debug) cout << "dist=" << dist << endl;
    int n = 1;      // 已用閘門數(每一閘門關一頭牛)
#if 0
    int pos = 0;    // 起始位置
    while (n < C) {
        int nextX = dist + x[pos];
        if (debug) cout << "n=" << n << ", pos=" << pos << ", nextX >= " << nextX;
        // 採用往下順序尋找
        do {
            if (++pos >= N) {
                if (debug) cout << " --> fails" << endl;
                return false;
            }
        } while (x[pos] < nextX);

        if (debug) cout << " --> new pos=" << pos << endl;
        n++;
    }
#else
    vint::iterator it = x.begin();
    while (n < C) {
        int nextX = dist + *it;
        if (debug) cout << "n=" << n << ", x[]=" << *it << ", nextX >= " << nextX;
        it = lower_bound(it+1, x.end(), nextX);
        if (it == x.end()) {
            if (debug) cout << " --> fails" << endl;
            return false;
        }

        if (debug) cout << " --> x[]=" << *it<< endl;
        n++;
    }
#endif

    if (debug) cout << "--> dist=" << dist << endl;
    return true;
}

//---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    for (int n = 1; n < argc; n++) {
        if (strncmp(argv[n], "-d", 2) == 0) {
            debug += strlen(&argv[n][1]);
        }
    }

    cin >> N >> C;
    assert(2 <= N && N <= 100000);
    assert(2 <= C && C <= N);
    
    x.resize(N);
    for (int i = 0; i < N; i++) cin >> x[i];
    sort(x.begin(), x.end());

    if (debug) {
        cout << "x[]:";
        for (vint::iterator it = x.begin(); it != x.end(); ++it) {
            cout << ' ' << *it;
        }
        cout << endl;
    }

    int l = 1;
    int r = (x[N-1] - x[0]) / (C - 1);  // 最大可能距離(理想上為等距)
    if (r == 0) {
        cout << 0 << endl;
    } else {
        while (l <= r) {
            int mid = (l + r) / 2;
            if (check(mid)) {
                // 成功: 往後找更大的距離
                l = mid + 1;
            } else {
                // 失敗: 往前找較小的距離
                r = mid - 1;
            }
        }
        cout << r << endl;
    }

    return 0;
}

