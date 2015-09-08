/* http://blog.csdn.net/c0de4fun/article/details/7318642/
 * http://cavenkaka.iteye.com/blog/1489588
 */

#include<stdio.h>
int p[150010];

int find(int x)
{
    return p[x] == x ? x : p[x] = find(p[x]);
}

bool same(int x, int y)
{
    return find(x) == find(y);
}

void unite(int x, int y)
{
    int u = find(x);
    int v = find(y);
    if (u != v) p[u] = v;
}

int main()
{
    int N, K, D, x, y, ans = 0;

    scanf("%d%d", &N, &K);
    for (int i = 1; i < 3 * N; i++) {
        p[i] = i;
    }
    for (int i = 1; i <= K; i++) {
        scanf("%d%d%d", &D, &x, &y);
        if (x > N || y > N) {
            printf("#%d: false\n", i);
            ans++;
            continue;
        }
        if (D == 1) {
            if (same(x, y + N) || same(x, y + 2 * N)) {
                printf("#%d: false\n", i);
                ans++;
            } else {
                unite(x, y);
                unite(x + N, y + N);
                unite(x + 2 * N, y + 2 * N);
            }
        } else {
            if (same(x, y) || same(x, y + 2 * N)) {
                printf("#%d: false\n", i);
                ans++;
            } else {
                unite(x, y + N);
                unite(x + N, y + 2 * N);
                unite(x + 2 * N, y);
            }
        }
    }
    printf("%d\n", ans);
    return 0;
}
