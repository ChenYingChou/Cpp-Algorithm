/* losertree.cc -- tonychen [@] finenet.com.tw

   描述
    给定一个整数数组，要求对数组中的元素构建败方树（数组相邻元素两两比较，
    从第一个元素开始）。之后修改数组中的元素，要求输出初始构建以及修改后得
    到的败方树的所有内部结点代表的整数(从左到右从上到下输出）

   输入
    第一行为数组的元素个数n和修改的次数m。
    第二行为n个整数，即数组的元素。
    接下来m行代表m次修改操作，每次操作修改数组中的一个元素，每一行包括两个
    整数，第一个为被修改元素在数组中的标号，第二个为修改之后的元素值。

   输出
    输出m+1行。
    第一行为初始构建的败方树的所有内部结点代表的整数 (按照树的结点从左到右
    从上到下的顺序输出）
    接下来m行为接下来m次修改后得到的败方树的所有内部结点代表的整数 (按照树
    的结点从左到右从上到下的顺序输出）

   样例输入
    8 1
    10 9 20 6 16 12 90 17
    3 15

   样例输出
    6 12 9 17 10 20 16 90
    9 12 15 17 10 20 16 90

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

const int MAX_NUM = 1000000;

static int debug;

//---------------------------------------------------------------------------

class LoserTree {
private:
    int _num;
    int _offset;        // 2^n
    int _lowExt;        // 有多少外節點在最後一層
    vint _node;
    vint _data;
    int winner(int k);
public:
    LoserTree(int n) { _data.reserve(n); }
    void add(int v) { _data.push_back(v); }
    int winner() const { return _num > 0 ? _node[0] : -1; }
    void start();
    void next(int nth, int v);
    void output() const;
};

void LoserTree::output() const
{
    for (int i = 0; i < _num; i++) {
        int nth = _node[i];
        if (debug > 1) cout << "[" << nth << "]:";
        cout << _data[nth] << ' ';
    }
    cout << endl;
}

int LoserTree::winner(int k)
{
    if (k >= _num) {
        int nth = k >= _offset ? k - _offset : k - _num + _lowExt;
        if (debug > 1) {
            cout << "--> Node[" << k
                << "]=Data[" << nth
                << "]:" << _data[nth]
                << endl;
        }
        return nth;
    }

    int nLeft = winner(2*k);
    int nRight = winner(2*k+1);
    if (_data[nLeft] < _data[nRight]) {
        _node[k] = nRight;      // keep loser(right) in node[k]
        if (debug > 1) {
            cout << "--> Node[" << k
                << "] Winner is [" << nLeft
                << "]:" << _data[nLeft]
                << ", Loser is [" << nRight
                << "]:" << _data[nRight]
                << endl;
        }
        return nLeft;
    } else {
        _node[k] = nLeft;       // keep loser(left) in node[k]
        if (debug > 1) {
            cout << "--> Node[" << k
                << "] Winner is [" << nRight
                << "]:" << _data[nRight]
                << ", Loser is [" << nLeft
                << "]:" << _data[nLeft]
                << endl;
        }
        return nRight;
    }
}

void LoserTree::start()
{
    _num = _data.size();
    if (_num <= 0) return;

    _node.resize(_num);

    int nth = 1;
    while (2*nth < _num) {
        nth <<= 1;
    }

    _offset = 2 * nth;
    _lowExt = 2 * (_num - nth);
    if (debug) {
        cout << "--> Start: Offset=" << _offset
            << ", LowExt=" << _lowExt
            << endl;
    }

    _node[0] = winner(1);
}

void LoserTree::next(int nth, int v)
{
    assert(0 <= nth && nth < _num);
    if (_node[0] != nth) {
        cout << "*** Top:" << (_node[0])
            << " <> nth:" << nth
            << ", data:" << v
            << endl;
        abort();
    }
    _data[nth] = v;

    int p = (nth < _lowExt ? nth+_offset : nth+_num-_lowExt) / 2;
    int wLast = nth;
    do {
        int k = _node[p];
        if (_data[k] < v) {
            _node[p] = wLast;
            v = _data[wLast=k];
        }
    } while ((p /= 2) >= 1);
    _node[0] = wLast;
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int m, n;
    cin >> n >> m;
    if (debug) {
        cout << "--> N=" << n << ", M=" << m << endl;
    }
    assert(0 < n && n <= max_num);

    LoserTree T(n);
    if (debug) cout << "--> Initial Data:";
    for (int i = 0; i < n; i++) {
        int v;
        cin >> v;
        if (debug) cout << " [" << i << "]:" << v;
        T.add(v);
    }
    if (debug) cout << endl;

    T.start();
    T.output();
    for (int i = 0; i < m; i++) {
        int nth, v;
        cin >> nth >> v;
        if (debug) {
            cout << "--> Data[" << nth << "]:" << v << endl;
        }
        T.next(nth, v);
        T.output();
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
