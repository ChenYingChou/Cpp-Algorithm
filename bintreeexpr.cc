/* bintreeexpr.cc -- tonychen [@] finenet.com.tw

   描述
    众所周知，任何一个表达式，都可以用一棵表达式树来表示。例如，表达式a+b*c，
    可以表示为如下的表达式树：

           +
          / \
         a   *
            / \
            b c

    现在，给你一个中缀表达式，这个中缀表达式用变量来表示（不含数字），请你将
    这个中缀表达式用表达式二叉树的形式输出出来。

   输入
    输入分为三个部分。
    第一部分为一行，即中缀表达式(长度不大于50)。中缀表达式可能含有小写字母代
    表变量（a-z），也可能含有运算符（+、-、*、/、小括号），不含有数字，也不含
    有空格。
    第二部分为一个整数n(n < 10)，表示中缀表达式的变量数。
    第三部分有n行，每行格式为C　x，C为变量的字符，x为该变量的值。

   输出
    输出分为三个部分，第一个部分为该表达式的逆波兰式，即该表达式树的后根遍历
    结果。占一行。
    第二部分为表达式树的显示，如样例输出所示。如果该二叉树是一棵满二叉树，则
    最底部的叶子结点，分别占据横坐标的第1、3、5、7…个位置(最左边的坐标是1)，
    然后它们的父结点的横坐标，在两个子结点的中间。如果不是满二叉树，则没有结
    点的地方，用空格填充（但请略去所有的行末空格）。每一行父结点与子结点中隔
    开一行，用斜杠(/)与反斜杠(\)来表示树的关系。/ 出现的横坐标位置为父结点的
    横坐标偏左一格，\出现的横坐标位置为父结点的横坐标偏右一格。也就是说，如果
    树高为m，则输出就有2m-1行。
    第三部分为一个整数，表示将值代入变量之后，该中缀表达式的值。需要注意的一
    点是，除法代表整除运算，即舍弃小数点后的部分。同时，测试数据保证不会出现
    除以0的现象。

   样例输入
    a+b*c
    3
    a 2
    b 7
    c 5

   样例输出
    abc*+
       +
      / \
     a   *
        / \
        b c
    37

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

const int MAX_NUM = 10;

static int debug;

static int vars[26];        // value for variables 'a'~'z'

//---------------------------------------------------------------------------

struct Node;
typedef Node * pNode;
typedef stack<pNode> StkNode;
typedef stack<int> StkInt;

struct Node {
    pNode left;
    pNode right;
    char sym;           // '+', '-', '*', '/', '^' or 'a'~'z' or '0'~'9'
    int tag;

    Node(char c) : sym(c), left(nullptr), right(nullptr), tag(0) {}
    Node(char c, pNode left, pNode right) : sym(c), left(left), right(right) {}
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
static int depth(pNode node, int seqno)
{
    if (node == nullptr) return 0;
    node->tag = seqno;
    return max(depth(node->left, 2*seqno), depth(node->right, 2*seqno+1)) + 1;
}

static string postorder(pNode node)
{
    string s;
    if (node) {
        s = postorder(node->left);
        s.append(postorder(node->right));
        s.push_back(node->sym);
    }
    return s;
}

static string inorder(pNode node)
{
    string s;
    if (node) {
        s.push_back('(');
        s.append(inorder(node->left));
        s.push_back(node->sym);
        s.append(inorder(node->right));
        s.push_back(')');
    }
    return s;
}

static long eval(pNode node)
{
    if (node == nullptr) return 0;

    char sym = node->sym;
    if (isdigit(sym)) {
        return sym - '0';
    } else if (isalpha(sym)) {
        return vars[sym & 0x1f];
    }

    long lval = eval(node->left);
    long rval = eval(node->right);

    switch(sym) {
      case '+':
        return lval + rval;
      case '-':
        return lval - rval;
      case '*':
        return lval * rval;
      case '/':
        return lval / rval;
      case '^':
        assert(rval >= 0);
        {
            long result = 1;
            while (rval) {
                if (rval & 1) result *= lval;
                lval = lval * lval;
                rval >>= 1;
            }
            return result;
        }
    }
    assert(0);
}

//---------------------------------------------------------------------------

static char prio_sym[16] = { '#','+','-','*','/','^','(',')' };

inline const char *next_sym(const char *p)
{
    while (isspace(*p)) p++;
    return p;
}

static string trim(const string &s)
{
    for (int i = s.size(); --i >= 0;) {
        if (!isspace(s[i])) return s.substr(0, i+1);
    }
    return s;
}

inline int PRIORITY(int n)
{
    return n << 4;
}

inline bool priority_LE(int a, int b)
{
    return (a >> 4) <= (b >> 4);
}

inline char priority_to_char(int prio)
{
    return prio_sym[prio & 0x0f];
}

// bits = xxxxyyy, xxxx:priority, yyy:symbol
static int priority(char c)
{
    switch(c) {
      case '#':
        return 0x00 + PRIORITY(0);
      case '+':
        return 0x01 + PRIORITY(1);
      case '-':
        return 0x02 + PRIORITY(1);
      case '*':
        return 0x03 + PRIORITY(2);
      case '/':
        return 0x04 + PRIORITY(2);
      case '^':
        return 0x05 + PRIORITY(3);
      case '(':
        return 0x06 + PRIORITY(0);
      case ')':
        return 0x07 + PRIORITY(0);
    }
    cout << "*** Invalid character for operator: [" << c
        << "], Hex=" << hex << (int)c << dec << endl;
    assert(0);
}

class ExprTree {
  private:
    pNode _root;
    StkInt _op;
    StkNode _operand;

    void do_operation();
    string inorder(pNode node) { return ::inorder(node); }

  public:
    ExprTree(const string &s);
    ~ExprTree();
    string postorder() { return ::postorder(_root); }
    string inorder() { return ::inorder(_root); }
    long eval() { return ::eval(_root); }
    void output_tree();
};

void ExprTree::do_operation()
{
    char op = priority_to_char(_op.top());
   _op.pop();
   if (debug > 1) cout << "--> pop op: " << op << endl;

   pNode right = _operand.top();
   _operand.pop();
   if (debug > 1) cout << "--> pop right operand: " << inorder(right) << endl;

   pNode left = _operand.top();
   _operand.pop();
   if (debug > 1) cout << "--> pop left operand: " << inorder(left) << endl;

   _operand.push(new Node(op, left, right));
   if (debug) cout << "--> expression: "
       << inorder(left)
       << ' ' << op << ' '
       << inorder(right)
       << endl;
}

ExprTree::ExprTree(const string &s)
    : _root(nullptr)
{
    const int prio_hash = priority('#');
    const int prio_leftparen = priority('(');

    _op.push(prio_hash);
    const char *p = s.c_str();
    while (*(p = next_sym(p)) != 0) {
        if (isalnum(*p)) {
            _operand.push(new Node(*p));
            if (debug > 1) cout << "--> push operand: " << *p << endl;
            p = next_sym(p+1);
            if (*p == 0) break;
        }

        char c = *p++;
        int c_prio = priority(c);

        switch(c) {
          case '(':
            _op.push(c_prio);
            if (debug > 1) cout << "--> push op: " << c << endl;
            break;
          case '+':
          case '-':
          case '*':
          case '/':
          case '^':
            while (priority_LE(c_prio, _op.top())) {
                do_operation();
            }
            _op.push(c_prio);
            if (debug > 1) cout << "--> push op: " << c << endl;
            break;
          case ')':
            while (_op.top() != prio_leftparen) {
                do_operation();
            }
            _op.pop();
            if (debug > 1) cout << "--> pop op: ()" << endl;
            break;
        }
    }

    while (_op.top() != prio_hash) {
        do_operation();
    }
    _op.pop();
    assert(_op.empty());

    if (!_operand.empty()) {
        _root = _operand.top();
        _operand.pop();
        assert(_operand.empty());
    }
}

ExprTree::~ExprTree()
{
    if (_root) {
        free_node(_root);
        _root = nullptr;
    }
}

void ExprTree::output_tree()
{
    if (_root == nullptr) {
        cout << "Empty tree." << endl;
        return;
    }

    int level = depth(_root, 0);    // 1~N
    int width = 1 << level;         // 最底層節點x2: 最大輸出寬度

    queue<pNode> Q;
    Q.push(_root);

    while (!Q.empty()) {
        level--;
        int pos0 = (1 << level) - 1;
        if (debug) cout << "--> Level:" << level << " pos0:" << pos0 << endl;

        string line1(width, ' ');
        string line2(width, ' ');
        pNode node;

        Q.push(nullptr);    // 這次迴圈的結束記號
        while ((node = Q.front()) != nullptr) {
            Q.pop();
            if (node->left) Q.push(node->left);
            if (node->right) Q.push(node->right);

            int pos = pos0 + (node->tag * (2 << level));
            if (debug) {
                cout << "--> Node: " << node->sym
                    << ", tag:" << node->tag
                    << ", pos:" << pos
                    << endl;
            }
            line1[pos] = node->sym;
            if (node->left) line2[pos-1] = '/';
            if (node->right) line2[pos+1] = '\\';
        }
        Q.pop();

        cout << trim(line1) << endl;
        if (level > 0) cout << trim(line2) << endl;
    }
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    string s;
    cin >> s;

    int n;
    cin >> n;
    assert(n <= max_num);

    fill(vars, vars+26, 0);
    for (int i = 0; i < n; i++) {
        string s;
        int val;
        cin >> s >> val;
        assert(!s.empty());

        char c = s[0];
        assert(isalpha(c));
        vars[c & 0x1f] = val;
    }

    ExprTree expr(s);
    cout << expr.postorder() << endl;
    expr.output_tree();
    cout << expr.eval() << endl;
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
