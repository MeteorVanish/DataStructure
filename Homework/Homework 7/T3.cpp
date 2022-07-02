#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;
using LL = long long;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(int i = start; i <= end; ++i)
#define ROF(i, start, end) for(int i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)

const int maxN = 105;
int n, arr[maxN << 1], dp[maxN << 1][maxN << 1], ans;

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline int read()
{
    int ret = 0, f = 1; char c = getchar();
    while(!(c >= '0' && c <= '9')) { if(c == '-') f = -1; c = getchar(); }
    while(c >= '0' && c <= '9') ret = (ret << 3) + (ret << 1) + c - '0', c = getchar();
    return f * ret;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read();
    FOR(i, 1, n)
        arr[i] = arr[n + i] = read();
    FOR(t, 1, n)
    {
        CLEAR(dp);
        FOR(k, 1, n - 1)
            FOR(i, t, t + n - k - 1)
                FOR(j, i + 1, i + k)
                    dp[i][i + k] = std::max(dp[i][i + k], dp[i][j - 1] + dp[j][i + k] + arr[i] * arr[j] * arr[i + k + 1]);
        ans = std::max(ans, dp[t][t + n - 1]);
    }
    printf("%d\n", ans);
    return 0;
}