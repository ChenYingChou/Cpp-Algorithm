/* spellcheck.cc -- tonychen [@] finenet.com.tw

   描述
    现在有一些英语单词需要做拼写检查，你的工具是一本词典。需要检查的单词，有
    的是词典中的单词，有的与词典中的单词相似，你的任务是发现这两种情况。单词
    A与单词B相似的情况有三种：
    1、删除单词A的一个字母后得到单词B；
    2、用任意一个字母替换单词A的一个字母后得到单词B；
    3、在单词A的任意位置增加一个字母后得到单词B。
    你的任务是发现词典中与给定单词相同或相似的单词。

   输入
    第一部分是词典中的单词，从第一行开始每行一个单词，以"#"结束。词典中的单词
    保证不重复，最多有10000个。
    第二部分是需要查询的单词，每行一个，以"#"结束。最多有50个需要查询的单词。
    词典中的单词和需要查询的单词均由小写字母组成，最多包含15个字符。

   输出
    按照输入的顺序，为每个需要检查的单词输出一行。如果需要检查的单词出现在词典
    中，输出“?x is correct"，?x代表需要检查的单词。如果需要检查的单词没有出现
    在词典中，则输出"?x: ?x1 ?x2 ...?xn"，其中?x代表需要检查的单词，?x1...?xn
    代表词典中与需要检查的单词相似的单词，这些单词中间以空格隔开。如果没有相似
    的单词，输出"?x:"即可。

   样例输入
    i
    is
    has
    have
    be
    my
    more
    contest
    me
    too
    if
    award
    #
    me
    aware
    m
    contest
    hav
    oo
    or
    i
    fi
    mre
    #

   样例输出
    me is correct
    aware: award
    m: i my me
    contest is correct
    hav: has have
    oo: too
    or:
    i is correct
    fi: i
    mre: more me

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   分析
    每一單字最多15個字母，若不在字典中則要換形(相似的字)在字典中再找一次，其
    可能變形最多為 806 次:
        刪除任一字母:   15               15
        替換任意字母:   15 x (26-1)     375
        任意點加字母:   16 x 26      +  416
                                  --------
        　　　　　　                    806

    查詢 50 個單字最多也才 4 萬多次，用一般的 set/map 資料結構應該還可以應付。

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

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

typedef map<string, int> Dict;
typedef map<int, string> Word;
typedef vector<Dict> vDict;

const int MAX_WORD_SIZE = 15;

static int debug;

//---------------------------------------------------------------------------

class Dictionary {
  private:
    int _dict_size;             // 整個字典有多少單字, 用來記錄各單字的唯一序號
    vDict _dictionary;          // 將整個字典依單字長度分打散到各個小字典
    Word _similar;

    void find_similar(const string &word);

  public:
    Dictionary() : _dict_size(0) {
        if (_dictionary.capacity() < MAX_WORD_SIZE) {
            _dictionary.reserve(MAX_WORD_SIZE);
        }
    }
    void add(const string &word) {
        int n = word.size();
        if (n >= _dictionary.size()) _dictionary.resize(n+1);
        Dict &dict = _dictionary[n];
        dict[word] = ++_dict_size;      // seqno of word
    }
    bool exist(const string &word) {
        int n = word.size();
        Dict &dict = _dictionary[n];
        return dict.find(word) != dict.end();
    }
    string similar_words(const string &word);
};

void Dictionary::find_similar(const string &word)
{
    int n = word.size();
    Dict &dict = _dictionary[n];
    Dict::const_iterator it = dict.find(word);
    if (it != dict.end()) {
        // key: seqno of word, value: similar word
        _similar[it->second] = it->first;
    }
}

string Dictionary::similar_words(const string &word)
{
    _similar.clear();
    int wsize = word.size();

    // 刪除任一字母: 單字長度 <= 15 次
    if (wsize > 1) {
        string s;
        s.resize(wsize-1);
        copy(word.begin()+1, word.end(), s.begin());
        for (int i = 0; i < wsize; i++) {
            if (debug > 1) cout << "> 1: " << s << endl;
            find_similar(s);
            s[i] = word[i];
        }
    }

    // 替換任意字母: 單字長度 x (26-1) <= 15 * 25 = 375 次
    if (wsize > 0) {
        string s(word);
        for (int i = 0; i < wsize; i++) {
            const char C = s[i];
            for (char j = 'a'; j <= 'z'; j++) {
                if (C != j) {
                    s[i] = j;
                    if (debug > 1) cout << "> 2: " << s << endl;
                    find_similar(s);
                }
            }
            s[i] = C;
        }
    }

    // 任意點加字母: (單字長度+1) x 26 <= 16 * 26 = 416 次
    if (wsize > 0) {
        string s;
        s.resize(wsize+1);
        copy(word.begin(), word.end(), s.begin()+1);
        for (int i = 0; i <= wsize; i++) {
            for (char j = 'a'; j <= 'z'; j++) {
               s[i] = j;
               if (debug > 1) cout << "> 3: " << s << endl;
               find_similar(s);
            }
            s[i] = word[i];
        }
    }

    ostringstream os;
    for (Word::const_iterator it = _similar.begin(); it != _similar.end(); ++it) {
        os << ' ' << it->second;
    }
    _similar.clear();
    return os.str();
}

//---------------------------------------------------------------------------

#if !defined(CPP_IOSTREAM)
static int get_string(string &str)
{
    char s[1024];
    if (fgets(s, sizeof(s), stdin) == 0) return 0;

    int n = strlen(s);
    while (n > 0 && s[n-1] <= ' ') n--;
    str = string(s, n);
    return 1;
}
#endif

static void run()
{
    Dictionary Dict;

    string s;
#if defined(CPP_IOSTREAM)
    while (cin >> s && s != "#") {
#else
    while (get_string(s) && s != "#") {
#endif
        if (s.empty()) continue;
        Dict.add(s);
        if (debug > 1) cout << "--> Add: " << s << endl;
    }

#if defined(CPP_IOSTREAM)
    while (cin >> s && s != "#") {
#else
    while (get_string(s) && s != "#") {
#endif
        if (s.empty()) continue;
        if (debug) cout << "--> Word: " << s << endl;

        bool found = Dict.exist(s);
        if (debug) cout << "--> ";
        if (found) {
            cout << s << " is correct" << endl;
        } else {
            string ss = Dict.similar_words(s);
            cout << s << ':' << ss << endl;
        }
    }
}

int main (int argc, char *argv[])
{
#if defined(CPP_IOSTREAM)
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
#endif

    {
        int n = 1;
        while (n < argc && argv[n][0] == '-') {
            if (strncmp(argv[n], "-d", 2) == 0) {
                debug += strlen(&argv[n][1]);
            } else if (strncmp(argv[n], "-i:", 3) == 0) {
                ifstream in(&argv[n][3]);
                //streambuf *cinbuf = cin.rdbuf(); //save old buf
                cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
                run();
                //cin.rdbuf(cinbuf);
                return 0;
            }
            n++;
        }
    }

    run();

    return 0;
}
