/* skiing.cc

   描述
    Michael喜欢滑雪百这并不奇怪， 因为滑雪的确很刺激。可是为了获得速度，滑的
    区域必须向下倾斜，而且当你滑到坡底，你不得不再次走上坡或者等待升降机来载
    你。Michael 想知道载一个区域中最长的滑坡。区域由一个二维数组给出。数组的
    每个数字代表点的高度。下面是一个例子

    1 2 3 4 5
    16 17 18 19 6
    15 24 25 20 7
    14 23 22 21 8
    13 12 11 10 9

    一个人可以从某个点滑向上下左右相邻四个点之一，当且仅当高度减小。在上面的
    例子中，一条可滑行的滑坡为24-17-16-1。当然25-24-23-...-3-2-1更长。事实上，
    这是最长的一条。

   输入
    输入的第一行表示区域的行数R和列数C(1 <= R,C <= 100)。下面是R行，每行有C
    个整数，代表高度h，0<=h<=10000。

   输出
    输出最长区域的长度。

   样例输入
    5 5
    1  2  3  4  5
    16 17 18 19 6
    15 24 25 20 7
    14 23 22 21 8
    13 12 11 10 9

   样例输出
    25

 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <assert.h>

using namespace std;

typedef vector<int> vint;

const int MAX_SIDE = 100;
const int MAX_HEIGHT = 10000;

static vint board;
static vint state;
static int rows, cols;
static int size;
static int debug;

static void output_board(const char *title, const vint &x)
{
    cout << title << ":";
    for (int i = 0; i < size; i++) {
        if (i % cols == 0) cout << endl; else cout << "\t";
        cout << x[i];
    }
    cout << endl << endl;
}

static int dfs(int n)
{
    int step = state[n];
    if (debug > 1) {
        cout << board[n] << ": [" << n << "] = " << step << endl;
    }
    if (step != 0) return step;
    state[n] = -1;                              // in processing

    int next = n - cols;
    if (next >= 0 && board[n] > board[next]) {          // up
        step = max(step, dfs(next));
    }

    next = n + cols;
    if (next < size && board[n] > board[next]) {        // down
        step = max(step, dfs(next));
    }

    if (n % cols > 0 && board[n] > board[n-1]) {        // left
        step = max(step, dfs(n-1));
    }

    if ((n+1) % cols > 0 && board[n] > board[n+1]) {    // right
        step = max(step, dfs(n+1));
    }

    state[n] = ++step;
    if (debug > 1) {
        cout << board[n] << ": [" << n << "] <-- " << step << endl;
    }
    return step;
}

static int find_max_step()
{
    state.resize(size);
    fill(state.begin(), state.end(), 0);

    int max_step = 0;
    for (int i = 0; i < size; i++) {
        if (state[i] == 0) {
            int step = dfs(i);
            if (step > max_step) max_step = step;
            if (debug) {
                if (debug > 1) cout << endl;
                output_board("State", state);
            }
            assert(step > 0 && step == state[i]);
        }
    }

    return max_step;
}

static int get_val()
{
    int v;
    cin >> v;
    assert(0 <= v && v <= MAX_HEIGHT);
    return v;
}

int main(int argc, char *argv[])
{
    if (argc > 1 && strncmp(argv[1], "-d", 2) == 0) {
        debug = strlen(&argv[1][1]);
    }

    while (!(cin >> rows >> cols).eof()) {
        assert(1 <= rows && rows <= MAX_SIDE && 1 <= cols && cols <= MAX_SIDE);

        size = rows * cols;
        board.resize(size);
        for (int i = 0; i < size; i++) {
            board[i] = get_val();
        }

        if (debug) output_board("Board", board);

        int step = find_max_step();
        if (debug) cout << "Answer: ";
        cout << step << endl;
        if (debug) cout << endl;
    }

    board.clear();
    state.clear();

    return 0;
}

