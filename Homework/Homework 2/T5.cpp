#include <iostream>

using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 1000005;
const int maxM = 2005;
int n, m, arr[maxN], t[maxM], start = 1, length = maxN, temp = 1, cnt = 0;

int main()
{
    scanf("%d%d", &n, &m);
    length = n;
    FOR(i,1,n) scanf("%d", arr + i);
    FOR(i,1,m) t[i] = 0;
    FOR(i,1,n)
    {
        if(!t[arr[i]]) ++cnt;
        ++t[arr[i]];
        if(cnt == m)
        {
            while(--t[arr[temp]]) ++temp;
            if(i - temp + 1 < length)
            {
                start = temp;
                length = i - temp + 1;
            }
            ++temp;
            --cnt;
        }
    }
    printf("%d %d\n", start, start + length - 1);
    return 0;
}