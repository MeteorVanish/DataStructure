#include <iostream>
#include <cstring>
using namespace std;
#define maxN 10005

template<class T>
class Stack
{
private:
    T *arr;
    int head;
public:
    Stack() {head = -1;arr = new T[maxN];}
    ~Stack() {delete arr;}
    void push(T x) {arr[++head] = x;}
    T pop() {return arr[head--];}
    void print() {for(int i=0;i<=head;++i) cout<<arr[i];}
};

int main()
{
    string s;
    cin >> s;
    Stack<int> num;
    Stack<char> ch;
    for(int i = 0; i < s.length(); ++i)
    {
        if(s[i] >= 'a' && s[i] <= 'z' || s[i] == '[')
        {
            ch.push(s[i]);
            continue;
        }
        if(s[i] >= '0' && s[i] <= '9')
        {
            int sum = s[i]-'0';
            while(s[i+1] >= '0' && s[i+1] <= '9')
                sum = sum*10 + s[++i] - '0';
            num.push(sum);
            continue;
        }
        if(s[i] == ']')
        {
            char temp = ch.pop(), temps[maxN];
            int pos = -1;
            while(temp != '[')
            {
                temps[++pos] = temp;
                temp = ch.pop();
            }
            int t = num.pop();
            for(int i = 1; i <= t; ++i)
                for(int j = 0;j <= pos; j++)
                    ch.push(temps[pos-j]);
        }
    }
    ch.print();
    return 0;
}