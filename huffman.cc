/* huffman.cc -- tonychen [@] finenet.com.tw

   描述
    构造一个具有n个外部节点的扩充二叉树，每个外部节点Ki有一个Wi对应，作为该
    外部节点的权。使得这个扩充二叉树的叶节点带权外部路径长度总和最小：

         Min( W1 * L1 + W2 * L2 + W3 * L3 + … + Wn * Ln)

    Wi:每个节点的权值。

    Li:根节点到第i个外部叶子节点的距离。

    编程计算最小外部路径长度总和。

   输入
    第一行输入一个整数n，外部节点的个数。第二行输入n个整数，代表各个外部节点
    的权值。

    2 <= N <= 100

   输出
    输出最小外部路径长度总和。

   样例输入
    4
    1 1 3 5

   样例输出
    17

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

class Node;
class CmpMinPQ;

typedef Node * pNode;
typedef vector<int> vint;
typedef priority_queue<pNode, vector<pNode>, CmpMinPQ> minpq;

const int MAX_NUM = 1000;

static int debug;

//---------------------------------------------------------------------------

class Node {
  public:
    pNode left;
    pNode right;
    int data;

    Node(int val) : data(val), left(nullptr), right(nullptr) {}
    Node(int val, pNode left, pNode right) : data(val), left(left), right(right) {}
};

bool operator< (const Node &x, const Node &y) {
    return x.data < y.data;
}

class CmpMinPQ {
  public:
    bool operator() (const pNode &x, const pNode &y) const {
        return operator< (*y, *x);
    }
};

static void free_node(pNode node)
{
    if (node) {
        free_node(node->left);
        free_node(node->right);
        delete node;
    }
}

// sum for Weighted External Path Length
static int weight_path(pNode node, int path)
{
    assert(node != nullptr);
    if (node->left == nullptr && node->right == nullptr) {
        return path * node->data;
    }

    // must be completed tree
    assert(node->left  != nullptr);
    assert(node->right != nullptr);

    return weight_path(node->left , path+1)
         + weight_path(node->right, path+1);
}

//---------------------------------------------------------------------------

class Huffman {
  private:
    vint _weight;
    pNode _root;
  public:
    Huffman() : _root(nullptr) {}
    ~Huffman() { free_node(_root); }
    void add(int w) { _weight.push_back(w); }
    void clear() { _weight.clear(); }
    int mwepl();    // Minimum Weighted External Path Length
};

// Minimum Weighted External Path Length
int Huffman::mwepl()
{
    if (_weight.size() <= 0) return 0;

    minpq pq;
    for (int i = 0; i < _weight.size(); i++) {
        pq.push(new Node(_weight[i]));
    }

    while (pq.size() > 1) {
        pNode p1 = pq.top(); pq.pop();
        pNode p2 = pq.top(); pq.pop();
        pq.push(new Node(p1->data+p2->data, p1, p2));
    }
    _root = pq.top(); pq.pop();

    return weight_path(_root, 0);
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    Huffman HFM;
    int n;
    cin >> n;
    assert(2 <= n && n <= max_num);

    for (int i = 0; i < n; i++) {
        int w;
        cin >> w;
        HFM.add(w);
    }

    int answer = HFM.mwepl();
    cout << answer << endl;
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
