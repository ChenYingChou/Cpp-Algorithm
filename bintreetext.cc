/* bintreetext.cc -- tonychen [@] finenet.com.tw

   描述
           ___A___
          /       \
         B         D
          \       /
           C     E
                  \
                   F

    如上图，一棵每个节点都是一个字母，且字母互不相同的二叉树，可以用以下若干
    行文本表示:

    A
    -B
    --*
    --C
    -D
    --E
    ---*
    ---F

    在这若干行文本中：

    1) 每个字母代表一个节点。该字母在文本中是第几行，就称该节点的行号是几。
       根在第1行
    2) 每个字母左边的'-'字符的个数代表该结点在树中的层次（树根位于第0层）
    3) 若某第 i 层的非根节点在文本中位于第n行，则其父节点必然是第 i-1 层的
       节点中，行号小于n,且行号与n的差最小的那个
    4) 若某文本中位于第n行的节点(层次是i) 有两个子节点，则第n+1行就是其左
       子节点，右子节点是n+1行以下第一个层次为i+1的节点
    5) 若某第 i 层的节点在文本中位于第n行，且其没有左子节点而有右子节点，
       那么它的下一行就是 i+1个'-' 字符再加上一个 '*'

    给出一棵树的文本表示法，要求输出该数的前序、后序、中序遍历结果

   输入
    第一行是树的数目 n

    接下来是n棵树，每棵树以'0'结尾。'0'不是树的一部分
    每棵树不超过100个节点

   输出
    对每棵树，分三行先后输出其前序、后序、中序遍历结果
    两棵树之间以空行分隔

   样例输入
    2
    A
    -B
    --*
    --C
    -D
    --E
    ---*
    ---F
    0
    A
    -B
    -C
    0

   样例输出
    ABCDEF
    CBFEDA
    BCAEFD

    ABC
    BCA
    BAC

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

typedef vector<char> vchar;

const int MAX_NUM = 65535;

static int debug;

//---------------------------------------------------------------------------

class TreeWalk {
  private:
    vchar _inorder;
    vchar _preorder;
    vchar _postorder;

    int _level;
    char _value;

    int next_node();
    void build(int level);

  public:
    void walk();
    string inorder() const { return string(_inorder.data(), _inorder.size()); }
    string preorder() const { return string(_preorder.data(), _preorder.size()); }
    string postorder() const { return string(_postorder.data(), _postorder.size()); }
};

int TreeWalk::next_node()
{
    if (_level >= 0) {
        string s;
        while (cin >> s && s.empty());

        _value = '*';
        if (s[0] == '0') {
            _level = -1;
        } else {
            _level = 0;
            for (string::iterator it = s.begin(); it != s.end(); ++it) {
                if (*it != '-') {
                    _value = *it;
                    break;
                }
                _level++;
            }
        }
    }
    return _level;
}

void TreeWalk::build(int level)
{
    if (_level < level) return;
    assert(_level == level);

    char C = _value;
    next_node();
    if (C == '*') return;

    // 前序輸出
    _preorder.push_back(C);

    // Left sub-tree?
    if (_level > level) build(level+1);

    // 中序輸出
    _inorder.push_back(C);

    // Right sub-tree?
    if (_level > level) build(level+1);

    // 後序輸出
    _postorder.push_back(C);
}

void TreeWalk::walk()
{
    _inorder.clear();
    _preorder.clear();
    _postorder.clear();

    _level = 0;
    next_node();

    build(_level);
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    TreeWalk tree;
    int n;
    cin >> n;
    while (--n >= 0) {
        tree.walk();
        cout << tree.preorder() << endl
            << tree.postorder() << endl
            << tree.inorder() << endl
            << endl;
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
