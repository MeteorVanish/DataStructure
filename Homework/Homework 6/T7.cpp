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

const int maxN = 1e5 + 5;
const int maxM = 1e5 + 5;
int n, m, ans[maxN];
int cnt, Head[maxN];
struct Edge { int to, next; } edge[maxM << 1];

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

void add(int x, int y)
{
    ++cnt;
    edge[cnt].to = y;
    edge[cnt].next = Head[x];
    Head[x] = cnt;
}

void dfs(int pos, int x)
{
    if(ans[pos]) return;
    ans[pos] = x;
    for(int i = Head[pos]; i; i = edge[i].next)
        dfs(edge[i].to, x);
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read(), m = read();
    FOR(i, 1, m)
    {
        int u = read(), v = read();
        add(v, u);
    }
    ROF(i, 1, n) dfs(i, i);
    FOR(i, 1, n)
        printf("%d%c", ans[i], " \n"[i == n]);
    return 0;
}