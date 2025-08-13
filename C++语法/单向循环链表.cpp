#include <iostream>

struct node
{
    node(int a = 0) : data(a), next(nullptr) {}
    int data;
    struct node *next;
};

class list
{

public:
    list()
    {
        head = new node;

        tail = head;
        head->next = head;
    };
    ~list()
    {
        node *p = head->next;
        while (p != head)
        {
            node *q = p->next;
            delete p;
            p = q;
        }
        head = nullptr;
        tail = nullptr;
    };

    void inserttail(int a)
    {
        node *p = new node(a);
        p->next = head;
        tail->next = p;
        tail = p;
    }

    void inserthead(int a)
    {
        node *p = new node(a);
        p->next = head->next;
        head->next = p;
        if (tail == head)
        {
            tail = p;
        }
    }

    void erase(int a)
    {
        node *p = head;
        node *q = head->next;
        while (q != head)
        {
            if (q->data == a)
            {
                p->next = q->next;
                delete q;
                if (p->next == head)
                {
                    tail = p;
                }
                return;
            }
            else
            {
                p = q;
                q = q->next;
            }
        }
    }

    bool find(int a)
    {
        node *p = head->next;
        while (p != head)
        {
            if (p->data == a)
            {
                return true;
            }
            else
            {
                p = p->next;
            }
        }
        return false;
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

public:
private:
    struct node *head;
    struct node *tail;
};

int main()
{
    list l;
    // l.inserttail(1);
    // l.inserttail(2);
    // l.inserttail(3);
    // l.inserttail(4);

    // l.show();
    l.inserthead(5);
    l.inserthead(6);
    l.inserthead(7);
    l.inserthead(8);
    l.inserthead(9);

    l.show();

    std::cout << "---------E-N-D---------" << std::endl;
    return 0;
}