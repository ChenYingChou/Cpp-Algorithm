/* fight.cc

   描述
    为了迎接08年的奥运会，让大家更加了解各种格斗运动，facer新开了一家热血格
    斗场。格斗场实行会员制，但是新来的会员不需要交入会费，而只要同一名老会员
    打一场表演赛，证明自己的实力。

    我们假设格斗的实力可以用一个正整数表示，成为实力值。另外，每个人都有一个
    唯一的id，也是一个正整数。为了使得比赛更好看，每一个新队员都会选择与他实
    力最为接近的人比赛，即比赛双方的实力值之差的绝对值越小越好，如果有两个人
    的实力值与他差别相同，则他会选择比他弱的那个（显然，虐人必被虐好）。

    不幸的是，Facer一不小心把比赛记录弄丢了，但是他还保留着会员的注册记录。
    现在请你帮facer恢复比赛纪录，按照时间顺序依次输出每场比赛双方的id。

   输入
    第一行一个数n(0 < n <=100000)，表示格斗场新来的会员数（不包括facer）。
    以后n行每一行两个数，按照入会的时间给出会员的id和实力值。一开始，facer
    就算是会员，id为1，实力值1000000000。输入保证两人的实力值不同。

   输出
    N行，每行两个数，为每场比赛双方的id，新手的id写在前面。

   样例输入
    3
    2 1
    3 3
    4 2

   样例输出
    2 1
    3 2
    4 2

   提示:
    C++ I/O stream 在大量資料時速度比起 stdin/stdout 慢了十倍, 為避免 Time Limit
    只好改用 scanf/printf。

 */

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>
#include <limits>
#include <cstring>
#include <cstdio>
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

typedef map<int, int> map_atk;
typedef pair<int, int> pair_atk;                // first: atk, second: id
typedef pair<map_atk::iterator, bool> ret_atk;

const int MAX_NUM = 100000;

static int debug;

//---------------------------------------------------------------------------

string int2str(int value)
{
    ostringstream os;
    os << value;
    return os.str();
}

//---------------------------------------------------------------------------

struct GetFirst {
    template <class First, class Second>
    First& operator()(std::pair<First, Second>& p) {
        return p.first;
    }

    template <class First, class Second>
    const First& operator()(const std::pair<First, Second>& p) {
        return p.first;
    }
};

struct GetSecond {
    template <class First, class Second>
    Second& operator()(std::pair<First, Second>& p) {
        return p.second;
    }

    template <class First, class Second>
    const Second& operator()(const std::pair<First, Second>& p) {
        return p.second;
    }
};

class Fight {
private:
    map_atk _atk;                               // key: atk, value: id

    int Atk(map_atk::iterator it) { return it->first; }
    int Id(map_atk::iterator it) { return it->second; }
public:
    Fight() {
        _atk.insert(pair_atk(1000000000, 1));
    }
    int add(int id, int atk);
};

int Fight::add(int id, int atk)
{
//    GetFirst GetAtk;
//    GetSecond GetId;
    GetFirst GetIterator;
    GetSecond GetResult;

    ret_atk ret = _atk.insert(pair_atk(atk, id));
    map_atk::iterator t1 = GetIterator(ret); // ret.first;
    if (GetResult(ret) == false) { // ret.second == false
        ostringstream os;
        os << "Member #" << id << " and #" << Id(t1) //GetId(*t1)
            <<" have same attack:" << atk;
        throw runtime_error(os.str());
    }

    int opponent_id = 0;
    int dist = std::numeric_limits<int>::max();;
    if (t1 != _atk.begin()) {
        map_atk::iterator t0 = t1; --t0;
        dist = atk - Atk(t0); //GetAtk(*t0);
        opponent_id = Id(t0); //GetId(*t0);
    }

    if (++t1 != _atk.end()) {
//      if (GetAtk(*t1) - atk < dist) opponent_id = GetId(*t1);
        if (Atk(t1) - atk < dist) opponent_id = Id(t1);
    }

    return opponent_id;
}

//---------------------------------------------------------------------------

static void run()
{
    int n;
#if defined(CPP_IOSTREAM)
    cin >> n;
#else
    scanf("%d", &n);
#endif
    assert(0 < n && n <= MAX_NUM);

    Fight facer;
    for (int i = 0; i < n; i++) {
        int id, atk;
#if defined(CPP_IOSTREAM)
        cin >> id >> atk;
#else
        scanf("%d%d", &id, &atk);
#endif
        int opponent = facer.add(id, atk);
#if defined(CPP_IOSTREAM)
        cout << id << ' ' << opponent << '\n'; //endl;
#else
        printf("%d %d\n", id, opponent);
#endif
    }
}

int main (int argc, char *argv[])
{
#if defined(CPP_IOSTREAM)
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#endif
    {
        int n = 1;
        while (n < argc && argv[n][0] == '-') {
            if (strncmp(argv[n], "-d", 2) == 0) {
                debug += strlen(&argv[n][1]);
#if defined(CPP_IOSTREAM)
            } else if (strncmp(argv[n], "-i:", 3) == 0) {
                ifstream in(&argv[n][3]);
                //streambuf *cinbuf = cin.rdbuf(); //save old buf
                cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
                run();
                //cin.rdbuf(cinbuf);
                return 0;
#endif
            }
            n++;
        }
    }

    run();

    return 0;
}
