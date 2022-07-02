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

const int maxN = 2e4 + 5;
const int maxM = 1e5 + 5;
int n, m, father[maxN], enemy[maxN], Rank[maxN], ans;

struct Node
{
    int a, b, c;
}Q[maxM], t[maxM];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

void Sort(Node arr[], Node temp[], int l, int r)
{
    if(l >= r) return;
    int mid = (l + r) >> 1;
    Sort(arr, temp, l, mid);
    Sort(arr, temp, mid + 1, r);
    int p = l, q = mid + 1;
    FOR(i, l, r)
    {
        if(p > mid || q <= r && arr[q].c > arr[p].c) temp[i] = arr[q], ++q;
        else temp[i] = arr[p], ++p;
    }
    FOR(i, l, r) arr[i] = temp[i];
}

int Find(int x) { return father[x] == x ? x : father[x] = Find(father[x]);}

void Union(int x, int y)
{
    int fx = Find(x), fy = Find(y);
    if(fx == fy) return;
    if(Rank[fx] < Rank[fy]) father[fx] = fy; else
    if(Rank[fx] > Rank[fy]) father[fy] = fx; else
    if(Rank[fx] == Rank[fy]) father[fx] = fy, ++Rank[fy];
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    scanf("%d%d", &n, &m);
    FOR(i, 1, n) father[i] = i;
    FOR(i, 1, m) scanf("%d%d%d", &Q[i].a, &Q[i].b, &Q[i].c);
    Sort(Q, t, 1, m);
    FOR(i, 1, m)
    {
        int fa = Find(Q[i].a), fb = Find(Q[i].b);
        if(fa == fb) { ans = Q[i].c; break;}
        if(!enemy[Q[i].a]) enemy[Q[i].a] = Q[i].b;
        else Union(enemy[Q[i].a], Q[i].b);
        if(!enemy[Q[i].b]) enemy[Q[i].b] = Q[i].a;
        else Union(enemy[Q[i].b], Q[i].a);
    }
    printf("%d\n", ans);
    return 0;
}