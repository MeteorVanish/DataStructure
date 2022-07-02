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
LL n, m, ans;
LL fa[maxN], num[maxN];
struct Edge
{
    LL from, to, val;
    bool operator<(const Edge &rhs) const { return val < rhs.val; }
} edge[maxN], temp[maxN];

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

void mySort(Edge arr[],Edge tmp[], LL l, LL r)
{
    if(l >= r) return;
    LL mid = (l + r) >> 1;
    mySort(arr, tmp, l, mid);
    mySort(arr, tmp, mid + 1, r);
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

void Union(LL x, LL y)
{
    LL fx= Find(x), fy = Find(y);
    if(fx == fy) return;
    fa[fx] = fy;
    num[fy] += num[fx];
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read();
    FOR(i, 1, n)
        fa[i] = i, num[i] = 1;
    FOR(i, 1, n - 1)
        edge[i].from = read(), edge[i].to = read(), edge[i].val = read();
    mySort(edge, temp, 1, n - 1);
    FOR(i, 1, n - 1)
    {
        LL u = Find(edge[i].from), v = Find(edge[i].to), w = edge[i].val;
        ans += (w + 1) * (num[u] * num[v] - 1) + w;
        Union(u, v);
    }
    printf("%lld\n", ans);
    return 0;
}