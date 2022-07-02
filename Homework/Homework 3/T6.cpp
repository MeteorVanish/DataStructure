#include <iostream>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 100005;
struct node
{
    int to, next;
}edge[maxN << 1];
int n, k, num, head[maxN], depth[maxN], diff[maxN], sum[maxN], lg[maxN], fa[maxN][20], ans = -1;

inline void file()
{
    string op = "1";
    string INPUT = "Input " + op + ".txt", OUTPUT = "Output " + op + ".txt";
    freopen(INPUT.c_str(), "r", stdin);
    freopen(OUTPUT.c_str(), "w", stdout);
}

void ADD(int x, int y)
{
    ++num;
    edge[num].to = y;
    edge[num].next = head[x];
    head[x] = num;
}

void build(int cur, int father)
{
    fa[cur][0] = father;
    depth[cur] = depth[father] + 1;
    FOR(i, 1, lg[depth[cur]])
        fa[cur][i] = fa[fa[cur][i - 1]][i - 1];
    for(int i = head[cur]; i; i = edge[i].next)
        if(edge[i].to != father) build(edge[i].to, cur);
}

int LCA(int x, int y)
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

int dfs(int cur, int father)
{
    sum[cur] = diff[cur];
    for(int i = head[cur]; i; i = edge[i].next)
        if(edge[i].to != father)
            sum[cur] += dfs(edge[i].to, cur);
    return sum[cur];
}

int main()
{
    //file();
    scanf("%d%d", &n, &k);
    FOR(i, 2, n) lg[i] = lg[i / 2] + 1;
    FOR(i, 1, n - 1)
    {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        ADD(u, v);
        ADD(v, u);
    }
    build(1, 0);
    while(k--)
    {
        int s, t;
        scanf("%d%d", &s, &t);
        int lca = LCA(s, t);
        ++diff[s], ++diff[t];
        --diff[lca], --diff[fa[lca][0]];
    }
    dfs(1, 0);
    FOR(i, 1, n)
        ans = std::max(ans, sum[i]);
    printf("%d\n", ans);
    return 0;
}