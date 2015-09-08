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
    vint _id;       // id[p] = parent of p (p=1..N)
    vint _sz;       // sz[p] = number of objects in subtree rooted at p
    vint _eat;      // eat[p] = p eats eat[p]
    int _count;     // number of components (not include id[0]

public:
    void dump() const {
        cout << "    " << setfill(' ');
        for (int i = 0; i < _id.size(); i++) {
            cout << setw(3) << i;
        }
        cout << endl;

        dump_list(" id:", _id);
        dump_list(" sz:", _sz);
        dump_list("eat:", _eat);
    }
    UnionFind(int N) {
        _count = N;
        _id.resize(N+1);
        _sz.resize(N+1, 1);
        _eat.resize(N+1, 0);
        for (int i = 0; i <= N; i++) {
            _id[i] = i;
        }
    }
    int count() const { return _count; }
    bool Connected(int p, int q) { return Find(p) == Find(q); }

    int Find(int p) {
        if (p <= 0) return p;
        if (_id[p] == p) return p;
        _id[p] = Find(_id[p]);
        return _id[p];
    }

    bool Eat(int p, int q) {
        int root = Find(p);
        int eaten = Find(_eat[root]);
        if (_eat[root] != eaten) _eat[root] = eaten;
        return eaten == Find(q);
    }

    // return p eats someone and someone eats q
    bool EatEat(int p, int q) {
        int root = Find(p);
        int eaten = Find(_eat[root]);
        if (eaten != _eat[root]) _eat[root] = eaten;
        return Eat(eaten, q);
    }

    int set_eat(int p, int q) {
        int rootP = Find(p);
        int rootQ = Union(q, _eat[rootP]);
        if (rootQ <= 0) return rootQ;
        _eat[rootP] = rootQ;
        return rootP;
    }

    int Union(int p, int q) {
        int rootP = Find(p);
        int rootQ = Find(q);
        if (rootP == rootQ || rootQ == 0) return rootP;
        if (rootP == 0) return rootQ;

        int eatP = Find(_eat[rootP]);
        int eatQ = Find(_eat[rootQ]);
        if (eatP == rootQ || eatQ == rootP) return 0;   // fails but not union

        int root;
        if (_sz[rootP] < _sz[rootQ]) {
            _id[rootP] = root = rootQ;
            _sz[rootQ] += _sz[rootP];
            if (eatQ == 0) _eat[rootQ] = eatQ = eatP;
        } else {
            _id[rootQ] = root = rootP;
            _sz[rootP] += _sz[rootQ];
            if (eatP == 0) _eat[rootP] = eatP = eatQ;
        }

        if (debug > 1) {
            cout << "--> Union(p:" << p << ", q:" << q << ")="
                << "(rootP:" << rootP << ", rootQ:" << rootQ << ") = root:"
                << root
                << " => Union(eatP:" << eatP << ", eatQ:" << eatQ << ")"
                << endl;
        }
        if (eatP > 0 && eatQ > 0 && eatP != eatQ) {
            // Union eatP and eatQ
            int eaten = Union(eatP, eatQ);
            if (eaten <= 0) return -1;      // fails but already union

            _eat[root] = eaten;
            if (_eat[eaten] == root) {
                if (debug > 1) {
                    cout << "--> Union(eatP:" << eatP << ", eatQ:" << eatQ
                        << ")=" << eaten << " == root:" << root
                        << endl;
                    dump();
                }
                return -1;                  // fails but already union
            }
        }

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
                if (uf.Eat(x, y) || uf.Eat(y, x)) {
                    if (debug) {
                        cout << "  lie: x eats y or y eats x" << endl;
                    }
                    lieCount++;
                } else if (uf.EatEat(x, y) || uf.EatEat(y, x)) {
                    // (x|y) eats z, z eats (y|x) --> (x, y) are't the same species
                    if (debug) {
                        cout << "  lie: (x|y) eats z and z eats (y|x)"
                            << endl;
                    }
                    lieCount++;
                } else {
                    if (debug > 1) uf.dump();
                    UnionFind uf0 = uf;
                    int sts = uf.Union(x, y);
                    if (sts <= 0) {
                        if (sts < 0) uf = uf0;
                        if (debug) {
                            cout << "  lie: union(x, y) fails" << endl;
                        }
                        lieCount++;
                    } else {
                        if (debug > 1) uf.dump();
                    }
                }
            } else if (d == 2) {
                // x eats y
                if (x == y || uf.Connected(x, y)) {
                    if (debug) {
                        cout << "  lie: (x, y) are the same species" << endl;
                    }
                    lieCount++;
                } else if (uf.Eat(y, x)) {
                    if (debug) {
                        cout << "  lie: y eats x" << endl;
                    }
                    lieCount++;
                } else {
                    UnionFind uf0 = uf;
                    int sts = uf.set_eat(x, y);
                    if (sts <= 0) {
                        if (sts < 0) uf = uf0;
                        if (debug) {
                            cout << "  lie: set_eat(x, y) fails" << endl;
                        }
                        lieCount++;
                    }
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
