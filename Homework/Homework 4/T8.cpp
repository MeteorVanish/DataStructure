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

const LL maxN = 4e5 + 5;
const LL maxQ = 1e5 + 5;
LL n, q, tot;
LL w[maxN], cnt[maxN];
bool flag[maxN];

struct Query
{
    LL op, w, k;
} Q[maxQ];

class SegmentTree
{
#define lc (pos << 1)
#define rc (pos << 1 | 1)
#define mid (l + ((r - l) >> 1))
#define num(x) tree[x].num
#define sum(x) tree[x].sum

private:
    struct treeNode
    {
        LL num, sum;
        treeNode operator+(const treeNode &rhs) const
        { return {num + rhs.num, sum + rhs.sum}; }
    } tree[maxN << 2];

    struct recordNode
    {
        LL id;
        treeNode data;
    } rec[maxN << 2];
    LL top;

    void pushup(LL pos) { tree[pos] = tree[lc] + tree[rc]; }

    void Record(LL pos)
    {
        ++top;
        rec[top].id = pos, rec[top].data = tree[pos];
    }

    void Build(LL l, LL r, LL pos)
    {
        if(l == r)
        {
            num(pos) = cnt[l];
            sum(pos) = w[l] * cnt[l];
            return;
        }
        Build(l, mid, lc);
        Build(mid + 1, r, rc);
        pushup(pos);
    }

    LL Find(LL W, LL l, LL r, LL pos)
    {
        if(!num(pos) || w[r] < W) return 0;
        if(l == r) return w[l];
        LL ret = Find(W, l, mid, lc);
        if(ret) return ret;
        return Find(W, mid + 1, r, rc);
    }

    LL Increse(LL &W, LL target, LL l, LL r, LL pos)
    {
        if(!num(pos) || w[l] >= W) return 0;
        if(w[r] < W && W + sum(pos) <= target)
        {
            Record(pos);
            W += sum(pos);
            LL ret = num(pos);
            sum(pos) = num(pos) = 0;
            return ret;
        }
        if(l == r && W + sum(pos) > target)
        {
            Record(pos);
            LL ret = (target - W + w[l] - 1) / w[l];
            num(pos) -= ret, sum(pos) -= ret * w[l];
            W += ret * w[l];
            return ret;
        }
        LL ret = 0;
        ret += Increse(W, target, mid + 1, r, rc);
        if(target - W > 0) ret += Increse(W, target, l, mid, lc);
        Record(pos);
        pushup(pos);
        return ret;
    }

    void Undo()
    {
        ROF(i, 1, top)
            tree[rec[i].id] = rec[i].data;
        top = 0;
    }

public:
    SegmentTree(LL num) :top(0)
    {
        CLEAR(tree), CLEAR(rec);
        Build(1, num, 1);
    }
    ~SegmentTree() = default;

    LL Solve(LL W, LL K)
    {
        if(W >= K) return 0;
        LL ans = 0;
        while(W < K)
        {
            LL nextW = Find(W, 1, tot, 1);
            LL target = nextW ? min(nextW + 1, K) : K;
            ans += Increse(W, target, 1, tot, 1);
            if(W < target) break;
        }
        Undo();
        return W >= K ? ans : -1;
    }

    void Insert(LL W, LL l, LL r, LL pos)
    {
        if(l == r)
        {
            ++num(pos), sum(pos) += W;
            return;
        }
        if(W <= w[mid]) Insert(W, l, mid, lc);
        else Insert(W, mid + 1, r, rc);
        pushup(pos);
    }

    void Delete(LL W, LL l, LL r, LL pos)
    {
        if(l == r)
        {
            --num(pos), sum(pos) -= W;
            return;
        }
        if(W <= w[mid]) Delete(W, l, mid, lc);
        else Delete(W, mid + 1, r, rc);
        pushup(pos);
    }

#undef lc
#undef rc
#undef mid
#undef num
#undef sum
};

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline LL read()
{
    LL ret = 0, f = 1;
    char c = getchar();
    while(!isdigit(c))
    {
        if(c == '-') f = -1;
        c = getchar();
    }
    while(isdigit(c))
        ret = (ret << 3) + (ret << 1) + c - '0', c = getchar();
    return f * ret;
}

void mySort(LL arr[], LL l, LL r)
{
    LL mid = arr[l + ((r - l) >> 1)];
    LL i = l, j = r;
    while(i <= j)
    {
        while(arr[i] < mid) ++i;
        while(arr[j] > mid) --j;
        if(i <= j)
        {
            swap(arr[i], arr[j]);
            swap(flag[i], flag[j]);
            ++i, --j;
        }
    }
    if(l < j) mySort(arr, l, j);
    if(i < r) mySort(arr, i, r);
}

void Unique(LL arr[], LL l, LL r)
{
    LL pos = l;
    cnt[l] = flag[l];
    FOR(i, l + 1, r)
    {
        if(arr[i] != arr[pos])
            arr[++pos] = arr[i], cnt[pos] = flag[i];
        else cnt[pos] += flag[i], --tot;
    }
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read();
    FOR(i, 1, n) w[++tot] = read();
    memset(flag, true, (n + 1) * sizeof(bool));
    q = read();
    FOR(i, 1, q)
    {
        Q[i].op = read();
        Q[i].w = read();
        if(Q[i].op == 1) Q[i].k = read();
        if(Q[i].op == 2) w[++tot] = Q[i].w;
    }
    mySort(w, 1, tot);
    Unique(w, 1, tot);
    //FOR(i, 1, tot) DEBUG(w[i]), DEBUG(cnt[i]);
    auto *tree = new SegmentTree(tot);
    FOR(i, 1, q)
    {
        switch(Q[i].op)
        {
            case 1:
                printf("%lld\n", tree->Solve(Q[i].w, Q[i].k)); break;
            case 2:
                tree->Insert(Q[i].w, 1, tot, 1); break;
            case 3:
                tree->Delete(Q[i].w, 1, tot, 1); break;
        }
    }
    delete tree;
    return 0;
}