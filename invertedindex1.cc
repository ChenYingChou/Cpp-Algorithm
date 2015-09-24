/* invertedindex1.cc -- tonychen [@] finenet.com.tw

   描述
    给定一些文档，要求求出某些单词的倒排表。

    对于一个单词，它的倒排表的内容为出现这个单词的文档编号。

   输入
    第一行包含一个数N，1 <= N <= 1000，表示文档数。
    接下来N行，每行第一个数ci，表示第i个文档的单词数。接下来跟着ci个用空格
    隔开的单词，表示第i个文档包含的单词。文档从1开始编号。1 <= ci <= 100。
    接下来一行包含一个数M，1 <= M <= 1000，表示查询数。
    接下来M行，每行包含一个单词，表示需要输出倒排表的单词。
    每个单词全部由小写字母组成，长度不会超过 256 个字符，大多数不会超过 10
    个字符。

   输出
    对于每一个进行查询的单词，输出它的倒排表，文档编号按从小到大排序。
    如果倒排表为空，输出"NOT FOUND"。

   样例输入
    3
    2 hello world
    4 the world is great
    2 great news
    4
    hello
    world
    great
    pku

   样例输出
    1
    1 2
    2 3
    NOT FOUND

 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef set<int> SetI;
typedef map<string, SetI> MyMap;

const int MAX_NUM = 1000;

static int debug;

//---------------------------------------------------------------------------

class InvertedIndex {
  private:
    MyMap _map;
  public:
    void add(int recno, const string &word) { _map[word].insert(recno); }
    void output(const string &word);
};

void InvertedIndex::output(const string &word)
{
    MyMap::iterator it = _map.find(word);
    if (it != _map.end()) {
        SetI &set = it->second;
        for (SetI::iterator p = set.begin(); p != set.end(); ++p) {
            cout << *p << ' ';
        }
        cout << endl;
    } else {
        cout << "NOT FOUND" << endl;
    }
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    InvertedIndex X;

    int N;
    cin >> N;
    assert(1 <= N && N <= max_num);
    for (int recno = 1; recno <= N; recno++) {
        int ci;
        cin >> ci;
        assert(1 <= ci && ci <= max_num);
        for (int j = 1; j <= ci; j++) {
            string s;
            cin >> s;
            X.add(recno, s);
        }
    }

    int M;
    cin >> M;
    for (int i = 0; i < M; i++) {
        string s;
        cin >> s;
        X.output(s);
    }
}

int main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int max_num = MAX_NUM;
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
