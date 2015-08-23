/* pq.cc -- tonychen [@] finenet.com.tw

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
#include <fstream>
#include <vector>
#include <queue>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cctype>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<unsigned int> vuint;

const unsigned int MAX_NUM = 10000000;

static vuint Primes;                        // not include 2: 3, 5, 7, 11 ...
static int debug;

//---------------------------------------------------------------------------

class Node {
  private:
    unsigned int _value;                    // 數值
    unsigned int _num;                      // 質因數個數
    bool _used;                             // 已取出過
  public:
    Node(unsigned int value, unsigned int num)
    : _value(value), _num(num), _used(false) {
        if (debug > 2) {
            cout << "--> Node(): " << this
                << " (" << _value
                << ", " << _num
                << ", " << _used
                << ")" << endl;
        }
    }
    ~Node() {
        if (debug > 2) {
            cout << "--> ~Node(): " << this
                << " (" << _value
                << ", " << _num
                << ", " << _used
                << ")" << endl;
        }
        assert(_used);
    }
    unsigned int value() const { return _value; }
    unsigned int num() const { return _num; }
    bool used() const { return _used; }
    void set_used() { _used = true; }
};

typedef Node * pNode;

bool operator< (const Node &x, const Node &y) {
    if (x.num() != y.num()) return x.num() < y.num();
    return x.value() < y.value();
}

class CmpMaxMin {
  private:
    bool _reverse;
  public:
    CmpMaxMin(bool reverse=false) : _reverse(reverse) {}
    bool operator() (const pNode &x, const pNode &y) const {
        return _reverse ? operator< (*y, *x) : operator< (*x, *y);
    }
};

typedef priority_queue<pNode, vector<pNode>, CmpMaxMin> pqMaxMin;

class PQ_MaxMin {
  private:
    pqMaxMin _pqMax;
    pqMaxMin _pqMin;
    bool get(pqMaxMin &pq, unsigned int &value);
  public:
    PQ_MaxMin() : _pqMin(CmpMaxMin(true)) {}
    ~PQ_MaxMin();
    void push(unsigned int value, unsigned int num);
    void pop(unsigned int &maxValue, unsigned int &minValue);
};

PQ_MaxMin::~PQ_MaxMin()
{
    if (debug > 2) cout << ">>> ~PQ_MaxMin(): Release _pqMax()" << endl;
    while (!_pqMax.empty()) {
        unsigned int value;
        get(_pqMax, value);
    }

    if (debug > 2) cout << endl << ">>> ~PQ_MaxMin(): Release _pqMin()" << endl;
    while (!_pqMin.empty()) {
        unsigned int value;
        get(_pqMin, value);
    }
}

void PQ_MaxMin::push(unsigned int value, unsigned int num)
{
    pNode p = new Node(value, num);
    _pqMax.push(p);
    _pqMin.push(p);
}

bool PQ_MaxMin::get(pqMaxMin &pq, unsigned int &value)
{
    while (!pq.empty()) {
        pNode p = pq.top();
        pq.pop();
        if (!p->used()) {
            p->set_used();
            value = p->value();
            return true;
        }
        delete p;
    }
    return false;
}

void PQ_MaxMin::pop(unsigned int &maxValue, unsigned int &minValue)
{
    assert(get(_pqMax, maxValue));
    assert(get(_pqMin, minValue));
}

//---------------------------------------------------------------------------

/*
template <typename T>
T modpow(T base, T exp, T modulus) {
  base %= modulus;
  T result = 1;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % modulus;    // <-- overflow
    base = (base * base) % modulus;                     // <-- overflow
    exp >>= 1;
  }
  return result;
}
*/

// (a * b) % n
static uint64_t MULMOD(uint64_t a, uint64_t b, uint64_t n) {
    if (a < b) swap(a, b);
    if (a <= UINT64_MAX / b) return a * b % n;
    uint64_t result = 0;
    while (b) {
        if (b & 1) {
            result += a;
            if (result < a || result >= n) result -= n;
        }
        uint64_t t = a;
        a += a;
        if (a < t || a >= n) a -= n;
        b >>= 1;
    }
    return result;
}

// (base ^ exp) mod modulus
static uint64_t POWMOD(uint64_t base, uint64_t exp, uint64_t modulus) {
  base %= modulus;
  uint64_t result = 1;
  while (exp > 0) {
    if (exp & 1) result = MULMOD(result, base, modulus);
    base = MULMOD(base, base, modulus);
    exp >>= 1;
  }
  return result;
}

