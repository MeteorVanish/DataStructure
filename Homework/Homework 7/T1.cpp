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

const int maxN = 1005;
int r, arr[maxN][maxN], dp[maxN][maxN], ans;

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
    r = read();
    FOR(i, 1, r) FOR(j, 1, i) arr[i][j] = read();
    FOR(i, 1, r) FOR(j, 1, i)
            dp[i][j] = std::max(dp[i - 1][j - 1], dp[i - 1][j]) + arr[i][j];
    FOR(j, 1, r) ans = std::max(ans, dp[r][j]);
    printf("%d\n", ans);
    return 0;
}