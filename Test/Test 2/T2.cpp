#include <iostream>
#include <cstring>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 1e5 + 5;
struct MeteorVanish
{
    int to, next;
}edge[maxN << 1];
int T, n, m, cnt, head[maxN], lg[maxN], depth[maxN], fa[maxN][25];

inline void file()
{
    string op = "1";
    string INPUT = "Input " + op + ".txt", OUTPUT = "Output " + op + ".txt";
    freopen(INPUT.c_str(), "r", stdin);
    freopen(OUTPUT.c_str(), "w", stdout);
}

void ADD(int x, int y)
{
    ++cnt;
    edge[cnt].to = y;
    edge[cnt].next = head[x];
    head[x] = cnt;
}

void dfs(int cur, int father)
{
    depth[cur] = depth[father] + 1;
    fa[cur][0] = father;
    FOR(i, 1, lg[depth[cur]])
        fa[cur][i] = fa[fa[cur][i - 1]][i - 1];
    for(int i = head[cur]; i; i = edge[i].next)
        if(edge[i].to != father) dfs(edge[i].to, cur);
}

int LCA(int x, int y)
{
    if(depth[x] > depth[y]) std::swap(x, y);
    while(depth[x] < depth[y])
        y = fa[y][lg[depth[y] - depth[x]]];
    if(x == y) return x;
    for(int i = lg[depth[x]]; i >= 0; --i)
        if(fa[x][i] != fa[y][i])
            x = fa[x][i], y = fa[y][i];
    return fa[x][0];
}

bool query(int x, int y, int LCA, int xydepth)
{
    int LCAdepth = depth[LCA];
    if(LCAdepth < xydepth) return false;
    while(depth[x] > LCAdepth)
        x = fa[x][lg[depth[x] - LCAdepth]];
    if(x == LCA) return true;
    while(depth[y] > LCAdepth)
        y = fa[y][lg[depth[y] - LCAdepth]];
    if(y == LCA) return true;
    return false;
}

int main()
{
    //file();
    scanf("%d", &T);
    FOR(i, 2, maxN - 1) lg[i] = lg[i >> 1] + 1;
    while(T--)
    {
        scanf("%d%d", &n, &m);
        FOR(i, 1, n - 1)
        {
            int x, y;
            scanf("%d%d", &x, &y);
            ADD(x, y);
            ADD(y, x);
        }
        dfs(1, 0);
        int x1, y1, x2, y2;
        while(m--)
        {
            scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
            int lca1 = LCA(x1, y1), lca2 = LCA(x2, y2);
            bool ans = false;
            ans |= query(x2, y2, lca1, depth[lca2]);
            ans |= query(x1, y1, lca2, depth[lca1]);
            if(ans) printf("YES\n");
            else printf("NO\n");
        }
        cnt = 0;
        memset(head, 0, sizeof(head));
        memset(edge, 0, sizeof(edge));
        memset(fa, 0, sizeof(fa));
    }
    return 0;
}