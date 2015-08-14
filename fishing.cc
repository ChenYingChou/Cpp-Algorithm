/* fishing -- tonychen@finenet.com.tw

   描述
    John is going on a fishing trip. He has h hours available (1 <= h <= 16),
    and there are n lakes in the area (2 <= n <= 25) all reachable along a
    single, one-way road. John starts at lake 1, but he can finish at any lake
    he wants. He can only travel from one lake to the next one, but he does
    not have to stop at any lake unless he wishes to. For each i = 1,...,n - 1,
    the number of 5-minute intervals it takes to travel from lake i to lake i+1
    is denoted ti (0 < ti <=192). For example, t3 = 4 means that it takes 20
    minutes to travel from lake 3 to lake 4. To help plan his fishing trip,
    John has gathered some information about the lakes. For each lake i, the
    number of fish expected to be caught in the initial 5 minutes, denoted fi
    ( fi >= 0 ), is known. Each 5 minutes of fishing decreases the number of
    fish expected to be caught in the next 5-minute interval by a constant rate
    of di (di >= 0). If the number of fish expected to be caught in an interval
    is less than or equal to di , there will be no more fish left in the lake
    in the next interval. To simplify the planning, John assumes that no one
    else will be fishing at the lakes to affect the number of fish he expects
    to catch.

    Write a program to help John plan his fishing trip to maximize the number
    of fish expected to be caught. The number of minutes spent at each lake
    must be a multiple of 5.

   输入
    You will be given a number of cases in the input. Each case starts with a
    line containing n. This is followed by a line containing h. Next, there is
    a line of n integers specifying fi (1 <= i <=n), then a line of n integers
    di (1 <=i <=n), and finally, a line of n - 1 integers ti (1 <=i <=n - 1).
    Input is terminated by a case in which n = 0.

   输出
    For each test case, print the number of minutes spent at each lake,
    separated by commas, for the plan achieving the maximum number of fish
    expected to be caught (you should print the entire plan on one line even
    if it exceeds 80 characters). This is followed by a line containing the
    number of fish expected.

    If multiple plans exist, choose the one that spends as long as possible
    at lake 1, even if no fish are expected to be caught in some intervals.
    If there is still a tie, choose the one that spends as long as possible
    at lake 2, and so on. Insert a blank line between cases.

   样例输入
    2
    1
    10 1
    2 5
    2
    4
    4
    10 15 20 17
    0 3 4 3
    1 2 3
    4
    4
    10 15 50 30
    0 3 4 3
    1 2 3
    0

   样例输出
    45, 5
    Number of fish expected: 31

    240, 0, 0, 0
    Number of fish expected: 480

    115, 10, 50, 35
    Number of fish expected: 724

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    是不是要用 Dynamic Programming?

    F(n, t) = max(
                G( n , t) + F(n+1, n - TG( n , t) - T(n)) ,
                G(n-1, t) + F(n+1, n - TG(n-1, t) - T(n)) ,
                G(n-2, t) + F(n+1, n - TG(n-2, t) - T(n)) ,
                ...
                G( 1 , t) + F(n+1, n - TG( 1 , t) - T(n)) ,
                G( 0 , t) + F(n+1, n - TG( 0 , t) - T(n)) -----+
                                                               | 
                   0      + F(n+1, n -     0      - T(n)) <----+
            )

     G(n,t): 在湖(n)最多花 t 單位時間所釣到的魚數量
       T(n): 從湖(n)到湖(n+1)花的單位時間
    TG(n,t): 在湖(n)花的單位時間, 最多花 t 單位時間, 或直到魚獲期望值為零

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    思路：暴力＋贪心。要釣到更多的鱼。就要尽可能利用每一分每一秒。所以人从第
    一个池塘开始肯定是往后走不回头的。

    所以我们只要枚举每一个区间, 1~1, 1~2, 1~3 ... 1~n 的情况，每个区间可以用
    的时间为：总时间減路程花费时间。找出其中最大的情况。在每种情况在用贪心去
    求。贪心的策略是：一个个单位时间去考虑，每次去找最大可以钓到的鱼数。这样
    到最后一定是钓得最多的鱼。

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    解析：由于是从第一个湖出发的，而且所有的湖都是一字排开的，所以只需枚举他
    走过的湖泊数X即可。即先假设他从[湖1]走到[湖X]，则路上总共花了:

        T = T1 + T2 + T3 + … + Tx

    在这个前提下，就可以认为他有能力在1~X 之间的任何两个湖之间"瞬移"，即在任
    一时刻可以任选一个1~X中的湖钓鱼。 想一想为什么？其实这跟汽车加油的道理是
    一样的，在每个湖的钓鱼顺序可以不是依次来的，你可能认为总时间肯定比这个花
    得多，其实不是的，顺序是不影响结果的。因为假如我要先去[湖1]钓5分钟，接着
    去[湖2]钓5分钟，再接着回来[湖1]钓5分钟，这个过程其实相当于先在[湖1]钓5+5
    =10分钟，然后再去[湖2]钓5分钟。 因此只需一直贪心的选择当前能钓到鱼最多的
    湖即可。还有就是贪心选择的时候，若有相同的湖时，优先选择编号较小的湖。

 */

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
#include <cstring>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<int> vint;

