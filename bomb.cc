/* bomb.cc -- tonychen [@] finenet.com.tw

   描述
    2014年巴西世界杯(2014 FIFA World Cup)开踢啦！为了方便球迷观看比赛，里约街道
    上很多路口都放置了的直播大屏幕，但是人群散去后总会在这些路口留下一堆垃圾。
    为此巴西政府决定动用一种最新发明——“垃圾炸弹”。这种“炸弹”利用最先进的
    量子物理技术，爆炸后产生的冲击波可以完全清除波及范围内的所有垃圾，并且不会
    产生任何其他不良影响。炸弹爆炸后冲击波是以正方形方式扩散的，炸弹威力（扩散
    距离）以d给出，表示可以传播d条街道。

    例如下图是一个d=1的“垃圾炸弹”爆炸后的波及范围。
    假设里约热内卢市的布局为严格的1025*1025的网格状，由于财政问题市政府只买得
    起一枚“垃圾炸弹”，希望你帮他们找到合适的投放地点，使得一次清除的垃圾总量
    最多（假设垃圾数量可以用一个非负整数表示，并且除设置大屏幕的路口以外的地点
    没有垃圾）。

   输入
    输入第一行是整数T (1 <= T <= 10)，表明有T组测试数据。
    对于每一组测试数据，第一行给出“炸弹”威力d(1 <= d <= 50)。第二行给出一个
    数组n(1 <= n <= 20)表示设置了大屏幕(有垃圾)的路口数目。接下来n行每行给出三
    个数字x, y, i, 分别代表路口的坐标(x, y)以及垃圾数量i. 点坐标(x, y)保证是有
    效的（区间在0到1024之间），同一坐标只会给出一次。

   输出
    对于每组测试数据，输出能清理垃圾最多的投放点数目，以及能够清除的垃圾总量。

   样例输入
    1
    1
    2
    4 4 10
    6 6 20

   样例输出
    1 30

 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

const int MAX_NUM = 1025;

static int debug;

//---------------------------------------------------------------------------

class Node;

typedef vector<Node> vNode;
typedef set<int> setCell;

class Node {
private:
    int _row;       // y
    int _col;       // x
    int _qty;
public:
    Node(int row, int col) : _row(row), _col(col), _qty(0) {}
    Node(int row, int col, int qty) : _row(row), _col(col), _qty(qty) {}
    int row() const { return _row; }
    int col() const { return _col; }
    int qty() const { return _qty; }
    bool operator< (const Node &node) const {
        if (_row != node._row) return _row < node._row;
        return _col < node._col;
    }
};

class Answer {
private:
    int _count;
    int _qty;
public:
    Answer() : _count(0), _qty(0) {}
    Answer(int count, int qty): _count(count), _qty(qty) {}
    int count() const { return _count; }
    int qty() const { return _qty; }
    void operator() (int qty) {
        if (qty > _qty) {
            _qty = qty;
            _count = 1;
        } else if (qty == _qty) {
            _count++;
        }
    }
};

class Garbage {
private:
    vNode _nodes;
    setCell _cell;          // [row,col]: gathered
    bool _sorted;
    int _rows;
    int _cols;

    int gather_by_rect(int r1, int c1, int r2, int c2);
    int gather_by_center(int row, int col, int dist) {
        return gather_by_rect(row-dist, col-dist, row+dist, col+dist);
    }
    Answer walk_by_street(int dist);
    Answer walk_by_nodes(int dist);

public:
    Garbage(int rows, int cols) : _rows(rows), _cols(cols), _sorted(false) {}
    void add_garbage(int row, int col, int qty) {
        _nodes.push_back(Node(row, col, qty));
        _sorted = false;
    }
    Answer find(int dist);
};

int Garbage::gather_by_rect(int r1, int c1, int r2, int c2)
{
    if (r1 > r2) swap(r1, r2);
    if (c1 > c2) swap(c1, c2);

    vNode::iterator p1 = lower_bound(_nodes.begin(), _nodes.end(), Node(r1, c1));
    if (p1 == _nodes.end()) return -1;
    vNode::iterator p2 = upper_bound(_nodes.begin(), _nodes.end(), Node(r2, c2));

    int result = 0;
    while (p1 != p2) {
        Node &n = *p1;
        if (r1 <= n.row() && n.row() <= r2 && c1 <= n.col() && n.col() <= c2) {
            result += n.qty();
        }
        ++p1;
    }

    if (result > 0 && debug) {
        cout << "Rect(" << r1 << ", " << c1 << ", "
            << r2 << ", " << c2
            << ") = " << result
            << endl;
    }
    return result;
}

Answer Garbage::walk_by_street(int dist)
{
    Answer answer;
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            int qty = gather_by_center(r, c, dist);
            if (qty < 0) return answer;
            answer(qty);
        }
    }
    return answer;
}

Answer Garbage::walk_by_nodes(int dist)
{
    _cell.clear();
    Answer answer;
    for (vNode::iterator it = _nodes.begin(); it != _nodes.end(); ++it) {
        Node &n = *it;
        int r1 = max(0, n.row()-dist);
        int r2 = min(_rows-1, n.row()+dist);
        int c1 = max(0, n.col()-dist);
        int c2 = min(_cols-1, n.col()+dist);

        for (int r = r1; r <= r2; r++) {
            for (int c = c1; c <= c2; c++) {
                int key = r * _cols + c;
                if (_cell.find(key) == _cell.end()) {
                    int qty = gather_by_center(r, c, dist);
                    assert(qty > 0);
                    answer(qty);
                    _cell.insert(key);
                }
            }
        }
    }
    return answer;
}

Answer Garbage::find(int dist)
{
    if (!_sorted) {
        sort(_nodes.begin(), _nodes.end());
        _sorted = true;
    }

    if (_rows*_cols < (1+2*dist)*(1+2*dist)*_nodes.size()) {
        return walk_by_street(dist);
    } else {
        return walk_by_nodes(dist);
    }
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int nCase;
    cin >> nCase;
    for (int i = 0; i < nCase; i++) {
        int dist;   // distance of power for bomb
        int num;    // number of data(x, y, qty)
        Garbage G(max_num, max_num);

        cin >> dist >> num;
        if (debug) cout << "Distance:" << dist << endl;

        for (int n = 0; n < num; n++) {
            int x, y, qty;
            cin >> x >> y >> qty;
            if (debug) {
                cout << "x:" << x
                    << ", y:" << y
                    << ", qty:" << qty
                    << endl;
            }
            assert((unsigned)x < max_num && (unsigned)y < max_num);
            G.add_garbage(y, x, qty);
        }

        Answer ans = G.find(dist);
        if (debug) cout << "--> ";
        cout << ans.count() << ' ' << ans.qty() << endl;
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
