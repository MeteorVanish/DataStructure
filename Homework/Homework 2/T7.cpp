#include <iostream>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 1000005;
int arr[maxN], stackUP[maxN], stackDOWN[maxN];

int main()
{
    int n, topUP = 0, topDOWN = 0, ans = 0;
    scanf("%d", &n);
    FOR(i, 1, n)
    {
        scanf("%d", arr + i);
        while(topUP && arr[stackUP[topUP]] >= arr[i]) --topUP;
        while(topDOWN && arr[stackDOWN[topDOWN]] < arr[i]) --topDOWN;
        stackUP[++topUP] = stackDOWN[++topDOWN] = i;
        ans = max(ans, i - *lower_bound(stackUP + 1, stackUP + topUP, stackDOWN[topDOWN - 1]) + 1);
    }
    printf("%d\n", ans);
    return 0;
}