static bool SPRP_is_prime(unsigned int n)
{
    // 預先判斷偶數與1, 節省一點時間
    if (n <= 2 || (n & 1) == 0) return n == 2;

    unsigned int u = n - 1, t = 0;
    while ((u & 1) == 0) {
        u >>= 1;
        t++;
    }

    // http://miller-rabin.appspot.com/
    // 2^64 (7 bases): {2, 325, 9375, 28178, 450775, 9780504, 1795265022}

    // 推定是質數，就實施下一次測試
    // 確定是合數，就馬上結束
    unsigned int sprp[3] = {2, 7, 61};   // 足以涵蓋 2^32 範圍
    for (int k = 0; k < 3; k++) {
        // a沒有大於1、小於n-1的限制
        // 沒有測試意義的數字，當作是通過測試
        // a是質數時, 除以n的餘數一定不是零, 不必特別判斷
        unsigned int a = sprp[k] % n;
        if (a == 0 || a == 1 || a == n-1) continue;

        unsigned int x = POWMOD(a, u, n);
        if (x == 1 || x == n-1) continue;

        for (int i = 0; i < t-1; i++) {
            x = MULMOD(x, x, n);
            if (x == 1) return false;
            if (x == n-1) break;
        }
        if (x == n-1) continue;

        return false;
    }

    // 通過全部測試，確定是質數
    return true;
}

//---------------------------------------------------------------------------

static bool is_prime(unsigned int n)
{
    if ((n & 1) == 0 || n < 2) return n == 2;
    for (int i = 0, k = Primes.size(); i < k; i++) {
        unsigned int m = Primes[i];
        if (m * m > n) break;
        if (n % m == 0) return false;
    }
    return true;
}

//---------------------------------------------------------------------------

// 將 n 做質因數分解, 返回質因數個數:
// 252 = 2^2 × 3^3 × 7 => (2,3,7) => 252 有 3 個質因數
// 221 = 13 × 17 => 2個質因數
// 開啟除錯模式(執行參數 -d)可輸出分解的質因數
static unsigned int prime_factoring(unsigned int n)
{
    unsigned int count = 0;
    if ((n & 1) == 0) {     // even
        if (n == 2) return 0;
        count++;
        unsigned int cnt = 1;
        while (n && ((n >>= 1) & 1) == 0) cnt++;
        if (debug) {
            cout << " 2";
            if (cnt > 1) cout << '^' << cnt;
        }
    }

    for (unsigned int i = 0, k = Primes.size(); i < k; i++) {
        unsigned int m = Primes[i];
        if (m >= n) {
            if (m == n) break;
            return count;
        }
        if (n % m == 0) {
            count++;
            unsigned int cnt = 1;
            while ((n /= m) && n % m == 0) cnt++;
            if (debug) {
                cout << (count == 1 ? " " : " × ") << m;
                if (cnt > 1) cout << '^' << cnt;
            }
        }
    }

    if (count) {    // 必須是因子才要計數, 本身是質數則不算
        count++;
        if (debug) cout << " × " << n;
    }
    return count;
}

//---------------------------------------------------------------------------

static bool (*isPrime)(unsigned int) = is_prime;

static void fill_primes(unsigned int max_num)
{
    Primes.clear();
    Primes.reserve(max_num / 10);

    if (debug > 1) cout << "Primes: 2 ";
    for (unsigned int i = 3; i <= max_num / i; i++) {
        if (isPrime(i)) {
            Primes.push_back(i);
            if (debug > 1) cout << i << ' ';
        }
    }
    if (debug > 1) cout << endl << "--> "<< Primes.size()+1 << endl << endl;
}

static void run(unsigned int max_num)
{
    if (Primes.size() == 0) fill_primes(max_num);

    PQ_MaxMin pq;
    unsigned int num;

    cin >> num;
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < 10; j++) {
            unsigned int value;
            cin >> value;
            assert(value < max_num);

            if (debug) cout << "--> " << value << ":";
            unsigned int fpNum = prime_factoring(value);
            if (debug) cout << " => " << fpNum << endl;

            pq.push(value, fpNum);
        }

        if (debug) cout << endl;

        unsigned int maxValue, minValue;
        pq.pop(maxValue, minValue);
        cout << maxValue << ' ' << minValue << endl;

        if (debug) cout << endl;
    }
}

int main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    unsigned int max_num = MAX_NUM;
    {
        int n = 1;
        while (n < argc && argv[n][0] == '-') {
            if (strncmp(argv[n], "-d", 2) == 0) {
                debug += strlen(&argv[n][1]);
            } else if (strncmp(argv[n], "-s", 2) == 0 ||
                       strncmp(argv[n], "-p", 2) == 0) {
                isPrime = SPRP_is_prime;
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
        if (n < argc) {     // testing whether a number is prime
            fill_primes(UINT_MAX);
            debug = 1;
            while (n < argc) {
                unsigned int x = atoi(argv[n++]);
                if (SPRP_is_prime(x)) {
                    cout << x << " is prime" << endl;
                    assert(prime_factoring(x) == 0);
                } else {
                    cout << x << ":";
                    int count = prime_factoring(x);
                    cout << " (" << count << ")" << endl;
                }
            }
            return 0;
        }
    }

    run(max_num);

    return 0;
}
