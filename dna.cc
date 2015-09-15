/* dna.cc -- tonychen [@] finenet.com.tw

   描述
    现在有一些长度相等的DNA串（只由ACGT四个字母组成），请将它们按照逆序对的
    数量多少排序。
    逆序对指的是字符串A中的两个字符 A[i]、A[j]，具有i < j 且 A[i] > A[j] 的
    性质。如字符串”ATCG“中，T和C是一个逆序对，T和G是另一个逆序对，这个字符
    串的逆序对数为2。

   输入
    第1行：两个整数n和m，n(0<n<=50)表示字符串长度，m(0<m<=100)表示字符串数量

    第2至m+1行：每行是一个长度为n的字符串

   输出
    按逆序对数从少到多输出字符串，逆序对数一样多的字符串按照输入的顺序输出。

   样例输入
    10 6
    AACATGAAGG
    TTTTGGCCAA
    TTTGGCCAAA
    GATCAGATTT
    CCCGGGGGGA
    ATCGATGCAT

   样例输出
    CCCGGGGGGA
    AACATGAAGG
    GATCAGATTT
    ATCGATGCAT
    TTTTGGCCAA
    TTTGGCCAAA

 */

#include <iostream>
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

const int MAX_NUM = 10000;

static int debug;

//---------------------------------------------------------------------------

class Node {
  private:
    int _id;
    string _str;
    int _value;
  public:
    Node(int id, const string &str, int value)
      : _id(id), _str(str), _value(value) {}
    string str() const { return _str; }
    int value() const { return _value; }
    int id() const { return _id; }
    bool operator< (const Node &a) const {
        if (_value != a._value) return _value < a._value;
        return _id < a._id;
    }
};

typedef vector<Node> vNode;

class Reverse_DNA {
  private:
    vNode _list;
    int reverse_count(const string &s);
  public:
    Reverse_DNA() {}
    Reverse_DNA(int size) { _list.reserve(size); }
    void add(const string &s);
    void output();
};

int Reverse_DNA::reverse_count(const string &s)
{
    int result = 0;
    const char *p = s.c_str();
    while (*p != 0) {
        char C = *p++;
        const char *q = p;
        while (*q != 0) {
            if (C > *q++) result++;
        }
    }
    return result;
}

void Reverse_DNA::add(const string &s)
{
    int id = _list.size();
    int value = reverse_count(s);
    if (debug) {
        cout << "--> add: id=" << id
            << ", string=[" << s
            << "], value=" << value
            << endl;
    }
    _list.push_back(Node(id, s, value));
}

void Reverse_DNA::output()
{
    sort(_list.begin(), _list.end());
    for (vNode::iterator it = _list.begin(); it != _list.end(); ++it) {
        cout << it->str() << endl;
    }
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int n, m;
    cin >> n >> m;
    assert(0 < m && m <= max_num);

    Reverse_DNA dna(m);
    string s;
    while (cin >> s) {
        if (s.empty()) continue;
        dna.add(s);
    }
    dna.output();
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
