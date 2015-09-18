/* setdiff.cc -- tonychen [@] finenet.com.tw

   描述
    设 A = {a1, a2, … , an}, B = {b1, b2, … , bm} 是非负整数集合，
    其中m = O(logn)。(意即集合B比集合A小很多, 至少少一半以上)
    设计算法计算集合 C = (A − B) ∪ (B − A)。

   输入
    输入文件包含2行。
    第1行的第一个数为n，表示A集合的大小，接著有A集合中的包含的n个非负整数。
    第2行的第一个数为m，表示B集合的大小，接著有B集合中的包含的m个非负整数。
    两个集合中的数按照从小到大的顺序给出，n <= 10^6

   输出
    输出文件包含1行，表示计算出的C集合中所包含的整数。
    请将这些整数按照从小到大的顺序排序。

   样例输入
    8 1 3 5 6 8 10 12 30
    3 1 3 7

   样例输出
    5 6 7 8 10 12 30

 */

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

#define CPP_IOSTREAM 1
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

const int MAX_NUM = 1000000;

static int debug;

//---------------------------------------------------------------------------

class MySet {
  private:
    vint _data;
    int _index;
  public:
    MySet() : _index(0) {}
    MySet(int n) : _index(0) { _data.reserve(n); }
    void reserve(int sz) { _data.reserve(sz); }
    void add(int v) { _data.push_back(v); }
    bool empty() const { return _index >= _data.size(); }
    int top() const { return empty() ? -1 : _data[_index]; }
    void start();
    void pop();
};

void MySet::start()
{
    sort(_data.begin(), _data.end());
    _index = 0;
}

void MySet::pop()
{
    if (!empty()) {
        int last = _data[_index++];
        while (!empty() && _data[_index] == last) {
            _index++;
        }
    }
}

//---------------------------------------------------------------------------

#if defined(CPP_IOSTREAM)
  #define input(x)      cin >> x
  #define output(x)     cout << x << ' '
  #define writeln()     cout << endl
#else
  #define input(x)      scanf("%d", &x)
  #define output(x)     printf("%d ", x)
  #define writeln()     printf("\n");
#endif

static void input_list(MySet &X)
{
    int n;
    input(n);
    X.reserve(n);

    for (int i = 0; i < n; i++) {
        int val;
        input(val);
        assert(val >= 0);
        X.add(val);
    }
    X.start();
}

static void output_list(MySet &X)
{
    while (!X.empty()) {
        output(X.top());
        X.pop();
    }
}

static void run(int max_num)
{
    MySet A;
    input_list(A);

    MySet B;
    input_list(B);

    // merge A & B for non replicated value
    while (!A.empty() && !B.empty()) {
        int diff = A.top() - B.top();
        if (diff < 0) {
            output(A.top());
            A.pop();
        } else if (diff > 0) {
            output(B.top());
            B.pop();
        } else {
            A.pop();
            B.pop();
        }
    }

    output_list(A);
    output_list(B);
    writeln();
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
