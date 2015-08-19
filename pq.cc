/* a6.cc

   描述
    我们定义一个正整数a比正整数b优先的含义是：
    *a的质因数数目（不包括自身）比b的质因数数目多；
    *当两者质因数数目相等时，数值较大者优先级高。


    现在给定一个容器，初始元素数目为0，之后每次往里面添加10个元素，每次添加
    之后，要求输出优先级最高与最低的元素，并把该两元素从容器中删除。

   输入
    第一行: num (添加元素次数，num <= 30)
    下面10*num行，每行一个正整数n（n < 10000000).

   输出
    每次输入10个整数后，输出容器中优先级最高与最低的元素，两者用空格间隔。

   样例输入
    1
    10 7 66 4 5 30 91 100 8 9

   样例输出
    66 5

*/

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <cstring>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<int> vint;

const unsigned int MAX_NUM = 10000000;
static vint prime;                          // not include 2: 3, 5, 7, 11 ...
static int debug;

static int factor_prime(int n);
//---------------------------------------------------------------------------

class Factor_Prime {
  private:
    int _value;                             // 數值
    int _fp_num;                            // 質因子個數
  public:
    Factor_Prime(int value, int num) : _value(value), _fp_num(num) {}
    int value() const { return _value; }
    virtual bool operator< (const Factor_Prime &x) const {
        int sts = _fp_num - x._fp_num;
        return sts ? (sts < 0) : (_value < x._value);
    }
};

class Factor_Prime_Low: public Factor_Prime {
  public:
    Factor_Prime_Low(int value, int num) : Factor_Prime(value, num) {}
    bool operator< (const Factor_Prime &x) const {
        return x.Factor_Prime::operator< (*this);
    }
};
    
typedef priority_queue<Factor_Prime> pqFPH;
typedef priority_queue<Factor_Prime_Low> pqFPL;

//---------------------------------------------------------------------------

// (base ^ exp) mod modulus
template <typename T>
T modpow(T base, T exp, T modulus) {
  base %= modulus;
  T result = 1;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exp >>= 1;
  }
  return result;
}

// ( x * y) mod n
static int MUL(int x, int y, int n)
{
    long long z = (long long)x * y;
    return z % n;
}

static bool SPRP_is_prime(int n)
{
    // 預先判斷偶數與1，節省一點時間。
    if (n <= 2 || (n & 1) == 0) return n == 2;
 
    int u = n - 1, t = 0;
    while ((u & 1) == 0) {
        u >>= 1;
        t++;
    }
 
    // 推定是質數，就實施下一次測試；
    // 確定是合數，就馬上結束。
    int sprp[3] = {2, 7, 61};   // 足以涵蓋int32範圍
    for (int k = 0; k < 3; k++) {
        // a沒有大於1、小於n-1的限制，
        // 沒有測試意義的數字，當作是通過測試。
        // （a是質數時，模n後不會等於零，不必特別判斷。）
        int a = sprp[k] % n;
        if (a == 0 || a == 1 || a == n-1) continue;
 
        int x = modpow(a, u, n);
        if (x == 1 || x == n-1) continue;
 
        for (int i = 0; i < t-1; i++) {
            x = MUL(x, x, n);
            if (x == 1) return false;
            if (x == n-1) break;
        }
        if (x == n-1) continue;
 
        return false;
    }
    // 通過全部測試，確定是質數。
    return true;
}

//---------------------------------------------------------------------------

static bool is_prime(int n)
{
    if ((n & 1) == 0 || n < 2) return n == 2;
    for (int i = 0, k = prime.size(); i < k; i++) {
        int m = prime[i];
        if (m * m > n) break;
        if (n % m == 0) return false;
    }
    return true;
}

//---------------------------------------------------------------------------

// 求n的質因數個數: 252 = 2^2 x 3^3 x 7 => (2,3,7) => 252 有 3 個質因數
// 221 = 13 x 17 => 2個
static int factor_prime(int n)
{
    int count = 0;
    if ((n & 1) == 0) {     // even
        count++;
        int cnt = 1;
        while (n && ((n >>= 1) & 1) == 0) cnt++;
        if (debug) {
            cout << " 2";
            if (cnt > 1) cout << '^' << cnt;
        }
    }
    int i = 0;
    int k = prime.size();
    for (; i < k; i++) {
        int m = prime[i];
        if (m >= n) {
            if (m == n) break;
            return count;
        }
        if (n % m == 0) {
            count++;
            int cnt = 1;
            while ((n /= m) && n % m == 0) cnt++;
            if (debug) {
                cout << ' ' << m;
                if (cnt > 1) cout << '^' << cnt;
            }
        }
    }

    if (count) {    // 必須是因子才要計算, 本身是質數則不算
        count++;
        if (debug) cout << ' ' << n;
    }
    return count;
}

//---------------------------------------------------------------------------

int main (int argc, char *argv[])
{
    bool (*isPrime)(int) = is_prime;
    {
        int n = 1;
        while (n < argc && argv[n][0] == '-') {
            if (strncmp(argv[n], "-d", 2) == 0) {
                debug += strlen(&argv[n][1]);
            } else if (strncmp(argv[n], "-sprp", min(5, (int)strlen(argv[n]))) == 0) {
                isPrime = SPRP_is_prime;
            }
            n++;
        }
        if (n < argc) {
            while (n < argc) {
                int x = atoi(argv[n++]);
                cout << x << " is"
                    << (SPRP_is_prime(x) ? " " : " not ")
                    << "prime" << endl;
            }
            return 0;
        }
    }

    prime.reserve(512);
    for (int i = 3; (unsigned)i*i < MAX_NUM; i++) {
        if (isPrime(i)) {
            prime.push_back(i);
            if (debug > 1) cout << i << ' ';
        }
    }
    if (debug > 1) cout << endl << "--> "<< prime.size() << endl;

    pqFPH dataH;
    pqFPL dataL;
    int num;
    cin >> num;
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < 10; j++) {
            int x;
            cin >> x;
            assert(x < MAX_NUM);

            if (debug) cout << x << ":";
            int n = factor_prime(x);
            if (debug) cout << " --> " << n << endl;

            dataH.push(Factor_Prime(x, n));
            dataL.push(Factor_Prime_Low(x, n));
        }

        if (debug) cout << endl;
        cout << dataH.top().value() << ' ' << dataL.top().value() << endl;
        if (debug) cout << endl;

        dataH.pop();
        dataL.pop();
    }

    return 0;
}

