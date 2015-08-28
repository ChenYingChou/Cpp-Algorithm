/* watch.cc -- tonychen [@] finenet.com.tw

   描述
    怀表是由一个表链和一个表盘连接而成的。同时，表链又是由多个长度为1的表链组
    件连接而成的。小明现有一个表盘和多个长度为1的表链组件。他希望组合成一个完
    整的怀表当做生日礼物送给爸爸。

    表盘和表链组件的两端都有表扣可以和其它的表链组件或者表盘相连。一共有两种表
    扣设计，分别是“L”和“V”。表链组件和表盘的左右两端可以使用相同的或者不同
    的表扣设计，因此我们有以下四种不同的表链组件和表盘设计方式，“LL”,“LV”,
    “VL”,“VV”，如“LV”表示左端的表扣设计是”L”,右端的表扣设计是“V”。
    另，表盘和表链是不允许上下左右翻转的。如果把两个表链组件或者一个表链组件和
    表盘相连接，那么相连部分的表扣设计必须一致。此外，还必须确保表盘可以连接到
    表链的任意一端。

    例子1：表盘是“LV”，共有5个表链组件分别是，“LL”,“LL”,“LV”,“VL”,
    “VL”。现在希望组合成长度为4的怀表（表链长度是4）。有两种正确的表链组合方
    式：“VLLLLVVL”和“VLLVVLLL”。而且，对于每一种表链的组合方式，表盘都可以
    连接到表链的左右两端(表盘放到表链左端使用V相连，表盘放到表链右端使用L相连)。

    例子2：表盘是“LL”，共有4个表链组件分别是，“LL”,“LV”,“VL”,“VV”。
    现在希望组合成长度为3的怀表（表链长度是3）。有三种正确的表链组合方式：
    “LLLVVL”,“LVVLLL”和“LVVVVL”。而且，对于每一种表链的组合方式，表盘都
    可以接到表链的任一端。

    注意怀表的组合方式可能不唯一。每一种表链的组合方式都可以当做是一个由”L”
    和“V”

    组成的字符串。如果两种表链对应的字符串是不匹配的，那么表链也是不相同的。

   输入
    标准的输入包含若干组测试数据。每组数据第一行是整数 N (0 < N ≤ 40) 和
    K (0 < K ≤ N)。
    N 代表表链组件的个数，K代表要组合成的怀表长度(表链组件的个数)。接着的
    N+ 1 行描述表盘（第二行）和表链组件的表扣设计。有四种可能输入：“LV”,
    “LL”,“VV”和“VL”。

   输出
    对于每组测试数据，输出一行。输出“YES”, 如果可能按照要求组合成怀表，
    并输出可能的组合方式的数目。如果不能，则输出”NO”.

   样例输入
    4 4
    LV
    LL
    LV
    VL
    VL
    4 4
    VL
    LL
    LV
    VL
    VL

   样例输出
    YES
    2
    NO

   提示: 本题需要使用 long long 数据类型。

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   疑惑:
    經提交得到回覆為"Time Limit Exceeded"。顯然不可以dfs遞歸方式來求解，應該
    要使用排列組合方式來求解，但又有每種樣式個數的限制。是否要以兩種方式來求
    解，或是僅用數學排列組合即可? 尚搞不清楚!

    例如: 表盤為LV，求n個表鍊的排列組合
        左 Ｖｘｘ...ｘｘ
        　 ↗↗↗...↗
        右 ？？？...？Ｌ

    第一個表鍊的左扣必需和表盤的右扣相同，且最後的表鍊右扣和表盤的左扣相同。
    由上圖可知表鍊右扣除了最後一個，其餘可隨意選擇，只要該表鍊樣式足夠即可。
    右扣樣式決定了，也就決定了下一個左扣的樣式，因此只要計算右扣 1~(n-1) 的
    變化即可。再來是計算若某一樣式不足時所產生的影響，也就是指定某些樣式值，
    而不是任意變化。但指定了右扣樣式，勢必影響下個表鍊的左扣樣式，也就是限定
    了下個表鍊的樣式可能變化，這樣又複雜了整個排列組合的計算!!

 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef unsigned long long ULL;

const unsigned int MAX_NUM = 40;

static int debug;

//---------------------------------------------------------------------------

static const char * LV(int x)
{
static const char *_s[] = { "LL", "LV", "VL", "VV" };
    return _s[x & 3];
}

static int LV(const string &s)
{
    int result = 0;
    for (int i = 0; i < 2; i++) {
        result <<= 1;
        if (i < s.size()) {
            switch(s[i]) {
              case 'v':
              case 'V':
              case '1':
                result |= 1;
                break;
            }
        }
    }
    return result;
}

//---------------------------------------------------------------------------

class Watch {
  private:
    int _chains[4];             // 各種表鏈的個數, LL:00, LV:01, VL:10, VV:11
    int _head;                  // 表盤: LL/LV/VL/VV

    int _lastLV;                // penultimate's right should match L:0 or V:1
    ULL _count;

    void dfs(int k, int prevChain);

  public:
    Watch(int head) : _head(head) {
        std::fill(_chains, _chains+4, 0);
    }
    void add_chain(int LV) { _chains[LV & 3]++; }
    ULL permutation(int k);
};

void Watch::dfs(int k, int prevChain)
{
    if (debug) {
        cout << "> dfs(" << k << ", " << LV(prevChain) << ")" << endl;
    }

    if (k <= 0) {
        if ((prevChain & 1) == _lastLV) {
            _count++;
            if (debug > 1) cout << "--> Find: " << _count << endl;
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        if (debug > 1) {
            cout << "> " << k << ":" << i << " "
                << LV(i) << "("<< _chains[i] << ") ";
        }
        if (_chains[i] == 0) {
            if (debug > 1) cout << "skip" << endl;
            continue;
        }

        // previous chain's right must be identical to currnet left
        if (((prevChain ^ (i >> 1)) & 1) == 0) {
            if (debug > 1) cout << "match" << endl;
            _chains[i]--;
            dfs(k-1, i);
            _chains[i]++;
        } else {
            if (debug > 1) cout << "unmatch" << endl;
        }
    }
}

ULL Watch::permutation(int k)
{
    if (debug) {
        cout << "> Watck[" << LV(_head) << "]: k=" << k << endl;
        for (int i = 0; i < 4; i++) {
            cout << "> [" << LV(i) << "]: " << _chains[i] << endl;
        }
    }

    if (k == 1) return _chains[_head ^ 3] ? 1 : 0;

    _count = 0;
    for (int i = 0; i < 4; i++) {
        if (_chains[i] == 0) continue;
        if ((_head >> 1) == (i & 1)) {  // match left of head
            _lastLV = i >> 1;
            if (debug) cout << "\n> Last[" << LV(i) << "]" << endl;
            _chains[i]--;
            dfs(k-1, _head);
            _chains[i]++;
        }
    }
    return _count;
}

//---------------------------------------------------------------------------

static void run(unsigned int max_num)
{
    int N, K;
    while ((cin >> N >> K) && N > 0) {
        assert(N <= max_num && 0 < K && K <= N);

        string s;
        cin >> s;

        Watch W(LV(s));
        for (int i = 0; i < N; i++) {
            cin >> s;
            W.add_chain(LV(s));
        }

        ULL answer = W.permutation(K);
        if (debug) cout << "\n--> ";
        if (answer) {
            cout << "YES\n" << answer << endl;
        } else {
            cout << "NO" << endl;
        }
        if (debug) cout << endl;
    }
}

int main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    unsigned int max_num = MAX_NUM;
    {
        int n = 1;
        while (n < argc && argv[n][0] == '-') {
            if (strncmp(argv[n], "-d", 2) == 0) {
                debug += strlen(&argv[n][1]);
            } else if (strncmp(argv[n], "-m", 2) == 0) {
                char *p = &argv[n][2];
                while (*p && !isdigit(*p)) p++;
                if (isdigit(*p)) {
                    max_num = atoi(p);
                    if (max_num < 10) max_num = MAX_NUM;
                }
            } else if (strncmp(argv[n], "-i:", 3) == 0) {
                ifstream in(&argv[n][3]);
                //streambuf *cinbuf = cin.rdbuf(); //save old buf
                cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
                run(max_num);
                //cin.rdbuf(cinbuf);
                return 0;
            }
            n++;
        }
    }

    run(max_num);

    return 0;
}
