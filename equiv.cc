/* equiv.cc -- tonychen [@] finenet.com.tw

   等价表达式 (equivalence expression)

   描述
    判断两个表达式在数学上是否是等价的。

   输入
    第一行：N（1<=N<=20），表示测试数据组数。
    接下来每组测试数据包括两行，每行包括一个数学表达式，每个表达式的长度不超
    过80个字符。输入数据没有空行。

    一个表达式可能包括：
    单个英文字母表示的变量（区分大小写）
    数字（只有一位数）
    配对的括号
    运算符加+、减-、乘*
    任意数量的空格或tab（可能出现在表达式中间的任何位置）

    注意: 表达式保证是语法正确的，且所有运算符的优先级相同，运算次序从左至右。
          变量的系数和指数保证不超过16位整数。

   输出
    对每个测试数据，输出一行：等价则输出“YES”，不等价则输出“NO”。

   样例输入
    3
    (a+b-c)*2
    (a+a)+(b*2)-(3*c)+c
    a*2-(a+c)+((a+c+e)*2)
    3*a+c+(2*e)
    (a-b)*(a-b)
    (a*a)-(2*a*b)-(b*b)

   样例输出
    YES
    YES
    NO

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

static int debug;

//---------------------------------------------------------------------------

class Node {
private:
    string _vars;       // Variables of product
    long _val;          // coefficient (constant)
public:
    Node(long val, const string &vars) : _val(val), _vars(vars) {}
    string vars() const { return _vars; }
    long val() const { return _val; }
    void set_val(long val) { _val = val; }
    long add_val(long val) { return _val += val; }
    long mul_val(long val) { return _val *= val; }
    Node& operator*= (const Node &a);
    string vars_formula() const;
};

// (<Constant3> <Vars3>) = (<Constant1> <Vars1>) * (<Constant2> <Vars2>)
Node& Node::operator*= (const Node &a)
{
    _val *= a._val;
    _vars += a._vars;
    sort(_vars.begin(), _vars.end());
    return *this;
}

string Node::vars_formula() const
{
    ostringstream os;
    for (string::const_iterator p = _vars.begin(); p != _vars.end();) {
        char C = *p++;
        int count = 1;
        while (p != _vars.end() && *p == C) {
            count++;
            ++p;
        }
        if (count == 1) {
            os << C;
        } else {
            os << C << '^' << count;
        }
    }
    return os.str();
}

Node operator* (Node a, const Node &b)
{
    a *= b;
    return a;
}

// compare variables for Nodes: size descending, vars ascending
bool operator< (const Node &a, const Node &b)
{
    int sts = b.vars().size() - a.vars().size();
    if (sts != 0) return sts < 0;
    return a.vars() < b.vars();
}

// compare variables only for Nodes
bool operator== (const Node &a, const Node &b)
{
    return a.vars() == b.vars();
}

typedef set<Node> Nodes;

class Polynomial {
private:
    Nodes _poly;        // sorting by variables

    Nodes::iterator add_term(const Node &x);
    friend Polynomial operator* (const Polynomial &x, const Polynomial &y);
public:
    Polynomial() {}
    Polynomial(long val) { add_term(val, ""); }
    Polynomial(const string vars) { add_term(1, vars); }
    Polynomial(const Polynomial &x) { _poly = x._poly; }
    void clear() { _poly.clear(); }
    Polynomial& operator= (const Polynomial &x) {
        if (this != &x) _poly = x._poly;
        return *this;
    }
    bool operator== (const Polynomial &a) {
        if (_poly.size() != a._poly.size()) return false;
        Nodes::iterator p = _poly.begin();
        Nodes::iterator q = a._poly.begin();
        while (p != _poly.end()) {
            if (p->val() != q->val() || p->vars() != q->vars()) return false;
            ++p;
            ++q;
        }
        return true;
    }
    bool is_constant(long &val) const;
    void add_term(long val, const string &vars) { add_term(Node(val, vars)); }
    Polynomial& operator+= (const Polynomial &x);
    Polynomial& mul(long val);
    string str() const;
};

bool Polynomial::is_constant(long &val) const
{
    if (_poly.size() > 1) return false;
    if (_poly.size() == 0) {
        val = 0;
        return true;
    }

    // only one node: must be constant (no vars)
    const Node x = *_poly.begin();
    if (!x.vars().empty()) return false;
    val = x.val();
    return true;
}

Nodes::iterator Polynomial::add_term(const Node &x)
{
    Nodes::iterator it = _poly.lower_bound(x);
    if (it != _poly.end() && *it == x) {
        // add coefficient for the same variables
        // set::iterator is always identical to const_iterator
        if (((Node*)&(*it))->add_val(x.val()) == 0) {
            // remove node for zero coefficient
            _poly.erase(it);
            return _poly.end();
        }
        return it;
    }

    // add new exponent for non-zeor coefficient
    if (x.val() == 0) return _poly.end();
    return _poly.insert(it, x);
}

Polynomial& Polynomial::operator+= (const Polynomial &x)
{
    for (Nodes::iterator it = x._poly.begin(); it != x._poly.end(); ++it) {
        add_term(*it);
    }
    return *this;
}

Polynomial& Polynomial::mul(long val)
{
    if (val == 0) {
        clear();
    } else {
        for (Nodes::iterator it = _poly.begin(); it != _poly.end(); ++it) {
            ((Node*)&(*it))->mul_val(val);
        }
    }
    return *this;
}

string Polynomial::str() const
{
    ostringstream os;
    for (Nodes::const_iterator it = _poly.begin(); it != _poly.end(); ++it) {
        if (it->val() == 0) continue;

        const Node &a = *it;
        if (it == _poly.begin()) {
            if (a.val() < -1 || a.val() > 1) {
                os << a.val();
            } else if (a.val() == -1) {
                os << '-';
            }
        } else {
            if (a.val() > 0) {
                os << " + ";
                if (a.val() != 1) os << a.val();
            } else {
                os << " - ";
                if (a.val() != -1) os << -a.val();
            }
        }
        if (a.vars().empty()) {
            if (a.val() == 1 || a.val() == -1) os << '1';
        } else {
            os << a.vars_formula();
        }
    }
    return os.str();
}

Polynomial operator+ (Polynomial x, const Polynomial& y)
{
    x += y;
    return x;
}

Polynomial operator* (const Polynomial &x, const Polynomial &y)
{
    Polynomial z;
    for (Nodes::const_iterator t1 = x._poly.begin(); t1 != x._poly.end(); ++t1) {
        const Node &node1 = *t1;
        for (Nodes::const_iterator t2 = y._poly.begin(); t2 != y._poly.end(); ++t2) {
            const Node &node2 = *t2;
            z.add_term(node1 * node2);
        }
    }
    return z;
}

//---------------------------------------------------------------------------

//
// <expression> ::= ( '+' | '-' | ) <term> { ( '+' | '-' ) <term> }
//       <term> ::= <termE> { ( '*' | '/' ) <termE> }
//      <termE> ::= <factor> { '^' <factor> }
//     <factor> ::= <ident> | <number> | '(' <expression> ')'
//
class PolyExpression {
private:
    const char *_buf;
    const char *_src;           // source pointer
    const char *_last;          // points to start of symbol (debugging)
    string _vars;
    long _number;
    char _sym;

    void getsym();
    Polynomial factor();
    Polynomial termE();
    Polynomial term();
    Polynomial expression();

public:
    Polynomial parse(const char *s) {
        _src = _buf = s;
        getsym();
        Polynomial x = expression();
        if (_sym && debug) {
            cout << _src << '\n' << string(_last-_src, ' ')
                << "^ Warning: Extra symbol." << endl;
        }
        return x;
    }
};

void PolyExpression::getsym()
{
    while (*_buf && isspace(*_buf)) _buf++;
    _last = _buf;

    if (*_buf == 0) {
        if (debug > 2) cout << "> getsym(): EOS" << endl;
        _sym = 0;
        return;
    }

    _sym = *_buf++;
    if (isdigit(_sym)) {
        _number = _sym - '0';
        while (isdigit(*_buf)) {
            _number = 10 * _number + (*_buf - '0');
            _buf++;
        }
        if (debug > 2) cout << "> getsym(): Number=" << _number << endl;
    } else if (isalpha(_sym)) {
        _vars = _sym;
        const char *p = _buf;
        while (isalpha(*_buf)) _buf++;
        if (_buf > p) {
            _vars.append(p, _buf-p);
            sort(_vars.begin(), _vars.end());
        }
        if (debug > 2) cout << "> getsym(): Vars=" << _vars << endl;
    } else {
        if (debug > 2) cout << "> getsym(): Symbol=[" << _sym << ']' << endl;
    }
}

Polynomial PolyExpression::factor()
{
    const char *p = _last;

    if (isalpha(_sym)) {
        // <ident>
        string s(_vars);
        getsym();
        if (debug > 1) {
            cout << "> factor: Vars('" << string(p, _last-p)
                    << "') = " << s
                    << endl;
        }
        return Polynomial(s);
    } else if (isdigit(_sym)) {
        long num = _number;
        getsym();
        if (debug > 1) {
            cout << "> factor: Number('" << string(p, _last-p)
                    << "') = " << num
                    << endl;
        }
        return Polynomial(num);
    } else if (_sym == '(') {
        getsym();
        Polynomial x = expression();
        if (_sym != ')') {
            cout << _src << '\n' << string(_last-_src, ' ')
                << "^ Missing ')' or operator" << endl;
            throw runtime_error("Missing ')' or operator");
        }
        getsym();
        if (debug > 1) {
            cout << "> factor: '" << string(p, _last-p)
                << "' = " << x.str()
                << endl;
        }
        return x;
    }
    cout << _src << '\n' << string(_last-_src, ' ')
        << "^ Invalid symbol." << endl;
    throw runtime_error("Invalid symbol: '"+string(_last)+"'");
}

Polynomial PolyExpression::termE()
{
    const char *p = _last;
    int cnt = 0;
    Polynomial x = factor();

    while (_sym == '^') {
        getsym();
        Polynomial y = factor();
        long expo;
        if (!y.is_constant(expo)) {
            cout << _src << '\n' << string(_last-_src-1, ' ')
                << "^ Must be constant for exponent." << endl;
            throw runtime_error("Non-constant exponent: '"+string(_last-1)+"'");
        }

        y = x;
        x.clear();
        x.add_term(1, "");
        while (expo > 0) {
            if (expo & 1) x = x * y;
            y = y * y;
            expo >>= 1;
        }
        cnt++;
    }

    if (debug > 1 && cnt > 0) {
        cout << "> term^: '" << string(p, _last-p)
            << "' = " << x.str()
            << endl;
    }

    return x;
}

Polynomial PolyExpression::term()
{
    const char *p = _last;
    int cnt = 0;
    Polynomial x = termE();

    while (_sym == '*' || _sym == '/') {
        if (_sym == '/') {
            cout << _src << '\n' << string(_last-_src, ' ')
                << "^ Not support operator '/'." << endl;
            throw runtime_error("Not support operator '/'");
        }
        getsym();
        x = x * termE();
        cnt++;
    }

    if (debug > 1 && cnt > 0) {
        cout << "> term: '" << string(p, _last-p)
            << "' = " << x.str()
            << endl;
    }

    return x;
}

Polynomial PolyExpression::expression()
{
    const char *p = _last;
    int cnt = 0;
    Polynomial x;

    if (_sym == '+' || _sym == '-') {
        char op = _sym;
        getsym();
        x = term();
        if (op == '-') x.mul(-1);
    } else {
        x = term();
    }

    while (_sym == '+' || _sym == '-') {
        char op = _sym;
        getsym();
        Polynomial y = term();
        if (op == '-') y.mul(-1);
        x += y;
        cnt++;
    }

    if (debug > 1 && cnt > 0) {
        cout << "> expression: '" << string(p, _last-p)
            << "' = " << x.str()
            << endl;
    }

    return x;
}

static Polynomial input()
{
    string s;
    getline(cin, s);
    if (debug) {
        if (debug > 1) cout << endl;
        cout << "--> Input: " << s << endl;
    }
    PolyExpression P;
    return P.parse(s.c_str());
}

//---------------------------------------------------------------------------

static void run()
{
    int nCase;
    {
        string s;
        getline(cin, s);
        istringstream os(s);
        os >> nCase;
    }

    while (--nCase >= 0) {
        Polynomial X = input();
        Polynomial Y = input();
        if (debug) {
            if (debug > 1) cout << endl;
            cout << "--> X = " << X.str() << endl;
            cout << "--> Y = " << Y.str() << endl;
            cout << "--> ";
        }
        cout << (X == Y ? "YES" : "NO") << endl;
        if (debug) cout << endl;
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

