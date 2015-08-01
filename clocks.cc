/* clocks.cca -- tonychen@finenet.com.tw

   有9个时钟，排成一个3*3的矩阵。

    |-------|    |-------|    |-------|
    |       |    |       |    |   |   |
    |---O   |    |---O   |    |   O   |
    |       |    |       |    |       |
    |-------|    |-------|    |-------|
       A:0          B:1          C:2  

    |-------|    |-------|    |-------|
    |       |    |       |    |       |
    |   O   |    |   O   |    |   O   |
    |   |   |    |   |   |    |   |   |
    |-------|    |-------|    |-------|
       D:3          E:4          F:5  

    |-------|    |-------|    |-------|
    |       |    |       |    |       |
    |   O   |    |   O---|    |   O   |
    |   |   |    |       |    |   |   |
    |-------|    |-------|    |-------|
       G:6          H:7          I:8  
   
   现在要用最少的移动，将9个时钟的指针都拨到12点的位置。共允许有9种不同的移动。
   如下表所示，每个移动会将若干个时钟的指针沿顺时针方向拨动90度。

    移动    影响的时钟
     1         ABDE
     2         ABC
     3         BCEF
     4         ADG
     5         BDEFH
     6         CFI
     7         DEGH
     8         GHI
     9         EFHI    

   输入
    从标准输入设备读入9个整数，表示各时钟指针的起始位置。
    0=12点、1=3点、2=6点、3=9点。

   输出
    输出一个最短的移动序列，使得9个时钟的指针都指向12点。
    按照移动的序号大小，输出结果。

   样例输入
    3 3 0 
    2 2 2 
    2 1 2 

   样例输出
    4 5 8 9 

 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <limits>
#include <cstring>
#include <assert.h>

using namespace std;

typedef vector<int> step_t;

//===========================================================================

inline int M3(char ch)
{
    return 3 << (ch-'A')*3;
}

const int MASKS = 0b011011011011011011011011011;
static int clocks;                                  // 9 x 3-bits for (A~I)
static int twist[10][2] = {
        {     0  ,   0                           }, // 0: not used
        // ready  twist for 9 clocks (each 3-bits)
        //  mask,      I  H  G  F  E  D  C  B  A 
        {     0  , 0b000000000000001001000001001 }, // 1:ABDE
        {     0  , 0b000000000000000000001001001 }, // 2:ABC
        {     0  , 0b000000000001001000001001000 }, // 3:BCEF
        {     0  , 0b000000001000000001000000001 }, // 4:ADG
        { M3('A'), 0b000001000001001001000001000 }, // 5:BDEFH -> A must be ready
        { M3('B'), 0b001000000001000000001000000 }, // 6:CFI   -> B must be ready
        { M3('C'), 0b000001001000001001000000000 }, // 7:DEGH  -> C must be ready
        { M3('D'), 0b001001001000000000000000000 }, // 8:GHI   -> D must be ready
        { M3('G'), 0b001001000001001000000000000 }  // 9:EFHI  -> G must be ready
    };
static int answerCount;
static step_t history;
static step_t minHistory;
static bool debug = false;
static int searchCount;

static string get_clocks_name(int t)
{
    string s;

    char ch = 'A';
    while (t != 0) {
        if (t & 0x03) s.append(1, ch);
        t >>= 3;
        ch++;
    }

    return s;
}

static void output_history(const string &title, const step_t &h)
{
    if (!title.empty()) cout << title << ": [" << h.size() << "] ";
    for (step_t::const_iterator it = h.begin(); it != h.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}

static void output_clocks()
{
    cout << "Clocks:" << endl;
    int t = clocks;
    for (int i = 1; i <= 9; i++) {
        cout << (t & 0x03);
        if (i % 3 == 0)
            cout << endl;
        else 
            cout << " ";
        t >>= 3;
    }
}

// nth = rule 1~9
static int dfs(int nth)
{
    searchCount++;

    if (clocks == 0) {
        answerCount++;
        if (answerCount == 1 || history.size() < minHistory.size()) {
            minHistory = history;
            if (debug) {
                output_history("--> History", history);
                cout << endl;
            }
        }
        return 1;   // found
    }

    if (nth > 9) return 0;  // try next

    // check clocks ready status for rule twist[nth]
    if ((clocks & twist[nth][0]) != 0) {
        if (debug) {
            ostringstream os;
            os << "#" << nth << " not ready("
                << get_clocks_name(clocks & twist[nth][0]) << ")";
            output_clocks();
            output_history(os.str(), history);
            cout << endl;
        }
        return -1;    // not ready for the new path: history + [nth ...]
    }

    int last_clocks = clocks;
    int n = 0;

    // twist the clocks with twist[nth][1] 4 times
    for ( ; ; ) {
        if (dfs(nth+1) > 0) break;   // success or failure
        if (answerCount > 0 && history.size()+1 >= minHistory.size()) break;

        if (n >= 3) break;
        clocks = (clocks + twist[nth][1]) & MASKS;
        history.push_back(nth);
        n++;
    }

    // restore last clocks and history
    clocks = last_clocks;
    while (n > 0) {
        history.pop_back();
        n--;
    }

    return 0;   // try next
}

static void run()
{
    answerCount = 0;
    searchCount = 0;
    history.clear();
    minHistory.clear();

    if (debug) {
        output_clocks();
        cout << endl;
    }
    
    dfs(1);

    if (answerCount > 0) {
        output_history(debug ? "Answer" : "", minHistory);
    } else {
        cout << "Unresolved!" << endl;
    }

    if (debug) {
        cout << "Search count = " << searchCount << endl;
    }
}

//===========================================================================

int main (int argc, char *argv[])
{
    int n = 1;
    if (n < argc && strcmp(argv[n], "-d") == 0) {
        debug = true;
        n++;
    }

    clocks = 0;
    if (n < argc) {
        int i = 0;
        while (n < argc && i < 9) {
            int t = atoi(argv[n++]);
            clocks |= (t & 3) << 3*i;
            i++;
        }
    } else {
        for (int i = 0; i < 9; i++) {
            int t;
            cin >> t;
            clocks |= (t & 3) << 3*i;
        }
    }

    run();

    return 0;
}

