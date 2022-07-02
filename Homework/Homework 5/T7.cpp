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

const int maxN = 1e5 + 5;
const int BASE = 233;
int n, m, ans;
int father[maxN << 1], Rank[maxN << 1], hashset[maxN];

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
    int ret = x % maxN;
    while(!ret || hashset[ret] && hashset[ret] != x) ret = (ret + 1) % maxN;
    hashset[ret] = x;
    return ret;
}

int Find(int x) { return x == father[x] ? x : father[x] = Find(father[x]); }

void Union(int x, int y)
{
    int fx = Find(x), fy = Find(y);
    if(fx == fy) return;
    if(Rank[fx] < Rank[fy]) father[fx] = fy; else
    if(Rank[fx] > Rank[fy]) father[fy] = fx; else
    if(Rank[fx] == Rank[fy]) father[fx] = fy, ++Rank[fy];
}

bool Check(int x, int y)
{
    int fx = Find(x), fy = Find(y);
    if(fx == fy) return true;
    else return false;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    FOR(i, 1, (maxN << 1) - 1) father[i] = i;
    n = read(), m = read();
    string s;
    FOR(i, 1, m)
    {
        int u = gethash(read()), v = gethash(read() + 1);
        cin >> s;
        if(s == "even")
        {
            if(Check(u + maxN, v) || Check(u, v + maxN))
            {
                ans = i - 1;
                break;
            }
            Union(u, v), Union(u + maxN, v + maxN);
        }
        else
        {
            if(Check(u, v) || Check(u + maxN, v + maxN))
            {
                ans = i - 1;
                break;
            }
            Union(u + maxN, v), Union(u, v + maxN);
        }
    }
    printf("%d\n", ans ? ans : m);
    return 0;
}