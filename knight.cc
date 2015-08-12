/* knight.cc -- tonychen@finenet.com.tw

   Background
    The knight is getting bored of seeing the same black and white squares
    again and again and has decided to make a journey around the world.
    Whenever a knight moves, it is two squares in one direction and one
    square perpendicular to this. The world of a knight is the chessboard
    he is living on. Our knight lives on a chessboard that has a smaller
    area than a regular 8 * 8 board, but it is still rectangular. Can you
    help this adventurous knight to make travel plans?

   Problem
    Find a path such that the knight visits every square once. The knight
    can start and end on any square of the board.

   输入
    The input begins with a positive integer n in the first line. The
    following lines contain n test cases. Each test case consists of a
    single line with two positive integers p and q, such that
    1 <= p * q <= 26. This represents a p * q chessboard, where p
    describes how many different square numbers 1, . . . , p exist,
    q describes how many different square letters exist. These are the
    first q letters of the Latin alphabet: A, . . .

   输出
    The output for every scenario begins with a line containing
    "Scenario #i:", where i is the number of the scenario starting at 1.
    Then print a single line containing the lexicographically first path
    that visits all squares of the chessboard with knight moves followed
    by an empty line. The path should be given on a single line by
    concatenating the names of the visited squares. Each square name
    consists of a capital letter followed by a number.

    If no such path exist, you should output impossible on a single line.

   样例输入
    3
    1 1
    2 3
    4 3

   样例输出
    Scenario #1:
    A1

    Scenario #2:
    impossible

    Scenario #3:
    A1B3C1A2B4C2A3B1C3A4B2C4

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
typedef vector<int> vint;

static int debug;

class Board {
  private:
    vchar _board;           // 0: empty
    vint _step;
    int _rows;
    int _cols;
    int _size;              // _rows x _cols

    void initialize();
    bool search(int npos);

  public:
    Board(int rows, int cols) : _rows(rows), _cols(cols) {
        initialize();
    }
    void output_board() const;
    void output(ostream &os) const;
    int solve();
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

void Board::output_board() const
{
    cout << "Chess board: " << _rows << " x " << _cols;
    for (int offset = 0; offset < _size; offset++) {
        if (offset % _cols == 0) cout << endl; else cout << "\t";
        int k = _board[offset];
        if (k) cout << k; else cout << "-";
    }
    cout << endl << endl;
}

void Board::output(ostream &os) const
{
    // impossible
    if (_step.size() == 0) {
        os << "impossible" << endl;
        return;
    }

    // A1B3C1A2B4C2A3B1C3A4B2C4
    for (int i = 0; i < _step.size(); i++) {
        int r = _step[i] / _cols;
        int c = _step[i] % _cols;
        cout << (char)('A'+r) << (char)('1'+c);
    }
    os << endl;
}

bool Board::search(int npos)
{
    if (npos < 0 || npos >= _size) return false;

    // 是否已經佔用
    if (_board[npos]) return false;

    // 記錄步驟及狀態
    _step.push_back(npos);
    _board[npos] = _step.size();
    if (_step.size() >= _size) return true;

    if (debug > 1) {
        cout << "Enter(" << _step.size() << "):";
        output(cout);
        if (debug > 2) output_board();
    }

    int c = npos % _cols;
    if (c+1 < _cols) {
        if (search(npos-2*_cols+1)) return true;    // step 1
        if (search(npos+2*_cols+1)) return true;    // step 4
    }
    if (c+2 < _cols) {
        if (search(npos-  _cols+2)) return true;    // step 2
        if (search(npos+  _cols+2)) return true;    // step 3
    }
    if (c > 0) {
        if (search(npos+2*_cols-1)) return true;    // step 5
        if (search(npos-2*_cols-1)) return true;    // step 8
    }
    if (c > 1) {
        if (search(npos+  _cols-2)) return true;    // step 6
        if (search(npos-  _cols-2)) return true;    // step 7
    }

    // 清除步驟及狀態
    _board[npos] = 0;
    _step.pop_back();

    if (debug > 1) {
        cout << "Return(" << _step.size() << "): ";
        output(cout);
    }

    return false;
}

int Board::solve()
{
    fill(_board.begin(), _board.end(), 0);
    _step.clear();

    for (int n = 0, sz2 = (_size+1)/2; n < sz2; n++) {
        if (search(n)) {
            assert(_step.size() > 0);
            return 1;
        }
    }

    assert(_step.size() == 0);
    return 0;
}

//---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    if (argc > 1 && strncmp(argv[1], "-d", 2) == 0) debug = strlen(&argv[1][1]);

    int n = 0;
    cin >> n;

    for (int i = 1; i <= n; i++) {
        int p, q;
        cin >> p >> q;
        if (cin.eof()) {
            cout << "*** EOF ***" << endl;
            break;
        }
        
        assert(1 <= p && p <= 26 && 1 <= q && q <= 26);

        Board B(q, p);
        B.solve();

        if (debug) B.output_board();

        cout << "Scenario #" << i << ":" << endl;
        B.output(cout);
        cout << endl;
    }

    return 0;
}

