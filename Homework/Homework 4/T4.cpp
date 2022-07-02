#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(LL i = start; i <= end; ++i)
#define ROF(i, start, end) for(LL i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)
using LL = long long;

const LL maxN = 1e5 + 5;
LL n, m;
double arr[maxN];

template <class T>
class TREE
{
private:
    T sum1[maxN << 2], sum2[maxN << 2], lazy[maxN << 2];

    void build(LL l, LL r, LL pos)
    {
        if(l == r)
        {
            sum1[pos] = arr[l];
            sum2[pos] = arr[l] * arr[l];
            return;
        }
        LL mid = l + (r - l >> 1);
        build(l, mid, pos << 1);
        build(mid + 1, r, pos << 1 | 1);
        pushup(pos);
    }
    void pushup(LL pos)
    {
        sum1[pos] = sum1[pos << 1] + sum1[pos << 1 | 1];
        sum2[pos] = sum2[pos << 1] + sum2[pos << 1 | 1];
        return;
    }
    void pushdown(LL l, LL r, LL pos)
    {
        if(lazy[pos])
        {
            LL mid = l + (r - l >> 1);
            sum2[pos << 1] += lazy[pos] * lazy[pos] * (mid - l + 1) + 2 * sum1[pos << 1] * lazy[pos];
            sum2[pos << 1 | 1] += lazy[pos] * lazy[pos] * (r - mid) + 2 * sum1[pos << 1 | 1] * lazy[pos];
            sum1[pos << 1] += lazy[pos] * (mid - l + 1);
            sum1[pos << 1 | 1] += lazy[pos] * (r - mid);
            lazy[pos << 1] += lazy[pos], lazy[pos << 1 | 1] += lazy[pos];
            lazy[pos] = 0;
        }
    }

public:
    TREE(LL n) {build(1, n, 1);}
    ~TREE() = default;

    void update(LL x, LL y, T num, LL l, LL r, LL pos)
    {
        if(x <= l && r <= y)
        {
            lazy[pos] += num;
            sum2[pos] += num * num * (r - l + 1) + 2 * sum1[pos] * num;
            sum1[pos] += num * (r - l + 1);
            return;
        }
        pushdown(l, r, pos);
        LL mid = l + (r - l >> 1);
        if(x <= mid) update(x, y, num, l, mid, pos << 1);
        if(y > mid) update(x, y, num, mid + 1, r, pos << 1 | 1);
        pushup(pos);
    }
    T query(LL x, LL y, LL l, LL r, LL pos, LL op) // op: 1->sum, 2->sum of squares
    {
        if(x <= l && r <= y)
        {
            if(op == 1) return sum1[pos];
            else return sum2[pos];
        }
        pushdown(l, r, pos);
        LL mid = l + (r - l >> 1);
        if(y <= mid) return query(x, y, l, mid, pos << 1, op);
        if(x > mid) return query(x, y, mid + 1, r, pos << 1 | 1, op);
        return query(x, y, l, mid, pos << 1, op) + query(x, y, mid + 1, r, pos << 1 | 1, op);
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
    FOR(i, 1, n) scanf("%lf", arr + i);
    TREE<double> *t = new TREE<double>(n);
    while(m--)
    {
        LL op, x, y, ans;
        double aver, sqsum;
        scanf("%lld%lld%lld", &op, &x, &y);
        switch(op)
        {
            case 1:
                double k;
                scanf("%lf", &k);
                t->update(x, y, k, 1, n, 1);
                break;
            case 2:
                aver = t->query(x, y, 1, n, 1, 1) / double(y - x + 1);
                ans = LL(aver * 100);
                printf("%lld\n", ans);
                break;
            case 3:
                aver = t->query(x, y, 1, n, 1, 1) / double(y - x + 1);
                sqsum = t->query(x, y, 1, n, 1, 2);
                ans = LL(100 * sqsum / (y - x + 1) - 100 * aver * aver);
                printf("%lld\n", ans);
                break;
        }
    }
    return 0;
}