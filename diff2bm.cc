/* diff2bm.cc -- tonychen [@] finenet.com.tw

   描述
    有一个整数数组A和一个目标整数T，希望从A中没有放回地取出两个数，使得两个
    数之差等于T。请问有多少种不同的取法？（取出的两个数分别相等时视为同一种
    取法）

   输入
    输入由两行组成。第一行为两个整型范围内的整数N和T，N为数组长度（N的范围
    是 [2,100000]），T为目标整数。第二行为N个整数，表示数组A，每个整数的范
    围是 [-1000000,1000000]。

   输出
    A中取出两个数之差为T的不同的取法的数目。

   样例输入
    6 1
    1 3 2 1 2 2

   样例输出
    2

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   分析:
    將數組以兩組集合表示:
      bit-map: (A1, A2, ..., An) --> 不重複的值
      set: (Ai, ..., Aj)         --> 重複的值

    判斷:
      T == 0, 則輸出 set 的元素次數 (有重複的值: 自己減自己 == 0)
      T != 0, 找出 bit-map(A1..An) i=1..n 找 (Ai+T) 存在的次數

 */

#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <bitset>
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

const int MAX_NUM   = 100000;
const int RANGE_NUM = 1000000;

typedef set<int> SetInt;
typedef bitset<2*RANGE_NUM+1> BitMap;

static int debug;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#if defined(CPP_IOSTREAM)
  #define input(x)      cin >> x
  #define output(x)     cout << x << endl
#else
  #define input(x)      scanf("%d", &x)
  #define output(x)     printf("%d\n", x)
#endif

static void run(int max_num)
{
    int N, T;

    input(N);
    assert(2 <= N && N <= max_num);

    input(T);

#if defined(CPP_IOSTREAM)
    if (debug) {
        cout << "--> N = " << N
            << ", T = " << T
            << endl;
    }
#endif

    int answer = 0;
    BitMap S1;          // Bitmap for [-RANGE_NUM ~ RANGE_NUM]
    SetInt S2;          // duplicated set of A

    int smallest = RANGE_NUM;
    int largest = -RANGE_NUM;
    for (int i = 0; i < N; i++) {
        int A;
        input(A);
        assert(-RANGE_NUM <= A && A <= RANGE_NUM);
        if (S1[A+RANGE_NUM]) {
            // A already existed in S1
            S2.insert(A);
#if defined(CPP_IOSTREAM)
            if (debug) cout << "--> duplicated: " << A << endl;
#endif
        } else {
            S1[A+RANGE_NUM] = true;
            if (A < smallest) smallest = A;
            if (A > largest ) largest  = A;
        }
    }

    if (T == 0) {
        answer = S2.size();
    } else {
        // make sure: -RANGE_NUM <= (smallest,smallest+T)
        //        and (largest,largest+T) <= RANGE_NUM
        if (smallest+T < -RANGE_NUM) smallest = -RANGE_NUM - T;
        if (largest+T > RANGE_NUM) largest = RANGE_NUM - T;

#if defined(CPP_IOSTREAM)
        if (debug) {
            cout << "--> Range(" << smallest
                << ", " << largest << ")" << endl;
        }
#endif

        // bit-map S1 range (-RANGE_NUM~RANGE_NUM) -> index[0~2*RANGE_NUM+1]
        smallest += RANGE_NUM;
        largest += RANGE_NUM;
        for (int i = smallest; i <= largest; i++) {
            if (S1[i] && S1[i+T] ) answer++;
        }
    }

    output(answer);
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
