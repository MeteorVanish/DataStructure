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

const int maxN = 1e3 + 5;
const int maxM = 1e4 + 5;
int n, m, k, inf, ans;
int cnt, Head[maxN];
int Queue[maxN * 10], head, tail, num[maxN], vis[maxN];
struct Edge { int to, val, next; } edge[maxM << 1];

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

void add(int u, int v, int w)
{
    ++cnt;
    edge[cnt].to = v, edge[cnt].val = w;
    edge[cnt].next = Head[u], Head[u] = cnt;
}

bool spfa(int lim)
{
    head = tail = 0;
    CLEAR(Queue);
    FOR(i, 2, n) num[i] = inf;
    num[1] = 0, vis[1] = 1, Queue[++tail] = 1;
    while(head < tail)
    {
        int u = Queue[++head];
        vis[u] = 0;
        for(int i = Head[u]; i; i = edge[i].next)
        {
            int v = edge[i].to, ifleqlim = edge[i].val > lim ? 1 : 0;
            if(num[v] > num[u] + ifleqlim)
            {
                num[v] = num[u] + ifleqlim;
                if(!vis[v])
                    Queue[++tail] = v, vis[v] = 1;
            }
        }
    }
    return num[n] <= k;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read(), m = read(), k = read();
    FOR(i, 1, m)
    {
        int u = read(), v = read(), w = read();
        add(u, v, w), add(v, u, w);
        if(w > inf) inf = w;
    }
    int l = 0, r = inf;
    while(l <= r)
    {
        int mid = (l + r) >> 1;
        if(spfa(mid)) r = mid - 1, ans = mid;
        else l = mid + 1;
    }
    printf("%d\n", ans);
    return 0;
}