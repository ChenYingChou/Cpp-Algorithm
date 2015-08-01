/* board.cc -- tonychen@finenet.com.tw

   一张普通的国际象棋棋盘，它被分成 8 乘 8 (8 行 8 列) 的 64 个方格。设有形状
   一样的多米诺牌，每张牌恰好覆盖棋盘上相邻的两个方格，即一张多米诺牌是一张 1
   行 2 列或者 2 行 1 列的牌。那么，是否能够把 32 张多米诺牌摆放到棋盘上，使得
   任何两张多米诺牌均不重叠，每张多米诺牌覆盖两个方格，并且棋盘上所有的方格都
   被覆盖住？我们把这样一种排列称为棋盘被多米诺牌完美覆盖。这是一个简单的排列
   问题，同学们能够很快构造出许多不同的完美覆盖。但是，计算不同的完美覆盖的总
   数就不是一件容易的事情了。不过，同学们发挥自己的聪明才智，还是有可能做到的。

   现在我们通过计算机编程对 3 乘 n 棋盘的不同的完美覆盖的总数进行计算。

   +--+--+-----+-----+-----+--+-----+--+
   |  |  |     |     |     |  |     |  |
   |  |  +--+--+--+--+-----+  +-----+  |
   |  |  |  |     |  |     |  |     |  |
   +--+--+  +-----+  +-----+--+--+--+--+
   |     |  |     |  |     |     |     |
   +-----+--+-----+--+-----+-----+-----+

   任务
    对 3 乘 n 棋盘的不同的完美覆盖的总数进行计算。

   输入
    一次输入可能包含多行，每一行分别给出不同的 n 值 (即 3 乘 n 棋盘的列数)。
    当输入 -1 的时候结束。

    n 的值最大不超过 30.

   输出
    针对每一行的 n 值，输出 3 乘 n 棋盘的不同的完美覆盖的总数。

   样例输入
    2
    8
    12
    -1

   样例输出
    3
    153
    2131

 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstring>

using namespace std;

//===========================================================================

const int MAX_ROWS = 31;
const int MAX_COLS = 31;
const char EOL = 0;

static char board[(MAX_ROWS+1)*(MAX_COLS+1)];
static char *end_board;
static int row_size = 3;
static int col_size;        // include EOL
static int answerCount;
static bool debug = false;

static void output()
{
    const char *p = board;
    for (int r = 0; r < row_size; r++) {
        cout << p << endl;
        p += col_size;
    }
    cout << endl;
}

static bool no_l_space(const char *p)
{
    // check left side
    if (*(p-1) == ' ' && *(p-2) != ' ') return true;

    return false;
}

static int dfs(char *p)
{
    // find next empty cell
    while (p < end_board) {
        if (p[0] == ' ') {
            int sts = (p[1] == ' ' ? 1 : 0);
            if (p[col_size] == ' ') sts |= 2;
            if (sts == 0) return -1;    // unresolved

            if (sts & 1) {
                p[0] = p[1] = '-';
                dfs(p+2);
                p[0] = p[1] = ' ';
            }
            if (sts & 2) {
                if (p+2*col_size >= end_board && no_l_space(p+col_size)) return -1;
                p[0] = p[col_size] = '|';
                dfs(p+1);
                p[0] = p[col_size] = ' ';
            }

            return 0; // try next
        }
        p++;
    }

    answerCount++;
    if (debug) output();
    return 1;   // done
}

static void run(int width)
{
    answerCount = 0;
    
    if (width > 0 && width <= MAX_COLS && (row_size & width & 1) == 0) {
        col_size = width+1;
        memset(board, ' ', sizeof(board));
        {
            char *p = board + width;
            for (int i = 0; i < row_size; i++) {
                *p = EOL;
                p += col_size;
            }
            end_board = board + (row_size * col_size);
            memset(end_board, EOL, col_size);
        }
        dfs(board);
    }

    cout << answerCount << endl;
}

//===========================================================================

int main (int argc, char *argv[])
{
    int n = 1;
    if (n < argc && strcmp(argv[n], "-d") == 0) {
        debug = true;
        n++;
    }
    if (n < argc && argv[n][0] == '-') {
        row_size = -atoi(argv[n]);
        if (row_size < 3 || row_size > MAX_ROWS) {
            cerr << "Invalid rows: " << row_size << endl;
            return 1;
        }
        n++;
    }

    if (n < argc) {
        while (n < argc) {
            run(atoi(argv[n++]));
        }
    } else {
        while (!(cin >> n).eof()) {
            if (n == -1) break;
            run(n);
        }
    }

    return 0;
}

