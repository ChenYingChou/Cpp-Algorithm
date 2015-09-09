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
typedef vector<vint *> vlist;

const int MAX_NUM = 100000;

static int debug;

//---------------------------------------------------------------------------

static void merge(vlist &list)
{
    while (list.size() > 1) {
        int left = 0;
        int right = list.size()-1;
        while (left < right) {
            vint::iterator p1 = list[left]->begin();
            vint::iterator p2 = list[right]->begin();
            vint::iterator p10 = list[left]->end();
            vint::iterator p20 = list[right]->end();
            vint *p0 = new vint;

            // merge p1, p2 to p0 (descending)
            while (p1 != p10 && p2 != p20) {
                if (*p1 > *p2) {
                    p0->push_back(*p1);
                    ++p1;
                } else {
                    p0->push_back(*p2);
                    if (*p1 == *p2) ++p1;
                    ++p2;
                }
            }
            while (p1 != p10) {
                p0->push_back(*p1);
                ++p1;
            }
            while (p2 != p20) {
                p0->push_back(*p2);
                ++p2;
            }

            delete list[left];
            delete list[right];
            list[left] = p0;

            left++;
            right--;
        }
        list.resize(left);
    }
}

class Toplogical_Order {
  private:
    vvint _vPath;
    vbool _visited;
    vint *_post;
    void dfs(int v);
  public:
    Toplogical_Order(int nVertex) : _vPath(nVertex) {}
    void add(int v1, int v2) { _vPath[v1-1].push_back(v2-1); }
    vint sort();
};

void Toplogical_Order::dfs(int v)
{
    _visited[v] = true;
    vint &vp = _vPath[v];
    for (vint::reverse_iterator it = vp.rbegin(); it != vp.rend(); ++it) {
        if (!_visited[*it]) dfs(*it);
    }
    _post->push_back(v);
}

vint Toplogical_Order::sort()
{
    int nV = _vPath.size();
    vint in(nV, 0);
    for (int v1 = 0; v1 < nV; v1++) {
        vint &vp = _vPath[v1];
        for (int j = 0; j < vp.size(); j++) {
            int v2 = vp[j];
            in[v2]++;
        }
    }

    vlist posts;
    for (int v = 0; v < nV; v++) {
        if (in[v] == 0) {
            _visited.clear();
            _visited.resize(nV);
            _post = new vint;
            posts.push_back(_post);
            dfs(v);

            if (debug) {
                cout << "--> dfs:" << (v+1) << " ( ";
                for (vint::iterator it = _post->begin(); it != _post->end(); ++it) {
                    cout << (*it)+1 << ' ';
                }
                cout << ")" << endl;
            }
        }
    }
    _post = nullptr;

    merge(posts);
    assert(posts.size() == 1);

    vint result;
    {
        vint &v = *posts[0];
        if (v.size() != nV) {
            cout << "*** It's a cycle graph." << endl;
            abort();
        }
        for (int i = v.size(); --i >= 0;) {
            result.push_back(v[i]+1);
        }
    }

    // free posts[]
    delete posts[0];
    posts.clear();

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
