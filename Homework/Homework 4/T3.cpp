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

const LL maxN = 2e5 + 5;
LL n, c, arr[maxN], ans;

class HEAP
{
private:
    LL num, a[maxN];

    void AdjustUP(LL pos)
    {
        for(LL i = pos, j = pos >> 1; j; i = j, j >>= 1)
        {
            if(a[i] < a[j]) swap(a[i], a[j]);
            else break;
        }
    }
    void AdjustDOWN(LL pos)
    {
        LL i = pos, j = pos << 1;
        while(j <= num)
        {
            if(j + 1 <= num && a[j + 1] < a[j]) ++j;
            if(a[i] > a[j]) swap(a[i], a[j]);
            i = j, j <<= 1;
        }
    }

public:
    HEAP() :num(0) {CLEAR(a);}
    void push(LL x)
    {
        a[++num] = x;
        AdjustUP(num);
    }
    LL pop()
    {
        LL ret = a[1];
        a[1] = a[num--];
        AdjustDOWN(1);
        return ret;
    }
};

void file(int op)
{
    string I = "Input " + to_string(op) + ".txt", O = "Output " + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

void sort()
{
    HEAP h;
    FOR(i, 1, n) h.push(arr[i]);
    FOR(i, 1, n)
        arr[i] = h.pop();
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    scanf("%lld%lld", &n, &c);
    FOR(i, 1, n) scanf("%lld", arr + i);
    sort();
    LL pos = 1, cnt1, cnt2;
    FOR(i, 1, n - 1)
    {
        while(arr[pos] - arr[i] < c)
        {
            if(pos == n) break;
            ++pos;
        }
        if(arr[pos] - arr[i] != c) continue;
        cnt1 = cnt2 = 1;
        while(arr[i + 1] == arr[i]) ++i, ++cnt1;
        while(arr[pos + 1] == arr[pos]) ++pos, ++cnt2;
        ans += cnt1 * cnt2;
    }
    printf("%lld\n", ans);
    return 0;
}