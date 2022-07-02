#include <iostream>
#include <cstring>
using namespace std;
#define maxN 1000005
char s[maxN];
int nextp[maxN], ans[maxN];

void getnextpval(int len)
{
    nextp[1] = 0;
    int j = 1, k = 0;
    while(j < len)
    {
        if(!k || s[k] == s[j])
        {
            ++k;++j;
            nextp[j] = k;
        }
        else k = nextp[k];
    }
}

int main()
{
    cin >> s+1;
    int n = strlen(s+1), cnt = 0;
    getnextpval(n);
    int p = n;
    ans[++cnt] = p;
    while(p)
    {
        p = nextp[p];
        if(s[p] == s[n])
            ans[++cnt] = p;
    }
    for(int i = cnt; i >= 1; --i)
        printf("%d\n", ans[i]);
    return 0;
}