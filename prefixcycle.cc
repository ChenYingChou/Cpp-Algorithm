/* prefixcycle.cc -- tonychen [@] finenet.com.tw

   描述
    一个字符串的前缀是从第一个字符开始的连续若干个字符，例如"abaab"共有5个前
    缀，分别是a, ab, aba, abaa,  abaab。

    我们希望知道一个N位字符串S的前缀是否具有循环节。换言之，对于每一个从头开
    始的长度为 i （i 大于1）的前缀，是否由重复出现的子串A组成，即 AAA...A
    (A重复出现K次, K 大于 1)。如果存在，请找出最短的循环节对应的K值（也就是
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

#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

#define USE_KMP 1
#define CPP_IOSTREAM 1

#if defined(CPP_IOSTREAM)
  #include <iostream>
#else
  #include <cstdio>
#endif

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<int> vint;

const int MAX_SIZE = 1000000;

static int debug;

//---------------------------------------------------------------------------

class KMP {
  private:
    string _pattern;
    vint _next;

    void make_table();
  public:
    KMP(const string &s) : _pattern(s) { make_table(); }
    void output();
    bool find(const string &text);
};

void KMP::make_table()
{
    int sz = _pattern.size();
    if (sz <= 1) return;

    // next[i]: where to go(what to compare next) if mismatch occur at i
    _next.resize(sz+1);

    const char *p = _pattern.data();
    int *next = _next.data();
#if 0
    next[0] = 0;    // obviously we'll stay at zero only even if its a mismatch
    next[1] = 0;    // just think naturally where will u go if mismatch occur at 1
    for (int k = 1; k < sz; k++) {
        // 計算下一個 next[k+1] 值: 應題目要需求多算一項, 即 next[0..字串長度]
        int curr = next[k];

        /* This is the case for mismatch:
         * if mismatch occur at k we seek to find the first occurence
         * of pattern[k-1] in the pattern such that the sequence preceding
         * k-1(0...k-2) locations have alreaddy been matched
         */
        while (curr && p[curr] != p[k]) {
            curr = next[curr];
        }

        // Case for match: length of prematched sequence increments by 1
        next[k+1] = p[curr] == p[k] ? curr+1 : 0;
    }
#else
    next[0] = -1;
    int k = 0;
    int curr = next[k];
    while (k < sz) {
        if (curr == -1 || p[curr] == p[k]) {
            next[++k] = ++curr;
            //--> optimize for find(), but not for this exercise
            // k++; curr++;
            // next[k] = p[curr] == p[k] ? next[curr] : curr;
        } else {
            curr = next[curr];
        }
    }
#endif
}

/* http://www.2cto.com/kf/201504/387671.html

   思路：kmp字符串匹配的性质运用。

   对于前 i 个字符，如果 next[i] 不等于零，说明在此字符串的前缀中，有一部分
   [0,next[i]] 和本字符串 [i-next[i],i] 的这一部分是相同的。如果这 i 个字符
   组成一个周期串，那么错开的部分 [next[i],i] 恰好是一个循环节。换句话说，
   如果满足 next[i] 不等于零 && [next[i],i] 是循环节这两点，就可以说明前 i
   个字符组成一个周期串。

*/
void KMP::output()
{
    for (int i = 2; i < _next.size(); i++) {
        if (_next[i] != 0 && i % (i-_next[i]) == 0) {
#if defined(CPP_IOSTREAM)
            cout << i << ' ' << (i / (i-_next[i])) << endl;
#else
            printf("%d %d\n", i, i / (i-_next[i]));
#endif
        }
    }
}

bool KMP::find(const string &text)
{
    int curr = 0; // curr: All locations before curr have been matched.
    for (int i = 0; i < text.size(); i++) {
        /* Key points:
          1. Mismatch occurs at curr.
          2. Suppose 'anand' is compared with 'ananandnd'.
          3. Mismatch occur at a i.e. cur=4. 'anand'.
          4. But we shift as above next[cur]=2 (and it matches 'a'). So now
             we only compare 'and'.
          5. We have saved our work of starting from beginning.
          6. This was luckily the longest possible one.In general,we try to
             save even the the smaller amount of work unitl we reach index 0
             in the pattern.
        */
        if (curr < 0) curr = 0;
        do {
            if (_pattern[curr] == text[i]) {
                // if all all locations before index p.size() have been matched
                if (++curr == _pattern.size()) return true;
                break;
            }
            curr = _next[curr];
        } while (curr >= 0);
    }

    return false;
}

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
        // 項次長度由大到小: 相當於重複次數由小到大
        return a._item_size < _item_size;
    }
};

typedef set<Cycle> PrefixCycle;
typedef set<Cycle>::iterator ItCycle;
typedef vector<ItCycle> vItCycle;

