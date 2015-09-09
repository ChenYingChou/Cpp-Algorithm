/* toplogicalsort.cc

   描述
    给出一个图的结构，输出其拓扑排序序列，要求在同等条件下，编号小的顶点在前

   输入
    若干行整数，第一行有2个数，分别为顶点数v和弧数a，接下来有a行，每一行有2
    个数，分别是该条弧所关联的两个顶点编号

   输出
    若干个空格隔开的顶点构成的序列(用小写字母)

   样例输入
    6 8
    1 2
    1 3
    1 4
    3 2
    3 5
    4 5
    6 4
    6 5

   样例输出
    v1 v3 v2 v6 v4 v5

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

typedef vector<int> vint;
typedef vector<bool> vbool;
typedef vector<vint> vvint;
typedef priority_queue<int, vint, less<int> > MaxPQ;
typedef priority_queue<int, vint, greater<int> > MinPQ;

const int MAX_NUM = 100000;

static int debug;

//---------------------------------------------------------------------------

class Toplogical_Order {
  private:
    vvint _vPath;
    vbool _visited;
    vint _in_degree;
  public:
    Toplogical_Order(int nV) : _vPath(nV), _visited(nV), _in_degree(nV) {}
    void add(int v1, int v2) {
        _vPath[v1-1].push_back(v2-1);
        _in_degree[v2-1]++;
    }
    vint sort();
};

vint Toplogical_Order::sort()
{
    int nV = _vPath.size();
    MinPQ pq;
    for (int v = 0; v < nV; v++) {
        if (_in_degree[v] == 0) {
            pq.push(v);
            if (debug) cout << "--> push: " << (v+1) << endl;
        }
    }

    vint result;
    while (!pq.empty()) {
        int v1 = pq.top(); pq.pop();
        if (!_visited[v1]) {
            _visited[v1] = true;
            result.push_back(v1+1);
            if (debug) cout << "-->  pop: " << (v1+1) << endl;

            vint &path = _vPath[v1];
            for (vint::iterator it = path.begin(); it != path.end(); ++it) {
                int v2 = *it;
                if (--_in_degree[v2] == 0) {
                    pq.push(v2);
                    if (debug) cout << "--> push: " << (v2+1) << endl;
                }
            }
        }
    }

    if (result.size() != nV) {
        cout << "*** It's a cycle graph." << endl;
        abort();
    }

    return result;
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int nV, nA;
    cin >> nV >> nA;

    Toplogical_Order t(nV);
    for (int i = 0; i < nA; i++) {
        int v1, v2;
        cin >> v1 >> v2;
        assert(1 <= v1 && v1 <= nV);
        assert(1 <= v2 && v2 <= nV);
        t.add(v1, v2);
    }

    vint orders = t.sort();
    for (vint::iterator it = orders.begin(); it != orders.end(); ++it) {
        cout << 'v' << *it << ' ';
    }
    cout << endl;
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
