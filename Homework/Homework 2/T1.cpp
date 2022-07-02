#include <iostream>

using namespace std;
#define forUP(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
#define forDOWN(alpha, start, end) for(int alpha = start; alpha >= end; --alpha)
const int maxN = 1000005;
int n, h[maxN], c[maxN], stack[maxN], num[maxN][2], top = 0, maxIndex, maxAns = -1;

int main()
{
    scanf("%d", &n);
    forUP(i,1,n) scanf("%d", &h[i]);
    forUP(i,1,n) scanf("%d", &c[i]);
    forUP(i,1,n) num[i][0] = num[i][1] = 0;
    forUP(i,1,n)
    {
        while(top && h[i] >= h[stack[top]])
        {
            num[i][0] += num[stack[top]][0] + 1;
            --top;
        }
        stack[++top] = i;
    }
    top = 0;
    forDOWN(i,n,1)
    {
        while(top && h[i] >= h[stack[top]])
        {
            num[i][1] += num[stack[top]][1] + 1;
            --top;
        }
        stack[++top] = i;
    }
    forUP(i,1,n)
    {
        int tempAns = c[i] * (num[i][0] + num[i][1] + 1);
        if(tempAns > maxAns)
        {
            maxIndex = i - 1;
            maxAns = tempAns;
        }
    }
    printf("%d %d\n", maxIndex, maxAns);
    return 0;
}