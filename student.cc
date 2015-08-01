// student.cpp
/*
  輸入:
   姓名，年龄，学号，第一学年平均成绩，第二学年平均成绩，第三学年平均成绩，
   第四学年平均成绩。
   其中姓名、学号为字符串，不含空格和逗号；年龄为正整数；成绩为非负整数。
   各部分内容之间均用单个英文逗号","隔开，无多余空格。

  输出:
    一行，按顺序输出：姓名，年龄，学号，四年平均成绩（向下取整）。
    各部分内容之间均用单个英文逗号","隔开，无多余空格。
*/

#include <iostream>

//===========================================================================

#include <string>
#include <vector>
#include <sstream>

typedef std::vector<std::string> vstring;

vstring &split( const std::string &s, char delim, vstring &elems )
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vstring split( const std::string &s, char delim ) {
	vstring elems;
	split(s, delim, elems);
	return elems;
}

std::string trim( const std::string &str )
{
	size_t first = str.find_first_not_of(" \t\n\r");
	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, (last-first+1));
}

int stoint( const std::string &s )
{
    int value;
    std::istringstream(s) >> value;
    return value;
}

//===========================================================================

using namespace std;

typedef std::vector<int> vint;

class Student {
  private:
    string name;
    string id;
    int age;
    vint grade;

    string make_str(const string &s);

  public:
    Student() : age(0) {}
    Student(const string &line);

    void init_data(const string &line);

    string get_name() const { return name; }
    void set_name(const string &name) { this->name = make_str(name); }

    string get_id() const { return id; }
    void set_id(const string &id) { this->id = make_str(id); }

    int get_age() const { return age; }
    void set_age(int age) { this->age = age; }

    vint get_grade() const { return this->grade; }
    void set_grade(vint grade) { this->grade = grade; }
    int grade_avg();
};

Student::Student(const string &line)
    : age(0)
{
    init_data(line);
}

string Student::make_str(const string &s)
{
    return s == "" ? "(null)" : trim(s);
}

int Student::grade_avg()
{
    if (grade.size() == 0) return 0;

    int sum = 0;
    for (int i = 0; i < grade.size(); i++) {
        sum += grade[i];
    }
    return sum / grade.size();
}

void Student::init_data(const string &line)
{
    vstring a = split(line, ',');
    set_name(a.size() > 0 ? a[0] : "");
    set_age (a.size() > 1 ? stoint(a[1]) : 0);
    set_id  (a.size() > 2 ? a[2] : "");

    grade.clear();
    for (int i = 3; i < a.size(); i++) {
        grade.push_back(stoint(a[i]));
    }
}

int main()
{
    string line;
    while (getline(cin, line)) {
        Student s(line);
        cout << s.get_name()
            << ',' << s.get_age()
            << ',' << s.get_id()
            << ',' << s.grade_avg()
            << endl;
    }
    return 0;
}
