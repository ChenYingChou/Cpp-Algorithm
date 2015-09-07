/* treemirror.cc -- tonychen [@] finenet.com.tw

  描述
    一棵树的镜面映射指的是对于树中的每个结点，都将其子结点反序。例如，对左边
    的树，镜面映射后变成右边这棵树。

        a                             a
      / | \                         / | \
     b  c  f       ===>            f  c  b
       / \                           / \
      d   e                         e   d

    我们在输入输出一棵树的时候，常常会把树转换成对应的二叉树，而且对该二叉树
    中只有单个子结点的分支结点补充一个虚子结点“$”，形成“伪满二叉树”。

    例如，对下图左边的树，得到下图右边的伪满二叉树

        a                             a
      / | \                          / \
     b  c  f       ===>             b   $
       / \                         / \
      d   e                       $   c
                                     / \
                                    d   f
                                   / \
                                  $   e

    然后对这棵二叉树进行前序遍历，如果是内部结点则标记为0，如果是叶结点则标
    记为1，而且虚结点也输出。

    现在我们将一棵树以“伪满二叉树”的形式输入，要求输出这棵树的镜面映射的宽
    度优先遍历序列。

   输入
    输入包含一棵树所形成的“伪满二叉树”的前序遍历。
    第一行包含一个整数，表示结点的数目。
    第二行包含所有结点。每个结点用两个字符表示，第一个字符表示结点的编号，第
    二个字符表示该结点为内部结点还是外部结点，内部结点为0，外部结点为1。结点
    之间用一个空格隔开。
    数据保证所有结点的编号都为一个小写字母。

   输出
    输出包含这棵树的镜面映射的宽度优先遍历序列，只需要输出每个结点的编号，编
    号之间用一个空格隔开。

   样例输入
    9
    a0 b0 $1 c0 d0 $1 e1 f1 $1

   样例输出
    a f c b e d

   提示
    样例输入输出对应着题目描述中的树。

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

const int MAX_NUM   = 100000;

static int debug;

//---------------------------------------------------------------------------

template <typename T>
class TreeNode {
  private:
    TreeNode *_child;
    TreeNode *_sibling;
    T _value;
  public:
    TreeNode(T value) : _value(value), _child(nullptr), _sibling(nullptr) {}
    T value() const { return _value; }
    TreeNode * child() const { return _child; }
    TreeNode * sibling() const { return _sibling; }
    void set_child(TreeNode *node) { _child = node; }
    void set_sibling(TreeNode *node) { _sibling = node; }
};

typedef TreeNode<char> Node;
typedef vector<Node*> vNode;

void output (const char *title, const Node *node, int degree)
{
    cout << "--> " << title << " Node(" << node->value()
        << ", " << degree << ')' << endl;
}

//---------------------------------------------------------------------------

static void free_node(Node *node)
{
    if (node) {
        free_node(node->child());
        free_node(node->sibling());
        delete node;
    }
}

static string walk_breadth_mirror(Node* root)
{
    string result;

    if (root) {
        vNode v[2];             // double buffer for current and next level

        // level 0: root only
        v[0].push_back(root);
        int id = 0;
        int level = 0;

        while (v[id].size() > 0) {
            vNode &vCurr = v[id];
            vNode &vNext = v[1-id];
            vNext.clear();

            if (debug) cout << "--> Level:" << level << " ( ";
            // reverse output for current level
            for (int i = vCurr.size(); --i >= 0;) {
                Node *p = vCurr[i];
                result.push_back(p->value());
                result.push_back(' ');
                if (debug) cout << p->value() << ' ';
            }
            if (debug) cout << ")" << endl;

            if (debug) cout << "--> Children: (";
            // collect children for next level (normal orders)
            for (int i = 0; i < vCurr.size(); i++) {
                Node *p = vCurr[i]->child();
                if (debug) {
                    if (i == 0) cout << ' '; else cout << "/ ";
                    if (p == nullptr) cout << "<none> ";
                }
                while (p) {
                    if (debug) cout << p->value() << ' ';
                    vNext.push_back(p);
                    p = p->sibling();
                }
            }
            if (debug) cout << ")" << endl;

            id = 1 - id;
            level++;
        }
    } else {
        if (debug) cout << "--> Empty tree";
    }

    return result;
}

// no need count N
static Node *get_node()
{
    string s;
    cin >> s;
    assert(s.size() == 2);

    if (s[0] == '$') {
        // virtual node must be a leaf
        assert(s[1] == '1');
        return nullptr;
    }

    Node *node = new Node(s[0]);
    if (s[1] == '0') {
        // internal node
        node->set_child(get_node());
        node->set_sibling(get_node());
    } else {
        // external node: leaf
        assert(s[1] == '1');
    }

    return node;
}

static void run(int max_num)
{
    int n;
    cin >> n;

    Node *tree = get_node();
    cout << walk_breadth_mirror(tree) << endl;
    free_node(tree);
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
