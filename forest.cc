/* forest.cc -- tonychen [@] finenet.com.tw

   描述
    对于树和森林等非线性结构，我们往往需要将其序列化以便存储。有一种树的存储
    方式称为带度数的层次序列。我们可以通过层次遍历的方式将森林序列转化为多个
    带度数的层次序列。

    例如对于以下森林：

                 C               D
               / | \           /   \
              E  F  G         X     I
            / | \
           K  H  J

    两棵树的层次遍历序列分别为：C E F G K H J / D X I

    每个结点对应的度数为：3 3 0 0 0 0 0 / 2 0 0

    我们将以上序列存储起来，就可以在以后的应用中恢复这个森林。在存储中，我们
    可以将第一棵树表示为 C 3 E 3 F 0 G 0 K 0 H 0 J 0，第二棵树表示为
    D 2 X 0 I 0。

    现在有一些通过带度数的层次遍历序列存储的森林数据，为了能够对这些数据进行
    进一步处理，首先需要恢复他们。

   输入
    输入数据的第一行包括一个正整数n，表示森林中非空的树的数目。
    随后的 n 行，每行给出一棵树的带度数的层次序列。
    树的节点名称为A-Z的单个大写字母。

   输出
    输出包括一行，输出对应森林的后根遍历序列。

   样例输入
    2
    C 3 E 3 F 0 G 0 K 0 H 0 J 0
    D 2 X 0 I 0

   样例输出
    K H J E F G C X I D

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

                   C
                 /   \
               E       D
              / \     /
             K   F   X
              \   \   \
               H   G   I
                \
                 J

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
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

void output (const char *title, const Node *node, int degree)
{
    cout << "--> " << title << " Node(" << node->value()
        << ", " << degree << ')' << endl;
}

//---------------------------------------------------------------------------

static void walk_postorder(const Node* node)
{
    while (node) {
        walk_postorder(node->child());
        cout << node->value() << ' ';
        node = node->sibling();
    }
}

static Node * get_node(istringstream &is, int &degree)
{
    string value;
    if (is >> value >> degree) {
        Node *node = new Node(value[0]);
        if (debug) output("new ", node, degree);
        return node;
    }

    cout << "*** Incompleted tree nodes" << endl;
    assert(0); abort();
}

static Node * get_tree()
{
    string s;
    while (getline(cin, s) && s.empty()) {}
    istringstream is(s);

    if (debug) cout << "--> Create tree:" << endl;
    int degree;
    Node *tree = get_node(is, degree);

    queue<Node*> QNode;
    queue<int> QDegree;

    if (degree) {
        QNode.push(tree);
        QDegree.push(degree);
        if (debug) output("push", tree, degree);
    }

    while (!QNode.empty()) {
        Node *root = QNode.front(); QNode.pop();
        int root_degree = QDegree.front(); QDegree.pop();
        if (debug) output(" pop", root, root_degree);

        Node *last = root;
        for (int i = 0; i < root_degree; i++) {
            int degree;
            Node *node = get_node(is, degree);
            if (degree) {
                QNode.push(node);
                QDegree.push(degree);
                if (debug) output("push", node, degree);
            }

            if (i == 0) {
                last->set_child(node);
            } else {
                last->set_sibling(node);
            }
            last = node;
        }
    }

    if (debug) cout << endl;
    return tree;
}

static void run(int max_num)
{
    int n;
    cin >> n;

    Node *forest = n > 0 ? get_tree() : nullptr;
    if (n > 1) {
        Node *lastTree = forest;
        while (--n > 0) {
            lastTree->set_sibling(get_tree());
            lastTree = lastTree->sibling();
        }
    }

    walk_postorder(forest);
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
