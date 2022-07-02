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

const int maxN = 505;
const int maxM = 1e6 + 5;
int s, n;
int cnt, Head[maxN], fa[maxN];
struct Input { int x, y; } point[maxN];
struct Edge
{
    int from, to, next;
    double val;
    bool operator<(const Edge &rhs) const { return val < rhs.val; }
} edge[maxM], temp[maxM];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline int read()
{
    int ret = 0, f = 1; char c = getchar();
    while(!(c >= '0' && c <= '9')) { if(c == '-') f = -1; c = getchar(); }
    while(c >= '0' && c <= '9') ret = (ret << 3) + (ret << 1) + c - '0', c = getchar();
    return f * ret;
}

double Calc(int i, int j)
{ return sqrt((point[i].x - point[j].x) * (point[i].x - point[j].x) + (point[i].y - point[j].y) * (point[i].y - point[j].y)); }

void add(int x, int y, double z)
{
    ++cnt;
    edge[cnt].from = x, edge[cnt].to = y, edge[cnt].val = z;
    edge[cnt].next = Head[x], Head[x] = cnt;
}

void MergeSort(Edge arr[], Edge tmp[], int l, int r)
{
    if(l >= r) return;
    int mid = (l + r) >> 1;
    MergeSort(arr, tmp, l, mid);
    MergeSort(arr, tmp, mid + 1, r);
    int p = l, q = mid + 1;
    FOR(i, l, r)
    {
        if(p > mid || q <= r && arr[q] < arr[p])
            tmp[i] = arr[q++];
        else
            tmp[i] = arr[p++];
    }
    FOR(i, l, r) arr[i] = tmp[i];
}

int Find(int x) { return x == fa[x] ? x : fa[x] = Find(fa[x]); }

void Union(int x, int y) { fa[Find(x)] = Find(y); }

bool Check(int x, int y) { return Find(x) == Find(y); }

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    s = read(), n = read();
    if(s == n)
    {
        printf("0\n");
        return 0;
    }
    FOR(i, 1, n) point[i].x = read(), point[i].y = read(), fa[i] = i;
    FOR(i, 1, n) FOR(j, i + 1, n) add(i, j, Calc(i, j));
    MergeSort(edge, temp, 1, cnt);
    int num = 0;
    double ans = 0;
    FOR(i, 1, cnt)
    {
        int u = edge[i].from, v = edge[i].to;
        if(!Check(u, v)) Union(u, v), ++num;
        if(num == n - s)
        {
            ans = edge[i].val;
            break;
        }
    }
    printf("%.2lf\n", ans);
    return 0;
}