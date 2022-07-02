#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(LL i = start; i <= end; ++i)
#define ROF(i, start, end) for(LL i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)
using LL = long long;

const LL maxN = (1 << 12) + 5;
LL n, m, arr[maxN][maxN];

void file(int op)
{
    string I = "Input " + to_string(op) + ".txt", O = "Output " + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline LL lowbit(LL x) {return x & -x;}

void add(LL x, LL y, LL k)
{
    for(LL i = x; i <= n; i += lowbit(i))
        for(LL j = y; j <= m;j += lowbit(j))
            arr[i][j] += k;
}

LL query(LL x, LL y)
{
    if(x <= 0 || y <= 0) return 0;
    LL sum = 0;
    for(LL i = x; i; i -= lowbit(i))
        for(LL j = y; j; j -= lowbit(j))
            sum += arr[i][j];
    return sum;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    scanf("%lld%lld", &n, &m);
    int op;
    while(scanf("%d", &op) != EOF)
    {
        if(op == 1)
        {
            LL x, y, k;
            scanf("%lld%lld%lld", &x, &y, &k);
            add(x, y, k);
        }
        else
        {
            LL x, y, z, w;
            scanf("%lld%lld%lld%lld", &x, &y, &z, &w);
            printf("%lld\n", query(z, w) - query(z, y - 1) - query(x - 1, w) + query(x - 1, y - 1));
        }
    }
    return 0;
}