const int T_INTERVAL = 5;   // 單位時間: 5分鐘
const int T_MAX = numeric_limits<int>::max() / T_INTERVAL;

static int debug;

static void output_list(const char *title, const vint &x, const int mul=1);

//---------------------------------------------------------------------------

class Lake;

typedef Lake *PLake;

class Lake {
  private:
    int _id;                // 湖的編號(比較用)
    int _fishes;            // 初始一單位時間漁獲期望值
    int _reduce;            // 每單位時間漁獲期望值減少量
    int _trip_next;         // 到下個湖花的單位時間
  public:
    Lake(const Lake &x) { *this = x; }
    Lake(const PLake p) { *this = *p; }
    Lake(int id, int fishes, int reduce, int trip_next) :
        _id(id), _fishes(fishes), _reduce(reduce), _trip_next(trip_next) { }
    void set_trip_next(int ntime) { _trip_next = ntime; }
    int id() const { return _id; }
    int fishes() const { return _fishes; }
    int reduce() const { return _reduce; }
    int trip_next() const { return _trip_next; }
    int max_consume_time() const;           // 返回最大消耗時間單位(直到漁獲期望值為零)
    int estimate_time(int fishes) const;    // 返回單位時間(消耗漁獲期望值到fishes)
    int consume_time(int n=1);              // 返回漁獲量(在n單位時間)
    string str() const;
};

string Lake::str() const
{
    stringstream os;
    os << "Lake[" << _id << "]=(fishes:" << _fishes 
        << ", reduce:" << _reduce
        << ", next:" << _trip_next
        << ")";
    return os.str();
}

// 返回最大消耗時間單位(直到漁獲期望值為零)
// 最後不足 _reduce 個也要算一個單位
int Lake::max_consume_time() const
{
    return _fishes == 0 ? 0 :
            (_reduce == 0 ? T_MAX : (_fishes+_reduce-1) / _reduce);
}

// 消耗漁獲期望值小於等於fishes的單位時間
int Lake::estimate_time(int fishes) const
{
    // _fishes - N x _reduce = fishes
    // N = (_fishes - fishes) / _reduce
    return _fishes < fishes ? 0 :
            (_reduce == 0 ? T_MAX : (_fishes-fishes+_reduce-1) / _reduce);
}

// 在n單位時間的漁獲量, 漁獲量最多累加到漁獲期望值為零止
int Lake::consume_time(int n)
{
    if (_reduce == 0) return n * _fishes;

    int k = (_fishes + _reduce - 1) / _reduce;
    if (k > n) k = n;

    // _fishes + (_fishes-_reduce) + ... + (_fishes - (k-1)*_reduce)
    // => k * _fishes - Σ(1..k-1) * _reduce
    // => k * _fishes - (k-1) * k / 2 * _reduce
    int fs = k * _fishes - (k * (k-1) / 2) * _reduce;

    _fishes -= k * _reduce;
    if (_fishes < 0) _fishes = 0;

    return fs;
}

