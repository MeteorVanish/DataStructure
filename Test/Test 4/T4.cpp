#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(int i = start; i <= end; ++i)
#define ROF(i, start, end) for(int i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)
using LL = long long;

const int maxN = 4e7 + 5;
const int MOD = 19260817;
int T, n, fa[maxN], hashSet[maxN];

struct Input
{
    int x, y, e;
}q[maxN];

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

int gethash(int x)
{
    int p = x % MOD;
    while(!p || hashSet[p] && hashSet[p] != x) p = (p + 1) % MOD;
    hashSet[p] = x;
    return p;
}

int Find(int x) { return x == fa[x] ? x : fa[x] = Find(fa[x]); }

void Union(int x, int y)
{
    int fx = Find(x), fy = Find(y);
    if(fx == fy) return;
    fa[fx] = fy;
}

bool Check(int x, int y) { return Find(x) == Find(y); }

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    T = read();
    while(T--)
    {
        bool ans = true;
        n = read();
        FOR(i, 1, MOD) fa[i] = i;
        FOR(i, 1, n) q[i].x = gethash(read()), q[i].y = gethash(read()), q[i].e = read();
        FOR(i, 1, n)
            if(q[i].e) Union(q[i].x, q[i].y);
        FOR(i, 1, n)
            if(!q[i].e && Check(q[i].x, q[i].y)) { ans = false; break; }
        if(ans) printf("YES\n");
        else printf("NO\n");
    }
    return 0;
}