#include <iostream>
using namespace std;

int main()
{
    int n;
    scanf("%d", &n);
    char *s = new char[n+1];
    int *next = new int[n+1];
    next[1] = 0;
    scanf("%s", s+1);
    int j = 1, k = 0;
    while(j <= n)
    {
        if(!k || s[j] == s[k])
        {
            j++; k++;
            next[j] = k;
        }
        else k = next[k];
    }
    printf("%d", n-next[n]);
    return 0;
}