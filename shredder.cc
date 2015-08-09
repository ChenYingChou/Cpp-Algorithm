/* shredder.cc -- tonychen@finenet.com.tw

   描述
    You have just been put in charge of developing a new shredder for the
    Shredding Company Although a "normal" shredder would just shred sheets
    of paper into little pieces so that the contents would become unreadable,
    this new shredder needs to have the following unusual basic characteristics.

    1.The shredder takes as input a target number and a sheet of paper with a
      number written on it.

    2.It shreds (or cuts) the sheet into pieces each of which has one or more
      digits on it.

    3.The sum of the numbers written on each piece is the closest possible
      number to the target number, without going over it.

    For example, suppose that the target number is 50, and the sheet of paper
    has the number 12346. The shredder would cut the sheet into four pieces,
    where one piece has 1, another has 2, the third has 34, and the fourth has
    6. This is because their sum 43 (= 1 + 2 + 34 + 6) is closest to the target
    number 50 of all possible combinations without going over 50. For example,
    a combination where the pieces are 1, 23, 4, and 6 is not valid, because
    the sum of this combination 34 (= 1 + 23 + 4 + 6) is less than the above
    combination's 43. The combination of 12, 34, and 6 is not valid either,
    because the sum 52 (= 12 + 34 + 6) is greater than the target number of 50.

    There are also three special rules :

    1.If the target number is the same as the number on the sheet of paper,
      then the paper is not cut.

    For example, if the target number is 100 and the number on the sheet of
    paper is also 100, then

    the paper is not cut.

    2.If it is not possible to make any combination whose sum is less than or
      equal to the target number, then error is printed on a display. For 
      example, if the target number is 1 and the number on the sheet of paper
      is 123, it is not possible to make any valid combination, as the
      combination with the smallest possible sum is 1, 2, 3. The sum for this
      combination is 6, which is greater than the target number, and thus error
      is printed.

    3.If there is more than one possible combination where the sum is closest
      to the target number without going over it, then rejected is printed on
      a display. For example, if the target number is 15, and the number on the
      sheet of paper is 111, then there are two possible combinations with the
      highest possible sum of 12: (a) 1 and 11 and (b) 11 and 1; thus rejected
      is printed. In order to develop such a shredder, you have decided to first
      make a simple program that would simulate the above characteristics and
      rules. Given two numbers, where the first is the target number and the
      second is the number on the sheet of paper to be shredded, you need to
      figure out how the shredder should "cut up" the second number.

   输入
    The input consists of several test cases, each on one line, as follows :

    tl num1
    t2 num2
    ...
    tn numn
    0 0

    Each test case consists of the following two positive integers, which are
    separated by one space : (1) the first integer (ti above) is the target
    number, (2) the second integer (numi above) is the number that is on the
    paper to be shredded.

    Neither integers may have a 0 as the first digit, e.g., 123 is allowed but
    0123 is not. You may assume that both integers are at most 6 digits in
    length. A line consisting of two zeros signals the end of the input.

   输出
    For each test case in the input, the corresponding output takes one of the
    following three types :

    sum part1 part2 ...
    rejected
    error

    In the first type, partj and sum have the following meaning :

    1.Each partj is a number on one piece of shredded paper. The order of partj
      corresponds to the order of the original digits on the sheet of paper.

    2.sum is the sum of the numbers after being shredded,
      i.e., sum = part1 + part2 +...

    Each number should be separated by one space.
    The message error is printed if it is not possible to make any combination,
    and rejected if there is more than one possible combination.

    No extra characters including spaces are allowed at the beginning of each
    line, nor at the end of each line.

   样例输入
    50 12346
    376 144139
    927438 927438
    18 3312
    9 3142
    25 1299
    111 33333
    103 862150
    6 1104
    0 0

   样例输出
    43 1 2 34 6
    283 144 139
    927438 927438
    18 3 3 12
    error
    21 1 2 9 9
    rejected
    103 86 2 15 0
    rejected

 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <assert.h>

using namespace std;

typedef vector<char> vchar;
typedef vector<int> vint;

static int debug;

class Shredder {
  private:
    vint _list;                 // 最接近答案記錄
    int _sum;                   // Σ_list[]
    int _count;                 // 和 _list[] 結果相同的次數

    vint _parts;                // 處理過程 number 拆組記錄
    int _closest;               // 處理過程中最接近的距離(T-sum)

    static int t10[10];

    void dfs(int T, int N, int digits);

  public:
    int cut_up(int target, int number);
    bool is_error() const { return _count == 0; }
    bool is_rejected() const { return _count > 1; }
    void clear() { 
        _list.clear();
        _sum = _count = 0;
    }
    string status() const;
};

int Shredder::t10[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
};

// value v[0] v[1] ...
static string tostr(const char *title, int value, const vint &v)
{
    stringstream os;

    if (debug) {
        os << title << value << ":";
    } else {
        os << value;
    }

    for (vint::const_iterator it = v.begin(); it != v.end(); ++it) {
        os << " " << *it;
    }

    return os.str();
}

string Shredder::status() const
{
    if (is_error()) return "error";
    if (is_rejected()) return "rejected";

    // sum part1 part2 ...
    return tostr("SUM=", _sum, _list);
}

int Shredder::cut_up(int target, int number)
{
    assert(target > 0 && number > 0);

    clear();

    if (number == target) {
        _list.push_back(number);
        _sum = number;
        _count = 1;
        return 1;
    }

    int digits = sizeof(t10) / sizeof(int);
    while (digits > 1 && number < t10[digits-1]) digits--;

    _parts.clear();
    _closest = target;
    dfs(target, number, digits);

    if (_count > 0) _sum = target - _closest;

    return _count;
}

// T: 目標值
// N: 不含前置零的數值(待拆組的值)
// digits: 數值 N 的位數
void Shredder::dfs(int T, int N, int digits)
{
    assert(T >= 0);

    if (digits == 0) {
        if (T < _closest) {         // 到目前為止的最接近解
            _list = _parts;
            _count = 1;
            _closest = T;
            if (debug) {
                cout << tostr("Closest=", T, _parts) << endl;
            }
        } else if (T == _closest) { // 有重複的最接近解(距離皆為T)
            _count++;
            if (debug) {
                cout << tostr("Closest=", T, _parts)
                    << " --> Duplicated=" << _count << endl;
            }
        }
    } else {
        if (debug > 1) {
            cout << "N=" << setfill('0') << setw(digits) << N
                << setfill(' ') << setw(11-digits) << " "
                << tostr("T=", T, _parts);
            if (T - N > _closest) cout << " --> Skip";
            cout << endl;
        }

        // N 組合最大值一定是 N, (T-N) 可能是這層最接近的解
        // 若 T - N > _closest, 那麼也不必繼續找了, 返回上層
        if (T - N > _closest) return;

        // 拆解可能組合: 依長度分成兩半 (digits,0) (digits-1,1) ... (1, digits-1)
        // 前半部加入sum (從T減掉), 後半部位數值帶入遞歸繼續求最佳解
        for (int i = 0; i < digits; i++) {
            int N1 = N / t10[i];

            // 確保進入下層 dfs 時 T >= 0
            if (N1 <= T) {
                int N2 = N % t10[i];

                _parts.push_back(N1);
                dfs(T-N1, N2, i);
                _parts.pop_back();
            }
        }
    }
}

//---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    if (argc > 1 && strncmp(argv[1], "-d", 2) == 0) debug = strlen(&argv[1][1]);

    int target, number;
    Shredder S;

    while (!(cin >> target >> number).eof() && target > 0 && number > 0) {
        S.cut_up(target, number);
        cout << S.status() << endl;
    }

    return 0;
}

