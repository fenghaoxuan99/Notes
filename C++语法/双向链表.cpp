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
    }
    ~DoubeleList()
    {
        node *p = head->next;
        while (p != nullptr)
        {
            node *q = p->next;
            delete p;
            p = q;
        }
        head = nullptr;
    }

public:
    void inserthead(int a)
    {
        node *p = new node(a);
        p->next = head->next;
        head->next = p;
        if (p->next != nullptr)
        {
            p->next->prev = p;
        }
        p->prev = head;
    }

    void inserttail(int a)
    {
        node *p = new node(a);
        node *q = head;
        while (q->next != nullptr)
        {
            q = q->next;
        }
        q->next = p;
        p->prev = q;
    }
    void erase(int a)
    {
        node *q = head;
        node *p = head->next;
        while (p != nullptr)
        {
            if (p->data == a)
            {
                q->next = p->next;
                if (p->next != nullptr)
                {
                    p->next->prev = q;
                }
                delete p;

                return;
            }
            else
            {
                q = p;
                p = p->next;
            }
        }
    }

    void show()
    {
        node *p = head->next;
        while (p != nullptr)
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
    l.inserttail(1);
    l.inserttail(2);
    l.inserttail(3);
    l.inserttail(4);
    l.inserttail(5);

    l.erase(5);

    l.show();

    std::cout << "---------E-N-D---------" << std::endl;
    return 0;
}