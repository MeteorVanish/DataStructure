#include <iostream>
using namespace std;
#define maxN 500005
int st[maxN][20];

inline int gcd(int a, int b) {return a%b == 0? b : gcd(b,a%b);}
inline int log2(int x)
{
    if(x == 0) return 0;
    int cnt = 0;
    while(x >>= 1) ++cnt;
    return cnt;
}
int RMQ(int l, int r)
{
    int t = log2(r-l);
    return gcd(st[l][t], st[r-(1<<t)+1][t]);
}
int Half(int l, int r, int gcdx)
{
    int left = l, right = r, mid = (left+right)>>1;
    while (left < right)
    {
        if(RMQ(l, mid) > gcdx) left = mid+1;
        else right = mid;
        mid = (left+right)>>1;
    }
    int ans1 = left;
    left = l, right = r, mid = ((left+right)>>1)+1;
    while(left+1 < right)
    {
        if(RMQ(l, mid) >= gcdx) left = mid;
        else right = mid-1;
        mid = (left+right)>>1;
    }
    int ans2;
    if(RMQ(l, right) == gcdx) ans2 = right;
    else ans2 = left;
    return ans2-ans1;
}

int main()
{
    int n, m, arr[maxN];
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; ++i)
    {
        scanf("%d", &arr[i]);
        st[i][0] = arr[i];
    }
    for(int j = 1; j <= log2(n); ++j)
        for(int i = 1; i+(1<<j)-1 <= n; ++i)
            st[i][j] = gcd(st[i][j-1], st[i+(1<<(j-1))][j-1]);
    while(m--)
    {
        int l, r;
        scanf("%d%d", &l, &r);
        int gcdAns = RMQ(l, r), cnt = Half(l, n, gcdAns);
        printf("%d %d\n", gcdAns, cnt);
    }
    return 0;
}