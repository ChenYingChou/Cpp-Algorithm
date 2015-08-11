/* flip.cc -- tonychen@finenet.com.tw

   描述
    Flip game is played on a rectangular 4x4 field with two-sided pieces
    placed on each of its 16 squares. One side of each piece is white and
    the other one is black and each piece is lying either it's black or
    white side up. Each round you flip 3 to 5 pieces, thus changing the
    color of their upper side from black to white and vice versa. The pieces
    to be flipped are chosen every round according to the following rules:

    Choose any one of the 16 pieces.
    Flip the chosen piece and also all adjacent pieces to the left, to the
    right, to the top, and to the bottom of the chosen piece (if there are
    any).

    Consider the following position as an example:
      bwbw              ●○●○
      wwww    flip      ○○○○
      bbwb    (3,1) --> ●●○●
      bwwb   	        ●○○●

    Here "b" denotes pieces lying their black side up and "w" denotes pieces
    lying their white side up. If we choose to flip the 1st piece from the
    3rd row (this choice is shown at the picture), then the field will become:
      bwbw              ●○●○
      bwww       ^      ●○○○
      wwwb     < + >    ○○○●
      wwwb       v      ○○○●

    The goal of the game is to flip either all pieces white side up or all
    pieces black side up. You are to write a program that will search for the
    minimum number of rounds needed to achieve this goal.

   输入
    The input consists of 4 lines with 4 characters "w" or "b" each that
    denote game field position.

   输出
    Write to the output file a single integer number - the minimum number of
    rounds needed to achieve the goal of the game from the given position.
    If the goal is initially achieved, then write 0. If it's impossible to
    achieve the goal, then write the word "Impossible" (without quotes).

   样例输入
    bwwb
    bbwb
    bwwb
    bwww

   样例输出
    4

 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <assert.h>

using namespace std;

typedef unsigned long long ULL;
typedef vector<ULL> vstate;
typedef vector<char> vchar;

const int MAX_SIZE = 8 * sizeof(ULL);       // bits for columns used

static int debug;

class Board {
  private:
    vstate _board;          // bit-map columns for each rows
    vstate _press;          // flip bit-map columns for each rows
    ULL _mask;              // bit-mask for columns: 011...110b
    int _rows;              // 1 ~
    int _cols;              // 1 ~ MAX_SIZE-2
    int _min_rounds;        // minimum number of rounds

    void initialize();
    void validate(int row, int col) const;
    void output_state(const vstate &state) const;
    void output_press1(int row, int rounds) const;
    void output_bits(const char *title, ULL t) const;
    int flip_to_zero(int minRounds);
    void flip_board();

  public:
    Board(int rows, int cols) : _rows(rows), _cols(cols) {
        initialize();
    }
    Board(int size) : _rows(size), _cols(size) {
        initialize();
    }
    void put_row(int row, const string &s);         // row: 1 ~ _rows
    void output_board() const;
    void output_press(int rounds) const;
    int resolve();
};

static string pair_str(const char *title, int a, int b)
{
    stringstream os;

    os << title << "(" << a << "," << b << ")";
    return os.str();
}

void Board::initialize()
{
    if (_rows <= 0 || _cols <= 0 || _cols >= MAX_SIZE-1)
        throw runtime_error("Invalid board "+pair_str("size", _rows, _cols));

    _board.resize(_rows+1, 0);
    _mask = (1 << (_cols+1)) - 2;       // 0011...110: there are (_cols)'s 1
    _min_rounds = 0;
}

void Board::validate(int row, int col) const
{
    if (row < 1 || row > _rows || col < 1 || col > _cols)
        throw runtime_error("Invalid board "+pair_str("cell", row, col));
}

void Board::put_row(int row, const string &s)
{
    validate(row, s.size());
    ULL cols = 0;
    for (int i = s.size(); --i >= 0;) {
        if (s[i] == '1' || s[i] == 'b' || s[i] == 'B') cols |= 1;
        cols <<= 1;
    }
    _board[row-1] = cols;
}

void Board::output_state(const vstate &state) const
{
    for (int i = 0; i < _rows; i++) {
        ULL t = i < state.size() ? state[i] : 0;
        for (int j = 0; j < _cols; j++) {
            if (j == 0) cout << endl; else cout << ' ';
            t >>= 1;
            cout << (t & 1 ? 'X' : '-');
        }
    }
    cout << endl << endl;
}

void Board::output_board() const
{
    cout << "Board: " << _rows << " x " << _cols;
    output_state(_board);
}

void Board::output_bits(const char *title, ULL t) const
{
    cout << title;
    for (int j = 0; j < _cols; j++) {
        t >>= 1;
        cout << ' ' << (t & 1 ? 'X' : '-');
    }
    cout << " |";
}

void Board::output_press1(int row, int rounds) const
{
    cout << "[" << setfill('0') << setw(2) << row << "]: "
        << setfill(' ') << setw(2) << rounds;
    output_bits(" |", _press[row-1]);
    cout << endl;
}

void Board::output_press(int rounds) const
{
    cout << "Flips: " << rounds;
    output_state(_press);
}

void Board::flip_board()
{
    for (vstate::iterator it = _board.begin(); it != _board.end(); ++it) {
        *it = ~(*it) & _mask;
    }
}

int Board::resolve()
{
    int rounds = flip_to_zero(MAX_SIZE*MAX_SIZE+1);

    flip_board();
    if (debug) output_board();

    rounds = min(rounds, flip_to_zero(rounds));
    flip_board();

    _min_rounds = rounds < MAX_SIZE * MAX_SIZE ? rounds : -1;
    return _min_rounds;
}

static int bit_count(ULL x)
{
    bitset<MAX_SIZE> t(x);
    return t.count();
}

int Board::flip_to_zero(int minRounds)
{
    _press.resize(_rows+1);

    // 將 row[0] 每一狀況都試一次: 2^(_cols)
    ULL flip0 = 1 << _cols;
    do {
        flip0--;
        if (bit_count(flip0) >= minRounds) continue;

        int rounds = 0;
        ULL row0 = flip0 << 1;          // 前一列狀態: 要設法將之變為零
        ULL row1 = _board[0];           // 即將處理這列棋盤狀態

        fill(_press.begin(), _press.end(), 0);
        _press[0] = row0;               // 第一列預期的點擊結果

        for (int n = 1; n <= _rows; n++) {
            // 預判這一列點擊次數, 若累計超過目前最小次數, 則換 row[0] 下一狀況
            rounds += bit_count(row0);
            if (rounds >= minRounds) {
                if (debug > 1) {
                    output_press1(n, rounds);
                    cout << "*** break: " << rounds << endl;
                }
                row0 = ~0;
                break;
            }

            // 試著將 row0 所有 bit 為 1 者變換為 0
            // 將對應 row1 翻轉, 影響: 自身左右(row1)/上(row0)/下(row2) 狀態

            // row2: 下一列, 可能超出指定列數, 設定時會多加1筆, 該值無所謂
            ULL row2 = _board[n];

            ULL bits_111 = 0x07;    // 111b
            ULL bits_010 = 0x02;    // 010b
            while ((row0 >>= 1) != 0) {
                if (row0 & 1) { // 根據 row0 來決定是否翻轉對應的 row1,row2
                    row1 ^= bits_111;   // 左/本身/右
                    row2 ^= bits_010;   // 下
                }
                bits_111 <<= 1;
                bits_010 <<= 1;
            }

            row0 = row1 & _mask;        // 下一列的解決方案: 應將 bit 為 1 的消去
            row1 = row2 & _mask;        // 下一列經前面步驟異動後的棋盤狀態
            _press[n] = row0;           // _press[] 有多準備一列, 多的這列沒作用

            if (debug > 1) {
                output_press1(n, rounds);
                if (debug > 2) {
                    output_bits("         |", row0);
                    output_bits("", row1);
                    cout << endl;
                }
            }
        }

        if (debug > 1) cout << endl;

        if (row0 == 0) {
            // 全部翻成白子
            if (rounds < minRounds) {
                minRounds = rounds;
                if (debug) output_press(rounds);
            }
        }
    } while (flip0 > 0);

    return minRounds;
}

//---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int rows = 4;
    int cols = 4;
    int n = 1;
    if (n < argc && strncmp(argv[n], "-d", 2) == 0) {
        debug = strlen(&argv[n][1]);
        n++;
    }
    if (n < argc) {
        rows = atoi(argv[n]);
        n++;
    }
    if (n < argc) {
        cols = atoi(argv[n]);
        n++;
    }

    Board B(rows, cols);
    for (int n = 1; n <= rows; n++) {
        string s;
        cin >> s;
        B.put_row(n, s);
    }

    if (debug) B.output_board();

    int answer = B.resolve();
    if (debug) {
        cout << "--> " << answer << endl << endl;
    } else {
        if (answer >= 0) {
            cout << answer << endl;
        } else {
            cout << "Impossible" << endl;
        }
    }

    return 0;
}

