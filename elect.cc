/* elect.cc -- tonychen [@] finenet.com.tw

   描述
    现在有 N (1 <= N <= 50000) 头牛在选举它们的总统，选举包括两轮：第一轮投票
    选举出票数最多的 K (1 <= K <= N) 头牛进入第二轮；第二轮对K头牛重新投票，
    票数最多的牛当选为总统。
    现在给出每头牛i在第一轮期望获得的票数 Ai (1 <= Ai <= 1,000,000,000)，以及
    在第二轮中(假设它进入第二轮)期望获得的票数 Bi (1 <= Bi <= 1,000,000,000)，
    请你预测一下哪头牛将当选总统。幸运的是，每轮投票都不会出现票数相同的情况。

   输入
    第1行：N和K
    第2至N+1行：第i+1行包括两个数字：Ai和Bi

   输出
    当选总统的牛的编号（牛的编号从1开始）

   样例输入
    5 3
    3 10
    9 2
    5 6
    8 4
    6 5

   样例输出
    5

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

const int MAX_NUM = 50000;

static int debug;

//---------------------------------------------------------------------------

class Expects {
  private:
    int _id;
    int _e1;
    int _e2;
  public:
    Expects(int id, int e1, int e2) : _id(id), _e1(e1), _e2(e2) {}
    int id() const { return _id; }
    int e1() const { return _e1; }
    int e2() const { return _e2; }
    bool operator< (const Expects &a) const {
        // descending for min-pq
        if (_e1 != _e2) return a._e1 < _e1;
        return a._e2 < _e2;
    }
};

typedef priority_queue<Expects> MinPQ;

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int n, k;
    cin >> n >> k;
    assert(1 <= n && n <= max_num);

    MinPQ pq;
    for (int i = 1; i <= n; i++) {
        int e1, e2;
        cin >> e1 >> e2;
        if (i <= k) {
            pq.push(Expects(i, e1, e2));
        } else {
            if (e1 > pq.top().e1()) {
                pq.pop();
                pq.push(Expects(i, e1, e2));
            }
        }
    }

    Expects a = pq.top(); pq.pop();
    while (!pq.empty()) {
        Expects b = pq.top(); pq.pop();
        if (b.e2() > a.e2()) a = b;
    }

    cout << a.id() << endl;
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
