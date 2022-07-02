#include <iostream>
#include <cstring>
using namespace std;

#define FOR(alpha, start, end) for(int (alpha) = (start); (alpha) <= (end); ++(alpha))
const int maxN = 1e5 + 5;
struct MeteorVanish
{
    int to, next;
}edge[maxN << 1];
int n, m, k, cnt;
int head[maxN], depth[maxN], lg[maxN], fa[maxN][20];
int material[maxN], flag[maxN][20];

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
    flag[cur][0] |= material[cur] | material[father];
    FOR(i, 1, lg[depth[cur]])
    {
        fa[cur][i] = fa[fa[cur][i - 1]][i - 1];
        flag[cur][i] |= flag[cur][i - 1] | flag[ fa[cur][i - 1] ][i - 1];
    }
    for(int i = head[cur]; i; i = edge[i].next)
        if(edge[i].to != father) dfs(edge[i].to, cur);
}

bool LCA(int x, int y, int query)
{
    if(!query) return true;
    int judge = 0;
    if(depth[x] > depth[y]) std::swap(x, y);
    judge |= material[x] | material[y];
    while(depth[x] < depth[y])
    {
        judge |= flag[y][ lg[depth[y] - depth[x]] ];
        y = fa[y][ lg[depth[y] - depth[x]] ];
    }
    if(x != y)
    {
        for(int i = lg[depth[x]]; i >= 0; --i)
            if(fa[x][i] != fa[y][i])
            {
                judge |= flag[x][i] | flag[y][i];
                x = fa[x][i], y = fa[y][i];
            }
        judge |= flag[x][0] | flag[y][0];
    }
    FOR(i, 0, 31)
        if(query & (1 << i) && !(judge & (1 << i))) return false;
    return true;
}

int main()
{
    //file();
    scanf("%d%d", &n, &m);
    FOR(i, 2, n) lg[i] = lg[i >> 1] + 1;
    FOR(i, 1, n)
    {
        int t, index;
        scanf("%d", &t);
        FOR(j, 0, t - 1)
        {
            scanf("%d", &index);
            material[i] |= 1 << index;
        }
    }
    FOR(i, 1, n - 1)
    {
        int u, v;
        scanf("%d%d", &u, &v);
        ADD(u, v);
        ADD(v, u);
    }
    dfs(1, 0);
    scanf("%d", &k);
    while(k--)
    {
        int A, B, t, q = 0, temp;
        scanf("%d%d%d", &A, &B, &t);
        while(t--)
        {
            scanf("%d", &temp);
            q ^= 1 << temp;
        }
        bool ans = LCA(A, B, q);
        if(ans) printf("YES\n");
        else printf("NO\n");
    }
    return 0;
}