#include <iostream>
#include <vector>
#include <functional>

class PriorityQueue
{
public:
    PriorityQueue(int cap = 20) : size_(0), capacity_(cap), data_(new int[cap])
    {
        comp_ = std::greater<int>();
    }

    ~PriorityQueue()
    {
        delete[] data_;
        data_ = nullptr;
    }

    void push(int val)
    {
        if (size_ == capacity_)
        {
            expand();
        }
        if (size_ == 0)
        {
            data_[size_] = val;
        }
        else
        {
            data_[size_] = val;
            shiftup(size_, val);
        }
        ++size_;
    }

    void pop()
    {
        if (size_ == 0)
        {
            return;
        }
        data_[0] = data_[--size_];
        int i = 0;
        while (i < size_ / 2)
        {
            int left = i * 2 + 1;
            int right = i * 2 + 2;
            if (left >= size_)
            {
                break;
            }
            int minIndex = left;
            if (right < size_ && comp_(data_[right], data_[left]))
            {
                minIndex = right;
            }
            if (comp_(data_[minIndex], data_[i]))
            {
                std::swap(data_[i], data_[minIndex]);
                i = minIndex;
            }
            else
            {
                break;
            }
        }
    }

    int top()
    {
        if (size_ == 0)
        {
            return -1; // or throw an exception
        }
        return data_[0];
    }

    void show()
    {
        for (int i = 0; i < size_; ++i)
        {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
    }

    void setComp(std::function<bool(int, int)> comp)
    {
        comp_ = comp;
    }

private:
    using Comp = std::function<bool(int, int)>;

    void expand()
    {
        int *new_data = new int[capacity_ * 2];
        std::copy(data_, data_ + size_, new_data);
        delete[] data_;
        data_ = new_data;
        capacity_ *= 2;
    }

    void shiftup(int size, int val)
    {
        while (size > 0)
        {
            int father = (size - 1) / 2;
            if (comp_(val, data_[father]))
            {
                std::swap(data_[size], data_[father]);
                size = father;
            }
            else
            {
                break;
            }
        }
    }

    Comp comp_;
    int *data_;
    int size_;
    int capacity_;
};

int main()
{
    PriorityQueue pq(10);
    pq.push(5);
    pq.push(1);
    pq.push(2);
    pq.push(3);
    pq.push(4);
    pq.push(6);
    pq.push(7);
    pq.push(8);
    pq.push(9);
    pq.show();

    return 0;
}
