#include <iostream>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 100005;
const int maxM = 1005;

class Heap
{
private:
    int cnt;
    int arr[maxN + maxM];
    void AdjustUP(int pos)
    {
        for(int i = pos, j = pos >> 1; j; i = j, j >>= 1)
            if(arr[i] < arr[j]) swap(arr[i], arr[j]);
            else break;
    }
    void AdjustDOWN(int pos)
    {
        int i = pos, j;
        while(true)
        {
            if(i << 1 <= cnt)
            {
                if((i << 1) + 1 <= cnt && arr[i << 1] > arr[(i << 1) + 1]) j = (i << 1) + 1;
                else j = i << 1;
                if(arr[i] > arr[j]) swap(arr[i], arr[j]);
                else break;
                i = j;
            }
            else break;
        }
    }

public:
    Heap() {cnt = 0;}
    int size() {return cnt;}
    void push(int x)
    {
        arr[++cnt] = x;
        AdjustUP(cnt);
    }
    int pop()
    {
        int ret = arr[1];
        arr[1] = arr[cnt--];
        AdjustDOWN(1);
        return ret;
    }
};

int main()
{
    int n, m, k = 0, x;
    long long ans = 0;
    Heap heap;
    scanf ("%d%d", &n, &m);
    if((n - 1) % (m - 1)) //compute 0 number
    {
        x = (n - 1) / (m - 1) + 1;
        k = 1 + (m - 1) * x - n;
    }
    FOR(i, 1, n)
    {
        scanf ("%d", &x);
        heap.push(x);
    }
    while(k--) heap.push(0);
    while(heap.size() > 1)
    {
        int sum = 0;
        FOR(i, 1, m)
            sum += heap.pop();
        heap.push(sum);
        ans += sum;
    }
    printf("%lld\n", ans);
    return 0;
}