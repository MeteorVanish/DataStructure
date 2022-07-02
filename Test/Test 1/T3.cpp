#include <iostream>
using namespace std;

const int maxN = 4000005;
int arr[maxN];
char stack[maxN];
string s;

int getnum(int &i)
{
    int temp = 0;
    while(s[i] >= '0' && s[i] <= '9')
    {
        temp = (temp << 3) + (temp << 1) + s[i] - '0';
        ++i;
    }
    return temp;
}

void ADD(int &topNum)
{
    int temp1, temp2;
    temp1 = arr[topNum--];
    temp2 = arr[topNum--];
    arr[++topNum] = temp2 + temp1;
}

void MINUS(int &topNum)
{
    int temp1, temp2;
    temp1 = arr[topNum--];
    temp2 = arr[topNum--];
    arr[++topNum] = temp2 - temp1;
}

void XOR(int &topNum)
{
    int temp1, temp2;
    temp1 = arr[topNum--];
    temp2 = arr[topNum--];
    arr[++topNum] = temp2 ^ temp1;
}

int main()
{
    int topNum = 0, topCh = 0, index = 0;
    char ch;
    cin >> s;
    while(1)
    {
        if(s[index] >= '0' && s[index] <= '9')
            arr[++topNum] = getnum(index);
        if(index >= s.size()) break;
        ch = s[index++];
        if(ch == '(') stack[++topCh] = ch;
        if(ch == '+' || ch == '-')
        {
            while(topCh && stack[topCh] != '^' && stack[topCh] != '(')
            {
                if(stack[topCh] == '+') ADD(topNum);
                if(stack[topCh] == '-') MINUS(topNum);
                --topCh;
            }
            stack[++topCh] = ch;
        }
        if(ch == '^')
        {
            while(topCh && stack[topCh] != '(')
            {
                if(stack[topCh] == '+') ADD(topNum);
                if(stack[topCh] == '-') MINUS(topNum);
                if(stack[topCh] == '^') XOR(topNum);
                --topCh;
            }
            stack[++topCh] = ch;
        }
        if(ch == ')')
        {
            while(topCh && stack[topCh] != '(')
            {
                if(stack[topCh] == '+') ADD(topNum);
                if(stack[topCh] == '-') MINUS(topNum);
                if(stack[topCh] == '^') XOR(topNum);
                --topCh;
            }
            --topCh;
        }
    }
    while(topCh)
    {
        if(stack[topCh] == '+') ADD(topNum);
        if(stack[topCh] == '-') MINUS(topNum);
        if(stack[topCh] == '^') XOR(topNum);
        --topCh;
    }
    printf("%d", arr[1]);
    return 0;
}