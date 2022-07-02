#include <iostream>

using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 1005;
int n, m, a, maxHead, maxTail, minHead, minTail, ans = 0x7fffffff;
int arr[maxN][maxN], maxAns[maxN][maxN], minAns[maxN][maxN], maxT[maxN][maxN], minT[maxN][maxN], maxQueue[maxN], minQueue[maxN];

inline int min(int x, int y, int z)
{
    int temp = x < y ? x : y;
    return temp < z ? temp : z;
}

inline int max(int x, int y, int z)
{
    int temp = x > y ? x : y;
    return temp > z ? temp : z;
}

int main()
{
    scanf("%d%d%d", &n, &m, &a);
    if(!a || a == 1)
    {
        printf("0\n");
        return 0;
    }
    FOR(i, 1, n) FOR(j, 1, m) scanf("%d", &arr[i][j]);
    FOR(i, 1, n)
    {
        maxHead = minHead = 0;
        maxTail = minTail = 1;
        for(int j = m; j >= 1; --j)
        {
            while(maxHead >= maxTail && arr[i][j] >= arr[i][maxQueue[maxHead]]) --maxHead;
            while(minHead >= minTail && arr[i][j] <= arr[i][minQueue[minHead]]) --minHead;
            maxQueue[++maxHead] = j;
            minQueue[++minHead] = j;
            if(maxQueue[maxTail] - j == a) ++maxTail;
            if(minQueue[minTail] - j == a) ++minTail;
            if(j <= m - a + 1)
            {
                maxT[i][j] = arr[i][maxQueue[maxTail]];
                minT[i][j] = arr[i][minQueue[minTail]];
            }
        }
    }
    FOR(j, 1, m - a + 1)
    {
        maxHead = minHead = 0;
        maxTail = minTail = 1;
        for(int i = n; i >= 1; --i)
        {
            while(maxHead >= maxTail && maxT[i][j] >= maxT[maxQueue[maxHead]][j]) --maxHead;
            while(minHead >= minTail && minT[i][j] <= minT[minQueue[minHead]][j]) --minHead;
            maxQueue[++maxHead] = i;
            minQueue[++minHead] = i;
            if(maxQueue[maxTail] - i == a) ++maxTail;
            if(minQueue[minTail] - i == a) ++minTail;
            if(i <= n - a + 1)
            {
                maxAns[i][j] = maxT[maxQueue[maxTail]][j];
                minAns[i][j] = minT[minQueue[minTail]][j];
            }
        }
    }
    FOR(i, 1, n - a + 1)
    {
        FOR(j, 1, m - a + 1)
        {
            int temp = maxAns[i][j] - minAns[i][j];
            ans = temp < ans ? temp : ans;
        }
    }
    printf("%d\n", ans);
    return 0;
}