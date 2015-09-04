/* distance.cc -- tonychen [@] finenet.com.tw

   描述
    给出三维空间中的n个点（不超过10个）,求出n个点两两之间的距离,并按距离由
    大到小依次输出两个点的坐标及它们之间的距离。

   输入
    输入包括两行，第一行包含一个整数n表示点的个数，第二行包含每个点的坐标
    (坐标都是整数)。点的坐标的范围是0到100，输入数据中不存在坐标相同的点。

   输出
    对于大小为n的输入数据，输出n*(n-1)/2行格式如下的距离信息：
        (x1,y1,z1)-(x2,y2,z2)=距离
    其中距离保留到数点后面2位。

   样例输入
    4
    0 0 0 1 0 0 1 1 0 1 1 1

   样例输出
    (0,0,0)-(1,1,1)=1.73
    (0,0,0)-(1,1,0)=1.41
    (1,0,0)-(1,1,1)=1.41
    (0,0,0)-(1,0,0)=1.00
    (1,0,0)-(1,1,0)=1.00
    (1,1,0)-(1,1,1)=1.00

   提示
    用cout输出时保留到小数点后2位的方法:
        cout << fixed << setprecision(2) << x

    注意：
    冒泡排序满足下面的性质,选择排序和快速排序(qsort或sort)需要对下面的情况
    进行额外处理
    使用冒泡排序时要注意边界情况的处理,保证比较的两个数都在数组范围内

    1.对于一行输出中的两个点(x1,y1,z1)和(x2,y2,z2)，点(x1,y1,z1)在输入数据
      中应出现在点(x2,y2,z2)的前面。
      比如输入：
        2
        0 0 0 1 1 1
        输出是：
        (0,0,0)-(1,1,1)=1.73
        但是如果输入：
        2
        1 1 1 0 0 0
        输出应该是：
        (1,1,1)-(0,0,0)=1.73

    2.如果有两对点p1,p2和p3,p4的距离相同，则先输出在输入数据中靠前的点对。
      比如输入：
        3
        0 0 0 0 0 1 0 0 2
      输出是：
        (0,0,0)-(0,0,2)=2.00
        (0,0,0)-(0,0,1)=1.00
        (0,0,1)-(0,0,2)=1.00
      如果输入变成：
        3
        0 0 2 0 0 1 0 0 0
      则输出应该是：
        (0,0,2)-(0,0,0)=2.00
        (0,0,2)-(0,0,1)=1.00
        (0,0,1)-(0,0,0)=1.00

 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

const int MAX_NUM = 100;

static int debug;

//---------------------------------------------------------------------------

class Point {
  private:
    int _x;
    int _y;
    int _z;
    int _seqno;
  public:
    Point(int x, int y, int z, int seqno)
      : _x(x), _y(y), _z(z), _seqno(seqno) {}
    int x() const { return _x; }
    int y() const { return _y; }
    int z() const { return _z; }
    int seqno() const { return _seqno; }
    bool operator== (const Point &p) const { return _seqno == p._seqno; }
    bool operator!= (const Point &p) const { return _seqno != p._seqno; }
    bool operator< (const Point &p) const { return _seqno < p._seqno; }
    double operator- (const Point &p) const {
        int dx = _x - p._x;
        int dy = _y - p._y;
        int dz = _z - p._z;
        return sqrt(dx*dx + dy*dy + dz*dz);
    }
};

class Distance {
  private:
    const Point *_p1;
    const Point *_p2;
    double _distance;
  public:
    Distance(const Point *p1, const Point *p2) : _p1(p1), _p2(p2) {
        _distance = *_p1 - *_p2;
    }
    bool operator< (const Distance &d) const {
        if (_distance != d._distance) return _distance > d._distance;
        if (*_p1 != *d._p1) return *_p1 < *d._p1;
        return *_p2 < *d._p2;
    }
    const Point &p1() const { return *_p1; }
    const Point &p2() const { return *_p2; }
    double distance() const { return _distance; }
};

typedef vector<Point> vPoint;
typedef vector<Distance> vDistance;

ostream & operator<< (ostream &os, const Point &p) {
    return os << '(' << p.x() << ',' << p.y() << ',' << p.z() << ')';
}

ostream & operator<< (ostream &os, const Distance &d) {
    return os << d.p1() << '-' << d.p2() << '='
        << fixed << setprecision(2) << d.distance();
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int n;
    cin >> n;
    assert(1 < n && n <= max_num);

    vPoint points;
    points.reserve(n);
    for (int i = 1; i <= n; i++) {
        int x, y, z;
        cin >> x >> y >> z;
        points.push_back(Point(x, y, z, i));
    }

    vDistance dists;
    dists.reserve(n*(n-1)/2);
    for (int i = 1; i < n; i++) {
        const Point *p0 = &points[i-1];
        for (int j = i; j < n; j++) {
            dists.push_back(Distance(p0, &points[j]));
        }
    }

    sort(dists.begin(), dists.end());

    for (vDistance::iterator it = dists.begin(); it != dists.end(); ++it) {
        cout << *it << endl;
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
