/* dictionary.cc -- tonychen [@] finenet.com.tw

   描述
    你旅游到了一个国外的城市。那里的人们说的外国语言你不能理解。不过幸运的
    是，你有一本词典可以帮助你。

   输入
    首先输入一个词典，词典中包含不超过 100000 个词条，每个词条占据一行。每
    一个词条包括一个英文单词和一个外语单词，两个单词之间用一个空格隔开。而
    且在词典中不会有某个外语单词出现超过两次。词典之后是一个空行，然后给出
    一个由外语单词组成的文档，文档不超过 100000 行，而且每行只包括一个外语
    单词。输入中出现单词只包括小写字母，而且长度不会超过10。

   输出
    在输出中，你需要把输入文档翻译成英文，每行输出一个英文单词。如果某个外
    语单词不在词典中，就把这个单词翻译成 "eh"。

   样例输入
    dog ogday
    cat atcay
    pig igpay
    froot ootfray
    loops oopslay

    atcay
    ittenkay
    oopslay

   样例输出
    cat
    eh
    loops

   提示
    输入比较大，推荐使用C语言的I / O函数。

 */

#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <assert.h>
#include <HashList.h>

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

const int MAX_NUM = 100000;

static int debug;

//---------------------------------------------------------------------------

#if defined(CPP_IOSTREAM)
  static bool get_string(string &str)
  {
      return getline(cin, str) && !str.empty();
  }
#else
  static bool get_string(string &str)
  {
    char s[1024];
    if (fgets(s, sizeof(s), stdin) == 0) return false;

    int n = strlen(s);
    while (n > 0 && s[n-1] <= ' ') n--;
    str = string(s, n);
    return n > 0;
  }
#endif

using namespace tony;

typedef THashList<string> Dictionary;

static void run(int max_num)
{
    Dictionary dict(max_num);
    string line;
    while (get_string(line)) {
        int npos = line.find(' ');
        if (npos == string::npos) {
#if defined(CPP_IOSTREAM)
            cout << "*** Invalid word map: " << line << endl;
#else
            printf("*** Invalid word map: %s\n", line.c_str());
#endif
            abort();
        }
        dict.Add(line.substr(npos+1), line.substr(0, npos));
    }

    while (get_string(line)) {
        string word;
        bool found = dict.Find(line, word);
        if (found) {
#if defined(CPP_IOSTREAM)
            cout << word << '\n';
#else
            printf("%s\n", word.c_str());
#endif
        } else {
#if defined(CPP_IOSTREAM)
            cout << "eh\n";
#else
            printf("%s\n", "eh");
#endif
        }
    }

    if (debug) {
        double density, avgDeeps;
        int maxDeeps;
        dict.GetStatistics(density, avgDeeps, maxDeeps);
#if defined(CPP_IOSTREAM)
        cout << "--> Hash size=" << dict.HashSize()
            << ", density=" << density
            << ", avg deeps=" << avgDeeps
            << ", max deeps=" << maxDeeps
            << endl;
#else
        printf("--> Hash size=%zd, density=%g, avg deeps=%g, max deeps=%d\n",
            dict.HashSize(), density, avgDeeps, maxDeeps);
#endif
    }
}

int main (int argc, char *argv[])
{
#if defined(CPP_IOSTREAM)
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
#endif

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
