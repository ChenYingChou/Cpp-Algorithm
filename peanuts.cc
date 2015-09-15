/* peanuts.cc -- tonychen [@] finenet.com.tw

   描述
    Mr. Robinson and his pet monkey Dodo love peanuts very much. One day
    while they were having a walk on a country road, Dodo found a sign by
    the road, pasted with a small piece of paper, saying "Free Peanuts Here!"
    You can imagine how happy Mr. Robinson and Dodo were.

    There was a peanut field on one side of the road. The peanuts were
    planted on the intersecting points of a grid as shown in Figure-1. At
    each point, there are either zero or more peanuts. For example, in
    Figure-2, only four points have more than zero peanuts, and the numbers
    are 15, 13, 9 and 7 respectively. One could only walk from an intersection
    point to one of the four adjacent points, taking one unit of time. It
    also takes one unit of time to do one of the following: to walk from the
    road to the field, to walk from the field to the road, or pick peanuts
    on a point.
                -----------------------------------------------
                          S       E     R O A D
                          |       ^
                ----------|-------|----------------------------
                          |       |
                  1 +----+|---+---|+----+----+----+
                    |    ||   |   ||    |    |    |
                    |    ||   |   ||    |    |    |
                  2 +----+|---+---|+---^13 --+----+
                    |    ||   |   ||   |||   |    |
                    |    ||   |   ||   |||   |    |
                  3 +----+|---+---|+---|+|---+--- 7
                    |    ||   |   ||   |||   |    |
                    |    |v_______|____|||   |    |
                  4 +--- 15 ------|-----+|---+----+
                    |    |    |   ||    ||   |    |
                    |    |    |   ||,____|   |    |
                  5 +----+----+--- 9 ---+----+----+
                    |    |    |    |    |    |    |
                    |    |    |    |    |    |    |
                  6 +----+----+----+----+----+----+
                    1    2    3    4    5    6    7   Figure-2

    According to Mr. Robinson's requirement, Dodo should go to the plant
    with the most peanuts first. After picking them, he should then go to
    the next plant with the most peanuts, and so on. Mr. Robinson was not
    so patient as to wait for Dodo to pick all the peanuts and he asked Dodo
    to return to the road in a certain period of time. For example, Dodo
    could pick 37 peanuts within 21 units of time in the situation given in
    Figure-2.

    Your task is, given the distribution of the peanuts and a certain period
    of time, tell how many peanuts Dodo could pick. You can assume that each
    point contains a different amount of peanuts, except 0, which may appear
    more than once.

   输入
    The first line of input contains the test case number T (1 <= T <= 20).
    For each test case, the first line contains three integers, M, N and K
    (1 <= M, N <= 50, 0 <= K <= 20000). Each of the following M lines contain
    N integers. None of the integers will exceed 3000. (M * N) describes the
    peanut field. The j-th integer X in the i-th line means there are X
    peanuts on the point (i, j). K means Dodo must return to the road in K
    units of time.

   输出
    For each test case, print one line containing the amount of peanuts Dodo
    can pick.

   样例输入
    2
    6 7 21
    0 0 0 0 0 0 0
    0 0 0 0 13 0 0
    0 0 0 0 0 0 7
    0 15 0 0 0 0 0
    0 0 0 9 0 0 0
    0 0 0 0 0 0 0
    6 7 20
    0 0 0 0 0 0 0
    0 0 0 0 13 0 0
    0 0 0 0 0 0 7
    0 15 0 0 0 0 0
    0 0 0 9 0 0 0
    0 0 0 0 0 0 0

   样例输出
    37
    28

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

const int MAX_NUM = 20000;

static int debug;

//---------------------------------------------------------------------------

class Node {
  private:
    int _x, _y;
    int _value;
  public:
    Node(int x, int y, int value) : _x(x), _y(y), _value(value) {}
    int x() const { return _x; }
    int y() const { return _y; }
    int value() const { return _value; }
    bool operator< (const Node &a) const { return _value < a._value; }
    int to_road() const { return _y; }
    int distance(const Node &a) const { return abs(a._x-_x) + abs(a._y-_y); }
};

typedef priority_queue<Node> MaxPQ;

class Peanuts {
  private:
    MaxPQ _pq;
  public:
    void add(int x, int y, int value) { _pq.push(Node(x, y, value)); }
    int peanuts(int k); // k units of time
};

int Peanuts::peanuts(int k)
{
    if (_pq.empty()) return 0;

    Node a(_pq.top().x(), 0, 0);
    int t = k;
    int result = 0;

    while (t > 0 && !_pq.empty()) {
        Node b = _pq.top(); _pq.pop();
        int distance = b.distance(a);
        if (debug) {
            cout << "--> To(" << b.y()
                << "," << b.x()
                << "):" << b.value()
                << ", Distance=(" << distance
                << "+1, ↑" << b.to_road()
                << ")";
        }

        if (distance+1+b.to_road() > t) {
            if (debug) cout << endl;
            break;
        }

        result += b.value();
        t -= (distance+1);
        if (debug) {
            cout << " --> K=" << t
                << ", Peanuts=" << result
                << endl;
        }

        a = b;
    }

    return result;
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int t;
    cin >> t;
    for (int i = 1; i <= t; i++) {
        int m, n, k;
        cin >> m >> n >> k;
        assert(m >= 1 && n >= 1);
        assert(0 <= k && k <= max_num);

        if (debug) {
            cout << "--> Case " << i
                << ": M=" << m
                << ", N=" << n
                << ", K=" << k
                << endl;
        }

        Peanuts X;
        for (int y = 1; y <= m; y++) {
            for (int x = 1; x <= n; x++) {
                int value;
                cin >> value;
                if (value > 0) {
                    if (debug) {
                        cout << "--> RC(" << y
                            << "," << x
                            << "):" << value
                            << endl;
                    }
                    X.add(x, y, value);
                }
            }
        }

        int answer = X.peanuts(k);
        cout << answer << endl;
    }
}

int main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);

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
    }

    run(max_num);

    return 0;
}
