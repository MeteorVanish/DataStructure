#include <iostream>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 500005;
const int maxE = 500005 * 12;
const int maxT = (1 << 12) + 5;
struct MeteorVanish
{
    int to, next, val;
}edge[maxE << 1];
int n, m, edgeNum, head[maxN], val[maxN], depth[maxE], alterXOR;
bool flag[maxT << 1][2];

class Trie
{
private:
    int num;
    int trie[maxT][2];

public:
    Trie() :num(0) {};
    void add(int x)
    {
        int pos = 0, id;
        FOR(i, 1, 12)
        {
            id = x >> 12 - i & 1;
            if(!trie[pos][id]) trie[pos][id] = ++num;
            pos = trie[pos][id];
        }
    }
    int query(int x)
    {
        int pos = 0, id, sum = 0;
        FOR(i, 1, 12)
        {
            id = x >> 12 - i & 1;
            if(trie[pos][1 - id])
            {
                sum = (sum << 1) + 1 - id;
                pos = trie[pos][1 - id];
            }
            else
            {
                sum = (sum << 1) + id;
                pos = trie[pos][id];
            }
        }
        return sum ^ x;
    }
}odd, even;

inline void file()
{
    string op = "1";
    string INPUT = "Input " + op + ".txt", OUTPUT = "Output " + op + ".txt";
    freopen(INPUT.c_str(), "r", stdin);
    freopen(OUTPUT.c_str(), "w", stdout);
}

void edgeAdd(int x, int y, int w)
{
    ++edgeNum;
    edge[edgeNum].to = y;
    edge[edgeNum].val = w;
    edge[edgeNum].next = head[x];
    head[x] = edgeNum;
}

void build(int cur, int father, int sum, int value)
{
    if(cur != 1)
        depth[cur] = depth[father] + 1;
    val[cur] = sum ^ value;
    flag[val[cur]][depth[cur] & 1] = true;
    for(int i = head[cur]; i; i = edge[i].next)
        if(edge[i].to != father) build(edge[i].to, cur, val[cur], edge[i].val);
}

void init()
{
    FOR(i, 0, 1 << 12)
        FOR(j, i, 1 << 12)
        {
            if(flag[i][0] && flag[j][1] || flag[i][1] && flag[j][0]) odd.add(i ^ j);
            if(flag[i][0] && flag[j][0] || flag[i][1] && flag[j][1]) even.add(i ^ j);
        }
}

int main()
{
    //file();
    scanf("%d%d", &n, &m);
    FOR(i, 1, n - 1)
    {
        int x, y, w;
        scanf("%d%d%d", &x, &y, &w);
        edgeAdd(x, y, w);
        edgeAdd(y, x, w);
    }
    build(1, 1, 0, 0);
    init();
    while(m--)
    {
        int opt;
        scanf("%d", &opt);
        if(opt == 1)
        {
            int x, y;
            scanf("%d%d", &x, &y);
            int normalXOR = val[x] ^ val[y];
            if((depth[x] ^ depth[y]) & 1) normalXOR ^= alterXOR;
            printf("%d\n", std::max(odd.query(normalXOR ^ alterXOR), even.query(normalXOR)));
        }
        else
        {
            int v;
            scanf("%d", &v);
            alterXOR ^= v;
        }
    }
    return 0;
}