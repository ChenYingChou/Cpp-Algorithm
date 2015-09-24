/* findcatch.cc -- tonychen [@] finenet.com.tw

   描述
    一个城市中有两个犯罪团伙A和B，你需要帮助警察判断任意两起案件是否是同一个
    犯罪团伙所为，警察所获得的信息是有限的。假设现在有N起案件（N<=100000），
    编号为 1 到 N，每起案件由团伙A或团伙B所为。你将按时间顺序获得M条信息
    （M<=100000），这些信息分为两类：

    1. D [a] [b]
       其中[a]和[b]表示两起案件的编号，这条信息表明它们属于不同的团伙所为

    2. A [a] [b]
       其中[a]和[b]表示两起案件的编号，这条信息需要你回答[a]和[b]是否是同一个
       团伙所为
       注意你获得信息的时间是有先后顺序的，在回答的时候只能根据已经接收到的信
       息做出判断。

   输入
    第一行是测试数据的数量T（1<=T<=20）。
    每组测试数据的第一行包括两个数N和M，分别表示案件的数量和信息的数量，其后
    M 行表示按时间顺序收到的M条信息。

   输出
    对于每条需要回答的信息，你需要输出一行答案。
    如果是同一个团伙所为，回答"In the same gang."，
    如果不是，回答"In different gangs."，
    如果不确定，回答”Not sure yet."。

   样例输入
    1
    5 5
    A 1 2
    D 1 2
    A 1 2
    D 2 4
    A 1 4

   样例输出
    Not sure yet.
    In different gangs.
    In the same gang.

 */

#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
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

const int MAX_NUM = 100000;

static int debug;

//---------------------------------------------------------------------------

class UnionFind {
  private:
    vint _id;       // id[p] = p的父節點 (p=1..N)
    vint _sz;       // sz[p] = p節點的權重 (p子樹的節點數, 含p)
    vint _rt;       // rt[p] = p和父節點id[p]關係, 0:同伙, 1:不同伙

  public:
    bool Connected(int p, int q) { return Find(p) == Find(q); }

    UnionFind(int N) {
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
        _rt[p] = (_rt[p] + _rt[pid]) % 2;       // p 和新父節點的關係
        return _id[p];
    }

    /*  _rt[p]  _rt[q] | Relation: (2+_rt[q]-_rt[p]) % 2
       ----------------+--------------
          0       0    |    0   同伙
          0       1    |    1   不同伙
       ----------------+--------------
          1       0    |    1   不同伙
          1       1    |    0   同伙
       ----------------+--------------
     */
    int Relation(int p, int q) {
        if (Find(p) != Find(q)) return -1;      // 無關係
        return (2 + _rt[q] - _rt[p]) % 2;       // 0:同伙, 1:不同伙
    }

    bool Group(int p, int q) {
        return Relation(p, q) == 0;
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
            _rt[rootQ] = (2 + _rt[p] - _rt[q] + p2q_relation) % 2;
        } else {
            // 合併 rootP 到 rootQ
            _id[rootP] = root = rootQ;
            _sz[rootQ] += _sz[rootP];

            // _rt[rootP] 計算: 減去 _rt[p] 表示回到 rootP 的關係起點,
            // 再以 _rt[q] 為基礎減去 p2q_relation 才為 rootP 對 rootQ 的關係
            _rt[rootP] = (4 + _rt[q] - _rt[p] - p2q_relation) % 2;
        }

        if (debug > 1) {
#if defined(CPP_IOSTREAM)
            cout << "--> Union(p:" << p << ", q:" << q
                << ", rt:" << p2q_relation << ")="
                << "(rootP:" << rootP << ", rootQ:" << rootQ << ") = root:"
                << root << endl;
#else
            printf("--> Union(p:%d, q:%d, rt:%d)=(rootP:%d, rootQ:%d) = root:%d\n",
                p, q, p2q_relation, rootP, rootQ, root);
#endif
        }

        return root;
    }
};

//---------------------------------------------------------------------------

#if defined(CPP_IOSTREAM)
  #define input(x)          cin >> x
  #define input2(x, y)      cin >> x >> y
  #define input3(x, y, z)   cin >> x >> y >> z
  #define output2(x, c)     cout << x << c << endl
  #define outputs(x)        cout << x << endl
#else
  #define input(x)          scanf("%d", &x)
  #define input2(x, y)      scanf("%d%d\n", &x, &y)
  #define input3(c, x, y)   scanf("%c%d%d\n", &c, &x, &y)
  #define output2(x, c)     printf("%s: %c\n", x, c)
  #define outputs(x)        printf("%s\n", x)
#endif

static void run(int max_num)
{
    int T;
    input(T);
    for (int i = 0; i < T; i++) {
        int N, M;
        input2(N, M);
        assert(1 <= N && N <= max_num);
        assert(0 <= M && M <= max_num);
        if (debug) {
#if defined(CPP_IOSTREAM)
            cout << "--> N = " << N
                << ", M = " << M
                << endl;
#else
            printf("--> N = %d, M = %d\n", N, M);
#endif
        }

        UnionFind uf(N);
        for (int j = 1; j <= M; j++) {
            char d;
            int a, b;
            input3(d, a, b);
            if (debug) {
#if defined(CPP_IOSTREAM)
                cout << "--> " << d
                    << " " << a
                    << " " << b
                    << endl;
#else
                printf("--> %c %d %d\n", d, a, b);
#endif
            }

            if (d == 'D') {
                // (a,b) 不是同伙
                uf.Union(a, b, 1);
            } else if (d == 'A'){
                switch(uf.Relation(a, b)) {
                  case 0: // (a,b) 同伙
                    outputs("In the same gang.");
                    break;
                  case 1: // (a,b) 不是同伙
                    outputs("In different gangs.");
                    break;
                  case -1: // (a,b) 尚無關
                    outputs("Not sure yet.");
                    break;
                }
            } else {
                output2("*** Unknow key: ", d);
            }
        }
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
