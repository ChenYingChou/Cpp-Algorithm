/* addsum.cc

   描述
    给出2个正整数集合A = {pi|1 <= i <= a}，B = {qj|1 <= j <= b}和一个正整数s。
    问题是：使得pi + qj = s的不同的(i, j)对有多少个。

   输入
    第1行是测试数据的组数n，后面跟着n组测试数据。

    每组测试数据占5行，第1行是和s (1 <= s <= 10000)，第2行是一个正整数a 
    (1 <= a <= 10000)，表示A中元素的数目。第3行是a个正整数，每个正整数不超过
    10000，表示A中的元素。第4行是一个正整数b (1 <= b <= 10000)，表示B中元素
    的数目。第5行是b个正整数，每个正整数不超过10000，表示B中的元素。

    注意：这里的集合和数学书上定义的集合有一点点区别——集合内可能包含相等的
          正整数。
   
   输出
    n行，每行输出对应一个输入。输出应是一个非负整数。
   
   样例输入
    2
    99
    2
    49 49
    2
    50 50
    11
    9
    1 2 3 4 5 6 7 8 9
    10
    10 9 8 7 6 5 4 3 2 1
   
   样例输出
    4
    9

 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;

typedef vector<int> vint;

static int find_sum(int sum, const vint &a, const vint &b)
{
    int v = sum - *(a.rbegin());
    vint::const_iterator ib = lower_bound(b.begin(), b.end(), v);
    // b.begin .. ib=b[?] ... b.end -> [b.begin, ib) < v -> v <= *ib

    v = sum - *a.begin();
    vint::const_iterator ibend = upper_bound(b.begin(), b.end(), v);
    // b.begin .. ibend=b[?] ... b.end -> [ibend, b.end) > v
    
    int count = 0;
    while (ib != ibend) {   // [ib, ibend)
        v = sum - *ib;
        vint::const_iterator ia = lower_bound(a.begin(), a.end(), v);
        while (ia != a.end() && *ia == v) {
            count++;
            ++ia;
        }
        ++ib;
    }

    return count;
}

static int get_val()
{
    int v;
    cin >> v;
    assert(1 <= v && v <= 10000);
    return v;
}

int main(int argc, char *argv[])
{
    int nCase;

    cin >> nCase;
    while (--nCase >= 0) {
        int sum = get_val();

        int numA = get_val();
        vint A;
        A.reserve(numA);
        for (int i = 0; i < numA; i++) {
            A.push_back(get_val());
        }
        sort(A.begin(), A.end());

        int numB = get_val();
        vint B;
        B.reserve(numB);
        for (int i = 0; i < numB; i++) {
            B.push_back(get_val());
        }
        sort(B.begin(), B.end());

        if (B.size() < A.size())
            cout << find_sum(sum, A, B) << endl;
        else
            cout << find_sum(sum, B, A) << endl;
    }

    return 0;
}

