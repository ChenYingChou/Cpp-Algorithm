/* polynomial.cc -- tonychen [@] finenet.com.tw

   描述
    我们经常遇到两多项式相加的情况，在这里，我们就需要用程序来模拟实现把两个多
    项式相加到一起。首先，我们会有两个多项式，每个多项式是独立的一行，每个多项
    式由系数、幂数这样的多个整数对来表示。

    如多项式 2x^20 -x^17 +5x^9 -7x^7 +16x^5 +10x^4 +22x^2 -15

    对应的表达式为：2 20 -1 17 5 9 -7 7 16 5 10 4 22 2 -15 0。

    为了标记每行多项式的结束，在表达式后面加上了一个幂数为负数的整数对。

    同时输入表达式的幂数大小顺序是随机的。

    我们需要做的就是把所给的两个多项式加起来。

   输入
    输入包括多行。
    第一行整数n,表示有多少组的多项式需要求和。(1 < n < 100)
    下面为2n行整数，每一行都是一个多项式的表达式。表示n组需要相加的多项式。
    每行长度小于300。

   输出
    输出包括n行，每行为1组多项式相加的结果。
    在每一行的输出结果中，多项式的每一项用“[x y]”形式的字符串表示，x是该
    项的系数、y 是该项的幂数。要求按照每一项的幂从高到低排列，即先输出幂数
    高的项、再输出幂数低的项。
    系数为零的项不要输出。

   样例输入
    2
    -1 17 2 20 5 9 -7 7 10 4 22 2 -15 0 16 5 0 -1
    2 19 7 7 3 17 4 4 15 10 -10 5 13 2 -7 0 8 -8
    -1 17 2 23 22 2 6 8 -4 7 -18 0 1 5 21 4 0 -1
    12 7 -7 5 3 17 23 4 15 10 -10 5 13 5 2 19 9 -7

   样例输出
    [ 2 20 ] [ 2 19 ] [ 2 17 ] [ 15 10 ] [ 5 9 ] [ 6 5 ] [ 14 4 ] [ 35 2 ] [ -22 0 ]
    [ 2 23 ] [ 2 19 ] [ 2 17 ] [ 15 10 ] [ 6 8 ] [ 8 7 ] [ -3 5 ] [ 44 4 ] [ 22 2 ] [ -18 0 ]

   提示
    第一组样例数据的第二行末尾的8 -8，因为幂次-8为负数，所以这一行数据结束，8 -8不要参与计算。

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

static int debug;

//---------------------------------------------------------------------------

class Node {
private:
    int _exp;       // exponent
    int _val;       // coefficient
public:
    Node(int val, int exp) : _val(val), _exp(exp) {}
    int exp() const { return _exp; }
    int val() const { return _val; }
    void set_val(int val) { _val = val; }
    int add_val(int val) { return _val += val; }
};

// descending exponent compare for Nodes
bool operator< (const Node &a, const Node &b)
{
    return b.exp() < a.exp();
}

// compare exponent only for Nodes
bool operator== (const Node &a, const Node &b)
{
    return a.exp() == b.exp();
}

typedef set<Node> Nodes;

class Polynomial {
private:
    Nodes _poly;        // sorting exponent by descending

    Nodes::iterator add_term(const Node &x);

public:
    Polynomial() {}
    Polynomial(const Polynomial &x) { _poly = x._poly; }
    void clear() { _poly.clear(); }
    Polynomial& operator= (const Polynomial &x) {
        if (this != &x) _poly = x._poly;
        return *this;
    }
    void add_term(int val, int exp) { add_term(Node(val, exp)); }
    Polynomial& operator+= (const Polynomial &x);
    string str(int is_formula) const;
};

Nodes::iterator Polynomial::add_term(const Node &x)
{
    Nodes::iterator it = _poly.lower_bound(x);
    if (it != _poly.end() && *it == x) {
        // add coefficient for the same exponent
        // set::iterator is always identical to const_iterator
        ((Node*)&(*it))->add_val(x.val());
        return it;
    }

    // add new exponent for non-zeor coefficient
    if (x.val() == 0) return _poly.end();
    return _poly.insert(it, x);
}

Polynomial& Polynomial::operator+= (const Polynomial &x)
{
    for (Nodes::const_iterator it = x._poly.begin(); it != x._poly.end(); ++it) {
        Nodes::iterator p = add_term(*it);
        // remove node if it's coefficient is zero
        if (p != _poly.end() && p->val() == 0) _poly.erase(p);
    }
    return *this;
}

string Polynomial::str(int is_formula) const
{
    ostringstream os;
    for (Nodes::const_iterator it = _poly.begin(); it != _poly.end(); ++it) {
        const Node &a = *it;
        if (a.val() == 0) continue;

        if (it != _poly.begin()) os << ' ';
        if (is_formula) {
            if (a.val() > 0 && it != _poly.begin()) {
                os << '+' << a.val();
            } else {
                os << a.val();
            }
            if (a.exp()) os << "x^" << a.exp();
        } else {
            os << "[ " << a.val() << ' ' << a.exp() << " ]";
        }
    }
    return os.str();
}

Polynomial operator+ (Polynomial x, const Polynomial& y)
{
    x += y;
    return x;
}

//---------------------------------------------------------------------------

static void input_polynomial(Polynomial &P)
{
    P.clear();
    int val, exp;
    while ((cin >> val >> exp) && exp >= 0) {
        P.add_term(val, exp);
    }
}

static void run()
{
    Polynomial X, Y;
    int nCase;
    cin >> nCase;
    while (!cin.eof()) {
        input_polynomial(X);
        input_polynomial(Y);
        if (debug) {
            cout << "X = " << X.str(debug)
                << "\nY = " << Y.str(debug)
                << endl;
        }
        X += Y;
        if (debug) cout << "--> ";
        cout << X.str(debug) << endl;
    }
}

int main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    {
        int n = 1;
        while (n < argc && argv[n][0] == '-') {
            if (strncmp(argv[n], "-d", 2) == 0) {
                debug += strlen(&argv[n][1]);
            } else if (strncmp(argv[n], "-i:", 3) == 0) {
                ifstream in(&argv[n][3]);
                //streambuf *cinbuf = cin.rdbuf(); //save old buf
                cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
                run();
                //cin.rdbuf(cinbuf);
                return 0;
            }
            n++;
        }
    }

    run();

    return 0;
}
