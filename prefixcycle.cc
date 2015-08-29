/* prefixcycle.cc -- tonychen [@] finenet.com.tw

   描述
    一个字符串的前缀是从第一个字符开始的连续若干个字符，例如"abaab"共有5个前
    缀，分别是a, ab, aba, abaa,  abaab。

    我们希望知道一个N位字符串S的前缀是否具有循环节。换言之，对于每一个从头开
    始的长度为 i （i 大于1）的前缀，是否由重复出现的子串A组成，即 AAA...A
    （A重复出现K次,K 大于 1）。如果存在，请找出最短的循环节对应的K值（也就是
    这个前缀串的所有可能重复节中，最大的K值）。

   输入
    输入包括多组测试数据。每组测试数据包括两行。
    第一行包括字符串S的长度N（2 <= N <= 1 000 000）。
    第二行包括字符串S。
    输入数据以只包括一个0的行作为结尾。

   输出
    对于每组测试数据，第一行输出 "Test case #“ 和测试数据的编号。
    接下来的每一行，输出前缀长度i和重复测数K，中间用一个空格隔开。前缀长度需
    要升序排列。
    在每组测试数据的最后输出一个空行。

   样例输入
    3
    aaa
    12
    aabaabaabaab
    0

   样例输出
    Test case #1
    2 2
    3 3

    Test case #2
    2 2
    6 2
    9 3
    12 4

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

const int MAX_SIZE = 1000000;

static int debug;

//---------------------------------------------------------------------------

class Cycle {
private:
    int _total_size;
    int _item_size;

public:
    typedef Cycle *iterator;
    typedef const Cycle *const_iterator;
    Cycle(int total_size, int item_size)
        : _total_size(total_size), _item_size(item_size) {}
    int total_size() const { return _total_size; }
    int item_size() const { return _item_size; }
    int repeat_count() const { return _total_size / _item_size; }
    bool operator< (const Cycle &a) const {
        // 總長度由小到大
        if (_total_size != a._total_size) return _total_size < a._total_size;
        // 重複次數小到大: 即項次長度大到小 (重複次數 = 總長度 / 項次長度)
        return a._item_size < _item_size;
    }
};

typedef set<Cycle> PrefixCycle;

static void prefix_cycle(const string &s)
{
    int sz = s.size();
    if (sz < 2) return;

    PrefixCycle cycle;          // 記錄已找到的前缀循環記錄(總長度, 項次長度)
    const char *s0 = s.data();  // s0 指到字串開頭
    const char c0 = *s0;        // c0 第一個字元
    const char *p = s0 + 1;

    for (int n = 1; n <= sz; n++, p++) {
        set<Cycle>::const_iterator it;

        // 輸出總長度為 n 的組合, 要依重複次數小到大輸出
        it = cycle.lower_bound(Cycle(n, n));
        while (it != cycle.end() && it->total_size() == n) {
            cout << it->total_size() << ' ' << it->repeat_count() << endl;
            ++it;
        }
        if (n == sz) break;     // 將輸出放在最前, 是為了最後一次的總長度

        // 先找到 s[n] == s[0]
        if (*p != c0) continue;


        // 和現有前缀循環記錄比較是否可以繼續加長
        // 找出總長度為 n 的集合來逐一比較, 先找到總長度為 n 的第一筆
        // 注意: Cycle 是依(總長度,重複次數)由小到大排序, 重複次數和項次長度成反比
        it = cycle.lower_bound(Cycle(n, n));
        while (it != cycle.end() && it->total_size() == n) {
            int isize = it->item_size();
            if (isize+n <= sz && strncmp(s0, p, isize) == 0) {
                // 可以繼續加長: Cycle(n, isize) -> Cycle(n+isize, isize)
                if (debug) {
                    cout << "--> Expand ("
                        << it->total_size() << "," << isize << ")"
                        << " to (" << n+isize << "," << isize << ")"
                        << endl;
                }
                cycle.erase(it);                            // 移除舊的
                cycle.insert(Cycle(n+isize, isize));        // 新增加長的
                it = cycle.lower_bound(Cycle(n, isize));    // 指標要重新定位
            } else {
                ++it;
            }
        }

        // 是否為新的前缀循環: s[0..n-1] == s[n..2n-1]
        // todo: 但之前有4倍、8倍、16倍...的話, 則捨棄這一次的
        if (2*n <= sz && strncmp(s0, p, n) == 0) {
            if (debug) cout << "--> New (" << 2*n << "," << n << ")" << endl;
            cycle.insert(Cycle(2*n, n));
        }
    }
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int nCase = 0;
    int n;
    while (cin >> n && n > 0) {
        string s;
        cin >> s;
        assert(s.size() == n);

        nCase++;
        cout << "Test case #" << nCase << endl;
        if (debug) cout << "--> string: " << s << endl;
        prefix_cycle(s);
        cout << endl;
    }
}

int main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int max_num = MAX_SIZE;
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
                    if (max_num < 10) max_num = MAX_SIZE;
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
