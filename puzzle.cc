// puzzle.cc
// 截自教材內容

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

#define MAX_SIZE 20
static int nRow = 5;
static int nCol = 6;
static int puzzle[MAX_SIZE+1][MAX_SIZE+2];
static int press[MAX_SIZE+1][MAX_SIZE+2];

static int guess()
{
    int c, r;
    int step = 0;

    for (c = 1; c <= nCol; c++) {
        step += press[1][c];
    }

    // 根据press第1行和 puzzle数组, 计算press其他行的值
    for (r = 1; r < nRow; r++) {
        for (c = 1; c <= nCol; c++) {
            int hit = (puzzle[r][c]+press[r][c]+ press[r-1][c]+
                       press[r][c-1]+press[r][c+1]) % 2;
            press[r+1][c] = hit;
            if (hit) step++;
        }
    }

    for (c = 1; c <= nCol; c++) {
        // 判断所计算的 press数组能否熄 灭第nRow行的所有灯
        if ((press[nRow][c-1] + press[nRow][c] + press[nRow][c+1] +
            press[nRow-1][c]) % 2 != puzzle[nRow][c]) return -1;
    }
    return step;
}

static void enumerate ()
{
    int minStep = MAX_SIZE * MAX_SIZE;
    int c;

    memset(press, 0, sizeof(press));

    c = 1;
    while (c <= nCol) {
        int step = guess();
        if (step >= 0) {
            // output press[][]
            int rr, cc;
            for (rr = 1; rr <= nRow; rr++){
                for (cc = 1; cc <= nCol; cc++) {
                    cout << press[rr][cc] << " ";
                }
                cout << endl;
            }
            cout << "--> " << step << endl;
            if (step < minStep) minStep = step;
        }

        press[1][1]++;
        c = 1;

        // 对press第1行的元素 press[1][1]~press[1][6]各种取值情况进行枚举,
        // 依次考虑:
        // 000000
        // 100000
        // 010000
        // 110000
        // 001000
        // ...
        // 111111
        while (press[1][c] > 1) {
            press[1][c] = 0;
            c++;
            if (c > nCol) break;
            press[1][c]++;
        }
    }

    if (minStep < MAX_SIZE*MAX_SIZE) {
        cout << "Minus step is " << minStep << endl;
    }
    return;
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        for (int n = 1; n < argc; n++) {
            nRow = nCol = atoi(argv[n]);
            for (int r = 1; r <= nRow; r++) {
                for (int c = 1; c <= nCol; c++) {
                    puzzle[r][c] = 1;
                }
            }
            cout << "PUZZLE #" << n << " (" << nRow << "x" << nCol << ")" << endl;
            enumerate();
        }
        return 0;
    }

    int cases, i, r, c;

    cin >> cases;

    for (i = 1; i <= cases; i++) {
        cin >> nRow >> nCol;

        //读入输入数据
        for (r = 1; r <= nRow; r++) {
            for (c = 1; c <= nCol; c++) {
                cin >> puzzle[r][c];
            }
        }

        cout << "PUZZLE #" << i << " (" << nRow << "x" << nCol << ")" << endl;
        enumerate();
    }

    return 0;
}
