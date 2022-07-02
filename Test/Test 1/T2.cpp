#include <iostream>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 105;
const int maxM = 100005;
int arr[maxN][maxM], p[maxN], sum[maxN][maxM], Queue[maxM], maxR[maxN][maxM], maxALL[maxM];

int main()
{
    int n, m, k, lowAns, highAns, ans = 0, head, tail;
    scanf ("%d%d%d", &n, &m, &k);
    FOR(i, 1, n) scanf ("%d", p + i);
    FOR(i, 1, n) FOR(j, 1, m) scanf("%d", &arr[i][j]);
    FOR(i, 1, n)
    {
        sum[i][0] = 0;
        FOR(j, 1, m)
            sum[i][j] = sum[i][j-1] + arr[i][j];
    }
    FOR(i, 1, n)
    {
        head = 1;
        tail = 0;
        FOR(j, 1, m)
        {
            while(head <= tail && arr[i][Queue[tail]] <= arr[i][j]) --tail;
            Queue[++tail] = j;
            if(j - Queue[head] == k) ++head;
            maxR[i][j] = arr[i][Queue[head]];
        }
    }
    FOR(j, k, m)
    {
        maxALL[j] = 0;
        FOR(i, 1, n)
            maxALL[j] =  max(maxALL[j], maxR[i][j]);
    }
    FOR(j, k, m)
    {
        lowAns = highAns = 0;
        FOR(i, 1, n)
            lowAns += p[i] * (sum[i][j] - sum[i][j - k]);
        FOR(i, 1, n)
            highAns += p[i];
        highAns *= maxALL[j] * k;
        ans = max(ans, highAns - lowAns) ;
    }
    printf("%d\n", ans);
    return 0;
}