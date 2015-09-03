/* bintree.cc -- tonychen [@] finenet.com.tw

   描述
    二叉搜索树在动态查表中有特别的用处，一个无序序列可以通过构造一棵二叉搜
    索树变成一个有序序列，构造树的过程即为对无序序列进行排序的过程。每次插
    入的新的结点都是二叉搜索树上新的叶子结点，在进行插入操作时，不必移动其
    它结点，只需改动某个结点的指针，由空变为非空即可。

    这里，我们想探究二叉树的建立和序列输出。

   输入
    只有一行，包含若干个数字，中间用空格隔开。（数字可能会有重复）

   输出
    输出一行，对输入数字建立二叉搜索树后进行前序周游的结果。

   样例输入
    41 467 334 500 169 724 478 358 962 464 705 145 281 827 961 491 995 942 827 436

   样例输出
    41 467 334 169 145 281 358 464 436 500 478 491 724 705 962 827 961 942 995

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

const int MAX_NUM = 1000;

static int debug;

//---------------------------------------------------------------------------

struct Node;
typedef Node * pNode;
typedef void (*func)(pNode node, int &val);

struct Node {
    pNode left;
    pNode right;
    int data;

    Node(int val) : data(val), left(nullptr), right(nullptr) {}
    Node(int val, pNode left, pNode right) : data(val), left(left), right(right) {}
};

static void free_node(pNode node)
{
    if (node) {
        free_node(node->left);
        free_node(node->right);
        delete node;
    }
}

// height of tree (one node counts as one)
static int depth(pNode node)
{
    if (node == nullptr) return 0;
    return max(depth(node->left), depth(node->right)) + 1;
}

//---------------------------------------------------------------------------

class Tree {
  private:
    pNode _root;
    func _func;
    int _walk;
    void walk_node(pNode node, int val);

  public:
    enum Walk { In_Order=1, Pre_Order=2, Post_Order=4 };
    Tree() : _root(nullptr) {}
    ~Tree() { free_node(_root); }
    bool insert(int value);
    void walk(func f, int nWalk, int val);
};

bool Tree::insert(int value)
{
    pNode *last = &_root;
    while (*last != nullptr) {
        pNode p = *last;
        if (value == p->data) return false;
        if (value < p->data) {
            last = &(p->left);
        } else {
            last = &(p->right);
        }
    }

    *last = new Node(value);
    return true;
}

void Tree::walk_node(pNode node, int val)
{
    if (node) {
        if (_walk & Pre_Order ) _func(node, val);
        walk_node(node->left, val);
        if (_walk & In_Order  ) _func(node, val);
        walk_node(node->right, val);
        if (_walk & Post_Order) _func(node, val);
    }
}

void Tree::walk(func f, int nWalk, int val)
{
    _func = f;
    _walk = nWalk;
    walk_node(_root, val);
}

//---------------------------------------------------------------------------

static void output(pNode node, int &val)
{
    cout << node->data << ' ';
}

static void run(int max_num)
{
    Tree tree;
    int val;
    while (cin >> val) {
        tree.insert(val);
    }

    tree.walk(output, Tree::Pre_Order, 0);
    cout << endl;
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
