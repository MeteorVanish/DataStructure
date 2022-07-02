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
LL n, m, p, q, ans1, ans2;
LL father[maxN], num[maxN];

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
    while(!isdigit(c))
    {
        if(c == '-') f = -1;
        c = getchar();
    }
    while(isdigit(c))
        ret = (ret << 3) + (ret << 1) + c - '0', c = getchar();
    return f * ret;
}

LL Find(LL x) { return x == father[x] ? x : father[x] = Find(father[x]); }

void Union(LL x, LL y)
{
    LL fx = Find(x), fy = Find(y);
    if(fx == fy) return;
    father[fx] = fy;
    num[fy] += num[fx];
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read(), m = read(), p = read(), q = read();
    FOR(i, 1, n) father[i] = i, num[i] = 1;
    while(p--)
    {
        LL xi = read(), yi = read();
        Union(xi, yi);
    }
    ans1 = num[Find(1)];
    CLEAR(father), CLEAR(num);
    FOR(i, 1, m) father[i] = i, num[i] = 1;
    while(q--)
    {
        LL xi = -read(), yi = -read();
        Union(xi, yi);
    }
    ans2 = num[Find(1)];
    printf("%lld\n", min(ans1, ans2));
    return 0;
}