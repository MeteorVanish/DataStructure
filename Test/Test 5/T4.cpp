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

const int maxN = 5e3 + 5;
const int maxM = 1e4 + 5;
const LL INF = 0x3fffffffffffffff;
int n, m;
int cnt, Head[maxN];
int head, tail, Queue[maxN << 8], vis[maxN];
LL ans = INF, dis[maxN];
struct Edge { int to, val, next; } edge[maxM << 1];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

void add(int x, int y, int z)
{
    ++cnt;
    edge[cnt].to = y, edge[cnt].val = z;
    edge[cnt].next = Head[x], Head[x] = cnt;
}

void SPFA(int pos)
{
    head = tail = 0;
    MEMSET(vis, 0);
    FOR(i, 1, n) dis[i] = INF;
    dis[pos] = 0, vis[pos] = 1, Queue[++tail] = pos;
    while(head < tail)
    {
        int u = Queue[++head];
        vis[u] = 0;
        for(int i = Head[u]; i; i = edge[i].next)
        {
            int v = edge[i].to, w = edge[i].val;
            if(u == pos && v == 1) continue;
            if(dis[v] > dis[u] + w)
            {
                dis[v] = dis[u] + w;
                if(!vis[v]) Queue[++tail] = v, vis[v] = 1;
            }
        }
    }
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    scanf("%d%d", &n, &m);
    FOR(i, 1, m)
    {
        int u, v, a, b;
        scanf("%d%d%d%d", &u, &v, &a, &b);
        add(u, v, a), add(v, u, b);
    }
    for(int i = Head[1]; i; i = edge[i].next)
    {
        SPFA(edge[i].to);
        if(edge[i].val + dis[1] < ans)
            ans = edge[i].val + dis[1];
    }
    printf("%lld\n", ans);
    return 0;
}