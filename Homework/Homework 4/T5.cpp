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

const int maxN = 105;
const int maxM = 3e4 + 5;
const int MOD = 19260817;
const int prime[maxN] =
        {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
         73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
         179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
         283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
         419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
         547, 557, 563, 569, 571};
int n, m, edgecnt, hashcnt, treehash;
struct node
{
    int to, next;
}edge[maxN << 1];
int head[maxN], Size[maxN], Hash[maxN], hashSet[maxM], num[maxM];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

void add(int x, int y)
{
    ++edgecnt;
    edge[edgecnt].to = y;
    edge[edgecnt].next = head[x];
    head[x] = edgecnt;
}

int gethash(int pos, int father)
{
    Hash[pos] = Size[pos] = 1;
    for(int i = head[pos]; i; i = edge[i].next)
        if(edge[i].to != father)
        {
            gethash(edge[i].to, pos);
            Size[pos] += Size[edge[i].to];
            Hash[pos] += (Hash[edge[i].to] * prime[Size[edge[i].to]]) % MOD;
            Hash[pos] %= MOD;
        }
    return Hash[pos] % MOD;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    scanf("%d%d", &n, &m);
    while(m--)
    {
        CLEAR(edge), CLEAR(head), CLEAR(Hash), CLEAR(Size);
        edgecnt = 0;
        int root, x, y;
        scanf("%d", &root);
        FOR(i, 1, n - 1)
        {
            scanf("%d%d", &x, &y);
            add(x, y), add(y, x);
        }
        treehash = gethash(root, -1);
        bool flag = false;
        FOR(i, 1, hashcnt)
            if(treehash == hashSet[i])
            {
                flag = true;
                num[i]++;
                break;
            }
        if(!flag) hashSet[++hashcnt] = treehash, num[hashcnt] = 1;
    }
    int ans = 0;
    FOR(i, 1, hashcnt)
        ans = max(ans, num[i]);
    printf("%d\n", ans);
    return 0;
}