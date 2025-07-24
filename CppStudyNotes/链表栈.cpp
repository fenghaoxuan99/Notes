#include <iostream>

class mystack
{
public:
    mystack() : head(new node), top(0) {}
    ~mystack()
    {
        node *p = head->next;
        while (p != nullptr)
        {
            node *q = p->next;
            delete p;
            p = q;
        }
        delete head;
    }

    struct node
    {
        node(int a = 0) : data(a), next(nullptr) {}
        int data;
        struct node *next;
    };

    void push(int a)
    {
        node *p = new node(a);
        p->next = head->next;
        head->next = p;
        top++;
    }

    int pop()
    {
        if (top == 0)
            return -1;
        node *p = head->next;
        head->next = p->next;
        int data = p->data;
        delete p;
        top--;
        return data;
    }
    bool isempty() { return top == 0; }
    int gettop()
    {
        if (top == 0)
        {
            return -1;
        }
        return head->next->data;
    }

private:
    node *head;
    int top;
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