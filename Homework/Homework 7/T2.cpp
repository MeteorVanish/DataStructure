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

const int maxN = 2005;
int n, arr[maxN], dp1[maxN], dp2[maxN], ans1, ans2;

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
    FOR(i, 1, n) arr[i] = read();
    FOR(i, 1, n) dp1[i] = dp2[i] = 1;
    FOR(i, 2, n) FOR(j, 1, i - 1)
            if(arr[j] >= arr[i]) dp1[i] = std::max(dp1[i], dp1[j] + 1);
    FOR(i, 2, n) FOR(j, 1, i - 1)
            if(arr[j] < arr[i]) dp2[i] = std::max(dp2[i], dp2[j] + 1);
    FOR(i, 1, n)
    {
        ans1 = std::max(ans1, dp1[i]);
        ans2 = std::max(ans2, dp2[i]);
    }
    printf("%d\n%d\n", ans1, ans2);
    return 0;
}