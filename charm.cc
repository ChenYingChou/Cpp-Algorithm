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

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   C[] 狀態表範例: 總重量M=9, 物品N=5, (Wi,Di)=(5,8),(2,3),(4,7),(3,6),(2,4)

     Wi  Di   N   0   1   2   3   4   5   6   7   8   9
     --  --  --  --  --  --  --  --  --  --  --  --  --
      5   8   1   -   -   -   -   -   8   8   8   8   8
      2   3   2   -   -   3   3   3   8   8  11  11  11
      4   7   3   -   -   3   3   7   8  10  11  11  15
      3   6   4   -   -   3   6   7   9  10  13  14  16
      2   4   5   -   -   4   6   7  10  11  13  14  17

     註: "-" 表示初始化值零, 表示該值並未變更

 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

typedef vector<int> vint;

static vint Wi;
static vint Di;
static vint CC;
static int debug;

static int dkp(int N, int M)
{
    CC.clear();
    CC.resize(M+1, 0);
    int *C = &CC[0];

    if (debug) {
        cout << "Wi\tDi\tN";
        for (int i = 0; i <= M; i++) cout << "\t" << i;
        cout << endl << "--";
        for (int i = M+2; i >= 0; i--) cout << "\t--";
        cout << endl;
    }

    for (int i = 0; i < N; i++) {
        // 累算第(i)個物品使用與不使用後, 各個重量(當成C[]的索引)的最大利益

        // 由第(i)列推導出(i+1)列, 為避免前次C[]值被新值覆蓋, 故由後往前計算
        // 因為每次計算會使用到前次小於等於目前位置的值
        for (int j = M, k = M-Wi[i]; k >= 0; j--, k--) {
            // k = j - Wi[i]
            // 取累計 max(不用第i個物品, 用第i個物品) 最大利益
            // 相對遞歸版本: C[i+1][j] = max(dpk(i,j), dpk(i,j-Wi[i])+Di[i])
            C[j] = max(C[j], C[k] + Di[i]);
        }

        if (debug) {
            cout << Wi[i] << "\t" << Di[i] << "\t" << (i+1);
            for (int i = 0; i <= M; i++) {
                cout << "\t";
                if (C[i]) cout << C[i]; else cout << "-";
            }
            cout << endl;
        }
    }

    return C[M];
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

        int answer = dkp(N, M);

        if (debug) cout << "--> ";
        cout << answer << endl;
    }

    Wi.clear();
    Di.clear();
    CC.clear();
    return 0;
}

