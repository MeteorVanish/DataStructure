#include <iostream>
#include <cstring>
using namespace std;

#define FOR(alpha, start, end) for(int (alpha) = (start); (alpha) <= (end); ++(alpha))
const int maxN = 4 * 1e5 + 5;
int n, cnt, arr[maxN], trie[maxN * 32][2], premax[maxN], postmax[maxN], XOR, ans;

inline void file()
{
    string op = "1";
    string INPUT = "Input " + op + ".txt", OUTPUT = "Output " + op + ".txt";
    freopen(INPUT.c_str(), "r", stdin);
    freopen(OUTPUT.c_str(), "w", stdout);
}

void ADD(int x)
{
    int pos = 0, id;
    FOR(i, 1, 32)
    {
        id = x >> 32 - i & 1;
        if(!trie[pos][id]) trie[pos][id] = ++cnt;
        pos = trie[pos][id];
    }
}

int query(int x)
{
    int pos = 0, id, sum = 0;
    FOR(i, 1, 32)
    {
        id = x >> 32 - i & 1;
        if(trie[pos][!id])
        {
            sum = (sum << 1) + !id;
            pos = trie[pos][!id];
        }
        else
        {
            sum = (sum << 1) + id;
            pos = trie[pos][id];
        }
    }
    return sum;
}

int main()
{
    //file();
    scanf("%d", &n);
    FOR(i, 1, n) scanf("%d", arr + i);
    ADD(0);
    FOR(i, 1, n)
    {
        premax[i] = std::max(premax[i - 1], query(arr[i] ^ XOR) ^ arr[i] ^ XOR);
        XOR ^= arr[i];
        ADD(XOR);
    }
    cnt = XOR = 0;
    memset(trie, 0, sizeof(trie));
    ADD(0);
    for(int i = n; i >= 1; --i)
    {
        postmax[i] = std::max(postmax[i + 1], query(arr[i] ^ XOR) ^ arr[i] ^ XOR);
        XOR ^= arr[i];
        ADD(XOR);
    }
    FOR(i, 1, n - 1)
        ans = std::max(ans, premax[i] + postmax[i + 1]);
    printf("%d\n", ans);
    return 0;
}