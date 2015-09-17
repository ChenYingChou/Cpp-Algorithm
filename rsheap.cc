/* rsheap.cc -- tonychen [@] finenet.com.tw

   Replacement Selection Sort with heap

   描述
    给定初始整数顺串，以及大小固定并且初始元素已知的二叉最小堆（为完全二叉树
    或类似完全二叉树，且父元素键值总小于等于任何一个子结点的键值），要求利用
    堆实现置换选择排序，并输出第一个顺串。例如给定初始顺串29,14,35,13,以及堆
    (记为16 19 31 25 21 56 40)，置换选择排序得到的第一个顺串为16 19 21 25。

                    16
                   /  \
                 19    31
                /  \  /  \
               25 21 56  40

   输入
    第一行包含两个整数，m为初始顺串的数的个数，n为二叉最小堆的大小
    第二行包含m个整数，即初始顺串
    第三行包含n个整数，即已经建好的堆的元素(有序，按照从堆顶到堆底，从左到右
    的顺序)

   输出
    输出包含一行，即第一个顺串。

   样例输入
    4 7
    29 14 35 13
    16 19 31 25 21 56 40

   样例输出
    16 19 21 25

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
typedef priority_queue<int, vint, greater<int> > MinPQ;

const int MAX_NUM = 100000;

static int debug;

//---------------------------------------------------------------------------

class Heap {
  private:
    MinPQ _pq;
    vint _next;
  public:
    void add_init(int v) { _pq.push(v); }
    bool eor() const { return _pq.empty(); }
    int add(int v) {
        int result = _pq.top(); _pq.pop();
        if (v < result) {
            _next.push_back(v);
        } else {
            _pq.push(v);
        }
        return result;
    }
    void dump() {
        if (!_pq.empty()) {
            cout << "--> Heap:";
            while (!_pq.empty()) {
                cout << ' ' << _pq.top();
                _pq.pop();
            }
            cout << endl;
        }
        if (!_next.empty()) {
            cout << "--> Next run:";
            for (vint::iterator it = _next.begin(); it != _next.end(); ++it) {
                cout << ' ' << *it;
            }
            cout << endl;
        }
    }
};

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int m, n;
    cin >> m >> n;
    if (debug) {
        cout << "--> M: " << m << ", N: " << n << endl;
    }

    vint data(m);
    for (int i = 0; i < m; i++) {
        cin >> data[i];
    }

    Heap X;
    if (debug) cout << "--> Initial data: ";
    for (int i = 0; i < n; i++) {
        int v;
        cin >> v;
        X.add_init(v);
        if (debug) cout << v << ' ';
    }
    if (debug) cout << endl;

    for (int i = 0; i < data.size(); i++) {
        if (X.eor()) break;
        int v = X.add(data[i]);
        if (debug) {
            cout << "--> Add: " << data[i]
                << ", Got: " << v
                << endl;
        } else {
            cout << v << ' ';
        }
    }

    if (debug) {
        X.dump();
    } else {
        cout << endl;
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
