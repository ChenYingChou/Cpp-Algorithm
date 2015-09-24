/* invertedindex2.cc -- tonychen [@] finenet.com.tw

   描述
    现在已经对一些文档求出了倒排索引，对于一些词得出了这些词在哪些文档中出现
    的列表。

    要求对于倒排索引实现一些简单的查询，即查询某些词同时出现，或者有些词出现
    有些词不出现的文档有哪些。

   输入
    第一行包含一个数N，1 <= N <= 100，表示倒排索引表的数目。
    接下来N行，每行第一个数ci，表示这个词出现在了多少个文档中。接下来跟着ci
    个数，表示出现在的文档编号，编号不一定有序。1 <= ci <= 1000，文档编号为
    32位整数。
    接下来一行包含一个数M，1 <= M <= 100，表示查询的数目。
    接下来 M 行每行 N 个数，每个数表示这个词要不要出现，1 表示出现，-1 表示
    不出现，0 表示无所谓。数据保证每行至少出现一个 1。

   输出
    共M行，每行对应一个查询。输出查询到的文档编号，按照编号升序输出。
    如果查不到任何文档，输出"NOT FOUND"。

   样例输入
    3
    3 1 2 3
    1 2
    1 3
    3
    1 1 1
    1 -1 0
    1 -1 -1

   样例输出
    NOT FOUND
    1 3
    1

 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

typedef vector<int> vint;
typedef vector<vint> vlist;

const int MAX_NUM = 1000;

static int debug;

//---------------------------------------------------------------------------

class InvertedIndex {
  private:
    vlist _data;
    int _size;
    vint get_list(int n, istringstream &is);
    vint list_and(const vint &a, const vint &b);
    vint list_exclude(const vint &a, const vint &b);
    void dump(const vint &x);
  public:
    InvertedIndex(int size) : _size(size) { _data.reserve(_size); }
    void add(const string &line);
    void output(const string &line);
};

void InvertedIndex::dump(const vint &x)
{
    for (int i = 0; i < x.size(); i++) {
        if (i > 0) cout << ' ';
        cout << x[i];
    }
    cout << endl;
}

vint InvertedIndex::get_list(int n, istringstream &is)
{
    vint result;
    result.reserve(n);

    int x;
    while (is >> x) {
        result.push_back(x);
    }
    return result;
}

vint InvertedIndex::list_and(const vint &a, const vint &b)
{
    vint result;
    result.reserve(a.size());

    vint::const_iterator ia = a.begin();
    vint::const_iterator ib = b.begin();
    while (ia != a.end() && ib != b.end()) {
        if (*ia == *ib) {
            result.push_back(*ia);
            ++ia;
            ++ib;
        } else if (*ia < *ib) {
            ++ia;
        } else {
            ++ib;
        }
    }

    return result;
}

vint InvertedIndex::list_exclude(const vint &a, const vint &b)
{
    vint result;
    result.reserve(a.size());

    vint::const_iterator ia = a.begin();
    vint::const_iterator ib = b.begin();
    while (ia != a.end() && ib != b.end()) {
        if (*ia == *ib) {
            ++ia;
            ++ib;
        } else if (*ia < *ib) {
            result.push_back(*ia);
            ++ia;
        } else {
            ++ib;
        }
    }

    while (ia != a.end()) {
        result.push_back(*ia);
        ++ia;
    }

    return result;
}

void InvertedIndex::add(const string &line)
{
    istringstream is(line);
    int n;
    is >> n;

    vint list = get_list(n, is);
    std::sort(list.begin(), list.end());

    vint::iterator it = std::unique(list.begin(), list.end());
    if (it != list.end()) {
        list.resize(std::distance(list.begin(), it));
    }

    if (debug) {
        cout << "--> data[" << _data.size() << "]: ";
        dump(list);
    }
    _data.push_back(list);
}

void InvertedIndex::output(const string &line)
{
    vint result;
    bool first = true;
    istringstream is(line);

    vint exclude;
    exclude.reserve(_size);

    for (int n = 0; n < _data.size(); n++) {
        int sts;
        if (!(is >> sts)) break;

        if (sts == 1) {
            if (first) {
                result = _data[n];
                if (debug) {
                    cout << "--> first data[" << n << "]: ";
                    dump(result);
                }
                first = false;
            } else {
                // result &= _data[i]
                vint &a = _data[n];
                if (debug) {
                    cout << "--> AND data[" << n << "]: ";
                    dump(a);
                }
                result = list_and(result, a);
                if (debug) {
                    cout << "--> result[]: ";
                    dump(result);
                }
            }
            if (!first && result.empty()) break;
        } else if (sts == -1) {
            exclude.push_back(n);
        }
    }

    if (!result.empty()) {
        for (int i = 0; i < exclude.size(); i++) {
            vint &a = _data[exclude[i]];
            if (debug) {
                cout << "--> EXCLUDE data[" << exclude[i] << "]: ";
                dump(a);
            }
            result = list_exclude(result, a);
            if (debug) {
                cout << "--> result[]: ";
                dump(result);
            }
            if (result.empty()) break;
        }
    }

    if (result.empty()) {
        cout << "NOT FOUND" << endl;
    } else {
        dump(result);
    }
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int N;
    cin >> N;
    assert(1 <= N && N <= max_num);
    if (debug) cout << "--> N = " << N << endl;

    InvertedIndex X(max_num);
    for (int wordno = 1; wordno <= N; wordno++) {
        string line;
        while (getline(cin, line) && line.empty());
        if (debug) cout << "--> Input List: " << line << endl;
        X.add(line);
    }

    int M;
    cin >> M;
    if (debug) cout << "\n--> M = " << M << endl;
    for (int i = 0; i < M; i++) {
        string line;
        while (getline(cin, line) && line.empty());
        if (debug) cout << "\n--> Check List: " << line << endl;
        X.output(line);
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
