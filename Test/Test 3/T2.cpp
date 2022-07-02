#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(int i = start; i <= end; ++i)
#define ROF(i, start, end) for(int i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)
#define toP(x, y) ((x) + (y) - 1)
#define toQ(x, y) ((x) + m - (y))
using LL = long long;

const int maxN = 3e3 + 5;
int n, m, arr[maxN][maxN], tree[maxN << 1][maxN << 1];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline int read()
{
    char ch = getchar();
    int x = 0, f = 1;
    while(ch < '0' || ch > '9')
    {
        if(ch == '-') f = -1;
        ch = getchar();
    }
    while('0' <= ch && ch <= '9')
    {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}

inline int lowbit(int x) { return x & -x;}

void update(int p, int q, int num)
{
    for(int i = p; i <= n + m - 1; i += lowbit(i))
        for(int j = q; j <= n + m - 1; j += lowbit(j))
            tree[i][j] += num;
}

LL query(int p, int q)
{
    LL sum = 0;
    for(int i = p; i; i -= lowbit(i))
        for(int j = q; j; j -= lowbit(j))
            sum += tree[i][j];
    return sum;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read(), m = read();
    FOR(i, 1, n) FOR(j, 1, m) arr[i][j] = read();
    FOR(i, 1, n) FOR(j, 1, m)
            update(toP(i, j), toQ(i, j), arr[i][j]);
    int op, x, y, a;
    while(scanf("%d%d%d%d", &op, &y, &x, &a) != EOF)
    {
        if(op == 1) update(toP(x, y), toQ(x, y), a);
        else
        {
            LL ans1 = query(toP(x + a, y), toQ(x + a, y)),
                    ans2 = query(toP(x, y - a) - 1, toQ(x, y - a)),
                    ans3 = query(toP(x, y + a), toQ(x, y + a) - 1),
                    ans4 = query(toP(x - a, y) - 1, toQ(x - a, y) - 1);
            printf("%lld\n", ans1 - ans2 - ans3 + ans4);
        }
    }
    return 0;
}