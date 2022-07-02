#include <iostream>
using namespace std;

int log2(int x)
{
    int ans = 0;
    while(x >>= 1) ++ans;
    return ans;
}
int RMQ(int left, int right, int **st)
{
    int len = log2(right-left);
    return max(st[left][len], st[right-(1<<len)+1][len]);
}

int main()
{
    int n, m, l, r;
    scanf("%d%d", &n, &m);
    int *arr = new int[n+1];
    int **st = new int*[n+1];
    for(int i = 1; i <= n; ++i)
    {
        st[i] = new int[log2(n)+1];
        scanf("%d", &arr[i]);
        st[i][0] = arr[i];
    }
    for(int j = 1; j <= log2(n); ++j)
        for(int i = 1; i+(1<<j)-1 <= n; ++i)
            st[i][j] = max(st[i][j-1], st[i+(1<<j-1)][j-1]);
    while(m--)
    {
        scanf("%d%d", &l, &r);
        printf("%d\n", RMQ(l,r,st));
    }
    return 0;
}