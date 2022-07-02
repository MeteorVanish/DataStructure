#include <iostream>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 1000005;
int n, m, trie[maxN][27], num, node[150005], depth[maxN], lg[maxN], fa[maxN][20];

inline void file()
{
    string op = "1";
    string INPUT = "Input " + op + ".txt", OUTPUT = "Output " + op + ".txt";
    freopen(INPUT.c_str(), "r", stdin);
    freopen(OUTPUT.c_str(), "w", stdout);
}

int ADD(const string &s)
{
    int len = s.size() - 1, prepos, pos = 0, id;
    for(int i = len; i >= 0; --i)
    {
        id = s[i] - 'a' + 1;
        if(!trie[pos][id]) trie[pos][id] = ++num;
        prepos = pos;
        pos = trie[pos][id];
        if(!fa[pos][0])
        {
            depth[pos] = depth[prepos] + 1;
            fa[pos][0] = prepos;
            FOR(j, 1, lg[depth[pos]])
                fa[pos][j] = fa[fa[pos][j - 1]][j - 1];
        }
    }
    return pos;
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

int main()
{
    //file();
    scanf("%d%d", &n, &m);
    FOR(i, 2, 10000) lg[i] = lg[i >> 1] + 1;
    string s;
    FOR(i, 1, n)
    {
        cin >> s;
        node[i] = ADD(s);
    }
    while(m--)
    {
        int op;
        scanf("%d", &op);
        if(op == 1)
        {
            string s;
            cin >> s;
            node[++n] = ADD(s);
        }
        else
        {
            int t, arr[11];
            scanf("%d", &t);
            FOR(i, 1, t) scanf("%d", arr + i);
            int lca = LCA(node[arr[1]], node[arr[2]]);
            FOR(i, 3, t)
                lca = LCA(lca, node[arr[i]]);
            printf("%d\n", depth[lca]);
        }
    }
    return 0;
}