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

   輸入
    The input consists of 4 lines with 4 characters "w" or "b" each that
    denote game field position.

   輸出
    Write to the output file a single integer number - the minimum number of
    rounds needed to achieve the goal of the game from the given position.
    If the goal is initially achieved, then write 0. If it's impossible to
    achieve the goal, then write the word "Impossible" (without quotes).

   輸入範例
    bwwb
    bbwb
    bwwb
    bwww

   輸出範例
    4

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   資料結構:
    每列(row)以bit-map來表示各格子(column)的值(0,1), 同時為方便翻子時的運算,
    在最左及最右多保留1個位元。故若該列所有格子均為1, 則其值為 0111...1110b。

   分析:
    假設正在翻轉(flip)第n列時, 1~(n-2)均已解答(全部已翻轉為零), 因此要設法消
    除第(n-1)列中的1, 這時就只有翻轉正下方第n列格子, 這樣會影響上方(目標1)、
    自己、左方、右方及正下方。逐一將第(n-1)列的1消滅, 這樣第n列的狀態在下一
    迴圈, 就成為決定第(n+1)翻轉的結果。最後一列狀態(由倒數第二列的狀態決定)
    必須為零才表示有解。

    以下圖例不限 4 x 4 棋盤:

        ○○○○
        ...
        ○○○○ <- 以上列已翻轉完成(全部為零)
        ●○●○ <- n-1列: 設法將這列的1消去, 也就是翻轉正下方的格子
        ●○○○ <- 第n列: 正要翻轉的列, 因此要翻轉第1和3的格子
        ○○○● <- n+1列: 本列狀態會受上列翻轉影響
        ○●○○

    由於不知第1列狀況應如何翻轉, 因此必須將第1列所有可能情況都試一次:
        ? ? ? ?  <- 虛擬例: 從 0000b 到 1111b 全部都試一次
        ●○○● <- 第1列: 正要翻轉的列
        ○○○● <- 第2列: 本列狀態會受上列翻轉影響
        ○●○●
        ...
        ○●○○

    最後一列開始處翻轉:
        ○○○○
        ...
        ○○○○ <- 以上列已翻轉完成(全部為零)
        ●○●○ <- 倒數第2列: 設法將這列的1消去, 也就是翻轉正下方的格子
        ●○○○ <- 最後一列: 正要翻轉的列, 因此要翻轉第1和3的格子
        ○○○● <- 虛擬列: 本列狀態會受上列翻轉影響, 本列不會用到

    最後一列翻轉後結果:
        ? ? ? ?  <- 虛擬列: 0000b 到 1111b 的其中之一  <--------------------+
        ○○○○                                                            |
        ...                                                                 |
        ○○○○ <- 以上列已翻轉完成(全部為零)                              |
        ○○○○ <- 倒數第2列: 這列的1已全部消去                            |
        ○○●● <- 最後一列: 翻轉後的狀態, 此即本次結果, 因含有1故失敗 ----+
        ●○●● <- 虛擬列: 已結束用不到

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
#include <cctype>
#include <assert.h>

using namespace std;

typedef unsigned long long ULL;
typedef vector<ULL> vstate;

const int MAX_SIZE = 8 * sizeof(ULL);       // bits for columns used

static int debug;

static int bit_count(ULL x)
{
    bitset<MAX_SIZE> t(x);
    return t.count();
}

static string pair_str(const char *title, int a, int b)
{
    stringstream os;

    os << title << "(" << a << "," << b << ")";
    return os.str();
}

class Board {
  private:
    vstate _board;          // bit-map columns for each rows
    vstate _press;          // flip bit-map columns for each rows
    ULL _mask;              // bit-mask for columns: 011...110b
    int _rotate;            // exchange (row, col)
    int _rows;              // 1 ~
    int _cols;              // 1 ~ MAX_SIZE-2
    int _min_rounds;        // minimum number of rounds

    void initialize(int rows, int cols);
    void validate(int row, int col) const;
    void output_state(const vstate &state) const;
    void output_press1(int row, int rounds) const;
    void output_bits(const char *title, ULL t) const;
    int flip_to_zero(int minRounds);
    void reverse_board();

  public:
    Board(int rows, int cols) { initialize(rows, cols); }
    Board(int size) { initialize(size, size); }
    void resize(int rows, int cols) { initialize(rows, cols); }
    void resize(int size) { initialize(size, size); }
    void clear();
    void put_row(int row, const string &s);         // row: 1 ~ _rows
    void output_board() const;
    void output_press(int rounds) const;
    int resolve(int mode);
};

void Board::initialize(int rows, int cols)
{
    _rotate = (rows < cols);
    if (_rotate) swap(rows, cols);

    if (rows <= 0 || cols <= 0 || cols >= MAX_SIZE-1)
        throw runtime_error("Invalid board "+pair_str("size", rows, cols));

    _rows = rows;
    _cols = cols;
    _board.resize(_rows+1);
    _mask = (1 << (_cols+1)) - 2;       // 0011...110: there are (_cols)'s 1
    clear();
}

