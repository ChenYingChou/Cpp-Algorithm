/* unipdd.cc -- UNIMODAL PALINDROMIC DECOMPOSITIONS

   Descripton:
   -----------
    A sequence of positive integers is Palindromic if it reads the same
    forward and backward. For example:

    23 11 15 1 37 37 1 15 11 23

    1 1 2 3 4 7 7 10 7 7 4 3 2 1 1

    A Palindromic sequence is Unimodal Palindromic if the values do not
    decrease up to the middle value and then (since the sequence is
    palindromic) do not increase from the middle to the end For example,
    the first example sequence above is NOT Unimodal Palindromic while
    the second example is.

    A Unimodal Palindromic sequence is a Unimodal Palindromic Decomposition
    of an integer N, if the sum of the integers in the sequence is N.

    For example, all of the Unimodal Palindromic Decompositions of the first
    few integers are given below:
      1: (1)
      2: (2), (1 1)
      3: (3), (1 1 1)
      4: (4), (1 2 1), (2 2), (1 1 1 1)
      5: (5), (1 3 1), (1 1 1 1 1)
      6: (6), (1 4 1), (2 2 2), (1 1 2 1 1), (3 3),
         (1 2 2 1), ( 1 1 1 1 1 1)
      7: (7), (1 5 1), (2 3 2), (1 1 3 1 1), (1 1 1 1 1 1 1)
      8: (8), (1 6 1), (2 4 2), (1 1 4 1 1), (1 2 2 2 1),
         (1 1 1 2 1 1 1), ( 4 4), (1 3 3 1), (2 2 2 2),
         (1 1 2 2 1 1), (1 1 1 1 1 1 1 1)

    Write a program, which computes the number of Unimodal Palindromic
    Decompositions of an integer.

   Input:
   ------
    Input consists of a sequence of positive integers, one per line ending
    with a 0 (zero) indicating the end.

   Output:
   -------
    For each input value except the last, the output is a line containing
    the input value followed by a space, then the number of Unimodal
    Palindromic Decompositions of the input value. See the example on the
    next page.

   Input sample:
   -------------
    2
    3
    4
    5
    6
    7
    8
    10
    23
    24
    131
    213
    92
    0

   Output sample:
   --------------
    2 2
    3 2
    4 4
    5 3
    6 7
    7 5
    8 11
    10 17
    23 104
    24 199
    131 5010688
    213 1055852590
    92 331143

   Hint:
   -----
    N < 250

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   分析: tonychen@finenet.com.tw

     數值    整數單峰回文個數及組合狀態
    ------  ------------------------------------------------------------------
       1     1: (1)
       2     2: (1 1) (2)
       3     2: (1 1 1) (3)
       4     4: (1 1 1 1) (1 2 1) (2 2) (4)
       5     3: (1 1 1 1 1) (1 3 1) (5)
       6     7: (1 1 1 1 1 1) (1 1 2 1 1) (1 2 2 1) (1 4 1) (2 2 2) (3 3) (6)
       7     5: (1 1 1 1 1 1 1) (1 1 3 1 1) (1 5 1) (2 3 2) 7

       8    11: 1111 1111 --+
                 1112111    |
                 112 211    |    把最外層1拿掉, 就是6的全部組合(大於等於1起頭者)
                  11411     |--> 1 (6) 1 => C(6) = 7
                  12221     |
                  13 31     |
                   161    --+

                  22 22   --+    把最外層2拿掉, 就是4組合中大於等於2起頭者
                            |--> 2 (4) 2 => C(4)-C(4,1) = 4 - 2 = 2
                   242    --+    即4組合中除了1起頭者

               X   323    -----> 3 (2) 3 非單峰回文, 不計次

                   4 4    -----> 能被分成一對者(可被2整除), 計一次
                    8     -----> 自己本身, 計一次

    建構狀態表: 索引0表示該數值全部組合, 即 C(N) = C(N,0) = ΣC(N,i=1~N)

              +-- C(N) = C(N,0) = ΣC(N,i=1~N) 全部(右方加總)
              |
      整數    V   起頭  起頭  起頭  起頭  起頭  起頭  起頭  起頭  起頭   起頭
       N      0     1     2     3     4     5     6     7     8     9     10
    ------  ------------------------------------------------------------------
       1      1     1
       2      2    (1)    1                       本表中的 "-" 表示零
       3      2     1     -     1
       4      4     2    (1)    -     1
       5      3     2     -     -     -     1
       6      7     4     1    (1)    -     -     1
       7      5     3     1     -     -     -     -     1
       8     11     7     2     -    (1)    -     -     -     1
       9      8     5     1     1     -     -     -     -     -     1
      10     17    11     3     1     -    (1)    -     -     -     -     1

    為節省空間, 括號或減號後的數值可不儲存, 即對 N 而言儲存到 N/3 位置即可。
    因為 N 可拆成一對(偶數, 可被2整除)及本身的計數均為 1, 沒儲存的必要。

    範例: 10 的單峰回文組合個數計算如下
        C(10) = C(10,0) = 11 + 3 + 1 + 1 + 1 = 17
              C(8)                      --> 1 (8) 1 = 11
            + C(6) - C(6,1)             --> 2 (6) 2 = 7 - 4 = 3
            + C(4) - C(4,1) - C(4,2)    --> 3 (4) 3 = 4 - 2 - 1 = 1
            + 1                         --> 拆一對(5 5), 加1次
            + 1                         --> 自己本身(10), 加1次
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef unsigned long long ULL;
typedef vector<ULL> vULL;
typedef vector<vULL*> pvULL;

static int debug;

class UNI_PDD {
  private:
    pvULL _state;

  public:
    ~UNI_PDD() {
        for (pvULL::iterator it = _state.begin(); it != _state.end(); ++it) {
            vULL *p = *it;
            if (p) delete p;
        }
        _state.clear();
    }
    ULL operator() (const int N) { return operator()(N, 0); }
    ULL operator() (const int N, const int index);
    void output();
};

void UNI_PDD::output()
{
    cout << "value";
    for (int v = 0; v < _state.size(); v++) cout << "\t" << v;
    cout << endl << "-----";
    for (int v = 0; v < _state.size(); v++) cout << "\t-----";
    cout << endl;

    for (int v = 1; v < _state.size(); v++) {
        cout << v;
        if (v > 2 && _state[v] == nullptr) {
            cout << "\t.... unknow ...";
        } else {
            for (int i = 0; i <= v; i++) {
                cout << "\t" << operator()(v, i);
            }
        }
        cout << endl;
    }
    cout << endl;
}

ULL UNI_PDD::operator() (const int N, const int index)
{
    assert(N >= 0 && index >= 0 && index <= N);

    if (N <= 2) {
        return index ? 1 : N;   // 化簡 C(1,?), C(2,?) 的輸出, 不儲存
    }

    int maxCol = N / 3;
    if (index > maxCol) {
        return index == N || 2 * index == N ? 1 : 0;
    }

    // 確保有足夠的空間儲存, 0~2 位置保留但不使用
    if (N >= _state.size()) _state.resize(N+1, nullptr);

    vULL *p = _state[N];
    if (p) return (*p)[index];  // 之前已計算有值, 直接返回儲存結果

    // 新的數值, 要計算所有欄位
    p = new vULL(maxCol+1, 0);
    _state[N] = p;
    vULL &X = *p;               // 用參考方便使用, X[i] 代表 C(N, i=0~N/3)

    // 計算 C(N, 0) =  ΣC(N, 1~N/3) + 1(本身) + 是否可拆成一對(偶數)
    X[0] = 2 - (N & 1);         // 本身 + 是否可拆成一對(偶數) = 2 - (odd ? 1 : 0)
    if (debug > 1) {
        cout << N << ": sum = " << X[0] << endl;
    }

    int N2 = N - 2;
    X[0] += X[1] = operator()(N2);          // C(N-2): 1 (N-2) 1
    if (debug > 1) {
        cout << N << ": sum = " << X[0]
            << " after plus C(" << N2 << ")="
            << X[1] << endl;
    }

    for (int i = 2; i <= maxCol; i++) {
        N2 -= 2;
        X[i] = operator()(N2);              // C(N-2*i): i (N-2*i) i, i = 2 ~ N/3
        if (debug > 1) {
            cout << N << ": X[" << i << "] = "
                << "C(" << N2 << ")="
                << X[i] << endl;
        }

        for (int j = 1; j < i; j++) {
            X[i] -= operator()(N2, j);      // 減去 C(N-2*i, j), j = 1 ~ i-1
            if (debug > 1) {
                cout << N << ": X[" << i << "] = " << X[i]
                    << " after minus C(" << N2 << "," << j << ")="
                    << X[i] << endl;
            }
        }

        X[0] += X[i];
        if (debug > 1) {
            cout << N << ": sum = " << X[0]
                << " after plus X[" << i << "]="
                << X[i] << endl;
        }
    }

    return X[index];
}

int main(int argc, char *argv[])
{
    if (argc > 1 && strncmp(argv[1], "-d", 2) == 0) debug = strlen(&argv[1][1]);

    UNI_PDD C;

    int N;
    while (!(cin >> N).eof() && N > 0) {
        ULL V = C(N);       // 不直接放在 cout 輸出中, 避免和除錯資訊混在一起
        if (debug) cout << "--> ";
        cout << N << " " << V << endl;
    }

    if (debug) C.output();

    return 0;
}

