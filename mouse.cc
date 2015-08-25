/* mouse.cc -- tonychen [@] finenet.com.tw

   描述

    阿尔吉侬是一只聪明又慵懒的小白鼠，它最擅长的就是走各种各样的迷宫。今天它要
    挑战一个非常大的迷宫，研究员们为了鼓励阿尔吉侬尽快到达终点，就在终点放了一
    块阿尔吉侬最喜欢的奶酪。现在研究员们想知道，如果阿尔吉侬足够聪明，它最少需
    要多少时间就能吃到奶酪。

    迷宫用一个R×C的字符矩阵来表示。字符S表示阿尔吉侬所在的位置，字符E表示奶酪
    所在的位置，字符#表示墙壁，字符.表示可以通行。阿尔吉侬在1个单位时间内可以
    从当前的位置走到它上下左右四个方向上的任意一个位置，但不能走出地图边界。

   输入
    第一行是一个正整数T（1 <= T <= 10），表示一共有T组数据。

    每一组数据的第一行包含了两个用空格分开的正整数R和C（2 <= R, C <= 200），
    表示地图是一个R×C的矩阵。

    接下来的R行描述了地图的具体内容，每一行包含了C个字符。字符含义如题目描述中
    所述。保证有且仅有一个S和E。

   输出
    对于每一组数据，输出阿尔吉侬吃到奶酪的最少单位时间。若阿尔吉侬无法吃到奶酪，
    则输出"oop!" (只输出引号里面的内容，不输出引号)。每组数据的输出结果占一行。

   样例输入
    3
    3 4
    .S..
    ###.
    ..E.
    3 4
    .S..
    .E..
    ....
    3 4
    .S..
    ####
    ..E.

   样例输出
    5
    1
    oop!

 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <assert.h>

using namespace std;

typedef vector<int> vint;
typedef queue<int> qint;

const int MAX_NUM = 200;
const int MIN_NUM = 2;
const int MAZE_WALL = -1;
const int MAZE_EMPTY = 0;
const int MAZE_TARGET = -2;

static int debug;

class Maze {
  private:
    vint _maze;             // 迷宫陣列, 0:表示空位, -1:表示牆, 外圍加一道牆
    int _rows;
    int _cols;
    int _width;             // _width == _cols + 2
    int _s_pos;             // 起始位置: (row, col) = _s_pos [ /, % ] _width
    int _t_pos;             // 目標位置: (row, col) = _t_pos [ /, % ] _width

    void initialize(int rows, int cols);
    int offset(int row, int col);
    int find_pos(int npos, int k) const;

  public:
    Maze(int rows, int cols) { initialize(rows, cols); }
    Maze(int size) { initialize(size, size); }
    void resize(int rows, int cols) { initialize(rows, cols); }
    void resize(int size) { initialize(size, size); }
    void clear();
    void put_row(int row, const string &s);         // row: 1 ~ _rows
    void output_maze(bool show_content=false) const;
    int solve();
    void output_path(ostream &os) const;
};

void Maze::initialize(int rows, int cols)
{
    if (rows < 1 || cols < 1)
        throw runtime_error("Invalid numbers of (rows, cols)");

    _rows = rows;
    _cols = cols;
    _width = cols + 2;
    _maze.resize((_rows+2) * _width);
    clear();
}

int Maze::offset(int row, int col)
{
    return row * _width + col;
}

void Maze::clear()
{
    fill(_maze.begin(), _maze.end(), MAZE_WALL);
    _s_pos = _t_pos = 0;
}

static bool empty(char c)
{
    switch(c) {
      case '0': // Zero
      case '.': // Dot
      case '-': // Desh
        return MAZE_EMPTY;
      case 'S': // Start
      case 's':
        return 'S';
      case 'E': // End
      case 'e':
        return 'E';
    }
    return MAZE_WALL;
}

void Maze::put_row(int row, const string &s)
{
    if (s.size() > _cols)
        throw runtime_error("put_row(string longer than _cols): "+s);

    int npos = offset(row, 1);
    for (string::const_iterator it = s.begin(); it != s.end(); ++it) {
        int C = MAZE_WALL;
        switch(*it) {
          case '0': // Zero
          case '.': // Dot
          case '-': // Desh
            C = MAZE_EMPTY;
            break;
          case 'S': // Start
          case 's':
            C = MAZE_EMPTY;
            _s_pos = npos;
            break;
          case 'E': // End
          case 'e':
            C = MAZE_EMPTY;
            _t_pos = npos;
            break;
        }
        _maze[npos++] = C;
    }
}

void Maze::output_maze(bool show_content) const
{
    int offset = _width + 1;
    for (int i = 1; i <= _rows; i++) {
        for (int j = 1; j <= _cols; j++, offset++) {
            int v = _maze[offset];
            if (show_content) {
                if (v == MAZE_WALL) {
                    cout << "  - ";
                } else {
                    cout << setw(3) << v << ' ';
                }
            } else {
                if (offset == _s_pos) {
                    cout << "S ";
                } else if (offset == _t_pos) {
                    cout << "E ";
                } else {
                    cout << (v == MAZE_WALL ? '#' : '.') << ' ';
                }
            }
        }
        offset += 2;
        cout << endl;
    }
    cout << endl;
}

int Maze::find_pos(int npos, int val) const
{
    int n = npos + 1;
    if (_maze[n] == val) return n;

    n = npos - 1;
    if (_maze[n] == val) return n;

    n = npos - _width;
    if (_maze[n] == val) return n;

    n = npos + _width;
    if (_maze[n] == val) return n;

    return -1;
}

void Maze::output_path(ostream &os) const
{
    int val = _maze[_t_pos];
    if (val <= MAZE_WALL) return;

    vint step;
    int npos = _t_pos;
    step.reserve(val+1);
    step.push_back(npos);

    // 回溯: 根據目標位置找週圍步驟比它小1者, 再繼續往下找, 將位置存入 step[]
    while (--val > MAZE_EMPTY) {
        int prev = find_pos(npos, val);
        if (prev < 0) {
            output_maze(true);
            os << "*** Internal error ***" << endl;
            return;
        }
        npos = prev;
        step.push_back(npos);
    }

    // 反向列出每一步驟位置, 將位置轉成座標(1-based)
    for (vint::reverse_iterator it = step.rbegin(); it != step.rend(); ++it) {
        int n = *it;
        int row = n / _width;
        int col = n % _width;
        os << "(" << row << ", " << col << ")" << endl;
    }
}

int Maze::solve()
{
    if (_maze[_s_pos] != MAZE_EMPTY ||
        _maze[_t_pos] != MAZE_EMPTY) return -1;

    if (_s_pos == _t_pos) return 0;

    // BFS
    qint Q;
    Q.push(_s_pos);
    _maze[_s_pos] = 1;
    _maze[_t_pos] = MAZE_TARGET;

    int dir[4] = { 1, _width, -1, -_width };

    while (!Q.empty()) {
        int npos = Q.front();
        Q.pop();

        int step = _maze[npos] + 1;
        for (int i = 0; i < 4; i++) {
            // 試看看四個方向是否有空位, 或是找到目標
            int n = npos + dir[i];

            switch (_maze[n]) {
              case MAZE_EMPTY:  // 空位: 填上第幾步可到達, 推入Queue
                _maze[n] = step;
                Q.push(n);
                break;

              case MAZE_TARGET: // 找到目標: 填入第幾步驟(回溯用), 結束搜尋
                _maze[n] = step;
                return step-1;

              default:          // 該格若為其他的值, 則應該比 step 小才對
                assert(_maze[n] <= step);
                break;
            }
        }
    }

    return -1;
}

//---------------------------------------------------------------------------

static void put_maze(Maze &M, int &row, const string &s)
{
    const char *p = s.c_str();
    while (*p != 0) {
        while (isblank(*p)) p++;

        const char *t = p;
        while (*p != 0 && !isblank(*p)) p++;

        if (p == t+1) {     // assume one line: 0 1 0 0 1 ...
            string ss;
            while (*t != 0) {
                if (!isblank(*t)) ss.push_back(*t);
                t++;
            }
            M.put_row(row++, ss);
            return;
        }

        string ss(t, p-t);
        M.put_row(row++, ss);
    }
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int nCase;
    cin >> nCase;
    while (nCase-- > 0) {
        int rows, cols;
        cin >> rows >> cols;
        assert(rows <= max_num && cols <= max_num);
        Maze M(rows, cols);

        {
            string s;
            int n = 1;
            while (n <= rows && getline(cin, s)) {
                if (!s.empty()) put_maze(M, n, s);
            }
        }
        if (debug) M.output_maze();

        int answer = M.solve();
        if (debug > 1) M.output_maze(true);
        if (answer >= 0) {
            if (debug) cout << "--> ";
            cout << answer << endl;
            if (debug) {
                M.output_path(cout);
                cout << endl;
            }
        } else {
            cout << "oop!" << endl;
        }
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
