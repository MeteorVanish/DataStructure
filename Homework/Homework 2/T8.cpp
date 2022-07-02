#include <iostream>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 1000005;
pair<int, int> arr[maxN];
int Queue[maxN], s[maxN] = {0};

int main()
{
    int n, head = 1, tail = 0, ans = 0;
    scanf("%d", &n);
    FOR(i, 1, n)
    {
        scanf("%d%d", &arr[i].first, &arr[i].second);
        while(head <= tail && arr[Queue[tail]].first <= arr[i].first) --tail;
        Queue[++tail] = i;
        while(head < tail && arr[Queue[head + 1]].first > arr[i].second) ++head;
        if(arr[Queue[head]].first > arr[i].second) s[i] = max(s[i-1], Queue[head]);
        else s[i] = s[i - 1];
        ans = max(ans, i - s[i]);
    }
    printf("%d\n", ans);
    return 0;
}