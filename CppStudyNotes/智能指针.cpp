#include <iostream>
#include <memory>

class A
{
public:
    A() { std::cout << "A constructor" << std::endl; }
    ~A() { std::cout << "A destructor" << std::endl; }
    void print() { std::cout << "A print" << std::endl; }
};

class APtr
{
public:
    APtr() {}
    APtr(A *a) : m_a(a) {}
    APtr(const APtr &a) = delete;
    APtr &operator=(const APtr &a) = delete;
    APtr(APtr &&a) : m_a(a.m_a)
    {
        delete a.m_a;
        a.m_a = nullptr;
    }
    APtr &operator=(APtr &&a)
    {
        if (this != &a)
        {
            delete m_a;
            m_a = a.m_a;
            a.m_a = nullptr;
            return *this;
        }
    }

    A *get() { return m_a; }
    void reset(A *a)
    {
        if (m_a != a)
        {
            delete m_a;
            m_a = a;
        }
    }
    A *release()
    {
        auto a = m_a;
        m_a = nullptr;

        return a;
    }
    ~APtr()
    {
        delete m_a;
        m_a = nullptr;
    }
    void print() { m_a->print(); }

private:
    A *m_a{nullptr};
};

int main()
{

    {
        std::unique_ptr<A> a = std::make_unique<A>();
        a->print();
        (*a).print();

        auto ptr2 = a.get();
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}