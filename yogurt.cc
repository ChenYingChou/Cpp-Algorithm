/* yogurt.cc: tonychen [@] finenet.com.tw

   描述
    The cows have purchased a yogurt factory that makes world-famous Yucky
    Yogurt. Over the next N (1 <= N <= 10,000) weeks, the price of milk and
    labor will fluctuate weekly such that it will cost the company C_i
    (1 <= C_i <= 5,000) cents to produce one unit of yogurt in week i.
    Yucky's factory, being well-designed, can produce arbitrarily many units
    of yogurt each week.

    Yucky Yogurt owns a warehouse that can store unused yogurt at a constant
    fee of S (1 <= S <= 100) cents per unit of yogurt per week. Fortuitously,
    yogurt does not spoil. Yucky Yogurt's warehouse is enormous, so it can
    hold arbitrarily many units of yogurt.

    Yucky wants to find a way to make weekly deliveries of Y_i
    (0 <= Y_i <= 10,000) units of yogurt to its clientele (Y_i is the delivery
    quantity in week i). Help Yucky minimize its costs over the entire N-week
    period. Yogurt produced in week i, as well as any yogurt already in
    storage, can be used to meet Yucky's demand for that week.

   输入
    Line 1: Two space-separated integers, N and S.
    Lines 2..N+1: Line i+1 contains two space-separated integers: C_i and Y_i.

   输出
    Line 1: Line 1 contains a single integer: the minimum total cost to
    satisfy the yogurt schedule. Note that the total might be too large for
    a 32-bit integer.

   样例输入
    4 5
    88 200
    89 400
    97 300
    91 500

   样例输出
    126900

   提示
    In week 1, produce 200 units of yogurt and deliver all of it.
    In week 2, produce 700 units: deliver 400 units while storing 300 units.
    In week 3, deliver the 300 units that were stored.
    In week 4, produce and deliver 500 units.

 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

class Weekly;

typedef vector<int> vint;
typedef unsigned long long ULL;
typedef vector<Weekly *> vWeekly;

static int debug;

class Weekly {      // 每週費用及要求運送量
private:
    int _cost;          // 生產一單位優格所需費用
    int _deliver;       // 運送優格的單位量

public:
    Weekly(int cost, int deliver): _cost(cost), _deliver(deliver) { }
    int cost() const { return _cost; }
    int deliver() const { return _deliver; }
};

class Schedule {
private:
    vWeekly _w;         // 每週的每單位優格生產費用及運送量
    int _s;             // S: 每週儲存一單位優格的倉儲費用(定量值)

public:
    Schedule(int S) : _s(S) { }
    ~Schedule() {
        for (int i = 0; i < _w.size(); i++) {
            delete _w[i];
        }
        _w.clear();
    }
    void store(int cost, int deliver);
    void output(const char *title) const;
    ULL optimize();
};

void Schedule::store(int cost, int deliver)
{
    _w.push_back(new Weekly(cost, deliver));
}

void Schedule::output(const char *title) const
{
    if (title) cout << title << ' ';
    cout << "weekly store fee of " << _s << " cents per unit" << endl;
    for (vWeekly::const_iterator it = _w.begin(); it != _w.end(); ++it) {
        const Weekly &w = *(*it);
        cout << "(" << w.cost() << ", " << w.deliver() << ")" << endl;
    }
}

ULL Schedule::optimize()
{
    ULL total_cost = 0;     // 至目前為止最低累計成本

    vWeekly::iterator it = _w.begin();
    while (it != _w.end()) {
        Weekly &w = *(*it);
        int cost = w.cost();        // 本週每單位生產成本

        // 本週出貨的成本
        total_cost += cost * w.deliver();
        if (debug) {
            cout << "Total cost: " << total_cost
                << ", deliver: " << w.deliver()
                << ", cost: " << cost 
                << endl;
        }

        // 檢查往後幾週的生產成本是否高於使用本週的單位成本+倉儲成本
        // 若是則使用這個新的單位成本
        while (++it != _w.end()) {
            cost += _s;             // 累計每單位成本: 每週增加倉儲成本

            Weekly &w1 = *(*it);
            if (cost >= w1.cost()) break;

            // 使用 w 這週的單位成本(已含倉儲成本)
            total_cost += cost * w1.deliver();
            if (debug) {
                cout << "Total cost: " << total_cost
                    << ", deliver: " << w1.deliver()
                    << ", cost: " << w1.cost() 
                    << " <-- last cost(+S): " << cost
                    << endl;
            }
        }
    }

    return total_cost;
}

//---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    for (int n = 1; n < argc; n++) {
        if (strncmp(argv[n], "-d", 2) == 0) {
            debug += strlen(&argv[n][1]);
        }
    }

    int N;       // 共有多少週
    int S;       // 每週儲存一單位優格的倉儲費用(定量值)

    cin >> N >> S;
    assert(1 <= N && N <= 10000);
    assert(1 <= S && S <= 100);
    
    Schedule YY(S);
    for (int i = 0; i < N; i++) {
        int C, Y;
        cin >> C >> Y;
        assert(1 <= C && C <= 5000);
        assert(0 <= Y && Y <= 10000);
        YY.store(C, Y);
    }

    if (debug) YY.output("Schedule:");

    ULL answer = YY.optimize();

    if (debug) cout << "--> ";
    cout << answer << endl;

    return 0;
}

