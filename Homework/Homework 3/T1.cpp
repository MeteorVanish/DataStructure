#include <iostream>
#include <cmath>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 100005 * 10;
int tree[maxN][30], num[maxN];

inline void file()
{
    string op = "1";
    string INPUT = "Input " + op + ".txt", OUTPUT = "Output " + op + ".txt";
    freopen(INPUT.c_str(), "r", stdin);
    freopen(OUTPUT.c_str(), "w", stdout);
}

int main()
{
    //file();
    int n, m, cnt = 0;
    string s;
    scanf("%d", &n);
    while(n--)
    {
        cin >> s;
        int root = 0;
        FOR(i, 0, s.size() - 1)
        {
            int id = s[i] - 'a' + 1;
            if(!tree[root][id]) tree[root][id] = ++cnt;
            root = tree[root][id];
            ++num[root];
        }
    }
    scanf("%d", &m);
    while(m--)
    {
        cin >> s;
        int ans = -1, root = 0;
        FOR(i, 0, s.size() - 1)
        {
            int id = s[i] - 'a' + 1;
            if(!tree[root][id])
            {
                ans = 0;
                break;
            }
            root = tree[root][id];
        }
        if(ans) ans = num[root];
        printf("%d\n", ans);
    }
    return 0;
}