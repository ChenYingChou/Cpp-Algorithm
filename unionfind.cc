/* unionfind.cc -- tonychen [@] finenet.com.tw

   描述
    动物王国中有三类动物 A,B,C，这三类动物的食物链构成了有趣的环形。A 吃 B，
    B 吃 C，C 吃 A。现有N个动物，以1－N编号。每个动物都是A,B,C中的一种，但是
    我们并不知道它到底是哪一种。

    有人用两种说法对这N个动物所构成的食物链关系进行描述：
    第一种说法是"1 X Y"，表示X和Y是同类。
    第二种说法是"2 X Y"，表示X吃Y。
    此人对N个动物，用上述两种说法，一句接一句地说出K句话，这K句话有的是真的，
    有的是假的。当一句话满足下列三条之一时，这句话就是假话，否则就是真话。
    1）当前的话与前面的某些真的话冲突，就是假话；
    2）当前的话中X或Y比N大，就是假话；
    3）当前的话表示X吃X，就是假话。
    你的任务是根据给定的N（1 <= N <= 50,000）和K句话（0 <= K <= 100,000），
    输出假话的总数。

   输入
    第一行是两个整数N和K，以一个空格分隔。
    以下K行每行是三个正整数 D，X，Y，两数之间用一个空格隔开，其中D表示说法的
    种类。
    若D=1，则表示X和Y是同类。
    若D=2，则表示X吃Y。

   输出
    只有一个整数，表示假话的数目。

   样例输入
    100 7
    1 101 1
    2 1 2
    2 2 3
    2 3 3
    1 1 3
    2 3 1
    1 5 5

   样例输出
    3

 */

#include <iostream>
#include <iomanip>
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

typedef vector<int> vint;

const int MAX_NUM   = 50000;
const int MAX_WORDS = 100000;

static int debug;

//---------------------------------------------------------------------------

static void dump_list(const char *title, const vint &x)
{
    cout << title;
    for (int i = 0; i < x.size(); i++) {
        cout << setw(3) << x[i];
    }
    cout << endl;
}

class UnionFind {
  private:
    vint _id;       // id[p] = p的父節點 (p=1..N)
    vint _sz;       // sz[p] = p節點的權重 (p子樹的節點數, 含p)
    vint _rt;       // rt[p] = p和父節點id[p]關係, 0:同類, 1:id[p]吃p, 2:p吃id[p]
    int _count;     // number of components (not include id[0])

    void dump() const {
        cout << "    " << setfill(' ');
        for (int i = 0; i < _id.size(); i++) {
            cout << setw(3) << i;
        }
        cout << endl;

        dump_list(" id:", _id);
        dump_list(" sz:", _sz);
        dump_list(" rt:", _rt);
    }

  public:
    int count() const { return _count; }
    bool Connected(int p, int q) { return Find(p) == Find(q); }

    UnionFind(int N) {
        _count = N;
        _id.resize(N+1);
        _sz.resize(N+1, 1);
        _rt.resize(N+1, 0);
        for (int i = 0; i <= N; i++) {
            _id[i] = i;
        }
    }

    int Find(int p) {
        if (p <= 0) return p;
        int pid = _id[p];
        if (pid == p) return p;

        _id[p] = Find(pid);                     // p 的新父節點
        _rt[p] = (_rt[p] + _rt[pid]) % 3;       // p 和新父節點的關係
        return _id[p];
    }

    /*  _rt[p]  _rt[q] | Relation: (3+_rt[q]-_rt[p]) % 3
       ----------------+--------------
          0       0    |    0   同類
          0       1    |    1   p吃q
          0       2    |    2   q吃p
       ----------------+--------------
          1       0    |    2   q吃p
          1       1    |    0   同類
          1       2    |    1   p吃q
       ----------------+--------------
          2       0    |    1   p吃q
          2       1    |    2   q吃p
          2       2    |    0   同類
       ----------------+--------------
     */
    int Relation(int p, int q) {
        if (Find(p) != Find(q)) return -1;      // 無關係
        return (3 + _rt[q] - _rt[p]) % 3;       // 0:同類, 1:p吃q, 2:q吃p
    }

