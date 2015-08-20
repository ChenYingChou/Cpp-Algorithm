/* maze.cc -- tonychen [@] finenet.com.tw

   描述
    定义一个二维数组：

      int maze[5][5] = {
          0, 1, 0, 0, 0,
          0, 1, 0, 1, 0,
          0, 0, 0, 0, 0,
          0, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
      };

    它表示一个迷宫，其中的1表示墙壁，0表示可以走的路，只能横着走或竖着走，
    不能斜着走，要求编程序找出从左上角到右下角的最短路线。

   输入
    一个 5 ×5 的二维数组，表示一个迷宫。数据保证有唯一解。

   输出
    左上角到右下角的最短路径，格式如样例所示。

   样例输入
    0 1 0 0 0
    0 1 0 1 0
    0 0 0 0 0
    0 1 1 1 0
    0 0 0 1 0

   样例输出
    (0, 0)
    (1, 0)
    (2, 0)
    (2, 1)
    (2, 2)
    (2, 3)
    (2, 4)
    (3, 4)
    (4, 4)

 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <assert.h>

using namespace std;

typedef vector<char> vchar;
typedef vector<int> vint;
typedef queue<int> qint;

const int MAX_SIZE = 20;
const int MAZE_WALL = -1;
const int MAZE_TARGET = -2;
const int MAZE_EMPTY = 0;

static int debug;

class Maze {
  private:
    vchar _maze;            // 迷宫陣列, 0:表示空位, -1:表示牆, 外圍加一道牆
    int _rows;
    int _cols;
    int _width;             // _width == _cols + 2
    int _t_offset;          // 目標位置: (row, col) = _t_offset [ /, % ] _width

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
    int solve(int row_s, int col_s, int row_t, int col_t);
    int solve();
    void output_path(ostream &os) const;
};

void Maze::initialize(int rows, int cols)
{
    if (rows <= 1 || cols <= 1 || rows > MAX_SIZE || cols >= MAX_SIZE)
        throw runtime_error("Invalid rows or cols value");

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
    _t_offset = 0;
}

static bool empty(char c)
{
    switch(c) {
      case '0': // One
      case '.': // Black
        return true;
    }
    return false;
}

void Maze::put_row(int row, const string &s)
{
    if (s.size() > _cols)
        throw runtime_error("put_row(string longer than _cols): "+s);

    int npos = offset(row, 1);
    for (string::const_iterator it = s.begin(); it != s.end(); ++it) {
        _maze[npos++] = empty(*it) ? MAZE_EMPTY : MAZE_WALL;
    }
}

void Maze::output_maze(bool show_content) const
{
    int offset = _width + 1;
    for (int i = 1; i <= _rows; i++) {
        for (int j = 1; j <= _cols; j++) {
            int v = _maze[offset++];
            if (show_content) {
                if (v == MAZE_WALL) {
                    cout << "  - ";
                } else {
                    cout << setw(3) << v << ' ';
                }
            } else {
                cout << (v == MAZE_WALL ? '#' : '.') << ' ';
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
    int val = _maze[_t_offset];
    if (val <= MAZE_WALL) return;

    vint step;
    int npos = _t_offset;
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

    // 反向列出每一步驟位置, 將位置轉成座標(zero-based)
    for (vint::reverse_iterator it = step.rbegin(); it != step.rend(); ++it) {
        int n = *it;
        int row = n / _width;
        int col = n % _width;
        os << "(" << (row-1) << ", " << (col-1) << ")" << endl;
    }
}

int Maze::solve()
{
    return solve(1, 1, _rows, _cols);
}

int Maze::solve(int row_s, int col_s, int row_t, int col_t)
{
    if (row_s > _rows || row_t > _rows || col_s > _cols || col_t > _cols) return -1;

    int npos = offset(row_s, col_s);
    _t_offset = offset(row_t, col_t);

    if (_maze[npos] != MAZE_EMPTY || _maze[_t_offset] != MAZE_EMPTY) return -1;

    _maze[_t_offset] = MAZE_TARGET;

    // BFS
    qint Q;
    Q.push(npos);
    _maze[npos] = 1;

    int dir[4] = { 1, _width, -1, -_width };

    while (!Q.empty()) {
        npos = Q.front();
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
                return step;

              default:          // 該格若為其他的值, 則應該比 step 小才對
                assert(_maze[n] <= step);
                break;
            }
        }
    }

    return 0;
}

//---------------------------------------------------------------------------

static int number_count(const string &s)
{
    const char *p = s.c_str();
    int n = 0;
    while (*p != 0) {
        while (isblank(*p)) p++;
        if (*p != 0 && !isdigit(*p)) return 0;
        while (isdigit(*p)) p++;
        n++;
    }

    return n;
}

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

int main(int argc, char *argv[])
{
    int rows = 0;
    int cols = 0;

    for (int n = 1; n < argc; n++) {
        if (strncmp(argv[n], "-d", 2) == 0) {
            debug += strlen(&argv[n][1]);
        } else {
            if (rows == 0) {
                rows = atoi(argv[n]);
            } else if (cols == 0) {
                cols = atoi(argv[n]);
            }
        }
    }

    if (rows == 0) rows = 5;
    if (cols == 0) cols = 5;

    Maze M(rows, cols);
    string s;
    while (getline(cin, s)) {
        if (s.empty()) continue;

        int n = 1;
        switch(number_count(s)) {
          case 2:
            {
                stringstream os(s);
                os >> rows >> cols;
                M.resize(rows, cols);
            }
            break;
          case 1:
            if (s.size() == 1) continue;
            // fall-through
          default:
            M.clear();
            put_maze(M, n, s);
            break;
        }

        while (n <= rows && getline(cin, s)) {
            if (!s.empty()) put_maze(M, n, s);
        }

        if (debug) M.output_maze();

        int answer = M.solve();
        if (answer >= 0) {
            if (debug > 1) M.output_maze(true);
            if (debug) cout << "--> " << answer << endl << endl;
            M.output_path(cout);
        } else {
            cout << "No solution" << endl;
        }
    }

    return 0;
}

