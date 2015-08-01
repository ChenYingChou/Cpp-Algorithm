/* toggle.cc -- tonychen@finenet.com.tw

   將牆面全部塗為黃色(y)

   Input:
    2           <- 組數
    3           <- 第1組 3x3
    yyy 
    yyy 
    yyy 
    5           <- 第2組 5x5
    wwwww 
    wwwww 
    wwwww 
    wwwww 
    wwwww 

   Output:
    0           <- 輸出最少按下次數
    15
 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstring>
#include <assert.h>

using namespace std;

//===========================================================================

const int MAX_NUM = 15;
const int MAX_SIZE = (MAX_NUM+1) * (MAX_NUM+2);

static char board[MAX_SIZE];
static char press[MAX_SIZE];
static int nSize;               // Input size
static int wSize;               // width size = nSize + 2
static bool debug = false;

static void output(const char *title, const char data[], int val)
{
    cout << title << ": " << val << endl;
    int offset = wSize+1;
    for (int r = 1; r <= nSize; r++) {
        for (int c = 1; c <= nSize; c++) {
            cout << (int)data[offset++];
        }
        cout << endl;
        offset += 2;
    }
    cout << endl;
}

static int hit_status(int offset) {
    return board[offset] ^ press[offset-wSize]
        ^ press[offset-1] ^ press[offset] ^ press[offset+1] ^ 1;
}

static int guess()
{
    int hitCount = 0;
    int offset = wSize+1;   // offset of row[1][1]

    for (int k = 0; k < nSize; k++) {
        hitCount += press[offset+k];
    }

    // calculate row[2] ~ row[last]
    for (int r = 1; r < nSize; r++) {       
        // calculate row[r+1]
        // offset = index of row[r+1][1]
        for (int c = 1; c <= nSize; c++) {
            int hit = hit_status(offset);
            press[offset+wSize] = hit;
            if (hit) hitCount++;
            offset++;
        }
        offset += 2;
    }

    for (int c = 1; c <= nSize; c++) {
        int status = hit_status(offset);
        if (status == 1) return -1;
        offset++;
    }

    if (debug) output("press", press, hitCount);
    return hitCount;
}

static void run(int width)
{
    int minStep = MAX_SIZE;
    int answerCount = 0;

    nSize = width;
    wSize = nSize + 2;

    if (debug) output("board", board, width);
    memset(press, 0, sizeof(press));
    for (int n = 1 << nSize; n > 0; n--) {
        int step = guess();
        if (step >= 0 && step < minStep) {
            answerCount++;
            minStep = step;
            if (step <= 1) break;
        }

        char *r1 = &press[wSize];
        for (int k = 1; k <= nSize; k++) {
            r1[k]++;
            if (r1[k] <= 1) break;
            r1[k] = 0;
        }
    }

    if (debug) cout << "--> ";

    if (answerCount > 0) {
        cout << minStep << endl;
    } else {
        cout << "inf" << endl;
    }

    if (debug) cout << endl;
}

//===========================================================================

int main (int argc, char *argv[])
{
    int n = 1;
    if (argc > 1 && strcmp(argv[1], "-d") == 0) {
        debug = true;
        n++;
    }

    if (n < argc) {
        while (n < argc) {
            int size = atoi(argv[n]);
            if (size > 0) {
                // empty board
                memset(board, 0, sizeof(board));
                run(size);
            } else {
                cout << "Invalid size: " << argv[n] << endl;
            }
            n++;
        }
        return 0;
    }

    int nCase;
    cin >> nCase;
    
    for (int n = 1; n <= nCase; n++) {
        int size;
        cin >> size;

        memset(board, 0, sizeof(board));
        {
            int offset = size+2+1;  // index of row[1][1]
            for (int row = 1; row <= size; row++) {
                for (int col = 1; col <= size; col++) {
                    char ch;
                    cin >> ch;
                    board[offset++] = (ch == 'y' ? 1 : 0);
                }
                offset += 2;
            }
        }
        run(size);
    }

    return 0;
}

