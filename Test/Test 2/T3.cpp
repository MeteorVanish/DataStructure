#include <iostream>
#include <cstring>
using namespace std;

#define FOR(alpha, start, end) for(int (alpha) = (start); (alpha) <= (end); ++(alpha))
const int maxN = 1e6 + 5;
int n, cnt, head, tail;
int trie[maxN][26], fail[maxN], queue[maxN];
bool exist[maxN];
string s;

inline void file()
{
    string op = "1";
    string INPUT = "Input " + op + ".txt", OUTPUT = "Output " + op + ".txt";
    freopen(INPUT.c_str(), "r", stdin);
    freopen(OUTPUT.c_str(), "w", stdout);
}

void add()
{
    cin >> s;
    int pos = 0, id;
    FOR(i, 0, s.size() - 1)
    {
        id = s[i] - 'a';
        if(!trie[pos][id]) trie[pos][id] = ++cnt;
        pos = trie[pos][id];
    }
    exist[pos] = true;
}

void build()
{
    int pos;
    FOR(i, 0, 25)
        if(trie[0][i]) queue[tail++] = trie[0][i];
    while(head < tail)
    {
        pos = queue[head++];
        FOR(i, 0, 25)
        {
            if(trie[pos][i])
            {
                fail[trie[pos][i]] = trie[fail[pos]][i];
                queue[tail++] = trie[pos][i];
            }
            else
                trie[pos][i] = trie[fail[pos]][i];
        }
    }
}

bool query(const string &s)
{
    int pos = 0, id;
    FOR(i, 0, s.size() - 1)
    {
        id = s[i] - 'a';
        pos = trie[pos][id];
        for(int j = pos; j; j = fail[j])
            if(exist[j]) return true;
    }
    return false;
}

int main()
{
    //file();
    scanf("%d", &n);
    FOR(i, 1, n) add();
    build();
    cin >> s;
    if(query(s)) printf("valar morghulis\n");
    else printf("valar dohaeris\n");
    return 0;
}