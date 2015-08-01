/* sudoku.cc -- tonychen@finenet.com.tw

   Input:
    1
    103000509
    002109400
    000704000
    300502006
    060000050
    700803004
    000401000
    009205800
    804000107

   Output:
   Case:1
   -----------
   143 628 579
   572 139 468
   986 754 231
   -----------
   391 542 786
   468 917 352
   725 863 914
   -----------
   237 481 695
   619 275 843
   854 396 127
   -----------

 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstring>
#include <assert.h>

using namespace std;

//===========================================================================

// row,col,box = 0~8

static int sdkRow[9];       // bits-map for 9 rows: bits 1~9
static int sdkCol[9];       // bits-map for 9 cols: bits 1~9
static int sdkBox[9];       // bits-map for 9 boxs: bits 1~9
static char board[9][9];    // contents(1~9) of board

static int rowcol2box(int row, int col)
{
    return row / 3 * 3 + col / 3;
}

static bool store(int row, int col, int val)
{
    assert(1 <= val && val <= 9);
    int bit = 1 << val;

    if (sdkRow[row] & bit) return false;
    if (sdkCol[col] & bit) return false;
    int nth = rowcol2box(row,col);
    if (sdkBox[nth] & bit) return false;

    sdkRow[row] |= bit;
    sdkCol[col] |= bit;
    sdkBox[nth] |= bit;

    board[row][col] = val;

    return true;
}

static void reset(int row, int col, int val)
{
    if (val) {
        int mask = ~(1 << val);
        sdkRow[row] &= mask;
        sdkCol[col] &= mask;
        sdkBox[rowcol2box(row,col)] &= mask;
    }
}

static int find_next_val(int row, int col)
{
    int val = board[row][col];
    reset(row, col, val);

    while (++val <= 9) {
        if (store(row, col, val)) return val;
    }

    board[row][col] = 0;
    return 0;
}

static bool fill_next_slot(int row, int col)
{
    while (row < 9) {
        while (col < 9) {
            // find empty slot
            if (board[row][col] == 0) {
                while (find_next_val(row, col)) {
                    if (fill_next_slot(row, col+1) == true) return true;
                }
                // none avaiable for the slot, so current board is unsolved
                return false;
            }
            col++;
        }
        col = 0;
        row++;
    }

    // all slots was filled, current board is answer
    return true;
}

static void run()
{
    fill(sdkRow, sdkRow+9, 0);
    fill(sdkCol, sdkCol+9, 0);
    fill(sdkBox, sdkBox+9, 0);

    // initialize bit-maps
    bool error_flag = false;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int val = board[row][col];
            if (val && store(row, col, val) == false) {
                cout << ">>> Duplicated for ("
                    << (row+1) << "," << (col+1)
                    << ") = " << val
                    << endl;
                error_flag = true;
            }
        }
    }

    if (error_flag || fill_next_slot(0, 0)) {
        for (int row = 0; row < 9; row++) {
            if (row % 3 == 0) cout << "-----------" << endl;
            for (int col = 0; col < 9; col++) {
                if (col > 0 && col % 3 == 0) cout << " ";
                cout << (int)board[row][col];
            }
            cout << endl;
        }
        cout << "-----------" << endl;
    } else {
        cout << ">>> no answer!" << endl;
    }
    cout << endl;
}

//===========================================================================

int main (int argc, char *argv[])
{
    int nCase;
    cin >> nCase;

    for (int n = 1; n <= nCase; n++) {
        memset(board, 0, sizeof(board));
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                char c;
                cin >> c;
                board[row][col] = c & 0x0f;
            }
        }
        cout << "Case:" << n << endl;
        run();
    }

    return 0;
}

