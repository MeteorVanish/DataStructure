#include <iostream>
#include <cmath>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 100005 * 32;
int tree[maxN][2], cnt = 0;

inline void file()
{
    string op = "1";
    string INPUT = "Input " + op + ".txt", OUTPUT = "Output " + op + ".txt";
    freopen(INPUT.c_str(), "r", stdin);
    freopen(OUTPUT.c_str(), "w", stdout);
}

void ADD()
{
    int temp, pos = 0, id;
    scanf("%d", &temp);
    FOR(i, 1, 32)
    {
        id = (temp >> (32 - i)) & 1;
        if(!tree[pos][id]) tree[pos][id] = ++cnt;
        pos = tree[pos][id];
    }
}

string DECtoBIN(int x)
{
    string ret = "";
    FOR(i, 1, 32)
        ret += to_string(x >> (32 - i) & 1);
    return ret;
}

int main()
{
    //file();
    int n, m;
    scanf("%d%d", &n, &m);
    FOR(i, 1, n) ADD();
    while(m--)
    {
        int x, sum = 0, pos = 0;
        scanf("%d", &x);
        string s = DECtoBIN(x);
        FOR(i, 1, 32)
        {
            int temp = s[i - 1] - '0';
            if(tree[pos][1 - temp])
            {
                sum = (sum << 1) + 1 - temp;
                pos = tree[pos][1 - temp];
            }
            else
            {
                sum = (sum << 1) + temp;
                pos = tree[pos][temp];
            }
        }
        printf("%d\n", sum ^ x);
    }
    return 0;
}