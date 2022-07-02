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

const int maxN = 1e4 + 5;
int n, m, fa[maxN << 1];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

int Find(int x) { return x == fa[x] ? x : fa[x] = Find(fa[x]); }

void Union(int x, int y) { fa[Find(x)] = Find(y); }

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    scanf("%d%d", &n, &m);
    FOR(i, 1, n << 1) fa[i] = i;
    FOR(i, 1, m)
    {
        char opt; cin >> opt;
        int p, q;
        scanf("%d%d", &p, &q);
        if(opt == 'F') Union(p, q);
        else Union(p + n, q), Union(q + n, p);
    }
    int ans = 0;
    FOR(i, 1, n) if(fa[i] == i) ++ans;
    printf("%d\n", ans);
    return 0;
}