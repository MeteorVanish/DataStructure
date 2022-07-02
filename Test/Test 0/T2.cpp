#include <iostream>
#include <cstring>
using namespace std;
#define maxN 1000005
int nextx[maxN];
char S[maxN], T[maxN];

void getNext(int len)
{
    nextx[1] = 0;
    int j = 1, k = 0;
    while(j <= len)
    {
        if(!k || T[j] == T[k])
        {
            j++, k++;
            nextx[j] = k;
        }
        else k = nextx[k];
    }
}

int KMP(int Slen, int Tlen, int pos)
{
    int j = pos-1, k = 0;
    
    while(j <= Slen && k <= Tlen)
    {
        if(!k || S[j] == T[k]) j++, k++;
        else k = nextx[k];
    }
    if(k > Tlen) return j-Tlen;
    else return 0;
}

int main()
{
    cin >> S+1 >> T+1;
    getNext(strlen(T+1));
    for(int i = 1; i <= strlen(S+1); ++i)
    {
        int ans = KMP(strlen(S+1), strlen(T+1), i);
        if(ans) printf("%d\n", ans);
        else break;
        i = ans;
    }
    for(int i = 1; i <= strlen(T+1); ++i)
        printf("%d ", nextx[i+1]-1);
    return 0;
}