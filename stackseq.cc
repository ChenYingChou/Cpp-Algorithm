/* stackseq.cc -- tonychen [@] finent.com.tw

   描述
    栈是常用的一种数据结构，有n个元素在栈顶端一侧等待进栈，栈顶端另一侧是出栈
    序列。你已经知道栈的操作有两种：push 和 pop，前者是将一个元素进栈，后者是
    将栈顶元素弹出。现在要使用这两种操作，由一个操作序列可以得到一系列的输出
    序列。请你编程求出对于给定的n，计算并输出由操作数序列1，2，…，n，经过一
    系列操作可能得到的输出序列总数。

   输入
    就一个数n(1≤n≤15)。

   输出
    一个数，即可能输出序列的总数目。

   样例输入
    3

   样例输出
    5

   提示
    先了解栈的两种基本操作，进栈 push 就是将元素放入栈顶，栈顶指针上移一位，等
    待进栈队列也上移一位，出栈 pop 是将栈顶元素弹出，同时栈顶指针下移一位。
    用一个过程采模拟进出栈的过程，可以通过循环加递归来实现回溯:重复这样的过程，
    如果可以进栈则进一个元素，如果可以出栈则出一个元素。就这样一个一个地试探下
    去，当出栈元素个数达到n时就计数一次(这也是递归调用结束的条件)。

 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <assert.h>

#if !defined(nullptr)
  #define nullptr 0
#endif

const int MAX_NUM = 18;

static int debug;

//---------------------------------------------------------------------------

// nWaitIn: 等待入棧個數
// nWaitOut: 等待出棧個數(棧內個數)
static int stack(int nWaitIn, int nWaitOut)
{
    // 將 if (nWaitIn == 0) 優化成 if (nWaitIn <= 1), 可節省一半以上的時間
    //if (nWaitIn == 0) {
    //    // 已無等待入棧, 只有出棧一種方法
    //    return nWaitOut > 0 ? 1 : 0;
    //}
    if (nWaitIn <= 1) {
        // 只有 {0,1} 個等待入棧, 出棧方法有 {0,1} + 棧內個數
        return nWaitIn+nWaitOut;
    }
    if (nWaitOut == 0) {
        // stack為空的, 只有入棧一種方法
        return stack(nWaitIn-1, 1);
    }

    // 當有等待入棧及等待出棧時, 有兩種方法: 出棧，或入棧
    return stack(nWaitIn, nWaitOut-1) + stack(nWaitIn-1, nWaitOut+1);
}

//---------------------------------------------------------------------------

static void run(int max_num)
{
    int n;
    while (std::cin >> n) {
        assert(0 <= n && n <= max_num);
        int answer = stack(n, 0);
        if (debug) std::cout << "--> ";
        std::cout << answer << std::endl;
    }
}

int main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int max_num = MAX_NUM;
    {
        int n = 1;
        while (n < argc && argv[n][0] == '-') {
            if (strncmp(argv[n], "-d", 2) == 0) {
                debug += strlen(&argv[n][1]);
            } else if (strncmp(argv[n], "-m", 2) == 0) {
                char *p = &argv[n][2];
                while (*p && !isdigit(*p)) p++;
                if (isdigit(*p)) {
                    max_num = atoi(p);
                    if (max_num < 10) max_num = MAX_NUM;
                }
            } else if (strncmp(argv[n], "-i:", 3) == 0) {
                std::ifstream in(&argv[n][3]);
                //streambuf *cinbuf = cin.rdbuf(); //save old buf
                std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
                run(max_num);
                //cin.rdbuf(cinbuf);
                return 0;
            }
            n++;
        }
    }

    run(max_num);

    return 0;
}