static void prefix_cycle(const string &s)
{
    int sz = s.size();
    if (sz < 2) return;

    int sz2 = sz / 2 + 1;
    vector<bool> skip(sz2, false);  // 倍數的循環不用處理
    PrefixCycle cycle;              // 現有的前缀循環記錄(總長度, 項次長度)
    const char *s0 = s.c_str();     // s0 指到字串開頭
    const char c0 = *s0;            // c0 第一個字元

    // 優化第1個字元的重複
    for (int k = 1; k < sz && s0[k] == c0;) {
        k++;
        cycle.insert(Cycle(k, 1));
        if (k < sz2) skip[k] = true;
    }

#if 0
    // 優化前2個字元的重複
    if (sz > 4 && !skip[2]) {
        const char c1 = s[1];
        for (int k = 2; k < sz && s0[k] == c0 && s0[k+1] == c1;) {
            k += 2;
            cycle.insert(Cycle(k, 2));
            if (k < sz2) skip[k] = true;
        }
    }

    // 優化前3個字元的重複
    if (sz > 6 && !skip[3]) {
        const char c1 = s[1];
        const char c2 = s[2];
        for (int k = 3; k < sz && s0[k] == c0 && s0[k+1] == c1 && s0[k+2] == c2;) {
            k += 3;
            cycle.insert(Cycle(k, 3));
            if (k < sz2) skip[k] = true;
        }
    }
#endif

    // 處理長度為 n: 檢查續接及輸出, 只要測試到一半的長度即可
    const char *p = s0 + 1;
    for (int n = 1; n < sz2; n++, p++) {
        // 若 s[n] == s[0] 則測試區塊長度n是否為循環
        // don't care last char, it's always EOS('\0')
        if (*p == c0 && !skip[n]) {
            // 是否為新的前缀循環(2...X倍): s[0..n-1] == s[n..2n-1]
            int k = n;
            while (k+n <= sz && memcmp(s0, s0+k, n) == 0) {
                k += n;
#if defined(CPP_IOSTREAM)
                if (debug) {
                    cout << "--> New (" << k << ", "
                        << n << ")" << endl;
                }
#endif
                cycle.insert(Cycle(k, n));
                if (k < sz2) skip[k] = true;
            }
        }

        // 蒐集總長度為 n 的記錄:
        // set<Cycle> 同一總長度集合會依項次長度大到小排, 故使用 lower_bound()
        // 時要指定項次長度為最大者, 這樣才會找到該總長度集合的第一筆
        vItCycle outList;
        ItCycle it = cycle.lower_bound(Cycle(n, n));
        while (it != cycle.end() && it->total_size() == n) {
            outList.push_back(it);
            ++it;
        }

        // 輸出並刪除之: 輸出依重複次數小到大
        for (vItCycle::iterator p = outList.begin(); p != outList.end(); ++p) {
            ItCycle &it = *p;
#if defined(CPP_IOSTREAM)
            cout << it->total_size() << ' ' << it->repeat_count() << endl;
#else
            printf("%d %d\n", it->total_size(), it->repeat_count());
#endif
            cycle.erase(it);
        }
    }

    for (ItCycle it = cycle.begin(); it != cycle.end(); ++it) {
#if defined(CPP_IOSTREAM)
        cout << it->total_size() << ' ' << it->repeat_count() << endl;
#else
        printf("%d %d\n", it->total_size(), it->repeat_count());
#endif
    }
}

//---------------------------------------------------------------------------

#if !defined(CPP_IOSTREAM)
static string get_string(int size)
{
    string s(size, ' ');
    if (fgets(&s[0], s.size()+1, stdin) == 0) s.clear();
    return s;
}
#endif

static void run(int max_num)
{
    int nCase = 0;
    int n;
#if defined(CPP_IOSTREAM)
    while (cin >> n && n > 0) {
        string s;
        cin >> s;
#else
    while (scanf("%d\n", &n) > 0 && n > 0) {
        string s = get_string(n);
#endif
        //assert(s.size() == n);

        nCase++;
#if defined(CPP_IOSTREAM)
        cout << "Test case #" << nCase << endl;
        if (debug) cout << "--> string: " << s << endl;
#else
        printf("Test case #%d\n", nCase);
#endif

#if defined(USE_KMP)
        KMP kmp(s);
        kmp.output();
#else
        prefix_cycle(s);
#endif

#if defined(CPP_IOSTREAM)
        cout << endl;
#else
        printf("\n");
#endif
    }
}

int main (int argc, char *argv[])
{
#if defined(CPP_IOSTREAM)
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
#endif

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
#if defined(CPP_IOSTREAM)
            } else if (strncmp(argv[n], "-i:", 3) == 0) {
                ifstream in(&argv[n][3]);
                //streambuf *cinbuf = cin.rdbuf(); //save old buf
                cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
                run(max_num);
                //cin.rdbuf(cinbuf);
                return 0;
#endif
            }
            n++;
        }
    }

    run(max_num);

    return 0;
}
