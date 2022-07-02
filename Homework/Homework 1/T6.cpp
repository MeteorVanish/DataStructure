#include <iostream>
#include <cmath>
using namespace std;
#define maxN 1000005
int n, m, s[maxN], LeftZeroNum[maxN], RightOneNum[maxN], maxlen[maxN][20], LastZeroPos[maxN], LastOnePos[maxN], TempZeroPos = 0, TempOnePos = 0;

inline int log2(int x) {return int(log(x)/log(2));}
inline int max(int a, int b) {return a>b?a:b;}
int RMQ(int left, int right)
{
    int t = log2(right-left+1);
    return max(maxlen[left][t], maxlen[right-(1<<t)+1][t]);
}

int main()
{
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; ++i)
    {
        scanf("%d", &s[i]);
        LastZeroPos[i] = TempZeroPos;
        LastOnePos[i] = TempOnePos;
        if(s[i]) TempOnePos = i;
        else TempZeroPos = i;
    }
    int cnt1 = 0;
    LeftZeroNum[0] = 0;
    for(int i = 1; i <= n; ++i)
        if(!s[i]) LeftZeroNum[i] = ++cnt1;
        else LeftZeroNum[i] = LeftZeroNum[i-1];
    int cnt2 = 0;
    RightOneNum[n+1] = 0;
    for(int i = n; i >= 1; --i)
        if(s[i]) RightOneNum[i] = ++cnt2;
        else RightOneNum[i] = RightOneNum[i+1];
    for(int i = 1; i <= n; ++i)
        maxlen[i][0] = LeftZeroNum[i] + RightOneNum[i];
    for(int j = 1; j <= log2(n); ++j)
        for(int i = 1; i+(1<<j)-1 <= n; ++i)
            maxlen[i][j] = max(maxlen[i][j-1], maxlen[i+(1<<j-1)][j-1]);
    while(m--)
    {
        int opt, l, r;
        scanf("%d%d%d", &opt, &l, &r);
        if(opt == 1)
        {
            int ans1 = RMQ(l,r)-LeftZeroNum[l-1]-RightOneNum[r+1];
            printf("%d\n", ans1);
        }
        else
        {
            int ans2;
            if(l == r) ans2 = 1;
            else
                {
                if((s[r] && LastZeroPos[r] >= l) || (!s[r] && LastZeroPos[LastOnePos[r]] >= l)) ans2 = 2;
                else ans2 = 1;
            }
            printf("%d\n", ans2);
        }
    }
    return 0;
}