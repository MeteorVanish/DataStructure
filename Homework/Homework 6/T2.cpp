#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(int i = start; i <= end; ++i)
#define ROF(i, start, end) for(int i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)
using LL = long long;

const int maxN = 305;
const int INF = 0x7fffffff;
int n, ans;
int f[maxN][maxN], dis[maxN];
bool exist[maxN];

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

inline int read()
{
    int ret = 0, f = 1;
    char c = getchar();
    while(!(c >= '0' && c <= '9'))
    {
        if(c == '-') f = -1;
        c = getchar();
    }
    while(c >= '0' && c <= '9')
        ret = (ret << 3) + (ret << 1) + c - '0', c = getchar();
    return f * ret;
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    n = read();
    FOR(i, 1, n) f[i][n + 1] = read(), f[n + 1][i] = f[i][n + 1];
    FOR(i, 1, n) FOR(j, 1, n) f[i][j] = read();
    FOR(i, 0, n + 1) dis[i] = INF;
    dis[1] = 0;
    FOR(t, 1, n + 1)
    {
        int pos = 0;
        FOR(i, 1, n + 1)
            if(!exist[i] && dis[i] < dis[pos]) pos = i;
        ans += dis[pos];
        exist[pos] = true;
        FOR(i, 1, n + 1)
            if(i != pos && !exist[i]) dis[i] = min(dis[i], f[pos][i]);
    }
    printf("%d\n", ans);
    return 0;
}