// 湖與湖的小於比較: 漁獲量要由小到大, 若相同則比序號由大到小(相反)
// 於 priority queue 比較時使用(見 Cmp_PLake)
bool operator< (const Lake &a, const Lake &b)
{
    int sts = a.fishes() - b.fishes();
    if (sts < 0) return true;
    return sts > 0 ? false : (b.id() < a.id());
}

//---------------------------------------------------------------------------

class Cmp_PLake {
  public:
    // 湖和湖的比較排序, 用於 priority queue 湖的物件指標比較
    bool operator() (const PLake a, const PLake b) {
        return operator< (*a, *b);
    }
};

typedef vector<PLake> vPLake;
typedef priority_queue<PLake, vPLake, Cmp_PLake> pqPLake;
 
class Fishing {
private:
    vPLake _lake;           // 湖的陣列指標(不是物件, 所以比較時要再包裝一次)
public:
    Fishing(int capacity) { _lake.reserve(capacity); }
    Fishing() { }
    ~Fishing();
    void add_lake(int fishes, int reduce, int next_trip);
    void go(int ntime);
};

Fishing::~Fishing()
{
    for (vPLake::iterator it = _lake.begin(); it != _lake.end(); ++it) {
        PLake p = *it;
        delete p;
    }
    _lake.clear();
}

void Fishing::add_lake(int fishes, int reduce, int next_trip)
{
    _lake.push_back(new Lake(_lake.size(), fishes, reduce, next_trip));
}

// ntime: 有多少單位時間
void Fishing::go(int ntime)
{
    int N = _lake.size();
    if (N == 0) {
        cout << "No lake to fishing!" << endl;
        return;
    }

    if (debug) cout << "Fishing(" << ntime << ")" << endl;

    int max_fishes = 0;
    vint max_stay(N, 0);
    max_stay[0] = ntime;

    // 累計到每一個湖時減掉在該湖可能最大的耗時單位
    // 含上個湖到這個湖時間 + 這個湖在用盡漁獲期望值時間
    // 直到零或負值時才開始用貪心法計算這回的最大漁獲量
    int nstart = ntime;

    for (int k = 0; k < N; k++) {
        // ntime: 到第k個湖時還剩下的時間單位(僅扣除掉湖到湖的時間)
        if (k > 0) {
            ntime -= _lake[k-1]->trip_next();
            if (ntime <= 0) break;
        }

        if (nstart > 0) {
            // 減掉在這個湖的漁獲期望值到零所耗費單位時間
            nstart -= _lake[k]->max_consume_time();

            if (nstart > 0) {
                // 減掉到下個湖的行程單位時間, 最後一個湖已無下一站, 不用比
                // 問題: 當時間夠到下個湖, 且該湖的漁獲期望值是零, 則應從這
                // 個湖開始計算起, 而不是下個湖起算, 否則時間分配會不符。
                if (_lake[k]->trip_next() > 0 && _lake[k+1]->fishes() > 0) {
                    nstart -= _lake[k]->trip_next();
                    if (nstart > 0) continue;   // 還有多的時間, 繼續到下個湖去
                } else {
                    // 最後一個湖, 或下個湖的漁獲期望值為零, 則從此湖起算
                    nstart = 0;
                }

                // 已到最後一個湖, 或剩下時間不夠到下個湖的行程時間
                // 從這個湖起才開始計算
            }
        }

        if (debug) cout << "Greedy(" << ntime << "): 0~" << k << endl;

        //
        // 以貪心法計算 _lake[0~k] 最大漁獲量
        //
        vint stay(N, 0);        // 每個湖釣魚單位時間
        int fishes = 0;         // 本次漁獲量
        int tn = ntime;         // 本次可用時間
        pqPLake Q;              // PQ: 每次從漁獲期望值最大者取出

        for (int i = 0; i <= k; i++) {
            Lake &a = *(_lake[i]);
            if (a.fishes() > 0) {
                if (debug > 1) cout << "Q.push: " << a.str() << endl;
                Q.push(new Lake(a));
            }
        }

        while (tn > 0 && !Q.empty()) {
            Lake &a = *Q.top();
            Q.pop();

            int t;  // 本次在 Lake(a) 可消耗時間
            if (Q.empty()) {
                t = a.max_consume_time();
            } else {
                Lake &b = *Q.top();
                // Lake(a) 還要多少時間漁獲期望值才會低於 Lake(b) (第2高值)
                // 如果相等, 還是要消耗 1 個單位時間
                t = max(1, a.estimate_time(b.fishes()));
            }

            if (debug > 1) {
                cout << "T(" << tn << ",";
                if (t == T_MAX) cout << "TMAX"; else cout << t;
                cout << ") Q.top: " << a.str() << endl;
            }

            if (t > tn) t = tn;
            assert(t > 0);

            int id = a.id();
            assert(0 <= id && id <= k);
            stay[id] += t;                  // 增加 Lake(a) 釣魚時間
            fishes += a.consume_time(t);    // Lake(a) 在 t 單位時間的漁獲量

            // 此次消耗掉 t 單位時間
            tn -= t;
            if (tn <= 0 || a.fishes() <= 0) {
                delete &a;
            } else {
                if (debug > 1) cout << "Q.push: " << a.str() << endl;
                Q.push(&a);
            }
        }

        // 若還有剩下時間則一律用在第一個湖垂釣
        if (tn) {
            stay[0] += tn;
            if (debug > 1) {
                cout << "Unspent time(" << tn
                    << ") add to Lake[0]" << endl;
            }
        }

        // 清除 Q[]
        while (!Q.empty()) {
            PLake p = Q.top();
            Q.pop();
            delete p;
        }

        if (debug) {
            cout << "--> " << fishes;
            if (fishes > max_fishes) cout << " <<< MAXIMUM";
            output_list(", Spent", stay, T_INTERVAL);
        }

        // 記錄最大魚獲量及每個湖釣魚所花的時間
        if (fishes > max_fishes) {
            max_fishes = fishes;
            max_stay = stay;
        }
    }

    output_list(debug ? "Spent" : nullptr, max_stay, T_INTERVAL);
    cout << "Number of fish expected: " << max_fishes << endl;
}

