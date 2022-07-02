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

const LL maxN = 1e5 + 5;
LL n, m;
char *s;
enum TYPE {RANDOM, UP, DOWN};

struct node
{
    LL cnt[26];
    TYPE lazy;
    node() {lazy = RANDOM; CLEAR(cnt);}
    ~node() = default;
};

class TREE
{
private:
    node tree[maxN << 2];

    void modify(node &t, LL len, LL pos, TYPE op)
    {
        CLEAR(tree[pos].cnt);
        if(op == UP)
        {
            for(LL i = 0, num = 0; i <= 25 && num < len; ++i)
            {
                if(num + t.cnt[i] <= len)
                    tree[pos].cnt[i] = t.cnt[i], num += t.cnt[i], t.cnt[i] = 0;
                else
                    tree[pos].cnt[i] = len - num, t.cnt[i] -= len - num, num = len;
            }
        }
        if(op == DOWN)
        {
            for(LL i = 25, num = 0; i >= 0 && num < len; --i)
            {
                if(num + t.cnt[i] <= len)
                    tree[pos].cnt[i] = t.cnt[i], num += t.cnt[i], t.cnt[i] = 0;
                else
                    tree[pos].cnt[i] = len - num, t.cnt[i] -= len - num, num = len;
            }
        }
    }
    void pushup(LL pos)
    {
        FOR(i, 0, 25)
            tree[pos].cnt[i] = tree[lc].cnt[i] + tree[rc].cnt[i];
    }
    void pushdown(LL l, LL r, LL pos)
    {
        if(tree[pos].lazy != RANDOM)
        {
            node temp = tree[pos];
            modify(temp, mid - l + 1, lc, tree[pos].lazy);
            modify(temp, r - mid, rc, tree[pos].lazy);
            tree[lc].lazy = tree[rc].lazy = tree[pos].lazy;
            tree[pos].lazy = RANDOM;
        }
    }
    void build(LL l, LL r, LL pos)
    {
        if(l == r)
        {
            tree[pos].cnt[s[l] - 'a'] = 1;
            tree[pos].lazy = RANDOM;
            return;
        }
        build(l, mid, lc);
        build(mid + 1, r, rc);
        pushup(pos);
    }

public:
    TREE(LL num) {build(1, num, 1);}
    ~TREE() = default;

    node query(LL x, LL y, LL l, LL r, LL pos)
    {
        if(x <= l && r <= y) return tree[pos];
        pushdown(l, r, pos);
        if(y <= mid) return query(x, y, l, mid, lc);
        if(x > mid) return query(x, y, mid + 1, r, rc);
        node retL, retR, ret;
        retL = query(x, y, l, mid, lc);
        retR = query(x, y, mid + 1, r, rc);
        FOR(i, 0, 25) ret.cnt[i] = retL.cnt[i] + retR.cnt[i];
        return ret;
    }
    void update(node &t, LL x, LL y, TYPE op, LL l, LL r, LL pos)
    {
        if(x <= l && r <= y)
        {
            modify(t, r - l + 1, pos, op);
            tree[pos].lazy = op;
            return;
        }
        pushdown(l, r, pos);
        if(x <= mid) update(t, x, y, op, l, mid, lc);
        if(y > mid) update(t, x, y, op, mid + 1, r, rc);
        pushup(pos);
    }
    void print(LL l, LL r, LL pos)
    {
        if(l == r)
        {
            FOR(i, 0, 25)
                if(tree[pos].cnt[i]) {putchar('a' + (int)i); break;}
            return;
        }
        pushdown(l, r, pos);
        print(l, mid, lc);
        print(mid + 1, r, rc);
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
    s = new char[n + 1];
    scanf("%s", s + 1);
    TREE *t = new TREE(n);
    while(m--)
    {
        LL l, r, x;
        scanf("%lld%lld%lld", &l, &r, &x);
        node q = t->query(l, r, 1, n, 1);
        t->update(q, l, r, (x == 1 ? UP : DOWN), 1, n, 1);
    }
    t->print(1, n, 1);
    delete[] s;
    return 0;
}