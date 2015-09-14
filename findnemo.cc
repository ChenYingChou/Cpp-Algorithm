/* findnemo.cc -- tonychen [@] finenet.com.tw

   描述
    Nemo is a naughty boy. One day he went into the deep sea all by himself.
    Unfortunately, he became lost and couldn't find his way home. Therefore,
    he sent a signal to his father, Marlin, to ask for help.

    After checking the map, Marlin found that the sea is like a labyrinth
    with walls and doors. All the walls are parallel to the X-axis or to
    the Y-axis. The thickness of the walls are assumed to be zero.

    All the doors are opened on the walls and have a length of 1. Marlin
    cannot go through a wall unless there is a door on the wall. Because
    going through a door is dangerous (there may be some virulent medusas
    near the doors), Marlin wants to go through as few doors as he could
    to find Nemo.

    Figure-1 shows an example of the labyrinth and the path Marlin went
    through to find Nemo.

          |                             (4,4)
        4 +     #########################
          |     #       |       |       |
          |     #                   +-------+
          |     #       |       |   |   |   |
        3 +     #################-- | --#   |
          |     #       |       |   |   #   |
          |     #   +---------------+   #   |
          |     #   |   |       |       #   |
        2 +     #-- | --#################   |
          |     #   |   |       |       #   |
          |     #   V                   #   |
          |     # nemo  |       |       #   |
        1 +     #########################   |
          | ________________________________|
          |/
          +-----+-------+-------+-------+------
       Marlin   1       2       3       4

    We assume Marlin's initial position is at (0, 0). Given the position
    of Nemo and the configuration of walls and doors, please write a program
    to calculate the minimum number of doors Marlin has to go through in
    order to reach Nemo.

   输入
    The input consists of several test cases. Each test case is started by
    two non-negative integers M and N. M represents the number of walls in
    the labyrinth and N represents the number of doors.

    Then follow M lines, each containing four integers that describe a wall
    in the following format:

        x y d t

    (x, y) indicates the lower-left point of the wall, d is the direction
    of the wall -- 0 means it's parallel to the X-axis and 1 means that it's
    parallel to the Y-axis, and t gives the length of the wall.

    The coordinates of two ends of any wall will be in the range of [1,199].
    Then there are N lines that give the description of the doors:

        x y d

    x, y, d have the same meaning as the walls. As the doors have fixed length
    of 1, t is omitted.

    The last line of each case contains two positive float numbers:

        f1 f2

    (f1, f2) gives the position of Nemo. And it will not lie within any wall
    or door.

    A test case of M = -1 and N = -1 indicates the end of input, and should
    not be processed.

   输出
    For each test case, in a separate line, please output the minimum number
    of doors Marlin has to go through in order to rescue his son. If he can't
    reach Nemo, output -1.

   样例输入
    8 9
    1 1 1 3
    2 1 1 3
    3 1 1 3
    4 1 1 3
    1 1 0 3
    1 2 0 3
    1 3 0 3
    1 4 0 3
    2 1 1
    2 2 1
    2 3 1
    3 1 1
    3 2 1
    3 3 1
    1 2 0
    3 3 0
    4 3 1
    1.5 1.5
    4 0
    1 1 0 1
    1 1 1 1
    2 1 1 1
    1 2 0 1
    1.5 1.7
    -1 -1

   样例输出
    5
    -1

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<int> vint;
typedef vector<vint> vvint;

const int MAX_NUM = 200;

static int debug;

//---------------------------------------------------------------------------

// 每一房間用4組2bits表示四週: aabbccdd, aa:上, bb:右, cc:下, dd:左
enum ENUM_DIR { E_LEFT, E_DOWN, E_RIGHT, E_UP };
const int C_VISITED = 0x100;
const int C_WALL = 0x03;
const int C_DOOR = 0x02;
const int C_WRAP = 0xaa;    // 10101010b: 表示此房間是否被圍住, 四週不是牆就是門
const int C_DIR[4][2] = {
        { -1,  0},  // left : (x-1, y  )
        {  0, -1},  // down : (x  , y-1)
        {  1,  0},  // right: (x+1, y  )
        {  0,  1}   // up   : (x  , y+1)
    };

inline int to_left(int v)  { return v; }
inline int to_down(int v)  { return v << 2; }
inline int to_right(int v) { return v << 4; }
inline int to_up(int v)    { return v << 6; }
inline bool is_free(int room) { return (room & C_WRAP) != C_WRAP; }
inline bool is_visited(int v) { return v & C_VISITED; }

static string walls(int v)
{
    string s(12, ' ');
    for (int i = 1; i <= 4; i++) {
        int k = 12 - 3*i;
        s[k+1] = v & 0x02 ? '1' : '0';
        s[k+2] = v & 0x01 ? '1' : '0';
        v >>= 2;
    }
    return s;
}

class Room {
  private:
    int _x, _y;
    int _doors;

  public:
    Room(int x, int y, int doors) : _x(x), _y(y), _doors(doors) {}
    int x() const { return _x; }
    int y() const { return _y; }
    int doors() const { return _doors; }
    bool operator< (const Room &r) const {
        // descending
        if (_doors != r._doors) return r._doors < _doors;
        if (_y != r._y) return r._y < _y;
        return r._x < _x;
    }
};

typedef priority_queue<Room> MinPQ;

class Labyrinth {
  private:
    vvint _cell;
    int _xL, _xH, _yL, _yH;     // 迷宮的有效範圍
    MinPQ _pq;

    void clear() {
        while (!_pq.empty()) _pq.pop();
        for (vvint::iterator y = _cell.begin(); y != _cell.end(); ++y) {
            vint &Y = *y;
            for (vint::iterator x = Y.begin(); x != Y.end(); ++x) {
                *x &= ~C_VISITED;
            }
        }
    }
    void visit(int x, int y, int doors) {
        if (debug) {
            cout << "--> Push (" << x << ", " << y << ", "
                << doors << ")" << endl;
        }
        _cell[y][x] |= C_VISITED;
        _pq.push(Room(x, y, doors));
    }
    bool is_visited(int x, int y) {
        return ::is_visited(_cell[y][x]);
    }
    bool is_free(int x, int y) {
        return (x == 0 && y == 0)
            || (x < _xL || x >= _xH || y < _yL || y >= _yH);
    }
    bool is_free(const Room &r) { return is_free(r.x(), r.y()); }
    int cell(const Room &r) { return _cell[r.y()][r.x()]; }

  public:
    Labyrinth(int maxSize) {
        _cell.resize(maxSize);
        for (int i = 0; i < maxSize; i++) {
            _cell[i].resize(maxSize, 0);
        }
        _xL = _yL = maxSize;
        _xH = _yH = 0;
    }
    void add_wall(int x, int y, int d, int t);
    void add_door(int x, int y, int d);
    int find(int posX, int posY);
    int find(double posX, double posY) { return find((int)posX, (int)posY); }
};

void Labyrinth::add_wall(int x, int y, int d, int t)
{
    if (t <= 0) return;
    if (d == 0) {
        // 和 x 軸平行(延伸): 使用 (y,y-1)
        vint &Y0 = _cell[y];
        vint &Y1 = _cell[y-1];
        for (int i = 0; i < t; i++) {
            Y0[x+i] |= to_down(C_WALL);
            Y1[x+i] |= to_up  (C_WALL);
        }
        if (y   < _yL) _yL = y;
        if (y   > _yH) _yH = y;
        if (x   < _xL) _xL = x;
        if (x+t > _xH) _xH = x+t;
    } else {
        // 和 y 軸平行(延伸): (x-1, x)
        for (int i = 0; i < t; i++) {
            vint &Y0 = _cell[y+i];
            Y0[x-1] |= to_right(C_WALL);
            Y0[x]   |= to_left (C_WALL);
        }
        if (y   < _yL) _yL = y;
        if (y+t > _yH) _yH = y+t;
        if (x   < _xL) _xL = x;
        if (x   > _xH) _xH = x;
    }
}

void Labyrinth::add_door(int x, int y, int d)
{
    if (d == 0) {
        // x 軸: (x,y  )下方
        //       ----------
        //       (x,y-1)上方
        vint &Y0 = _cell[y];
        vint &Y1 = _cell[y-1];
        Y0[x] = (Y0[x] & ~to_down(C_WALL)) | to_down(C_DOOR);
        Y1[x] = (Y1[x] & ~to_up  (C_WALL)) | to_up  (C_DOOR);
    } else {
        // y 軸: (x-1,y)右方|(x,y)左方
        vint &Y0 = _cell[y];
        Y0[x-1] = (Y0[x-1] & ~to_right(C_WALL)) | to_right(C_DOOR);
        Y0[x  ] = (Y0[x  ] & ~to_left (C_WALL)) | to_left (C_DOOR);
    }
}

int Labyrinth::find(int posX, int posY)
{
    if (debug > 1) {
        for (int y = _yL; y < _yH; y++) {
            vint &Y = _cell[y];
            for (int x = _xL; x < _xH; x++) {
                cout << "(" << x << ", " << y << "):"
                    << walls(Y[x])
                    << endl;
            }
        }
    }

    if (is_free(posX, posY)) return 0;

    clear();
    visit(posX, posY, 0);
    while (!_pq.empty()) {
        Room r = _pq.top(); _pq.pop();
        if (debug) {
            cout << "-->  Pop (" << r.x() << ", " << r.y() << ", "
                << r.doors() << "):" << walls(cell(r)) << endl;
        }
        if (is_free(r)) return r.doors();

        int val = cell(r);
        int doors = r.doors();
        for (int i = 0; i < 4; i++) {
            int v = val & C_WALL;
            if (v != C_WALL) {
                int x = r.x() + C_DIR[i][0];
                int y = r.y() + C_DIR[i][1];
                if (!is_visited(x, y)) {
                    visit(x, y, v == C_DOOR ? doors+1 : doors);
                }
            }
            val >>= 2;
        }
    }

    return -1;
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int nWall, nDoor;
    while (cin >> nWall >> nDoor && nWall >= 0 && nDoor >= 0) {
        if (debug) cout << "--> Case: " << nWall << ", " << nDoor << endl;
        Labyrinth X(max_num);

        for (int i = 0; i < nWall; i++) {
            int x, y, d, t;
            cin >> x >> y >> d >> t;
            assert(0 < x && x < max_num);
            assert(0 < y && y < max_num);
            X.add_wall(x, y, d, t);
        }

        for (int i = 0; i < nDoor; i++) {
            int x, y, d;
            cin >> x >> y >> d;
            assert(0 < x && x < max_num);
            assert(0 < y && y < max_num);
            X.add_door(x, y, d);
        }

        double posX, posY;
        cin >> posX >> posY;
        int answer = X.find(posX, posY);
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
