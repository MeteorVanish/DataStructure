#include <iostream>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 50005;
int n, q, num, head[maxN], depth[maxN], lg[maxN], fa[maxN][20], sum[maxN][20];
struct node
{
    int to, next, val;
}edge[maxN << 1];

inline void file()
{
    string op = "1";
    string INPUT = "Input " + op + ".txt", OUTPUT = "Output " + op + ".txt";
    freopen(INPUT.c_str(), "r", stdin);
    freopen(OUTPUT.c_str(), "w", stdout);
}

void ADD(int x, int y, int z)
{
    ++num;
    edge[num].to = y;
    edge[num].val = z;
    edge[num].next = head[x];
    head[x] = num;
}

void dfs(int cur, int father, int value)
{
    fa[cur][0] = father;
    sum[cur][0] = value;
    depth[cur] = depth[father] + 1;
    FOR(i, 1, lg[depth[cur]])
    {
        fa[cur][i] = fa[fa[cur][i - 1]][i - 1];
        sum[cur][i] = sum[cur][i - 1] + sum[fa[cur][i - 1]][i - 1];
    }
    for(int i = head[cur]; i; i = edge[i].next)
        if(edge[i].to != father) dfs(edge[i].to, cur, edge[i].val);
}

int LCApos(int x, int y)
{
    if(depth[x] > depth[y]) swap(x, y);
    while(depth[x] < depth[y])
        y = fa[y][lg[depth[y] - depth[x]]];
    if(x == y) return x;
    for(int i = lg[depth[x]]; i >= 0; --i)
        if(fa[x][i] != fa[y][i])
            x = fa[x][i], y = fa[y][i];
    return fa[x][0];
}

int LCAval(int x, int y)
{
    int ret = 0;
    if(depth[x] > depth[y]) swap(x, y);
    while(depth[x] < depth[y])
    {
        ret += sum[y][lg[depth[y] - depth[x]]];
        y = fa[y][lg[depth[y] - depth[x]]];
    }
    if(x != y)
    {
        for(int i = lg[depth[x]]; i >= 0; --i)
            if(fa[x][i] != fa[y][i])
            {
                ret += sum[x][i] + sum[y][i];
                x = fa[x][i], y = fa[y][i];
            }
        ret += sum[x][0] + sum[y][0];
    }
    return ret;
}

int min(int a, int b, int c)
{
    if(a < b) return a < c ? a : c;
    else return b < c ? b : c;
}

int main()
{
    //file();
    scanf("%d", &n);
    FOR(i, 1, n - 1)
    {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        ADD(a, b, c);
        ADD(b, a, c);
    }
    FOR(i, 2, n) lg[i] = lg[i / 2] + 1;
    dfs(1, 0, 0);
    scanf("%d", &q);
    while(q--)
    {
        int x, y, z, xy, yz, zx;
        scanf("%d%d%d", &x, &y, &z);
        xy = LCApos(x, y);
        yz = LCApos(y, z);
        zx = LCApos(z, x);
        printf("%d\n", min(LCAval(xy, x) + LCAval(xy, y) + LCAval(xy, z), LCAval(yz, x) + LCAval(yz, y) + LCAval(yz, z), LCAval(zx, x) + LCAval(zx, y) + LCAval(zx, z)));
    }
    return 0;
}