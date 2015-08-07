/* mvchess.cc -- tonychen@finenet.com.tw

   描述:
    在一个给定形状的棋盘（形状可能是不规则的）上面摆放棋子，棋子没有区别。
    要求摆放时任意的两个棋子不能放在棋盘中的同一行或者同一列，请编程求解对
    于给定形状和大小的棋盘，摆放k个棋子的所有可行的摆放方案C。

   输入:
    输入含有多组测试数据。
    每组数据的第一行是两个正整数，n k，用一个空格隔开，表示了将在一个n*n的
    矩阵内描述棋盘，以及摆放棋子的数目。 n <= 8 , k <= n
    当为-1 -1时表示输入结束。
    随后的n行描述了棋盘的形状：每行有n个字符，其中 # 表示棋盘区域， . 表示
    空白区域（数据保证不出现多余的空白行或者空白列）。

   输出:
    对于每一组数据，给出一行输出，输出摆放的方案数目C（数据保证C<2^31）。

   样例输入:
    2 1
    #.
    .#
    4 4
    ...#
    ..#.
    .#..
    #...
    -1 -1

   样例输出:
    2
    1

   註: '#' 表示可以放棋子的位置!!

 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <assert.h>

using namespace std;

typedef vector<char> vchar;
typedef unsigned long long vstate;

const int MAX_SIZE = 8 * sizeof(vstate);        // bits for columns used
const char EMPTY = '#';     // 可放棋子
const char STONE = '.';     // 不可放棋子
const char CHESS = '*';     // 棋子記號

static int debug;

class Board {
  private:
    vchar _board;
    vstate _used;           // bit-map: columns 是否被使用
    int _rows;
    int _cols;
    int _size;              // _rows x _cols

    int _fk;                // 還剩多少棋子待擺
    int _fcount;            // 找到次數

    void initialize();
    void validate(int row, int col) const;
    int offset(int row, int col) const {
        validate(row, col);
        return (row-1) * _cols + (col-1);
    }
    void search(int npos);
    bool valid_pos(int &npos) const;
    void restore_pos(int npos);
    int store_pos(int npos);

  public:
    Board(int rows, int cols) : _rows(rows), _cols(cols) {
        initialize();
    }
    Board(int size) : _rows(size), _cols(size) {
        initialize();
    }
    void put_row(int row, const string &s);         // row: 1 ~ MAX_SIZE
    void put(int row, int col, char ch) {           // row, col: 1 ~ MAX_SIZE
        _board[offset(row, col)] = ch;
    }
    char get(int row, int col) const {              // row, col: 1 ~ MAX_SIZE
        return _board[offset(row, col)];
    }
    void output(const char *title, int k) const;
    int resolve(int k);
};

static string pair_str(const char *title, int a, int b)
{
    stringstream os;

    os << title << "(" << a << "," << b << ")";
    return os.str();
}

void Board::initialize()
{
    if (_rows <= 0 || _cols <= 0) 
        throw runtime_error("Invalid board "+pair_str("size", _rows, _cols));

    _size = _rows * _cols;
    _board.resize(_size);
}

void Board::validate(int row, int col) const
{
    if (row < 1 || row > _rows || col < 1 || col > _cols)
        throw runtime_error("Invalid "+pair_str("step", row, col));
}

void Board::put_row(int row, const string &s)
{
    int npos = offset(row, 1);
    if (npos + s.size() > _size)
        throw runtime_error("String too large: "+s);

    copy(s.begin(), s.end(), &_board[npos]);
}

void Board::output(const char *title, int k) const
{
    cout << title << k;
    for (int offset = 0; offset < _size; offset++) {
        if (offset % _cols == 0) cout << endl; else cout << " ";
        char c = _board[offset];
        cout << (c == EMPTY ? '-' : c);
    }
    cout << endl << endl;
}

int Board::resolve(int k)
{
    assert(k > 0);

    // 若要擺放的棋子數大於列數或行數則無解
    if (k > _rows || k > _cols) return 0;

    // 若只有一個棋子, 則計算可下子的位置即可
    if (k == 1) {   
        // count for EMPTY
        int count = 0;
        for (int n = 0; n < _size; n++) {
            if (_board[n] == EMPTY) count++;
        }
        return count;
    }

    _used = 0;
    _fk = k;
    _fcount = 0;
    search(0);

    return _fcount;
}

bool Board::valid_pos(int &npos) const
{
    if (_board[npos] != EMPTY) return false;

    int row = npos / _cols;
    if (_fk + row > _rows) {
        // 剩下列數不足以擺 _fk 個棋子, 放棄目前棋盤(將npos改到棋盤外)即返回上層
        npos = _size;
        return false;
    }

    int col = npos % _cols;
    return (_used & (1 << col)) == 0;   // 返回該 column 是否已被佔用?
}

int Board::store_pos(int npos)
{
    int col = npos % _cols;
    int row = npos / _cols + 1;     // 下一列

    _fk--;
    _board[npos] = CHESS;
    _used |= 1 << col;              // 佔用該 column 的 bit

    return row * _cols;             // 跳到下列開頭位置
}

void Board::restore_pos(int npos)
{
    int col = npos % _cols;

    _fk++;
    _board[npos] = EMPTY;
    _used &= ~(1 << col);           // 清除該 column 的 bit
}

void Board::search(int npos)
{
    while (npos < _size) {
        if (valid_pos(npos)) {
            if (_fk == 1) {         // 完成
                _fcount++;
                if (debug > 1) {
                    store_pos(npos);
                    output("Resolve: ", _fcount);
                    restore_pos(npos);
                }
            } else {                // 佔用現在位置, 繼續下個棋子(下一列)
                search(store_pos(npos));
                restore_pos(npos);
            }
        }
        npos++;
    }
}

//---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    if (argc > 1 && strncmp(argv[1], "-d", 2) == 0) debug = strlen(&argv[1][1]);

    int n, k;
    while (!(cin >> n >> k).eof() && n > 0 && k > 0) {
        assert(n <= MAX_SIZE && k <= n);
        Board B(n);

        for (int r = 1; r <= n; r++) {
            string s;
            cin >> s;
            if (s.size() != n)
                throw runtime_error("Invalid input board line: "+s);
            B.put_row(r, s);
        }

        if (debug) B.output("Board: K=", k);

        int answer = B.resolve(k);
        if (debug) {
            cout << "--> " << answer << endl << endl;
        } else {
            cout << answer << endl;
        }
    }

    return 0;
}

