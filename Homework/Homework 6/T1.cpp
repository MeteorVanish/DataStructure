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

const LL maxN = 1e5 + 5;
LL n, cnt, Value[maxN], Head[maxN];
struct Edge
{
    LL to, next;
}edge[maxN << 1];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline LL read()
{
    LL ret = 0, f = 1;
    char c = getchar();
    while(!(c >= '0' && c <= '9'))
    {
        if(c == '-') f = -1;
        c = getchar();
    }
    while(c >= '0' && c <= '9')
        ret = (ret << 3) + (ret << 1) + c - '0', c = getchar();
    return f * ret;
}

void add(LL x, LL y)
{
    ++cnt;
    edge[cnt].to = y;
    edge[cnt].next = Head[x];
    Head[x] = cnt;
}

void dfs(int pos, int fa)
{
    for(LL i = Head[pos]; i; i = edge[i].next)
    {
        LL nxt = edge[i].to;
        if(nxt != fa)
        {
            dfs(nxt, pos);
            Value[pos] += Value[nxt];
        }
    }
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read();
    FOR(i, 0, n - 1) Value[i] = read();
    FOR(i, 1, n - 1)
    {
        LL u = read(), v = read();
        add(u, v), add(v, u);
    }
    dfs(0, 0);
    FOR(i, 0, n - 1)
        printf("%lld\n", Value[i]);
    return 0;
}