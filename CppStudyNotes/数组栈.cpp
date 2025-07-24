#include <iostream>

class mystack
{
public:
    mystack(int cap = 10) : arr(new int[cap]), top(0), capacity(cap) {}
    ~mystack() { delete[] arr; }
    bool isempty() { return top == 0; }

    void push(int a)
    {
        if (top == capacity)
        {
            expand(2 * capacity);
            capacity *= 2;
        }
        arr[top++] = a;
    }
    void expand(int newsize)
    {
        int *newarr = new int[newsize];
        std::copy(arr, arr + capacity, newarr);
        delete[] arr;
        arr = newarr;
    }
    int pop() { return arr[--top]; }

    int gettop()
    {
        if (isempty())
        {
            return -1;
        }
        return arr[top - 1];
    }

private:
    int *arr;
    int top;
    int capacity;
};

int main()
{
    mystack s;
    for (int i = 0; i < 15; i++)
    {
        s.push(i);
    }
    while (!s.isempty())
    {
        std::cout << s.pop();
    }

    return 0;
}