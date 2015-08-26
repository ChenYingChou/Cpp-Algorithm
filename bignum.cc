/* bignum.cc -- tonychen [@] finenet.com.tw

   描述:
    给出两个正整数以及四则运算操作符（+ - * /），求运算结果。

   输入:
    第一行：正整数 a, 长度不超过200
    第二行：四则运算符 op，op 是 "+"，"-"，"*"，"/" 中的某一个
            省略時預設為乘法(配合另一長整數乘法習題)
    第三行：正整数 b, 长度不超过200
    保证输入不含多余的空格或其它字符

   输出:
    一行：表达式 "<a> <op> <b>" 的值。

   补充说明:
    1. 减法结果有可能为负数
    2. 除法结果向下取整
    3. 输出符合日常书写习惯，不能有多余的0、空格或其它字符

   样例输入:
    9876543210
    +
    9876543210

   样例输出:
    19753086420

*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <exception>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <assert.h>

using namespace std;

const int MAX_SIZE = 200;

static int debug;

//--------------------------------------------------------------------------

inline int add_carry(int c, int &carry)
{
    c += carry;
    if (c >= 10) {
        carry = 1;
        c -= 10;
    } else {
        carry = 0;
    }
    return c;
}

inline int mul_carry(int c, int &carry)
{
    c += carry;
    if (c >= 10) {
        carry = c / 10;
        c %= 10;
    } else {
        carry = 0;
    }
    return c;
}

inline int subtract_borrow(int c, int &borrow)
{
    c -= borrow;
    if (c < 0) {
        borrow = 1;
        c += 10;
    } else {
        borrow = 0;
    }
    return c;
}

class Big_Number {
  private:
    size_t _size;
    bool _minus;
    unsigned char _digit[2*MAX_SIZE];

  protected:
    void normalize();
    void set_size(size_t size) { _size = size; }
    void set_minus(bool minus) { _minus = minus; }
    Big_Number shift_mul(int shift, int multiplier) const;

  public:
    Big_Number()  { zero(); }
    Big_Number(const string& s);
    Big_Number(const Big_Number& x);
    Big_Number& operator= (const Big_Number& x);
    Big_Number operator- () const;
    size_t size() const { return _size; }
    bool minus() const { return _minus; }
    bool is_zero() const { return _size == 0 || (_size == 1 && _digit[0] == 0); }
    Big_Number& zero() { _size = 1; _digit[0] = 0; _minus = false; return *this; }
    unsigned char& operator[] (int index) { return _digit[index]; }
    unsigned char operator[] (int index) const { return _digit[index]; }
    Big_Number& operator+= (const Big_Number& x);
    Big_Number& operator-= (const Big_Number& x);
    Big_Number& shift(int n);

    friend Big_Number operator* (const Big_Number& x, const Big_Number& y);
    friend Big_Number operator/ (Big_Number x, Big_Number y);
    friend bool operator< (const Big_Number& x, const Big_Number& y);
};

Big_Number::Big_Number(const string& s)
{
    const char *p = s.c_str();
    size_t sz = s.size();

    _minus = (*p == '-');
    if (_minus) {
        p++;
        sz--;
    }

    if (sz > sizeof(_digit)) sz = sizeof(_digit);
    _size = sz;
    while (sz > 0) {
        _digit[--sz] = (*p++) & 0x0f;
    }

    normalize();
}

Big_Number::Big_Number(const Big_Number& x)
{
    _size = x._size;
    _minus = x._minus;
    std::copy(x._digit, x._digit+x._size, _digit);
}

Big_Number& Big_Number::operator= (const Big_Number& x)
{
    if (this != &x) {
        _size = x._size;
        _minus = x._minus;
        std::copy(x._digit, x._digit+x._size, _digit);
    }
    return *this;
}

void Big_Number::normalize() {
    if (_size == 0) {
        zero();
    } else {
        while (_size > 1 && _digit[_size-1] == 0) _size--;
        if (minus() && is_zero()) _minus = false;
    }
}

Big_Number Big_Number::operator- () const
{
    Big_Number x(*this);
    x._minus = !x._minus;
    x.normalize();
    return x;
}

Big_Number& Big_Number::operator+= (const Big_Number& x)
{
    if (minus() != x.minus()) {
        *this -= (-x);
        return *this;
    }

    size_t n = 0;
    int carry = 0;
    while (n < size() && n < x.size()) {
        _digit[n] = add_carry(_digit[n]+x[n], carry);
        n++;
    }

    while (n < x.size()) {
        _digit[n] = add_carry(x[n], carry);
        n++;
    }

    while (carry && n < size()) {
        _digit[n] = add_carry(_digit[n], carry);
        n++;
    }
    if (n < size()) n = size();

    if (carry && n < sizeof(_digit)) _digit[n++] = 1;
    _size = n;
    normalize();

    return *this;
}

Big_Number& Big_Number::operator-= (const Big_Number& x)
{
    if (minus() != x.minus()) {
        *this += (-x);
        return *this;
    }

    size_t n = 0;
    int borrow = 0;
    while (n < size() && n < x.size()) {
        _digit[n] = subtract_borrow(_digit[n]-x[n], borrow);
        n++;
    }

    while (n < x.size()) {
        _digit[n] = subtract_borrow(-x[n], borrow);
        n++;
    }

    while (borrow && n < size()) {
        _digit[n] = subtract_borrow(_digit[n], borrow);
        n++;
    }
    if (n < size()) n = size();

    _size = n;
    if (borrow) {
        // 和x不同號, 取補數之後再加1
        _minus = !minus();
        for (size_t k = 0; k < n; k++) {
            _digit[k] = 9 - _digit[k];
        }

        size_t i = 0;
        int carry = 1;
        while (i < n && carry) {
            if (_digit[i] < 9) {
                _digit[i]++;
                carry = 0;
            } else {
                _digit[i] = 0;
            }
            i++;
        }
        if (carry && n < sizeof(_digit)) {
            _digit[n] = 1;
            _size++;
        }
    }

    normalize();
    return *this;
}

Big_Number& Big_Number::shift(int n)
{
    if (n > 0) {        // shift left = * 10^n
        if (n >= sizeof(_digit)) {
            zero();
        } else {
            _size += n;
            if (_size > sizeof(_digit)) _size = sizeof(_digit);
            memmove(_digit+n, _digit, _size-n);
            std::fill(_digit, _digit+n, 0);
        }
    } else if (n < 0) { // shift right = / 10^|n|
        n = -n;
        if (_size <= n) {
            zero();
        } else {
            _size -= n;
            memmove(_digit, _digit+n, _size);
        }
    }

    return *this;
}

Big_Number Big_Number::shift_mul(int shift, int multiplier) const
{
    Big_Number x;
    if (shift > 0) std::fill(x._digit, x._digit+shift, 0);

    size_t n = shift;
    int carry = 0;
    for (size_t i = 0; i < _size; i++) {
        if (n >= sizeof(x._digit)) break;
        x[n++] = mul_carry(_digit[i]*multiplier, carry);
    }

    while (carry && n < sizeof(x._digit)) {
        x[n++] = carry % 10;
        carry /= 10;
    }

    x._size = n;
    x._minus = _minus;
    return x;
}

//--------------------------------------------------------------------------

ostream& operator<< (ostream& os, const Big_Number& x)
{
    if (x.minus()) cout << '-';
    for (int i = x.size(); --i >= 0; ) {
        os << (int)x[i];
    }
    return os;
}

Big_Number operator+ (Big_Number x, const Big_Number& y)
{
    x += y;
    return x;
}

Big_Number operator- (Big_Number x, const Big_Number& y)
{
    x -= y;
    return x;
}

Big_Number operator* (const Big_Number& x, const Big_Number& y)
{
    Big_Number z;
    if (!x.is_zero() && !y.is_zero()) {
        const Big_Number& a = x.size() >= y.size() ? x : y;
        const Big_Number& b = x.size() >= y.size() ? y : x;

        for (size_t i = 0; i < b.size(); i++) {
            z += a.shift_mul(i, b[i]);
        }

        z.set_minus(a.minus() != b.minus());
        z.normalize();
    }
    return z;
}

Big_Number operator/ (Big_Number x, Big_Number y)
{
    assert(!y.is_zero());

    Big_Number z;
    if (x.is_zero()) return z;

    bool minus = (x.minus() != y.minus());
    x.set_minus(false);
    y.set_minus(false);

    if (!(x < y)) {
        size_t shift = x.size() - y.size();
        y.shift(shift);
        z.set_size(++shift);

        if (debug) {
            cout << "-----: x=" << x << ", y=" << y << endl;
        }

        // x, y 對齊最高位數後開始對每一位數做減法, 直到 x < y
        // 將結果(除數)存入 z[]
        do {
            int v = 0;
            while (!(x < y)) {
                x -= y;
                v++;
            }
            z[--shift] = v;

            y.shift(-1);

            if (debug) {
                cout << "[" << shift << "]=" << v
                    << ": x=" << x
                    << ", y=" << y
                    << endl;
            }
        } while (shift > 0);
    }

    z.set_minus(minus);
    z.normalize();
    return z;
}

bool operator< (const Big_Number& x, const Big_Number& y)
{
    if (x.minus() != y.minus()) return x.minus();

    if (x.size() == y.size()) {
        for (int i = x.size(); --i >= 0; ) {
            if (x[i] < y[i]) return true;
            if (x[i] > y[i]) return false;
        }
        return false;
    }

    return x.minus() ? x.size() > y.size() : x.size() < y.size();
}

//--------------------------------------------------------------------------

static bool is_numeric(string &s)
{
    size_t i = 0;
    size_t sz = s.size();
    if (sz > 0 && s[0] == '-') i++;
    if (i >= sz) return false;

    while (i < sz) {
        if (!isdigit(s[i])) return false;
        i++;
    }
    return true;
}

static bool is_operator(string &op)
{
    return op == "+" || op == "-" || op == "*" || op == "/";
}

static void run()
{
    while (!cin.eof()) {
        string a;
        string b;
        string op;

        cin >> a >> op;
        if (a == "" || op == "") break;

        {
            bool err = false;
            if (!is_numeric(a)) {
                cout << "Invalid numeric(a): " << a << endl;
                err = true;
            }
            if (is_numeric(op)) {
                b = op;
                op = '*';
            } else if (!is_operator(op)) {
                cout << "Invalid operator: " << op << endl;
                err = true;
            } else {
                cin >> b;
                if (!is_numeric(b)) {
                    cout << "Invalid numeric(b): " << b << endl;
                    err = true;
                }
            }
            if (err) break;
        }

        if (a.size() < 10 && b.size() < 10) {
            long long x = atol(a.c_str());
            long long y = atol(b.c_str());
            long long z;
            switch(op[0]) {
            case '+':
                z = x + y;
                break;
            case '-':
                z = x - y;
                break;
            case '*':
                z = x * y;
                break;
            case '/':
                z = x / y;
                break;
            }
            cout << z << endl;
        } else {
            Big_Number x(a);
            Big_Number y(b);
            Big_Number z;

            if (debug) {
                cout << "Input: (" << x << ") " << op << " (" << y << ")"
                    << endl;
            }

            switch(op[0]) {
            case '+':
                z = x + y;
                break;
            case '-':
                z = x - y;
                break;
            case '*':
                z = x * y;
                break;
            case '/':
                z = x / y;
                break;
            }
            cout << z << endl;
        }
    }
}

int main ( int argc, char *argv[] )
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

