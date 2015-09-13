/* shortpath.cc -- tonychen [@] finenet.com.tw

   输入
    输入分为三个部分。
    第一个部分有P+1行，第一行为一个整数P，之后的P行表示北大的地点。地点长度
        不超过20个字符。
    第二个部分有Q+1行，第一行为一个整数Q，之后的Q行每行分别为两个字符串与一
        个整数，表示这两点有直线的道路，并显示二者之间的矩离（单位为米）。
    第三个部分有R+1行，第一行为一个整数R，之后的R行每行为两个字符串，表示需
        要求的路线。
    p<=30,Q<=50,R<=20

   输出
    输出有R行，分别表示每个路线最短的走法。其中两个点之间，用->(矩离)->相隔。

   样例输入
    6
    XueYiShiTang
    CanYinZhongXin
    XueWuShiTang
    XueYiXiaoBaiFang
    BaiNianJiangTang
    GongHangQuKuanJi
    6
    XueYiShiTang CanYinZhongXin 80
    XueWuShiTang CanYinZhongXin 40
    XueYiShiTang XueYiXiaoBaiFang 35
    XueYiXiaoBaiFang XueWuShiTang 85
    CanYinZhongXin GongHangQuKuanJi 60
    GongHangQuKuanJi BaiNianJiangTang 35
    2
    XueYiXiaoBaiFang BaiNianJiangTang
    GongHangQuKuanJi GongHangQuKuanJi

   样例输出
    XueYiXiaoBaiFang->(35)->XueYiShiTang->(80)->CanYinZhongXin->(60)->GongHangQuKuanJi->(35)->BaiNianJiangTang
    GongHangQuKuanJi

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<int> vint;
typedef map<string, int> mapsite;

const int MAX_INT = std::numeric_limits<int>::max();
const int MAX_NUM = 100000;

static int debug;

//---------------------------------------------------------------------------

class Vertex {
  private:
    int _id;
    string _site;
    int _cost;
    int _prev;
  public:
    Vertex(int id, const string &site) : _id(id), _site(site) {
        _cost = 0;
        _prev = id;
    }
    int id() const { return _id; }
    string site() const { return _site; }
    int cost() const { return _cost; }
    int prev() const { return _prev; }
    void set_cost(int cost) { _cost = cost; }
    void set_prev(int prev) { _prev = prev; }
};

class Cost {
  private:
    int _id;
    int _cost;
  public:
    Cost(const Vertex &v) : _id(v.id()), _cost(v.cost()) {}
    int id() const { return _id; }
    int cost() const { return _cost; }
    bool operator< (const Cost &v) const { return v._cost < _cost; }
};

class Edge {
  private:
    int _v1;
    int _v2;
    int _cost;
  public:
    Edge(int v1, int v2, int cost) : _v1(v1), _v2(v2), _cost(cost) {}
    int v1() const { return _v1; }
    int v2() const { return _v2; }
    int from() const { return _v1; }
    int to() const { return _v2; }
    int cost() const { return _cost; }
    int other(int v) const {
        if (v == _v1) return _v2;
        if (v == _v2) return _v1;
        cout << "*** vertex(" << v << ") neither v1(" << _v1
            << ") nor v2(" << _v2 << ")" << endl;
        abort();
    }
};

typedef vector<Vertex> vVertex;
typedef vector<Edge> vEdge;
typedef vector<vint> vPath;
typedef priority_queue<Cost> MinPQ;

class Graph {
  private:
    vVertex _node;
    vEdge _edge;
    mapsite _site;          // site name to index of _node[]
    vPath _path;            // _path[node] = list(index of _edge[])

    int find_site(const string &site) const {
        mapsite::const_iterator it = _site.find(site);
        if (it == _site.end()) {
            cout << "*** site: " << site << " not found." << endl;
            abort();
        }
        return it->second;
    }

  public:
    Graph() {}
    Graph(int nSite) {
        _node.reserve(nSite);
        _path.resize(nSite);
    }
    void add_site(const string &site) {
        int n = _site.size();
        std::pair<mapsite::iterator, bool> ret;
        ret = _site.insert(std::pair<string, int>(site, n));
        if (ret.second == true) {
            _node.push_back(Vertex(n, site));
            if (_path.size() <= n) _path.resize(n+1);
        }
    }
    void add_path(const string &site1, const string &site2, int cost) {
        int v1 = find_site(site1);
        int v2 = find_site(site2);
        int eth = _edge.size();
        _edge.push_back(Edge(v1, v2, cost));
        _path[v1].push_back(eth);
        _path[v2].push_back(eth);
    }
    void shortest(const string &from, const string &to);
};

void Graph::shortest(const string &from, const string &to)
{
    if (debug) {
        for (int i = 0; i < _node.size(); i++) {
            Vertex &v = _node[i];
            cout << "--> Node[" << v.id() << "]: " << v.site() << endl;
        }
        for (int i = 0; i < _path.size(); i++) {
            vint &path = _path[i];
            cout << "--> Path[" << i << "]:";
            for (int j = 0; j < path.size(); j++) {
                Edge &edge = _edge[path[j]];
                cout << " (" << path[j] << ", " << edge.cost() << ")";
            }
            cout << endl;
        }
        cout << "--> From " << from << " to " << to << endl;
    }

    int nFrom = find_site(from);
    int nTo = find_site(to);
    if (nFrom == nTo) {
        if (debug) cout << endl;
        cout << from << endl;
        return;
    }

    for (vVertex::iterator it = _node.begin(); it != _node.end(); ++it) {
        Vertex &node = *it;
        node.set_cost(MAX_INT);
        node.set_prev(nFrom);
    }

    vint visited(_node.size(), 0);
    _node[nFrom].set_cost(0);
    {
        MinPQ pq;
        pq.push(Cost(_node[nFrom]));
        if (debug) {
            cout << "--> Push Node(" << nFrom << "), cost=0" << endl;
        }

        while (!pq.empty()) {
            Cost C = pq.top(); pq.pop();
            int v1 = C.id();
            if (debug) {
                cout << "--> Pop  Node(" << v1
                    << "), cost=" << C.cost();
            }
            if (visited[v1]) {
                if (debug) cout << "--> skip" << endl;
                continue;
            }

            if (debug) cout << endl;
            visited[v1] = 1;
            if (v1 == nTo) break;

            vint &path = _path[v1];
            for (int i = 0; i < path.size(); i++) {
                Edge &edge = _edge[path[i]];
                int v2 = edge.other(v1);
                Vertex &node = _node[v2];
                if (C.cost() + edge.cost() < node.cost()) {
                    node.set_cost(C.cost()+edge.cost());
                    node.set_prev(v1);
                    pq.push(Cost(node));
                    if (debug) {
                        cout << "--> Push Node(" << node.id()
                            << "), cost=" << node.cost()
                            << endl;
                    }
                }
            }
        }
    }

    if (debug) cout << endl;
    if (visited[nTo]) {
        vint path;
        int nth = nTo;
        while (nth != nFrom) {
            path.push_back(nth);
            nth = _node[nth].prev();
        }

        cout << _node[nth].site();
        int last_cost = 0;
        for (int i = path.size(); --i >= 0;) {
            Vertex &v = _node[path[i]];
            int cost = v.cost() - last_cost;
            cout << "->(" << cost
                << ")->" << v.site();
            last_cost = v.cost();
        }
        cout << endl;
    } else {
        cout << "**** Unreachable " << to
            << " from " << from
            << endl;
    }
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int n;
    cin >> n;

    Graph G(n);
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        G.add_site(s);
    }

    cin >> n;
    for (int i = 0; i < n; i++) {
        string from, to;
        int cost;
        cin >> from >> to >> cost;
        G.add_path(from, to, cost);
    }

    cin >> n;
    for (int i = 0; i < n; i++) {
        string from, to;
        cin >> from >> to;
        G.shortest(from, to);
        if (debug) cout << endl;
    }
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
