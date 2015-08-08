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

const int MAX_M = 12880;
const int MAX_N = 3402;

static vint Wi;
static vint Di;
static int C[MAX_N+1][MAX_M+1];
static int debug;
static ULL counter;

static int dkp(int N, int M)
{
    if (N <= 0 || M == 0) return 0;

    if (C[N][M]) return C[N][M];
    counter++;
    N--;

    int D = dkp(N, M);
    if (Wi[N] <= M) D = max(D, dkp(N, M-Wi[N]) + Di[N]);
    return C[N+1][M] = D;

}

int main(int argc, char *argv[])
{
    if (argc > 1 && strncmp(argv[1], "-d", 2) == 0) debug = strlen(&argv[1][1]);

    int N, M;
    while (!(cin >> N >> M).eof() && N > 0 && M > 0) {
        assert(N <= MAX_N && M <= MAX_M);
        
        Wi.resize(N, 0);
        Di.resize(N, 0);
        for (int i = 0; i < N; i++) {
            cin >> Wi[i] >> Di[i];
        }

        memset(C, 0, sizeof(C));
        if (debug) counter = 0;
        int answer = dkp(N, M);

        if (debug) cout << "--> counter = " << counter << endl;
        cout << answer << endl;
    }

    Wi.clear();
    Di.clear();
    return 0;
}

