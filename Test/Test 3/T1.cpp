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
LL n, arr[maxN];

class HEAP
{
private:
    LL heap[maxN], cnt;

    void AdjustUP(LL pos)
    {
        for(LL i = pos, j = pos >> 1; j; i = j, j >>= 1)
            if(heap[i] > heap[j]) swap(heap[i], heap[j]);
    }
    void AdjustDOWN(LL pos)
    {
        LL i = pos, j = pos << 1;
        while(j <= cnt)
        {
            if(j + 1 <= cnt && heap[j] < heap[j + 1]) ++j;
            if(heap[i] < heap[j]) swap(heap[i], heap[j]);
            i = j, j <<= 1;
        }
    }

public:
    HEAP() { cnt = 0; CLEAR(heap);}
    ~HEAP() = default;

    void push(LL num)
    {
        heap[++cnt] = num;
        AdjustUP(cnt);
    }
    LL pop()
    {
        LL ret = heap[1];
        heap[1] = heap[cnt--];
        AdjustDOWN(1);
        return ret;
    }
    void print()
    {
        FOR(i, 1, cnt) printf("%lld%c", heap[i], " \n"[i == cnt]);
    }
};

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    HEAP h;
    scanf("%lld", &n);
    FOR(i, 1, n)
    {
        scanf("%lld", arr + i);
        h.push(arr[i]);
    }
    h.print();
    ROF(i, 1, n) arr[i] = h.pop();
    FOR(i, 1, n) printf("%lld%c", arr[i], " \n"[i == n]);
    return 0;
}