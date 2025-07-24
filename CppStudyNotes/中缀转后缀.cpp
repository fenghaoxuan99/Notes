#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;
// 中缀转后缀表达式

int priority(char c)
{
    if (c == '+' || c == '-')
    {
        return 1;
    }
    else if (c == '*' || c == '/')
    {
        return 2;
    }
    else if (c == '(')
    {
        return 0;
    }
    else
    {
        return 0;
    }
}
string infixToSuffix(string infix)
{
    string suffix;
    stack<char> st;

    for (char c : infix)
    {
        if (c == ' ')
            continue;

        if (isdigit(c))
        {
            suffix += c;
        }
        else if (c == '(')
        {
            st.push(c);
        }
        else if (c == ')')
        {
            while (!st.empty() && st.top() != '(')
            {
                suffix += st.top();
                st.pop();
            }
            if (!st.empty())
                st.pop(); // 弹出左括号
        }
        else
        { // 运算符
            while (!st.empty() && priority(c) <= priority(st.top()))
            {
                suffix += st.top();
                st.pop();
            }
            st.push(c);
        }
    }

    while (!st.empty())
    {
        suffix += st.top();
        st.pop();
    }
    return suffix;
}

int main()
{
    string infix = "(2+1)*(3-4)-9/3";
    string suffix = infixToSuffix(infix);
    cout << suffix << endl;

    return 0;
}