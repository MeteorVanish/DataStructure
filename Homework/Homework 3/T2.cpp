#include <iostream>
using namespace std;

#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 100005 * 30;
int n, tree[maxN][27], cnt = 0;

inline void file()
{
    string op = "1";
    string INPUT = "Input " + op + ".txt", OUTPUT = "Output " + op + ".txt";
    freopen(INPUT.c_str(), "r", stdin);
    freopen(OUTPUT.c_str(), "w", stdout);
}

void Insert(const string &s)
{
    int pos = 0, id, len = s.size() - 1;
    ++tree[0][0];
    FOR(i, 0, len)
    {
        id = s[i] - 'a' + 1;
        if(!tree[pos][id]) tree[pos][id] = ++cnt;
        pos = tree[pos][id];
        ++tree[pos][0];
    }
}

void Delete(const string &s)
{
    int pos = 0, id, len = s.size() - 1, del;
    FOR(i, 0, len)
    {
        id = s[i] - 'a' + 1;
        if(!tree[pos][id]) return;
        pos = tree[pos][id];
    }
    del = tree[pos][0];
    tree[0][0] -= del;
    pos = 0;
    FOR(i, 0, len - 1)
    {
        id = s[i] - 'a' + 1;
        if(!tree[pos][id]) return;
        pos = tree[pos][id];
        tree[pos][0] -= del;
    }
    tree[pos][s[s.size() - 1] - 'a' + 1] = 0;
}

bool Search(const string &s)
{
    int pos = 0, id, len = s.size() - 1;
    FOR(i, 0, len)
    {
        id = s[i] - 'a' + 1;
        if(!tree[pos][id]) return false;
        pos = tree[pos][id];
    }
    return tree[pos][0];
}

int main()
{
    //file();
    string op, s;
    scanf("%d", &n);
    while(n--)
    {
        cin >> op >> s;
        if(op == "insert") {Insert(s);continue;}
        if(op == "delete") {Delete(s);continue;}
        if(op == "search")
        {
            bool flag = Search(s);
            if(flag) printf("Yes\n");
            else printf("No\n");
        }
    }
    return 0;
}