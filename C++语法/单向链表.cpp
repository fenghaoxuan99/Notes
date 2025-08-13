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
    list() : head(new node()) {};
    ~list()
    {
        node *p = head;
        while (p != nullptr)
        {
            node *q = p->next;
            delete p;
            p = q;
        }
        head = nullptr;
    };

public:
    void inserttail(int a)
    {
        node *p = head;

        while (p->next != nullptr)
        {
            p = p->next;
        }
        p->next = new node(a);
    }

    void inserthead(int a)
    {
        node *p = new node(a);
        p->next = head->next;
        head->next = p;
    }

    void Remove(int a)
    {
        node *p = head;
        while (p->next != nullptr)
        {
            if (p->next->data == a)
            {
                node *q = p->next;
                p->next = q->next;
                delete q;
                break;
            }
            else
            {
                p = p->next;
            }
        }
    }
    void RemoveAll(int a)
    {
        node *p = head;
        while (p->next != nullptr)
        {
            if (p->next->data == a)
            {
                node *q = p->next;
                p->next = q->next;
                delete q;
            }
            else
            {
                p = p->next;
            }
        }
    }
    bool find(int a)
    {
        node *p = head->next;
        while (p != nullptr)
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

    void reverse1()
    {
        node *p = head->next;
        node *q = nullptr;
        while (p != nullptr)
        {
            node *r = p->next;
            p->next = q;
            q = p;
            p = r;
        }
        head->next = q;
    }

    void reverse2()
    {
        node *p = head->next;
        if (p == nullptr)
        {
            return;
        }

        head->next = nullptr;
        while (p != nullptr)
        {
            node *q = p->next;
            p->next = head->next;
            head->next = p;
            p = q;
        }
    }

    void showlist()
    {
        node *p = head->next;
        while (p != nullptr)
        {
            std::cout << p->data << "  ";
            p = p->next;
        }
        std::cout << std::endl;
    }

private:
    struct node *head;
};

int main()
{
    list l;
    l.inserttail(1);
    l.inserttail(2);
    l.inserttail(3);
    l.inserttail(4);
    l.inserttail(5);
    l.showlist();
    l.reverse2();

    l.showlist();

    std::cout << "---------E-N-D---------" << std::endl;
    return 0;
}