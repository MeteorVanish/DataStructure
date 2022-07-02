#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(LL i = start; i <= end; ++i)
#define ROF(i, start, end) for(LL i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)
#define lc (pos << 1)
#define rc (pos << 1 | 1)
#define mid (l + ((r - l) >> 1))
using LL = long long;

const int maxN = 1e5 + 5;
int n, arr[maxN], tree[maxN], ans[maxN];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline int lowbit(int x) { return x & -x;}

void build(int num)
{
    FOR(i, 1, n)
        if(i + lowbit(i) <= n)
            tree[i + lowbit(i)] += tree[i];
}

void update(int pos)
{
    while(pos <= n)
    {
        --tree[pos];
        pos += lowbit(pos);
    }
}

int query(int pos)
{
    int sum = 0;
    while(pos)
    {
        sum += tree[pos];
        pos -= lowbit(pos);
    }
    return sum;
}

int find(int num)
{
    int l = 1, r = n;
    while(l < r)
    {
        int now = query(mid - 1);
        if(l == r - 1)
        {
            if(query(r - 1) == num) return r;
            else return l;
        }
        if(now <= num) l = mid;
        else r = mid - 1;
    }
    return l;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    scanf("%d", &n);
    FOR(i, 1, n) tree[i] = 1;
    build(n);
    FOR(i, 1, n) scanf("%d", arr + i);
    ROF(i, 1, n)
    {
        ans[i] = find(arr[i]);
        update(ans[i]);
    }
    FOR(i, 1, n) printf("%d%c", ans[i], " \n"[i == n]);
    return 0;
}