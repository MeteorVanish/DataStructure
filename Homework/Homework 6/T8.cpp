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

const LL maxN = 1e5 + 5;
const LL maxM = 5e5 + 5;
LL n, m, ans;
LL cnt = -1, Head[maxN];
LL dfncnt, dfn[maxN], low[maxN], vis[maxN], top, Stack[maxN], scc[maxN], color;
LL fa[maxN];
struct Edge
{
    LL from, to, a, b, next, val, flag;
    bool operator<(const Edge &rhs) const { return val < rhs.val; }
} edge[maxM << 1], temp[maxM << 1];

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

void add(LL x, LL y, LL a, LL b)
{
    ++cnt;
    edge[cnt].from = x, edge[cnt].to = y, edge[cnt].a = a, edge[cnt].b = b;
    edge[cnt].next = Head[x], Head[x] = cnt;
}

void Tarjan(LL u)
{
    dfn[u] = low[u] = ++dfncnt, vis[u] = 1, Stack[++top] = u;
    for(LL i = Head[u]; i; i = edge[i].next)
    {
        if(edge[i].flag) continue;
        edge[i ^ 1].flag = 1;
        LL v = edge[i].to;
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
        LL v;
        while(v = Stack[top--])
        {
            scc[v] = color;
            vis[v] = 0;
            if(u == v) break;
        }
    }
}

void MergeSort(Edge arr[], Edge tmp[], LL l, LL r)
{
    if(l >= r) return;
    LL mid = (l + r) >> 1;
    MergeSort(arr, tmp, l, mid);
    MergeSort(arr, tmp, mid + 1, r);
    LL p = l, q = mid + 1;
    FOR(i, l, r)
    {
        if(p > mid || q <= r && arr[q] < arr[p])
            tmp[i] = arr[q++];
        else
            tmp[i] = arr[p++];
    }
    FOR(i, l, r) arr[i] = tmp[i];
}

LL Find(LL x) { return x == fa[x] ? x : fa[x] = Find(fa[x]); }

void Union(LL x, LL y) { fa[Find(x)] = Find(y); }

bool Check(LL x, LL y) { return Find(x) == Find(y); }

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read(), m = read();
    FOR(i, 1, n) fa[i] = i;
    FOR(i, 1, m)
    {
        LL s = read(), t = read(), a = read(), b = read();
        add(s, t, a, b), add(t, s, a, b);
    }
    FOR(i, 1, n)
        if(!dfn[i]) Tarjan(i);
    FOR(i, 0, (m << 1) - 1)
    {
        LL u = scc[edge[i].from], v = scc[edge[i].to];
        if(u != v) edge[i].val = edge[i].b;
        else edge[i].val = edge[i].a;
    }
    MergeSort(edge, temp, 0, (m << 1) - 1);
    for(LL i = 0; i < (m << 1); i += 2)
    {
        LL k = 0;
        if(!Check(edge[i].from, edge[i].to))
        {
            ans += edge[i].val;
            Union(edge[i].from, edge[i].to);
            ++k;
        }
        if(k == n - 1) break;
    }
    printf("%lld\n", ans);
    return 0;
}