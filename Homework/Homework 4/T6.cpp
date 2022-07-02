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

const LL maxN = 5e4 + 5;
LL n, m;

class TREE
{
private:
    LL maxlen[maxN << 2], lazy[maxN << 2], llen[maxN << 2], rlen[maxN << 2];

    void build(LL l, LL r, LL pos)
    {
        if(l == r)
        {
            maxlen[pos] = llen[pos] = rlen[pos] = 1;
            lazy[pos] = 0;
            return;
        }
        build(l, mid, lc);
        build(mid + 1, r, rc);
        pushup(l, r, pos);
    }
    void pushup(LL l, LL r, LL pos)
    {
        llen[pos] = (maxlen[lc] == mid - l + 1 ? maxlen[lc] + llen[rc] : llen[lc]);
        rlen[pos] = (maxlen[rc] == r - mid ? maxlen[rc] + rlen[lc] : rlen[rc]);
        maxlen[pos] = max(rlen[lc] + llen[rc], max(maxlen[lc], maxlen[rc]));
    }
    void pushdown(LL l, LL r, LL pos)
    {
        if(l == r) return;
        if(lazy[pos] == 1)
        {
            lazy[lc] = lazy[rc] = 1;
            maxlen[lc] = llen[lc] = rlen[lc] = 0;
            maxlen[rc] = llen[rc] = rlen[rc] = 0;
        }
        else if(lazy[pos] == 2)
        {
            lazy[lc] = lazy[rc] = 2;
            maxlen[lc] = llen[lc] = rlen[lc] = mid - l + 1;
            maxlen[rc] = llen[rc] = rlen[rc] = r - mid;
        }
        lazy[pos] = 0;
    }

public:
    TREE(LL num = 0) { build(1, num, 1);}
    ~TREE() = default;

    LL query(LL d, LL l, LL r, LL pos)
    {
        if(maxlen[pos] < d) return 0;
        pushdown(l, r, pos);
        if(maxlen[lc] >= d) return query(d, l, mid, lc);
        else if(rlen[lc] + llen[rc] >= d) return mid - rlen[lc] + 1;
        else return query(d, mid + 1, r, rc);
    }
    void update(LL x, LL y, LL l, LL r, LL pos, LL op)
    {
        pushdown(l, r, pos);
        if(x <= l && r <= y)
        {
            if(op == 1)
                maxlen[pos] = llen[pos] = rlen[pos] = 0;
            else
                maxlen[pos] = llen[pos] = rlen[pos] = r - l + 1;
            lazy[pos] = op;
            return;
        }
        if(x <= mid) update(x, y, l, mid, lc, op);
        if(y > mid) update(x, y, mid + 1, r, rc, op);
        pushup(l, r, pos);
    }

};

void file(int op)
{
    string I = "Input " + to_string(op) + ".txt", O = "Output " + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    scanf("%lld%lld", &n, &m);
    TREE *t = new TREE(n);
    while(m--)
    {
        LL op, x, d;
        scanf("%lld", &op);
        if(op == 1)
        {
            scanf("%lld", &d);
            LL ans = t->query(d, 1, n, 1);
            if(ans) t->update(ans, ans + d - 1, 1, n, 1, 1);
            printf("%lld\n", ans);
        }
        else
        {
            scanf("%lld%lld", &x, &d);
            t->update(x, x + d - 1, 1, n, 1, 2);
        }
    }
    return 0;
}