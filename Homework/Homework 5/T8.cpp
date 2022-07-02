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

const LL maxN = 1e6 + 5;
const LL MOD = 1e9 + 7;
const LL BASE = 23;

LL T, C, n, m, cnt, hashT;
LL ptr[maxN], dis[maxN], nextS[maxN], hashS[maxN], powBASE[maxN], ans[maxN];

void file(LL op)
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

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    T = read(), C = read();
    powBASE[0] = 1, powBASE[1] = BASE;
    FOR(i, 2, maxN - 1) powBASE[i] = (powBASE[i - 1] * BASE) % MOD;
    while(T--)
    {
        CLEAR(dis), CLEAR(nextS), CLEAR(hashS), CLEAR(ans);
        cnt = hashT = 0;
        n = read(), m = read();
        CLEAR(ptr);
        FOR(i, 1, n)
        {
            LL x = read();
            if(ptr[x])
            {
                nextS[ptr[x]] = i;
                dis[i] = i - ptr[x];
            }
            else dis[i] = 0;
            ptr[x] = i;
            if(i <= m) hashS[i] = (hashS[i - 1] * BASE + dis[i]) % MOD;
        }
        CLEAR(ptr);
        FOR(i, 1, m)
        {
            LL x = read();
            hashT = (hashT * BASE + (ptr[x] ? i - ptr[x] : 0)) % MOD;
            ptr[x] = i;
        }
        LL hash = hashS[m];
        FOR(i, m, n)
        {
            if(hashT == hash) ans[++cnt] = i - m + 1;
            if(nextS[i - m + 1])
            {
                if(nextS[i - m + 1] <= i)
                    hash = (hash - dis[nextS[i - m + 1]] * powBASE[i - nextS[i - m + 1]] % MOD + MOD) % MOD;
                dis[nextS[i - m + 1]] = 0;
            }
            hash = (hash * BASE + dis[i + 1]) % MOD;
        }
        printf("%lld\n", cnt);
        FOR(i, 1, cnt) printf("%lld%c", ans[i], " \n"[i == cnt]);
    }
    return 0;
}