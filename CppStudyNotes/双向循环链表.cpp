#include <iostream>

struct node
{
    node(int a = 0)
        : data(a), prev(nullptr), next(nullptr) {}
    int data;
    struct node *next;
    struct node *prev;
};

class DoubeleList
{
public:
    DoubeleList()
    {
        head = new node;
        head->next = head;
        head->prev = head;
    }
    ~DoubeleList()
    {
        node *p = head->next;
        while (p != head)
        {
            node *q = p->next;
            if (q != nullptr)
            {
                q->prev = head;
            }
            delete p;
            p = q;
        }
        delete head;
        head = nullptr;
    }

public:
    void inserthead(int a)
    {
        node *p = new node(a);
        p->next = head->next;
        head->next = p;
        head->next->prev = p;
        p->prev = head;
    }
    void inserttail(int a)
    {
        node *p = new node(a);
        node *q = head->prev;
        q->next = p;
        p->prev = q;
        p->next = head;
        head->prev = p;
    }
    void erase(int a)
    {
        node *p = head->next;
        while (p != head)
        {
            if (p->data == a)
            {
                p->prev->next = p->next;
                p->next->prev = p->prev;
                delete p;
                return;
            }
            else
            {
                p = p->next;
            }
        }
    }

    void show()
    {
        node *p = head->next;
        while (p != head)
        {
            std::cout << p->data << " ";
            p = p->next;
        }
        std::cout << std::endl;
    }

private:
    struct node *head;
};

int main()
{
    DoubeleList l;
    for (int i = 0; i < 10; i++)
    {
        l.inserttail(i);
    }

    l.erase(0);

    l.show();
    std::cout << "---------E-N-D---------" << std::endl;
    return 0;
}