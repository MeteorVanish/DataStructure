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
LL n, m, k, fa[maxN];

struct Input
{
    LL x, y, l;
    bool operator<(const Input &rhs) const { return l < rhs.l; }
}Q[maxN], temp[maxN];

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

void Sort(Input arr[], Input t[], LL l, LL r)
{
    if(l >= r) return;
    LL mid = (l + r) >> 1;
    Sort(arr, t, l, mid);
    Sort(arr, t, mid + 1, r);
    LL p = l, q = mid + 1;
    FOR(i, l, r)
    {
        if(p > mid || q <= r && arr[q] < arr[p]) t[i] = arr[q++];
        else t[i] = arr[p++];
    }
    FOR(i, l, r) arr[i] = t[i];
}

LL Find(LL x) { return x == fa[x] ? x : fa[x] = Find(fa[x]); }

bool Check(LL x, LL y) { return Find(x) == Find(y); }

void Union(LL x, LL y) { fa[Find(x)] = Find(y); }

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read(), m = read(), k = read();
    FOR(i, 1, m) Q[i].x = read(), Q[i].y = read(), Q[i].l = read();
    Sort(Q, temp, 1, m);
    LL cnt = n, cost = 0;
    FOR(i, 1, n) fa[i] = i;
    FOR(i, 1, m)
    {
        if(!Check(Q[i].x, Q[i].y))
            Union(Q[i].x, Q[i].y), cost += Q[i].l, --cnt;
        if(cnt == k) break;
    }
    if(cnt == k) printf("%lld\n", cost);
    else printf("No Answer\n");
    return 0;
}