    bool Eat(int p, int q) {
        return Relation(p, q) == 1;
    }

    int Union(int p, int q, int p2q_relation) {
        int rootP = Find(p);
        int rootQ = Find(q);
        if (rootP == rootQ || rootQ == 0) return rootP;
        if (rootP == 0) return rootQ;

        int root;
        // 權重合併集合 (rootP, rootQ)
        if (_sz[rootP] >= _sz[rootQ]) {
            // 合併 rootQ 到 rootP
            _id[rootQ] = root = rootP;
            _sz[rootP] += _sz[rootQ];

            // _rt[rootQ] 計算: 減去 _rt[q] 表示回到 rootQ 的關係起點,
            // 再以 _rt[p] 為基礎加上 p2q_relation 才為 rootQ 對 rootP 的關係
            _rt[rootQ] = (3 + _rt[p] - _rt[q] + p2q_relation) % 3;
        } else {
            // 合併 rootP 到 rootQ
            _id[rootP] = root = rootQ;
            _sz[rootQ] += _sz[rootP];

            // _rt[rootP] 計算: 減去 _rt[p] 表示回到 rootP 的關係起點,
            // 再以 _rt[q] 為基礎減去 p2q_relation 才為 rootP 對 rootQ 的關係
            _rt[rootP] = (6 + _rt[q] - _rt[p] - p2q_relation) % 3;
        }

        if (debug > 1) {
            cout << "--> Union(p:" << p << ", q:" << q
                << ", rt:" << p2q_relation << ")="
                << "(rootP:" << rootP << ", rootQ:" << rootQ << ") = root:"
                << root << endl;
        }
        if (debug > 2) dump();

        _count--;
        return root;
    }
};

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int N, K;
    cin >> N >> K;
    assert(1 <= N && N <= max_num);
    assert(0 <= K && K <= MAX_WORDS);
    if (debug) {
        cout << "--> N = " << N
            << ", K = " << K
            << endl;
    }

    UnionFind uf(N);
    int lieCount = 0;
    for (int i = 1; i <= K; i++) {
        int d, x, y;
        cin >> d >> x >> y;
        if (debug) {
            cout << '#' << setfill('0') << setw(3) << i << setfill(' ') << "> ";
            if (d == 1) {
                cout << "d:1, (x:" << x << ", y:" << y
                    << ") are the same species" << endl;
            } else if (d == 2) {
                cout << "d:2, x:" << x << " eats y:" << y << endl;
            } else {
                cout << "d:" << d << ", x:" << x << ", y:" << y << endl;
            }
        }

        if (x <= 0 || x > N || y <= 0 || y > N) {
            if (debug) {
                cout << "  lie: (x, y) out of range" << endl;
            }
            lieCount++;
        } else {
            if (d == 1) {
                // x, y is same species
                switch(uf.Relation(x, y)) {
                  case 0: // x,y 同類
                    // 真話, 已在同一集合
                    break;
                  case 1: // x 吃 y
                  case 2: // y 吃 x
                    if (debug) {
                        cout << "  lie: x eats y or y eats x" << endl;
                    }
                    lieCount++;
                    break;
                  case -1: // x,y 無關, 合併 (x, y, 同類)
                    uf.Union(x, y, 0);
                    break;
                }
            } else if (d == 2) {
                // x eats y
                switch(uf.Relation(x, y)) {
                  case 0: // x,y 同類
                    if (debug) {
                        cout << "  lie: (x, y) are the same species" << endl;
                    }
                    lieCount++;
                    break;
                  case 1: // x 吃 y
                    // 真話, 已在同一集合
                    break;
                  case 2: // y 吃 x
                    if (debug) {
                        cout << "  lie: y eats x" << endl;
                    }
                    lieCount++;
                    break;
                  case -1: // x,y 無關, 合併 (x, y, x吃y)
                    uf.Union(x, y, 1);
                    break;
                }
            } else {
                cout << "*** Invalid D:" << d << endl;
                assert(0); abort();
            }
        }
    }

    cout << lieCount << endl;
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
