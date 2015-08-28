/* engnum.cc

   描述
    在这个问题中，将用英语给你一个或多个整数。你的任务是将这些数字转换成整型
    表示。数字范围从-999,999,999到999,999,999.下面是你的程序必须考虑的详尽的
    英语单词表：
    negative, zero, one, two, three, four,five, six, seven, eight, nine, ten,
    eleven, twelve, thirteen, fourteen,fifteen, sixteen, seventeen, eighteen,
    nineteen, twenty, thirty, forty, fifty,sixty, seventy, eighty, ninety,
    hundred, thousand, million

   输入
    输入包括多个样例，注意：

    1.负数前面有词negative

    2.当能用thousand的时候，将不用hundred。例如1500将写为"one thousand five
    hundred",而不是"fifteen hundred".

    输入将以一个空行结束

   输出
    输出将是每一个单独一行，每一个后面一个换行符

   样例输入
    six
    negative seven hundred twenty nine
    one million one hundred one
    eight hundred fourteen thousand twenty two

   样例输出
    6
    -729
    1000101
    814022

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

static int debug;

//---------------------------------------------------------------------------

enum ENG_NUMBER { E_NULL, E_SIGN, E_DIGIT, E_MULTIPLY, E_FIXED };

struct EngNumber;

typedef struct EngNumber {
    typedef EngNumber* iterator;
    string name;
    ENG_NUMBER category;
    int value;
} EngNumber;

static EngNumber engnum[] = {
	{ "and"      , E_NULL    , 0          },
	{ "negative" , E_SIGN    , -1         },
	{ "zero"     , E_DIGIT   , 0          },
	{ "one"      , E_DIGIT   , 1          },
	{ "two"      , E_DIGIT   , 2          },
	{ "three"    , E_DIGIT   , 3          },
	{ "four"     , E_DIGIT   , 4          },
	{ "five"     , E_DIGIT   , 5          },
	{ "six"      , E_DIGIT   , 6          },
	{ "seven"    , E_DIGIT   , 7          },
	{ "eight"    , E_DIGIT   , 8          },
	{ "nine"     , E_DIGIT   , 9          },
	{ "ten"      , E_DIGIT   , 10         },
	{ "eleven"   , E_DIGIT   , 11         },
	{ "twelve"   , E_DIGIT   , 12         },
	{ "thirteen" , E_DIGIT   , 13         },
	{ "fourteen" , E_DIGIT   , 14         },
	{ "fifteen"  , E_DIGIT   , 15         },
	{ "sixteen"  , E_DIGIT   , 16         },
	{ "seventeen", E_DIGIT   , 17         },
	{ "eighteen" , E_DIGIT   , 18         },
	{ "nineteen" , E_DIGIT   , 19         },
	{ "twenty"   , E_DIGIT   , 20         },
	{ "thirty"   , E_DIGIT   , 30         },
	{ "forty"    , E_DIGIT   , 40         },
	{ "fifty"    , E_DIGIT   , 50         },
	{ "sixty"    , E_DIGIT   , 60         },
	{ "seventy"  , E_DIGIT   , 70         },
	{ "eighty"   , E_DIGIT   , 80         },
	{ "ninety"   , E_DIGIT   , 90         },
	{ "hundred"  , E_MULTIPLY, 100        },
	{ "thousand" , E_FIXED   , 1000       },
	{ "million"  , E_FIXED   , 1000000    },
	{ "billion"  , E_FIXED   , 1000000000 }
};

const int engnum_size = sizeof(engnum) / sizeof(EngNumber);

bool operator< (const EngNumber &a, const EngNumber &b)
{
    return a.name < b.name;
}

bool operator== (const EngNumber &a, const EngNumber &b)
{
    return a.name == b.name;
}

bool operator< (const EngNumber &a, const string &s)
{
    return a.name < s;
}

bool operator== (const EngNumber &a, const string &s)
{
    return a.name == s;
}

//---------------------------------------------------------------------------

static long long english_number(const string &ss)
{
    int signedMark = 1;
    long long result = 0;
    long long fixed = 0;        // save for million/billion/thousand
    int fixedMarks = 0;         // bits for million/billion/thousand
    istringstream is(ss);

    string s;
    while (is >> s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        EngNumber::iterator p = lower_bound(engnum, engnum+engnum_size, s);
        if (p->name != s) {
            cout << "Error> Invalid number symbol: " << s << endl;
            break;
        }
        switch(p->category) {
          case E_NULL:
            break;
          case E_SIGN:
            signedMark = -signedMark;
            break;
          case E_MULTIPLY:
            result *= p->value;
            break;
          case E_FIXED:
            {
                int val = p->value;
                int bit = 1;
                while ((val /= 1000) > 1) bit <<= 1;
                if (fixedMarks && bit >= fixedMarks) {
                    if (!debug) cout << "--> Input> " << ss << endl;
                    cout << "*** Error> "
                        << (bit & fixedMarks
                            ? "Duplicated symbol"
                            : "Invalid sequence")
                        << ": " << p->name << endl;
                }
                fixedMarks |= bit;
            }
            fixed += result * p->value;
            result = 0;
            break;
          default:
            result += p->value;
            break;
        }
    }

    return (result+fixed) * signedMark;
}

static void run()
{
    sort(engnum, engnum+engnum_size);

    string s;
    while (getline(cin, s)) {
        if (s.empty()) continue;
        if (debug) cout << "--> Input> " << s << endl;
        long long num = english_number(s);
        if (debug) cout << "--> ";
        cout << num << endl;
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
