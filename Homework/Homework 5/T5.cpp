#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(LL i = start; i <= end; ++i)
#define ROF(i, end, start) for(LL i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)
using LL = long long;

const LL maxN = 2e5 + 5;
LL n, m;
LL father[maxN], delnum[maxN];
bool exist[maxN], ans[maxN];

struct MeteorVanish
{
    LL from, to, next;
}edge[maxN << 1];
LL cnt, head[maxN];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

void Add(LL u, LL v)
{
    ++cnt;
    edge[cnt].from = u, edge[cnt].to = v;
    edge[cnt].next = head[u], head[u] = cnt;
}

inline LL read()
{
    LL ret = 0, f = 1;
    char c = getchar();
    while(!isdigit(c))
    {
        if(c == '-') f = -1;
        c = getchar();
    }
    while(isdigit(c))
        ret = (ret << 3) + (ret << 1) + c - '0', c = getchar();
    return f * ret;
}

LL Find(LL x) { return father[x] == x ? x : father[x] = Find(father[x]); }

bool Union(LL x, LL y)
{
    LL fx = Find(x), fy = Find(y);
    if(fx == fy) return false;
    father[fx] = fy;
    return true;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read(), m = read();
    FOR(i, 1, n) father[i] = i;
    FOR(i, 1, m)
    {
        LL u = read(), v = read();
        Add(u, v), Add(v, u);
    }
    FOR(i, 1, n) delnum[i] = read();
    LL k = 0;
    ROF(i, n, 1)
    {
        exist[delnum[i]] = true;
        for(LL j = head[delnum[i]]; j; j = edge[j].next)
            if(exist[edge[j].to] && Union(edge[j].from, edge[j].to)) ++k;
        if(k == n - i) ans[i] = true;
    }
    FOR(i, 1, n)
        if(ans[i]) printf("YES\n");
        else printf("NO\n");
    return 0;
}