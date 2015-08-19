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
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<int> vint;

const unsigned int MAX_NUM = 10000000;
static vint prime;                          // not include 2: 3, 5, 7, 11 ...
static int debug;

//---------------------------------------------------------------------------

class Node {
  private:
    int _value;                             // 數值
    int _num;                               // 質因子個數
    bool _used;                             // 已取出過
  public:
    Node(int value, int num) : _value(value), _num(num), _used(false) {
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
    int value() const { return _value; }
    int num() const { return _num; }
    bool used() const { return _used; }
    void set_used() { _used = true; }
};

typedef Node * pNode;

bool operator< (const Node &x, const Node &y) {
    int sts = x.num() - y.num();
    return sts ? (sts < 0) : (x.value() < y.value());
}

struct cmpMax {
    bool operator() (const pNode &x, const pNode &y) {
        return operator< (*x, *y);
    }
};

struct cmpMin {
    bool operator() (const pNode &x, const pNode &y) {
        return operator< (*y, *x);
    }
};

typedef priority_queue<pNode, vector<pNode>, cmpMax> pqMax;
typedef priority_queue<pNode, vector<pNode>, cmpMin> pqMin;

class PQ_MaxMin {
  private:
    pqMax _pqMax;
    pqMin _pqMin;
  public:
    ~PQ_MaxMin();
    void push(int value, int num);
    void pop(int &maxValue, int &minValue);
};

PQ_MaxMin::~PQ_MaxMin()
{
    if (debug > 2) cout << ">>> ~PQ_MaxMin(): Release _pqMax()" << endl;
    while (!_pqMax.empty()) {
        pNode p = _pqMax.top();
        _pqMax.pop();
        if (p->used()) delete p; else p->set_used();
    }

    if (debug > 2) cout << endl << ">>> ~PQ_MaxMin(): Release _pqMin()" << endl;
    while (!_pqMin.empty()) {
        pNode p = _pqMin.top();
        _pqMin.pop();
        assert(p->used());
        delete p;
    }
}

void PQ_MaxMin::push(int value, int num)
{
    pNode p = new Node(value, num);
    _pqMax.push(p);
    _pqMin.push(p);
}

void PQ_MaxMin::pop(int &maxValue, int &minValue)
{
    maxValue = -1;
    while (!_pqMax.empty()) {
        pNode p = _pqMax.top();
        _pqMax.pop();
        if (!p->used()) {
            maxValue = p->value();
            p->set_used();
            break;
        }
        delete p;
    }
    assert(maxValue >= 0);

    minValue = -1;
    while (!_pqMin.empty()) {
        pNode p = _pqMin.top();
        _pqMin.pop();
        if (!p->used()) {
            minValue = p->value();
            p->set_used();
            break;
        }
        delete p;
    }
    assert(minValue >= 0);
}

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
static inline int MUL(int x, int y, int n)
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

    for (int i = 0, k = prime.size(); i < k; i++) {
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

static bool (*isPrime)(int) = is_prime;

static void run()
{
    prime.clear();
    prime.reserve(512);

    if (debug > 1) cout << "Primes: 2 ";
    for (int i = 3; (unsigned)i*i < MAX_NUM; i++) {
        if (isPrime(i)) {
            prime.push_back(i);
            if (debug > 1) cout << i << ' ';
        }
    }
    if (debug > 1) cout << endl << "--> "<< prime.size()+1 << endl << endl;

    PQ_MaxMin pq;
    int num;

    cin >> num;
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < 10; j++) {
            int value;
            cin >> value;
            assert(0 < value && value < MAX_NUM);

            if (debug) cout << "--> " << value << ":";
            int fpNum = factor_prime(value);
            if (debug) cout << " => " << fpNum << endl;

            pq.push(value, fpNum);
        }

        if (debug) cout << endl;

        int maxValue, minValue;
        pq.pop(maxValue, minValue);
        cout << maxValue << ' ' << minValue << endl;

        if (debug) cout << endl;
    }
}

int main (int argc, char *argv[])
{
    {
        int n = 1;
        while (n < argc && argv[n][0] == '-') {
            if (strncmp(argv[n], "-d", 2) == 0) {
                debug += strlen(&argv[n][1]);
            } else if (strncmp(argv[n], "-sprp", min(5, (int)strlen(argv[n]))) == 0) {
                isPrime = SPRP_is_prime;
            } else if (strncmp(argv[n], "-i:", 3) == 0) {
                ifstream in(&argv[n][3]);
                //streambuf *cinbuf = cin.rdbuf(); //save old buf
                cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
                run();
                //cin.rdbuf(cinbuf);
                return 0;
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

    run();

    return 0;
}

