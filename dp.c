/* dp.c

   http://blog.csdn.net/woshi250hua/article/details/7952496
   
   挺经典的一类状态DP--子矩形覆盖父矩形，这类DP一般对一行的01状态进行压缩，
   然后按行进行转移。

   这题中每列的0表示未被覆盖，下一行的就必须覆盖它，我是理解成一个插头，
   留给下一行一个插头。每列的1表示已被覆盖，或者理解成没有向下的插头。
   因为当前行只会受上一行影响，所以可以一行一行进行转移，如果前一个状态
   能到下一个状态，那么就能转移。

   设dp[i][j]表示到第i行状态为j的方案数，那么
   
    dp[i][j] += dp[i][k] (if (Ok(j->k)))
   
   问题就变成Ok函数怎么写呢？易知0->1（上一行为0，当前行为1）,那么1->?1->0
   肯定可以，上面无插头，下面留一个插头。1->1呢？不能单独判，必须判下一列
   是不是也是1->1，不是则不Ok。其实最早我不是这样写的，最早的写法很好理解，
   如果上一行为0那么当前行必须为1，如果有一排的1，那么当前行也必须有0*2或
   1*2或2*2个1与之对应，2个1必须连续，不过这样写跑了1000ms，而用现在这个Ok
   则跑了200ms。

   本题还有一个剪枝，当i * j为奇数的时候方案数为0，因为子矩形的方格数为2
   是偶数，没办法组成一个奇数。

   还有我的写法是从0行开始转移的，这样可以if(dp[i][j]) xxxxoooo进行剪枝

 */

#include <stdio.h>
#include <string.h>

#define ROWS    32
#define COLS    11
#define MAX     (1 << COLS)

static unsigned long long dp[2][MAX];
static int rows, cols;

int Ok(int pre, int cur) {

    for (int i = 0; i < cols; ++i) {
        int t1 = pre & (1 << i);
        int t2 = cur & (1 << i);
        if (!t1 && !t2) return 0;
        if (t1 && t2) {
            i++;
            if (i == cols) return 0;
            if ((pre & (1 << i)) == 0) return 0;
            if ((cur & (1 << i)) == 0) return 0;
        }
    }
    return 1;
}

int main()
{
    int i, j, k, bigest;
    int T;

    while (scanf("%d%d", &rows ,&cols) == 2) {
        if (rows <= 0 && cols <= 0) break;
        
        if (cols > rows) {
            k = cols;
            cols = rows;
            rows = k;
        }

        if (rows % 2 && cols % 2) {
            printf("0\n");
            continue;
        }

        if (cols > COLS) {
            printf("*** cols(%d) must be 0~%d\n", cols, COLS);
            continue;
        }
        if (rows > ROWS) {
            printf("*** rows(%d) must be 0~%d\n", rows, ROWS);
            continue;
        }

        memset(dp, 0, sizeof(dp));
        bigest = (1 << cols) - 1;
        dp[T=0][bigest] = 1;

        for (i = 0; i < rows; ++i) {
            memset(dp[T^1], 0, sizeof(dp[0]));
            for (j = 0; j <= bigest; ++j) {
                if (dp[T][j]) {
                    for (k = 0; k <= bigest; ++k) {
                        if (Ok(j,k)) dp[T^1][k] += dp[T][j];
                    }
                }
            }
            T ^= 1;
        }
        printf("%llu\n", dp[T][bigest]);
    }
}
