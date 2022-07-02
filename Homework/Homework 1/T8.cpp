#include <iostream>
#include <cmath>
using namespace std;

inline int log2(const int x) {return int(log(x)/log(2));}
int RMQmin(int left, int right, int **t, int *sum)
{
    if(left == right) return -1;
    int len = log2(right-left);
    return min(t[left][len], sum[right-(1<<len)] - sum[left-1] + t[right-(1<<len)+1][len]);
}
int RMQmax(int left, int right, int **t, int *sum)
{
    if(left == right) return -1;
    int len = log2(right-left);
    return max(t[left][len], sum[right-(1<<len)] - sum[left-1] + t[right-(1<<len)+1][len]);
}

int main()
{
    int n, m;
    scanf("%d%d", &n, &m);
    int *a = new int[n+1], *b = new int[n+1], *d = new int[n+1], *sum = new int[n+1];
    int **tmax = new int*[n+1], **tmin = new int*[n+1];
    for(int i = 1; i <= n; ++i)
    {
        tmax[i] = new int[log2(n)+1];
        tmin[i] = new int[log2(n)+1];
    }    
    for(int i = 1; i <= n; ++i) scanf("%d", &a[i]);
    for(int i = 1; i <= n; ++i) scanf("%d", &b[i]);
    sum[0] = 0;
    for(int i = 1; i <= n; ++i)
    {
        d[i] = b[i]-a[i];
        sum[i] = sum[i-1]+d[i];
        tmax[i][0] = d[i];
        tmin[i][0] = d[i];
    }
    for(int j = 1; j <= log2(n); ++j)
        for(int i = 1; i+(1<<j)-1 <= n; ++i)
        {
            tmax[i][j] = max(tmax[i][j-1], sum[i+(1<<j-1)-1] - sum[i-1] + tmax[i+(1<<j-1)][j-1]);
            tmin[i][j] = min(tmin[i][j-1], sum[i+(1<<j-1)-1] - sum[i-1] + tmin[i+(1<<j-1)][j-1]);
        }
    int l, r;   
    while(m--)
    {
        scanf("%d%d", &l, &r);
        if(sum[r]-sum[l-1]) printf("-1\n");
        else
        {
            if(RMQmin(l, r, tmin, sum) < 0) printf("-1\n");
            else printf("%d\n", RMQmax(l, r, tmax, sum));
        }
    }
    return 0;
}