void Board::validate(int row, int col) const
{
    if (row < 1 || row > _rows || col < 1 || col > _cols)
        throw runtime_error(pair_str("Cell", row, col)+" out of board");
}

void Board::clear()
{
    fill(_board.begin(), _board.end(), 0);
    _press.clear();
    _min_rounds = 0;
}

static bool is_one(char c)
{
    switch(tolower(c)) {
      case '1': // One
      case 'b': // Black
      case 't': // True
      case 'x': // X-mark
        return true;
    }
    return false;
}

void Board::put_row(int row, const string &s)
{
    if (_rotate) {
        validate(s.size(), row);
        int bit1 = 1 << (row+1);
        for (int i = 0; i < s.size(); i++) {
            if (is_one(s[i])) _board[i] |= bit1;
        }
    } else {
        validate(row, s.size());
        ULL cols = 0;
        for (int i = s.size(); --i >= 0;) {
            if (is_one(s[i])) cols |= 1;
            cols <<= 1;
        }
        _board[row-1] = cols;
    }
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
    if (_rotate) cout << " (rotation)";
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

void Board::reverse_board()
{
    for (vstate::iterator it = _board.begin(); it != _board.end(); ++it) {
        *it = ~(*it) & _mask;
    }
}

// mode == 0,1,3: resolve normal board
// mode == 0,2,3: resolve reverse board
int Board::resolve(int mode)
{
    int rounds = MAX_SIZE*MAX_SIZE+1;

    if (mode != 2) rounds = flip_to_zero(rounds);

    if (mode != 1) {
        reverse_board();
        if (debug) {
            cout << "Reverse ";
            output_board();
        }

        rounds = min(rounds, flip_to_zero(rounds));
        reverse_board();
    }

    _min_rounds = rounds < MAX_SIZE * MAX_SIZE ? rounds : -1;
    return _min_rounds;
}

int Board::flip_to_zero(int minRounds)
{
    _press.resize(_rows+1);

    // 將 row[0] 每一狀況都試一次: 2^(_cols)
    ULL flips = 0;
    do {
        if (bit_count(flips) >= minRounds) continue;

        int rounds = 0;
        ULL row0 = flips << 1;          // 前一列狀態: 要設法將之變為零
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

            ULL bits_111 = 0x07;    // 111b: 左/本身/右
            ULL bits_010 = 0x02;    // 010b: 下
            while ((row0 >>= 1) != 0) {
                if (row0 & 1) { // 根據 row0 來決定是否翻轉對應的 row1,row2
                    row1 ^= bits_111;   // 左/本身/右
                    row2 ^= bits_010;   // 下
                }
                bits_111 <<= 1;
                bits_010 <<= 1;
            }

            row0 = row1 & _mask;        // 下一列的解決方案: 消去狀態為 1 的
            row1 = row2 & _mask;        // 下一列經前面步驟異動後的棋盤狀態
            _press[n] = row0;           // _press[] 有多一列, 多的這列沒用到

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
    } while (++flips < (1 << _cols));

    return minRounds;
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

static void put_board(Board &B, int &row, const string &s)
{
    const char *p = s.c_str();
    while (*p != 0) {
        while (isblank(*p)) p++;

        const char *t = p;
        while (*p != 0 && !isblank(*p)) p++;

        if (p == t+1) {     // assume one line: X X - X ...
            string ss;
            while (*t != 0) {
                if (!isblank(*t)) ss.push_back(*t);
                t++;
            }
            B.put_row(row++, ss);
            return;
        }

        string ss(t, p-t);
        B.put_row(row++, ss);
    }
}

int main(int argc, char *argv[])
{
    int rows = 0;
    int cols = 0;
    int mode = 0;

    for (int n = 1; n < argc; n++) {
        if (strncmp(argv[n], "-d", 2) == 0) {
            debug += strlen(&argv[n][1]);
        } else if (argv[n][0] == '-') {
            mode = atoi(&argv[n][1]) & 3;
        } else {
            if (rows == 0) {
                rows = atoi(argv[n]);
            } else if (cols == 0) {
                cols = atoi(argv[n]);
            }
        }
    }

    if (rows == 0) rows = 4;
    if (cols == 0) cols = 4;

    Board B(rows, cols);
    string s;
    while (!getline(cin, s).eof()) {
        if (s.empty()) continue;

        int n = 1;
        switch(number_count(s)) {
          case 2:
            {
                stringstream os(s);
                os >> rows >> cols;
                B.resize(rows, cols);
            }
            break;
          case 1:
            if (s.size() == 1) continue;
            // fall-through
          default:
            B.clear();
            put_board(B, n, s);
            break;
        }

        while (n <= rows && !getline(cin, s).eof()) {
            if (!s.empty()) put_board(B, n, s);
        }

        if (cin.eof()) {
            cout << "<<< EOF >>>" << endl;
            break;
        }

        if (debug) B.output_board();

        int answer = B.resolve(mode);
        if (debug) {
            cout << "--> " << answer << endl << endl;
        } else {
            if (answer >= 0) {
                cout << answer << endl;
            } else {
                cout << "Impossible" << endl;
            }
        }
    }

    return 0;
}

