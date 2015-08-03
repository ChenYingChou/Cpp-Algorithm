// fsmap.cc
/*
   输入
    你的任务是写一个程序读取一些测试数据。每组测试数据表示一个计算机的文件结
    构。每组测试数据以'*'结尾，而所有合理的输入数据以'#'结尾。一组测试数据包
    括一些文件和目录的名字（虽然在输入中我们没有给出，但是我们总假设ROOT目录
    是最外层的目录）。在输入中,以']'表示一个目录的内容的结束。目录名字的第一
    个字母是'd'，文件名字的第一个字母是'f'。文件名可能有扩展名也可能没有（比
    如fmyfile.dat和fmyfile）。文件和目录的名字中都不包括空格。
    
   输出
    在显示一个目录中内容的时候，先显示其中的子目录（如果有的话），然后再显示
    文件（如果有的话）。文件要求按照名字的字母表的顺序显示（目录不用按照名字
    的字母表顺序显示，只需要按照目录出现的先后显示）。对每一组测试数据，我们
    要先输出 "DATA SET x:"，这里x是测试数据的编号（从1开始）。在两组测试数据
    之间要输出一个空行来隔开。

    你需要注意的是，我们使用一个'|'和5个空格来表示出缩排的层次。

   样例输入:
    file1
    file2
    dir3
    dir2
    file1
    file2
    ]
    ]
    file4
    dir1
    ]
    file3
    *
    file2
    file1
    *
    #

   样例输出:
    DATA SET 1:
    ROOT
    |     dir3
    |     |     dir2
    |     |     file1
    |     |     file2
    |     dir1
    file1
    file2
    file3
    file4

    DATA SET 2:
    ROOT
    file1
    file2

 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <assert.h>

using namespace std;

typedef vector<string> vstring;
typedef vector<int> vint;

const string PREFIXED_STR = "|     ";

class FS_Map {
  private:
    vstring _files;
    vstring _dirs;
    vstring _maps;
    vint _num_files;    // number files for each deep
    
    bool _done;
    int _deep;          // _deep == _num_files.size()-1
    string _map;

  protected:
    void generate_last_dir();

  public:
    FS_Map();
    bool done() const { return _done; }
    bool push(const string &s);         // return !done()
    string map() { return _map; }
};

FS_Map::FS_Map() : _done(false), _deep(0)
{
    _dirs.push_back("ROOT");
    _maps.push_back("");
    _num_files.push_back(0);    
}

void FS_Map::generate_last_dir()
{
    assert(_num_files.size() == _dirs.size());
    assert(_deep == _num_files.size()-1);

    // output dirs[deep] & files[last numFiles[deep] files]
    string ps = "";
    for (int i = 0; i < _deep; i++) ps += PREFIXED_STR;

    ostringstream os;
    os << _maps.back();                     // 同一層之前目錄優先
    os << ps << _dirs.back() << endl;       // 輸出現在目錄名稱
    os << _map;                             // 再輸出 _map (子目錄的內容)

    // 輸出排序檔案
    int nfiles = _num_files[_deep];         // 這層目錄的檔案數
    if (nfiles > 0) {
        // 1.從 _files[] 中取出這層目錄的檔案
        vstring t;
        do {
            t.push_back(_files.back());
            _files.pop_back();
        } while (--nfiles > 0);
        
        // 2.排序
        sort(t.begin(), t.end());

        // 3.輸出(iterator loop)
        for (vstring::const_iterator it = t.begin(); it != t.end(); ++it) {
            os << ps << *it << endl;
        }
    }
    _map = os.str();

    // 移除這層的目錄相關資料, 並減少一層
    _dirs.pop_back();
    _maps.pop_back();
    _num_files.pop_back();
    _deep--;
    _done = _deep < 0;
}

bool FS_Map::push(const string &s)
{
    if (done())
        throw runtime_error("FS_Map::push("+s+")> already closed");

    switch(s[0]) {
      case 'f': // file
        _files.push_back(s);
        _num_files[_deep]++;
        break;
      case 'd': // dir, 增加一層目錄結構
        _dirs.push_back(s);
        _maps.push_back(_map);
        _num_files.push_back(0);
        _map.clear();
        _deep++;
        break;
      case ']': // dir-close, 產生現在這一層目錄結構, 存在 _map 字串中
        generate_last_dir();
        break;
      case '*': // end of file system
      case '#': // end of program
        while (!_done) {
            generate_last_dir();
        }
        break;
    }

    return !done();
}

int main(int argc, char *argv[])
{
    int nth = 0;
    while (!cin.eof()) {
        string s;
        cin >> s;
        if (s == "#") break;

        nth++;
        cout << "DATA SET " << nth << ":" << endl;

        FS_Map fs;
        while (fs.push(s)) {
            cin >> s;
        }

        cout << fs.map() << endl;
    }

    return 0;
}

