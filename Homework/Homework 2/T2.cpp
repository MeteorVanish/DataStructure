#include <iostream>

using namespace std;
const int maxN = 100005;
int n, k, id, arr[maxN], ans[maxN], queue[maxN], head = 0, tail = 1;

int main()
{
    scanf("%d%d", &n, &k);
    for(int i = 1; i <= n; ++i) scanf("%d", &arr[i]);
    for(int i = n; i >= 1; --i)
    {
        while(head >= tail && arr[i] <= arr[queue[head]]) --head;
        queue[++head] = i;
        if(queue[tail] - i == k) tail++;
        if(i <= n - k + 1)
            ans[i] = arr[queue[tail]];
    }
    while(scanf("%d", &id) != EOF)
        printf("%d\n", ans[id + 1]);
    return 0;
}