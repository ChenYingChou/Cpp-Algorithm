// fs.cc -- file system list, ref fsmap.cc

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<string> vstring;

const string PREFIXED_STR = "|     ";

static string _in_string;
static string next_string()
{
    string s;
    if (_in_string != "") {
        s = _in_string;
        _in_string.clear();
    } else {
        if ((cin >> s).eof()) return "";
    }
    return s;
}

static void list_files(vstring &files, const string &ps)
{
    sort(files.begin(), files.end());
    for (vstring::iterator it = files.begin(); it != files.end(); ++it) {
        cout << ps << *it << endl;
    }
}

static int dir_start(const string &dir_name, const string &ps)
{
    cout << ps << dir_name << endl;

    vstring files;
    string s;

    while ((s=next_string()) != "") {
        switch(s[0]) {
          case 'f':
            files.push_back(s);
            break;
          case 'd':
            if (dir_start(s, ps+PREFIXED_STR) < 0) {
                list_files(files, ps);
                return -1;
            }
            break;
          case ']':
          case '*':
          case '#':
            list_files(files, ps);
            return s[0] == ']' ? 1 : -1;
          default:
            cout << "*** Invalid line: " << s << endl;
            return -2;
        }
    }

    return -1;
}

int main()
{
    int nth = 0;
    while (!(cin >> _in_string).eof() && _in_string != "#") {
        nth++;
        cout << "DATA SET " << nth << ":" << endl;

        dir_start("ROOT", "");
        cout << endl;
    }

    return 0;
}

