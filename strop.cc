/* strop.cc -- tonychen@finenet.com.tw

   描述
    给定n个字符串（从1开始编号），每个字符串中的字符位置从0开始编号，
    长度为1-500，现有如下若干操作：

    copy N X L：取出第N个字符串第X个字符开始的长度为L的字符串。
    add S1 S2：判断S1，S2是否为0-99999之间的整数，若是则将其转化为整数做加法，
        若不是，则作字符串加法，返回的值为一字符串。
    find S N：在第N个字符串中从左开始找寻S字符串，返回其第一次出现的位置，
        若没有找到，返回字符串的长度。
    rfind S N：在第N个字符串中从右开始找寻S字符串，返回其第一次出现的位置，
        若没有找到，返回字符串的长度。
    insert S N X：在第N个字符串的第X个字符位置中插入S字符串。
    reset S N：将第N个字符串变为S。
    print N：打印输出第N个字符串。
    printall：打印输出所有字符串。
    over：结束操作。

    其中N，X，L可由find与rfind操作表达式构成，S，S1，S2可由copy与add操作表达式
    构成。

   输入
    第一行为一个整数n（n在1-20之间）
    接下来n行为n个字符串，字符串不包含空格及操作命令等。
    接下来若干行为一系列操作，直到over结束。

   输出
    根据操作提示输出对应字符串。

   样例输入
    3
    329strjvc
    Opadfk48
    Ifjoqwoqejr
    insert copy 1 find 2 1 2 2 2
    print 2
    reset add copy 1 find 3 1 3 copy 2 find 2 2 2 3
    print 3
    insert a 3 2
    printall
    over

   样例输出
    Op29adfk48
    358
    329strjvc
    Op29adfk48
    35a8

   提示
    推荐使用string类中的相关操作函数。

 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <assert.h>

using namespace std;

static int debug;

//-----------------------------------------------------------------------------
//#include <sstream>
//#include <string>
//#include <vector>
//#include <stdexcept>

typedef std::vector<std::string> vstring;

vstring &split(const std::string &s, char delim, vstring &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vstring split(const std::string &s, char delim) {
	vstring elems;
	split(s, delim, elems);
	return elems;
}

std::string trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t\n\r");
	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, (last-first+1));
}

int str_to_int(const std::string &s)
{
    int value = 0;
    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
        if ('0' < *it && *it > '9')
            throw std::runtime_error("str_to_int(\""+s+"\"): non-digits");
        value = 10 * value + (*it) - '0';
    }
    return value;
}

std::string int_to_str(int n)
{
    std::stringstream os;
    os << n;
    return os.str();
}

bool is_numeric(const std::string &s)
{
    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
        if (*it < '0' || *it > '9') return false;
    }
    return true;
}

//-----------------------------------------------------------------------------

class StringOP {
  private:
    vstring _item;

    vstring _input;
    int _index;

    // debuging
    vstring _stack;
    stringstream _ss;

    friend class Mark;
    class Mark {
      private:
        StringOP *_owner;
      public:
        Mark(StringOP *owner, const string &id) : _owner(owner) {
            _owner->_stack.push_back(id);
            if (debug > 1) {
                this->_owner->_ss << string(_owner->_stack.size(), '>')
                    << ' ' << id << endl;
            }
        }
        ~Mark() {
            if (debug > 1) {
                this->_owner->_ss << string(_owner->_stack.size(), '<')
                    << ' ' << _owner->_stack.back() << endl;
            }
            _owner->_stack.pop_back();
        }
    };

    stringstream& error(const char *title) {
        _ss << "*** " << title << " --> "
            << _stack.back() << ": ";
        return _ss;
    }

    string Get_Item(int N) {
        if (N <= 0 || N > _item.size()) {
            error("Out of range for Get_Item()")
                << "N must in (1.." << _item.size()
                << "), N=" << N
                << endl;
            throw range_error(_ss.str());
        }
        return _item[N-1];
    }

    string Next() {
        if (_index < 0 || _index >= _input.size()) {
            error("Out of range for Next()")
                << "Index must in (0.." << (_input.size()-1)
                << "), Index=" << _index
                << endl;
            throw range_error(_ss.str());
        }
        return _input[_index++];
    }

    int Get_Int();          // 其中N，X，L可由find与rfind操作表达式构成
    string Get_Str();       // S，S1，S2可由copy与add操作表达式

    string copy();
    string add();
    int find();
    int rfind();
    void insert();
    void reset();
    void print();
    string ItemStr(int N);
    string ItemStr(int N, const string &s);

  public:
    void add_string(const string &s) { _item.push_back(s); }
    string str() { return trim(_ss.str()); }
    void clear() { _ss.str(""); _ss.clear(); _stack.clear(); }
    void printall();
    void run(const string &cmd);
};

void StringOP::run(const string &cmd)
{
    clear();
    _input = split(trim(cmd), ' ');
    if (debug) _ss << "Input[" << _input.size() << "]: " << cmd << endl;

    if (_input.size() == 0) return;

    _index = 1;
    try {
        if (_input[0] == "copy")
            copy();
        else if (_input[0] == "add")
            add();
        else if (_input[0] == "find")
            find();
        else if (_input[0] == "rfind")
            rfind();
        else if (_input[0] == "insert")
            insert();
        else if (_input[0] == "reset")
            reset();
        else if (_input[0] == "print")
            print();
        else if (_input[0] == "printall")
            printall();
        else
            assert(_input[0] == "over");

        assert(_stack.empty());

    } catch (std::exception &e) {
        cerr << "Exception> " << trim(e.what()) << endl;
    }

    if (_ss.tellp()) cout << str() << endl;
}

// 其中N，X，L可由find与rfind操作表达式构成
int StringOP::Get_Int()
{
    string s = Next();
    if (s == "find") return find();
    if (s == "rfind") return rfind();
    return str_to_int(s);
}

// S，S1，S2可由copy与add操作表达式
string StringOP::Get_Str()
{
    string s = Next();
    if (s == "copy") return copy();
    if (s == "add") return add();
    return s;
}

// copy N X L：取出第N个字符串第X个字符开始的长度为L的字符串
string StringOP::copy()
{
    Mark _tmp(this, "copy");

    int N = Get_Int();
    int X = Get_Int();
    int L = Get_Int();

    string s = Get_Item(N);
    if (X+L > s.size()) {
        error("X+L > S[N].size()")
            << "S[" << N << "]=\"" << s
            << "\", X=" << X
            << ", L=" << L
            << endl;
    }

    s = s.substr(X, L);

    if (debug) {
        _ss << "copy(" << ItemStr(N) << ", " << X << ", " << L << ") = "
            << s << endl;
    }
    return s;
}

// add S1 S2：判断S1，S2是否为0-99999之间的整数，若是则将其转化为整数做加法，
//     若不是，则作字符串加法，返回的值为一字符串。
string StringOP::add()
{
    Mark _tmp(this, "add");

    string S1 = Get_Str();
    string S2 = Get_Str();

    if (is_numeric(S1) && is_numeric(S2)) {
        int n1 = str_to_int(S1);
        int n2 = str_to_int(S2);
        if (n1 < 100000 && n2 < 100000) {
            string s = int_to_str(n1+n2);
            if (debug) {
                _ss << "add(" << n1 << ", " << n1 << ") = "
                    << s << endl;
            }
            return s;
        }
    }

    string s = S1 + S2;

    if (debug) {
        _ss << "add(\"" << S1 << "\", \"" << S2 << "\") = "
            << s << endl;
    }
    return s;
}

// find S N：在第N个字符串中从左开始找寻S字符串，返回其第一次出现的位置，
//     若没有找到，返回字符串的长度。
int StringOP::find()
{
    Mark _tmp(this, "find");

    string S = Get_Str();
    int N = Get_Int();

    string src = Get_Item(N);
    int k = src.find(S);
    if (k == string::npos) k = src.size();

    if (debug) {
        _ss << "find(\"" << S << "\", " << ItemStr(N) << ") = "
            << k << endl;
    }
    return k;
}

// rfind S N：在第N个字符串中从右开始找寻S字符串，返回其第一次出现的位置，
//     若没有找到，返回字符串的长度。
int StringOP::rfind()
{
    Mark _tmp(this, "rfind");

    string S = Get_Str();
    int N = Get_Int();

    string src = Get_Item(N);
    int k = src.rfind(S);
    if (k == string::npos) k = src.size();

    if (debug) {
        _ss << "rfind(\"" << S << "\", " << ItemStr(N) << ") = "
            << k << endl;
    }
    return k;
}

// insert S N X：在第N个字符串的第X个字符位置中插入S字符串。
void StringOP::insert()
{
    Mark _tmp(this, "insert");

    string S = Get_Str();
    int N = Get_Int();
    int X = Get_Int();

    string src = Get_Item(N);
    if (X >= src.size()) {
        error("X >= S[N].size()")
            << "S[" << N << "]=\"" << src
            << "\", X=" << X
            << endl;
    }
    (_item[N-1] = src).insert(X, S);

    if (debug) {
        _ss << "insert(\"" << S << "\", "
            << ItemStr(N, src) << ", " << X << ") = "
            << ItemStr(N) << endl;
    }
}

// reset S N：将第N个字符串变为S。
void StringOP::reset()
{
    Mark _tmp(this, "reset");

    string S = Get_Str();
    int N = Get_Int();
    assert(1 <= N && N <= _item.size());
    _item[N-1] = S;

    if (debug) {
        _ss << "reset(\"" << S << "\", " << N << ") = "
            << ItemStr(N) << endl;
    }
}

// print N：打印输出第N个字符串。
void StringOP::print()
{
    Mark _tmp(this, "print");

    int N = Get_Int();

    if (debug) _ss << "print(" << N << ") = ";
    _ss << ItemStr(N) << endl;
}

string StringOP::ItemStr(int N)
{
    assert(1 <= N && N <= _item.size());
    return ItemStr(N, _item[N-1]);
}

string StringOP::ItemStr(int N, const string &s)
{
    stringstream ss;

    if (debug) ss << "S[" << N << "]:\"";
    ss << s;
    if (debug) ss << "\"";

    return ss.str();
}

// printall：打印输出所有字符串。
void StringOP::printall()
{
    //if (debug) _ss << "printall:" << endl;

    for (int N = 1; N <= _item.size(); N++) {
        _ss << ItemStr(N) << endl;
    }
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    if (argc > 1 && strncmp(argv[1], "-d", 2) == 0) debug = strlen(&argv[1][1]);

    StringOP X;
    string s;
    int n;

    getline(cin, s);
    n = str_to_int(s);

    for (int i = 0; i < n; i++) {
        getline(cin, s);
        X.add_string(trim(s));
    }

    if (debug) {
        X.printall();
        cout << X.str() << endl;
    }

    while (!getline(cin, s).eof()) {
        if (debug) {
            for (int i = 0; i < 5; i++) cout << "----------";
            cout << endl;
        }
        X.run(s);
    }

    return 0;
}

