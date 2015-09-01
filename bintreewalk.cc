/* bintreewalk.cc -- tonychen [@] finenet.com.tw

   描述
    我们知道如何按照三种深度优先次序来周游一棵二叉树，来得到中根序列、前根
    序列和后根序列。反过来，如果给定二叉树的中根序列和后根序列，或者给定中
    根序列和前根序列，可以重建一二叉树。本题输入一棵二叉树的中根序列和后根
    序列，要求在内存中重建二叉树，最后输出这棵二叉树的前根序列。

    用不同的整数来唯一标识二叉树的每一个结点，下面的二叉树:

           5
          / \
         9  67
            /
           32

    中根序列是 9 5 32 67
    后根序列　 9 32 67 5
    前根序列　 5 9 67 32

   输入
    两行。第一行是二叉树的中根序列，第二行是后根序列。每个数字表示的结点之
    间用空格隔开。结点数字范围0～65535。暂不必考虑不合理的输入数据。

   输出
    一行。由输入中的中根序列和后根序列重建的二叉树的前根序列。每个数字表示
    的结点之间用空格隔开。

   样例输入
    9 5 32 67
    9 32 67 5

   样例输出
    5 9 67 32

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   分析: 後根序列最後一個值即為整棵樹的根節點，以此找到中根序列的位置，一分
    為左右兩半邊的子樹。以此類推，再以後根序倒數第2個值去找看是那半邊的根節
    點，再分兩半...

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

typedef vector<int> vint;

const int MAX_NUM = 65535;

static int debug;

//---------------------------------------------------------------------------

class Node;
typedef Node * pNode;
typedef vector<Node> vNode;

class Node {
private:
    int _data;
    int _left;
    int _right;
public:
    Node(int data) : _data(data), _left(-1), _right(-1) {}
    int data() const { return _data; }
    int left() const { return _left; }
    int right() const { return _right; }
    void left(int node_id) { _left = node_id; }
    void right(int node_id) { _right = node_id; }
};

class Walk {
  private:
    vint _postorder;
    vint _inorder;
    vint _preorder;
    int _last;                  // index of _postorder[] for next_node()
    vNode _nodes;

    pNode node(int id) { return id >= 0 ? &_nodes[id] : nullptr; }
    int node_id(const Node &node) const { return &node - &_nodes[0]; }
    Node& next_node();
    bool next_in(int low, int high);
    int find_pos(int data, int low, int high);
    int new_tree(int low, int high);
    void walk_tree(pNode p);
    void walk_tree(int id) { walk_tree(node(id)); }

  public:
    Walk(const vint &post, const vint &in) : _postorder(post), _inorder(in) {}
    vint create_pre_order();
};

// fetch new Node(_postorder[--_last]) and push to _nodes[]
Node& Walk::next_node()
{
    assert(_last > 0);
    int data = _postorder[--_last];
    _nodes.push_back(Node(data));
    return _nodes[_nodes.size()-1];
}

// return true if next_node().data in [low, high) of _inorder[]
bool Walk::next_in(int low, int high)
{
    return _last > 0 && find_pos(_postorder[_last-1], low, high) < high;
}

// return position of data in [low, high) of _inorder[]
int Walk::find_pos(int data, int low, int high)
{
    int pos = low;
    while (pos < high) {
        if (data == _inorder[pos]) break;
        pos++;
    }
    return pos;
}

// _inorder[]: low <= pos[node] < high
int Walk::new_tree(int low, int high)
{
    Node &node = next_node();
    if (debug) {
        cout << "--> new_tree: Node[" << node_id(node)
            << "] = (" << node.data()
            << ") in Range (" << low
            << ", " << high
            << "]" << endl;
    }

    if (low < high && _last > 0) {
        int mid = find_pos(node.data(), low, high);
        assert(low <= mid && mid < high);

        if (next_in(mid+1, high)) {
            node.right(new_tree(mid+1, high));
        }

        if (next_in(low, mid)) {
            node.left(new_tree(low, mid));
        }
    }

    if (debug) {
        cout << "--> new_tree: Node[" << node_id(node)
            << "] = (" << node.data()
            << ", " << node.left()
            << ", " << node.right()
            << ")" << endl;
    }
    return node_id(node);
}

// create pre-order list in _preorder[]
void Walk::walk_tree(pNode p)
{
    if (p) {
        _preorder.push_back(p->data());
        walk_tree(p->left());
        walk_tree(p->right());
    }
}

vint Walk::create_pre_order()
{
    _preorder.clear();
    _last = _postorder.size();
    if (_last > 0) {
        _nodes.reserve(_last);
        int root = new_tree(0, _inorder.size());
        walk_tree(root);
    }
    return _preorder;
}

//---------------------------------------------------------------------------

static string get_line()
{
    string s;
    while (getline(cin, s)) {
        if (!s.empty()) break;
    }
    return s;
}

static vint get_list(const string &s)
{
    istringstream is(s);
    vint x;
    int n;
    while (is >> n) {
        x.push_back(n);
    }
    return x;
}

static void output_list(const char *title, const vint &x)
{
    if (title) cout << title;
    for (int i = 0; i < x.size(); i++) {
        if (i) cout << ' ';
        cout << x[i];
    }
    cout << endl;
}

static void run(int max_num)
{
    string s1, s2;
    while (!(s1=get_line()).empty() && !(s2=get_line()).empty()) {
        vint in_order = get_list(s1);
        vint post_order = get_list(s2);
        if (debug) output_list("-->   In-order: ", in_order);
        if (debug) output_list("--> Post-order: ", post_order);

        Walk walk(post_order, in_order);

        output_list(debug ? "-->  Pre-order: " : nullptr,
            walk.create_pre_order());
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
