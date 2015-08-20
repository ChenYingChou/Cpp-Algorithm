/* unipdd1.cc -- UNIMODAL PALINDROMIC DECOMPOSITIONS

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

   分析: tonychen [@] finenet.com.tw

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
       N      0   > 1   > 2   > 3   > 4   > 5   > 6   > 7   > 8   > 9     10
    ------  ------------------------------------------------------------------
       1      1     0
       2      2    (1)    0                       本表中的 "-" 表示同左值
       3      2     1     -     0
       4      4     2    (1)    -     0
       5      3     1     -     -     -     0
       6      7     3     2    (1)    -     -     0
       7      5     2     1     -     -     -     -     0
       8     11     4     2     -    (1)    -     -     -     0
       9      8     3     2     1     -     -     -     -     -     0
      10     17     6     3     2     -    (1)    -     -     -     -     0

    為節省空間, 括號或減號後的數值可不儲存, 即對 N 而言儲存到 N/3 位置即可。
    因為 N 可拆成一對(偶數, 可被2整除)及本身的計數均為 1, 沒儲存的必要。

    範例:
     1) 9 的單峰回文組合個數計算如下
        C(9) = C(9,0) = 5 + 1 + 1 + 1 = 9
            + C(7,0)            --> 1 (7) 1 = 5
            + C(5,1)            --> 2 (5) 2 = 1
            + C(3,2)            --> 3 (3) 3 = 1
     [不處理] C(2,3) 非單峰回文 --> 4 (1) 4
            + 自己本身(9)       --> 1

     2) 10 的單峰回文組合個數計算如下
        C(10) = C(10,0) = 11 + 3 + 1 + 1 + 1 = 17
            + C(8,0)            --> 1 (8) 1 = 11
            + C(6,1)            --> 2 (6) 2 = 7 - 4 = 3
            + C(4,2)            --> 3 (4) 3 = 4 - 2 - 1 = 1
     [不處理] C(2,3) 非單峰回文 --> 4 (2) 4
            + 1                 --> 拆一對(5 5), 加1次
            + 1                 --> 自己本身(10), 加1次
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
        int last;
        if (_state[v] == nullptr) {
            if (v > 2) {
                cout << "\t.... unknow ...";
                last = v+1;
            } else {
                last = 0;
            }
        } else {
            vULL &X = *_state[v];
            for (vULL::const_iterator it = X.begin(); it != X.end(); ++it) {
                cout << "\t" << *it;
            }
            last = X.size();
        }

        if (last <= v) {
            cout << "  (\t";
            do {
                cout << operator()(v, last);
                last++;
                cout << (last > v ? "  )" : "\t");
            } while (last <= v);
        }

        cout << endl;
    }
    cout << endl;
}

ULL UNI_PDD::operator() (const int N, const int index)
{
    assert(N > 0 && index >= 0 && index <= N);

    // N 單峰文中大於 N/2 的只有自己本身
    if (2 * index >= N) return index < N ? 1 : 0;       

    if (N <= 2) return N - index;       // 化簡 C(1,?), C(2,?) 的輸出, 不儲存

    // 確保有足夠的空間儲存, 0~2 位置保留但不使用
    if (N >= _state.size()) _state.resize(N+1, nullptr);

    // 儲存前 1/3 的值即可: N 分成三份, 中間數必需比兩邊的數大, 兩邊的要相等
    int maxCol = N / 3;
    vULL *p = _state[N];

    // 若之前已計算有值, 則直接返回儲存結果
    if (p == nullptr) {
        // 新的數值, 要計算本數值所有欄位可能欄位值
        p = new vULL(maxCol+1, 0);
        _state[N] = p;
        vULL &X = *p;   // 用參考方便使用, X[i] 代表 C(N, i=0~N/3)

        // 計算 C(N, i=0~N/3) => C(N, i) = C(N-2,i+1) + C(N, i+1)
        int N2 = N - 2*(maxCol+1);
        ULL last = 1;                           // 本身
        if ((N & 1) == 0 && N2 > 0) last++;     // 一對但不儲存, N >= 8 的偶數

        if (debug > 1) {
            cout << N << ": last = " << last << endl;
        }

        for (int i = maxCol; i >= 0; i--) {
            // N = 2*(i+1) + N2 => (i+1) (N2) (i+1) -> C(N2, i)
            if (N2 == 0) {
                // 一對要儲存: 其實只有 N = 4, 6 時才會在此執行
                last++;
                if (debug > 1) {
                    cout << N << ": " << i+1 << " (" << N2 << ") " << i+1 
                        << " -> pair = 1"
                        << endl;
                }
            } else if (N2 >= i) {
                ULL v = operator()(N2, i);
                last += v;
                if (debug > 1) {
                    cout << N << ": " << i+1 << " (" << N2 << ") " << i+1 
                        << " -> C(" << N2 << ", " << i << ") = " << v
                        << endl;
                }
            }

            X[i] = last;
            if (debug > 1) {
                cout << N << ": X[" << i << "] = " 
                    << last << endl;
            }

            N2 += 2;
        }
    }

    return (*p)[min(index,maxCol)];  
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

    if (debug) {
        debug = 0;
        C.output();
    }

    return 0;
}

