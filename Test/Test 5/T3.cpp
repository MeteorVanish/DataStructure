#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;
using LL = long long;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(LL i = start; i <= end; ++i)
#define ROF(i, start, end) for(LL i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)

const LL maxN = 1e4 + 5;
const LL maxM = 5e4 + 5;
LL n, m, t, ans1, ans2, dfsroot, tmp;
LL cnt, Head[maxN];
LL dfncnt, dfn[maxN], low[maxN], flag, Stack[maxN];
bool exist[maxN], cut[maxN], vis[maxN];
struct Edge { LL to, next, flag; } edge[maxM << 1];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline LL read()
{
    LL ret = 0, f = 1; char c = getchar();
    while(!(c >= '0' && c <= '9')) { if(c == '-') f = -1; c = getchar(); }
    while(c >= '0' && c <= '9') ret = (ret << 3) + (ret << 1) + c - '0', c = getchar();
    return f * ret;
}

void add(LL x, LL y)
{
    ++cnt;
    edge[cnt].to = y;
    edge[cnt].next = Head[x];
    Head[x] = cnt;
}

void Tarjan(LL u, LL root)
{
    int child = 0;
    dfn[u] = low[u] = ++dfncnt;
    for(LL i = Head[u]; i; i = edge[i].next)
    {
        LL v = edge[i].to;
        if(!dfn[v])
        {
            Tarjan(v, root);
            low[u] = std::min(low[u], low[v]);
            if(u == root) ++child;
            if(u != root && low[v] >= dfn[u]) cut[u] = true;
        }
        else low[u] = std::min(low[u], dfn[v]);
    }
    if(u == root && child >= 2) cut[root] = true;
}

void dfs(LL u)
{
    ++tmp;
    vis[u] = true;
    for(LL i = Head[u]; i; i = edge[i].next)
    {
        LL v = edge[i].to;
        if(vis[v]) continue;
        else if(cut[v])
        {
            Stack[++flag] = v;
            vis[v] = true;
            continue;
        }
        dfs(v);
    }
}

void Calc()
{
    if(n == 1) ans1 = ans2 = 1;
    else
    {
        ans1 = 2;
        ans2 = n * (n - 1) / 2;
    }
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    while(1)
    {
        m = read();
        if(!m) break;
        ++t;
        n = ans1 = ans2 = dfncnt = cnt = 0;
        CLEAR(dfn), CLEAR(low), CLEAR(Head), CLEAR(exist), CLEAR(cut), CLEAR(vis), CLEAR(edge);
        FOR(i, 1, m)
        {
            LL u = read(), v = read();
            if(!exist[u]) ++n, exist[u] = true;
            if(!exist[v]) ++n, exist[v] = true;
            add(u, v), add(v, u);
        }
        FOR(i, 1, n)
            if(!dfn[i]) Tarjan(i, i);
        LL cutcnt = 0;
        FOR(i, 1, n)
            if(cut[i]) ++cutcnt;
        FOR(i, 1, n)
        {
            tmp = flag = 0;
            CLEAR(Stack);
            if(!cut[i] && !vis[i]) dfs(i);
            if(tmp && flag < 2)
            {
                ++ans1;
                if(ans2) ans2 *= tmp;
                else ans2 = tmp;
            }
            FOR(j, 1, flag) vis[Stack[j]] = false;
        }
        if(!ans1 || !cutcnt) Calc();
        printf("Case %lld: %lld %lld\n", t, ans1, ans2);
    }
    return 0;
}