//---------------------------------------------------------------------------

static vint input_list(int n)
{
    vint x;
    x.resize(n);
    for (int i = 0; i < n; i++) cin >> x[i];
    return x;
}

static void output_list(const char *title, const vint &x, const int mul)
{
    if (title) cout << title << ": (";
    for (vint::const_iterator it = x.begin(); it != x.end(); ++it) {
        if (it != x.begin()) cout << ", ";
        cout << mul * (*it);
    }
    if (title) cout << ")";
    cout << endl;
}

int main(int argc, char *argv[])
{
    for (int n = 1; n < argc; n++) {
        if (strncmp(argv[n], "-d", 2) == 0) {
            debug += strlen(&argv[n][1]);
        }
    }

    int loopCount = 0;
    int N;               // 有幾個湖
    while (!(cin >> N).eof() && N > 0) {
        assert(2 <= N && N <= 25);
        if (++loopCount > 1) cout << endl;

        int H;          // 有多少小時
        cin >> H;
        assert(1 <= H && H <= 16);
        int ntime = H * 60 / T_INTERVAL;

        vint F = input_list(N);     // 開始第一單位時間期望釣到魚的數量
        vint D = input_list(N);     // 每過一單位時間釣到魚的減少數量
        vint T = input_list(N-1);   // 從一個湖到下個湖所花的單位時間
        T.push_back(0);

        if (debug) {
            output_list(" Expected", F);
            output_list(" Decrease", D);
            output_list("Trip Next", T);
            cout << endl;
        }

        Fishing plan;
        for (int i = 0; i < N; i++) {
            plan.add_lake(F[i], D[i], T[i]);
        }

        F.clear();
        D.clear();
        T.clear();

        // ntime 單位時間為5分鐘
        plan.go(ntime);
    }

    return 0;
}

