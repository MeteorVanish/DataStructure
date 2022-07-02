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

const int maxN = 85;
int n, m, arr[maxN];
LL tmp, ans, dp[maxN][maxN];

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

inline int Calc(int x) { return x * x + x + 1; }

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read(), m = read();
    FOR(t, 1, n)
    {
        tmp = 0;
        CLEAR(dp);
        FOR(i, 1, m) arr[i] = read();
        FOR(k, 1, m) FOR(i, 0, k)
            {
                LL maxtmp;
                if(!i) maxtmp = dp[i][k - i - 1] + arr[m - k + 1] * Calc(k);
                else if(i == k) maxtmp = dp[i - 1][k - i] + arr[i] * Calc(k);
                else maxtmp = std::max(dp[i - 1][k - i] + arr[i] * Calc(k), dp[i][k - i - 1] + arr[m - k + i + 1] * Calc(k));
                dp[i][k - i] = std::max(dp[i][k - i], maxtmp);
            }
        FOR(i, 0, m) tmp = std::max(tmp, dp[i][m - i]);
        ans += tmp;
    }
    printf("%lld\n", ans);
    return 0;
}