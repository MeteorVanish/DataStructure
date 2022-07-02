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

const LL maxM = 1e5 + 5;
const LL base = 23333;
const LL MOD = 19260817;
LL n, m, arr[maxM], flag[MOD + 5], ans1, ans2 = maxM;
bool exist[MOD + 5];
string s;

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

LL gethash(const string &s)
{
    LL ret = 0;
    FOR(i, 0, s.length() - 1)
        ret = (LL)(ret * base + s[i]) % MOD;
    return ret % MOD;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read();
    FOR(i, 1, n)
    {
        LL hash = 0;
        cin >> s;
        exist[gethash(s)] = true;
    }
    m = read();
    FOR(i, 1, m)
    {
        cin >> s;
        arr[i] = gethash(s);
        if(exist[arr[i]] && !flag[arr[i]])
            ++ans1, flag[arr[i]] = true;
    }
    CLEAR(flag);
    LL p = 0, q = 1, cnt = 0;
    while(q <= m)
    {
        while(cnt != ans1 && q <= m)
        {
            if(exist[arr[q]] && !flag[arr[q]]) ++cnt;
            ++flag[arr[q]], ++q;
        }
        while(1)
        {
            if(cnt < ans1) break;
            ++p;
            if(exist[arr[p]]) --flag[arr[p]];
            if(!flag[arr[p]]) --cnt;
        }
        if(q - p < ans2) ans2 = q - p;
    }
    printf("%lld\n%lld\n", ans1, ans2);
    return 0;
}