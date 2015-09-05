/* diff2.cc -- tonychen [@] finenet.com.tw

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
      set1: (A1, A2, ..., An)   --> 不重複的值
      set2: (Ai, ..., Aj)       --> 重複的值

    判斷:
      T == 0, 則輸出 set2 的元素次數 (有重複的值: 自己減自己 == 0)
      T != 0, 找出 set1 上下限: min(A1-T, An-T)~max(A1-T, An-T)
              在此範圍內 (Ai..Aj) k=i..j 找 (Ak+T) 存在的次數

 */

#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

//#define CPP_IOSTREAM 1
#if defined(CPP_IOSTREAM)
  #include <iostream>
#else
  #include <cstdio>
#endif

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef set<int> SetInt;
typedef SetInt::iterator SetIntIT;
typedef pair<SetIntIT, bool> SetRet;

const int MAX_NUM = 100000;

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
    SetInt S1;  // unique set of A
    SetInt S2;  // duplicated set of A

    for (int i = 0; i < N; i++) {
        int A;
        input(A);
        SetRet ret = S1.insert(A);
        if (ret.second == false) {
            // A already existed in S1
            S2.insert(A);
        }
    }

    if (T == 0) {
        answer = S2.size();
    } else {
        int a = *S1.begin() - T;
        int b = *S1.rbegin() - T;
        SetIntIT first = S1.lower_bound(min(a, b));
        SetIntIT last  = S1.upper_bound(max(a, b));

#if defined(CPP_IOSTREAM)
        if (debug) {
            cout << "--> Range(" << min(a, b)
                << ", " << max(a, b) << ") ==> A(";
            if (first == S1.end()) cout << "end"; else cout << *first;
            cout << ", ";
            if (last == S1.end()) cout << "end"; else cout << *last;
            cout << "]" << endl;
        }
#endif

        while (first != last) {
            if (S1.find(*first + T) != S1.end()) answer++;
            ++first;
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
