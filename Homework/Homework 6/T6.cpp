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

const int maxN = 3e5 + 5;
const int MOD = 1e7 + 19;
const int INF = 1e8;
int n, arr[maxN];
int cnt, Head[MOD + maxN];
int HashSet[MOD];
int dis[MOD + maxN], vis[MOD + maxN], head, tail, Queue[MOD + maxN];
struct Edge { int to, val, next; } edge[maxN << 2];

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

int Hash(int x)
{
    int p = abs(x % MOD);
    while(HashSet[p] && HashSet[p] != x) p = (p + 1) % MOD;
    HashSet[p] = x;
    return p;
}

void add(int x, int y, int z)
{
    ++cnt;
    edge[cnt].to = y, edge[cnt].val = z;
    edge[cnt].next = Head[x], Head[x] = cnt;
}

void SPFA()
{
    MEMSET(dis, 127);
    dis[MOD + 1] = 0, vis[MOD + 1] = 0, Queue[++tail] = MOD + 1;
    while(head < tail)
    {
        int u = Queue[++head];
        vis[u] = 0;
        for(int i = Head[u]; i; i = edge[i].next)
        {
            int v = edge[i].to, w = edge[i].val;
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
    n = read();
    FOR(i, 2, n)
    {
        add(MOD + i - 1, MOD + i, 2);
        add(MOD + i, MOD + i - 1, 2);
    }
    FOR(i, 1, n)
    {
        arr[i] = Hash(read());
        add(MOD + i, arr[i], 1);
        add(arr[i], MOD + i, 1);
    }
    SPFA();
    printf("%d\n", dis[MOD + n] / 2);
    return 0;
}