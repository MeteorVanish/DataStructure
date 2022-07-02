#include <iostream>
using namespace std;

#define FOR(alpha, start, end) for(long long alpha = start; alpha <= end; ++alpha)
const long long maxN = 2005;
long long arr[maxN][maxN], sum[maxN][maxN];

inline long long sigma(long long start, long long end) {return (end * (end + 1) >> 1) - (start * (start + 1) >> 1);}
inline long long formula(long long x) {return x * (x + 1) * (x + 2) / 6;}
long long calc(long long h[], long long w[], long long &i, long long base)
{
    long long ret = 0;
    long long tempw = 0;
    while(1)
    {
        tempw += w[i];
        if(h[i - 1] <= base)
        {
            ret += sigma(base, h[i]) * formula(tempw);
            break;
        }
        else ret += sigma(h[i - 1], h[i]) * formula(tempw);
        --i;
    }
    if(h[i - 1] < base)
    {
        h[i] = base;
        w[i] = tempw + 1;
    }
    else w[--i] += tempw + 1;
    return ret;
}

int main()
{
    long long n, m;
    long long ans = 0;
    scanf("%lld%lld", &n, &m);
    FOR(i, 1, n)
    {
        char c;
        c = getchar();
        c = getchar();
        FOR(j, 1, m)
        {
            c = getchar();
            if(c == '.') arr[i][j] = 0;
            else arr[i][j] = 1;
        }
    }
    FOR(i, 1, n)
        FOR(j, 1, m)
            if(arr[i][j]) sum[i][j] = 0;
            else sum[i][j] = sum[i - 1][j] + 1;
    bool flag; //!stack.empty()
    long long height[maxN], width[maxN], index;
    FOR(i, 1, n)
    {
        flag = false;
        height[0] = 0, width[0] = 0, index = 0;
        FOR(j, 1, m)
        {
            if(!sum[i][j])
            {
                if(flag)
                {
                    ans += calc(height, width, index, sum[i][j]);
                    flag = false;
                    index = 0;
                }
                else continue;
            }
            else
            {
                flag = true;
                if(!index || sum[i][j] >= height[index])
                {
                    if(sum[i][j] > height[index])
                    {
                        ++index;
                        height[index] = sum[i][j];
                        width[index] = 1;
                    }
                    else ++width[index];
                }
                else ans += calc(height, width, index, sum[i][j]);
            }
        }
        if(flag) ans += calc(height, width, index, 0);
    }
    printf("%lld\n", ans);
    return 0;
}