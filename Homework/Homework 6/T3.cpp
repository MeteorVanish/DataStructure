#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;
using LL = long long;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(int i = start; i <= end; ++i)
#define ROF(i, start, end) for(int i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)

const int maxN = 5e5 + 5;
const int maxM = 5e5 + 5;
const int INF = 0x7fffffff;
int n, m, s, p;
int Ecnt, Head[maxN], val[maxN];
int dfncnt, dfn[maxN], low[maxN], top, Stack[maxN], vis[maxN];
int color, scc[maxN], sccval[maxN];
int dis[maxN], SPFAvis[maxN], Queue[maxN * 5], head, tail;
bool bar[maxN], sccbar[maxN];
struct Edge { int from, to, nxt; } edge[maxM];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline int read()
{
    int ret = 0, f = 1;
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

void add(int u, int v)
{
    ++Ecnt;
    edge[Ecnt].from = u, edge[Ecnt].to = v;
    edge[Ecnt].nxt = Head[u], Head[u] = Ecnt;
}

void Tarjan(int u)
{
    dfn[u] = low[u] = ++dfncnt;
    Stack[++top] = u, vis[u] = 1;
    for(int i = Head[u]; i; i = edge[i].nxt)
    {
        int v = edge[i].to;
        if(!dfn[v])
        {
            Tarjan(v);
            low[u] = min(low[u], low[v]);
        }
        else if(vis[v])
            low[u] = min(low[u], dfn[v]);
    }
    if(dfn[u] == low[u])
    {
        ++color;
        int v;
        while(v = Stack[top--])
        {
            scc[v] = color;
            sccval[color] -= val[v];
            if(bar[v]) sccbar[color] = true;
            if(v == s) s = color;
            vis[v] = 0;
            if(u == v) break;
        }
    }
}

void SPFA()
{
    FOR(i, 1, color) dis[i] = 0;
    dis[s] = sccval[s], SPFAvis[s] = 1;
    Queue[++tail] = s;
    while(head < tail)
    {
        int u = Queue[++head];
        SPFAvis[u] = 0;
        for(int i = Head[u]; i; i = edge[i].nxt)
        {
            int v = edge[i].to;
            if(dis[v] > dis[u] + sccval[v])
            {
                dis[v] = dis[u] + sccval[v];
                if(!vis[v]) Queue[++tail] = v, SPFAvis[v] = 1;
            }
        }
    }
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read(), m = read();
    FOR(i, 1, m)
    {
        int u = read(), v = read();
        add(u, v);
    }
    FOR(i, 1, n) val[i] = read();
    s = read(), p = read();
    FOR(i, 1, p) { int tmp = read(); bar[tmp] = true; }
    FOR(i, 1, n)
        if(!dfn[i]) Tarjan(i);
    Ecnt = 0;
    CLEAR(Head);
    FOR(i, 1, m)
    {
        int u = scc[edge[i].from], v = scc[edge[i].to];
        if(u != v) add(u, v);
    }
    SPFA();
    int ans = 0;
    FOR(i, 1, color)
        if(sccbar[i] && ans > dis[i]) ans = dis[i];
    printf("%d\n", -ans);
    return 0;
}