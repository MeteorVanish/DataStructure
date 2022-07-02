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

const LL maxN = 5e5 + 5;
LL n, m, arr[maxN];

void file(LL op)
{
    string I = "Input " + to_string(op) + ".txt", O = "Output " + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline LL lowbit(LL x) {return x & -x;}

void build()
{
    FOR(i, 1, n)
        if(i + lowbit(i) <= n)
            arr[i + lowbit(i)] += arr[i];
}

void add(LL pos, LL data)
{
    while(pos <= n)
    {
        arr[pos] += data;
        pos += lowbit(pos);
    }
}

LL query(LL pos)
{
    LL sum = 0;
    while(pos)
    {
        sum += arr[pos];
        pos -= lowbit(pos);
    }
    return sum;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    scanf("%lld%lld", &n, &m);
    LL last = 0, data;
    FOR(i, 1, n)
    {
        scanf("%lld", &data);
        arr[i] = data - last;
        last = data;
    }
    build();
    while(m--)
    {
        LL op;
        scanf("%lld", &op);
        if(op == 1)
        {
            LL x, y, k;
            scanf("%lld%lld%lld", &x, &y, &k);
            add(x, k);
            add(y + 1, -k);
        }
        else
        {
            LL x;
            scanf("%lld", &x);
            printf("%lld\n", query(x));
        }
    }
    return 0;
}