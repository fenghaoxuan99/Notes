#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

class Queue
{
public:
    Queue(int size) : arr(new int[size]), front(0), rear(0), size(size) {};
    ~Queue() { delete[] arr; }
    void push(int value)
    {
        if ((rear + 1) % size == front)
        {
            expand(size * 2);
        }
        arr[rear] = value;
        rear = (rear + 1) % size;
    }

    int pop()
    {
        if (isEmpty())
        {
            return -1;
        }
        int top = arr[front];
        front = (front + 1) % size;
        return top;
    }

    int top() const { return arr[front]; }

    int back() const { return arr[(rear - 1 + size) % size]; }

    int getFront() { return arr[front]; }

    void expand(int newSize)
    {
        int *newArr = new int[newSize];
        int index = 0;
        for (int i = front; i != rear; i = (i + 1) % size)
        {
            newArr[index++] = arr[i];
        }
        delete[] arr;
        arr = newArr;
        front = 0;
        rear = index;
        size = newSize;
    }

    bool isEmpty() { return front == rear; }

    bool isFull() { return (rear + 1) % size == front; }

    void show()
    {
        for (int i = front; i != rear; i = (i + 1) % size)
        {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

private:
    int *arr;
    int front;
    int rear;
    int size;
};

int main()
{
    Queue q(5);
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    q.push(5);

    q.show();

    std::cout << "Top element: " << q.top() << std::endl;
    std::cout << "Back element: " << q.back() << std::endl;

    q.pop();

    q.show();
    return 0;
}