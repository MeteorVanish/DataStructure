#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define DEBUG(x) cout << "<debug> " << #x << " = " << x << '\n'
#define FOR(i, start, end) for(LL i = start; i <= end; ++i)
#define ROF(i, start, end) for(LL i = end; i >= start; --i)
#define MEMSET(ptr, c) memset(ptr, c, sizeof(ptr))
#define CLEAR(ptr) MEMSET(ptr, 0)
using LL = long long;

void file(int op)
{
    string I = "Input_" + to_string(op) + ".txt", O = "Output_" + to_string(op) + ".txt";
    freopen(I.c_str(), "r", stdin);
    freopen(O.c_str(), "w", stdout);
}

int main()
{
#ifndef ONLINE_JUDGE
    file(1);
#endif
    string s;
    while(1)
    {
        cin >> s;
        if(s[0] == '.') break;
        int k = 0;
        FOR(i, 1, s.length() - 1)
        {
            if(s[i] == s[k]) ++k;
            else if(s[i] == s[0]) k = 1;
            else k = 0;
        }
        int sublen = s.length() - k, ans;
        if(s.length() % sublen) ans = 1;
        else ans = s.length() / sublen;
        printf("%d\n", ans);
    }
    return 0;
}