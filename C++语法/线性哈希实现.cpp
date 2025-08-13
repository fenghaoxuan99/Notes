#include <iostream>
#include <vector>
#include <functional>
using namespace std;

enum state
{
    STATE_UNUSED,
    STATE_DELETED,
    STATE_OCCUPIED
};

struct BUCKET
{
    BUCKET() : key(0), status(STATE_UNUSED) {}
    int key;
    state status;
};

// 线性探测哈希表的实现
class LinearProbingHashTable
{
public:
    LinearProbingHashTable() : capacity(PRIME[0]), size(0), usedbucketnumber(0), LOADFACTOR(0.75), PRIME_INDEX(0)
    {
        if (capacity != PRIME[0])
        {
            while (PRIME[PRIME_INDEX] < capacity)
            {
                PRIME_INDEX++;
            }
            capacity = PRIME[PRIME_INDEX];
        }

        table = new BUCKET[capacity];
    }
    ~LinearProbingHashTable()
    {
        delete[] table;
        table = nullptr;
    }

public:
    // 插入键值对
    bool insert(int key)
    {
        double loadfactor = (double)usedbucketnumber / capacity;
        if (loadfactor > LOADFACTOR)
        {
            expand();
        }
        int idx = key % capacity;

        if (table[idx].status == STATE_UNUSED)
        {
            table[idx].key = key;
            table[idx].status = STATE_OCCUPIED;
            usedbucketnumber++;
            return true;
        }
        for (int i = (idx + 1) % capacity; i != idx; i = (i + 1) % capacity)
        {
            if (table[i].status == STATE_UNUSED)
            {
                table[i].key = key;
                table[i].status = STATE_OCCUPIED;
                usedbucketnumber++;
                return true;
            }
        }
        return false;
    }

    bool erase(int key)
    {
        int idx = key % capacity;
        if (table[idx].status == STATE_UNUSED)
        {
            return true;
        }
        if (table[idx].status == STATE_OCCUPIED && table[idx].key == key)
        {
            table[idx].status = STATE_DELETED;
            usedbucketnumber--;
            return true;
        }
        for (int i = (idx + 1) % capacity; i != idx; i = (i + 1) % capacity)
        {
            if (table[i].status == STATE_UNUSED)
            {
                return true; // 没有找到
            }
            if (table[i].status == STATE_OCCUPIED && table[i].key == key)
            {
                table[i].status = STATE_DELETED;
                usedbucketnumber--;
                return true;
            }
        }
        return true;
    }

    bool find(int key)
    {
        int idx = key % capacity;
        if (table[idx].status == STATE_UNUSED)
        {
            return false;
        }
        if (table[idx].status == STATE_OCCUPIED && table[idx].key == key)
        {
            return true;
        }
        for (int i = (idx + 1) % capacity; i != idx; i = (i + 1) % capacity)
        {
            if (table[i].status == STATE_UNUSED)
            {
                return false; // 没有找到
            }
            if (table[i].status == STATE_OCCUPIED && table[i].key == key)
            {
                return true;
            }
        }
        return false;
    }

private:
    BUCKET *table;
    int capacity;
    int size;
    int usedbucketnumber;             // 记录已使用的桶的数量
    double LOADFACTOR;                // 负载因子
    static const int PRIME_SIZE = 10; // 默认容量
    static int PRIME[PRIME_SIZE];     // 素数表
    int PRIME_INDEX;                  // 素数表的索引

    void expand()
    {
        int oldcapacity = capacity;
        BUCKET *oldtable = table;
        capacity = PRIME[++PRIME_INDEX];
        table = new BUCKET[capacity];
        for (int i = 0; i < oldcapacity; i++)
        {
            if (oldtable[i].status == STATE_OCCUPIED)
            {
                int idx = oldtable[i].key % capacity;
                if (table[idx].status == STATE_UNUSED)
                {
                    table[idx].key = oldtable[i].key;
                    table[idx].status = STATE_OCCUPIED;
                }
                else
                {
                    for (int j = (idx + 1) % capacity; j != idx; (j + 1) % capacity)
                    {
                        if (table[j].status == STATE_UNUSED)
                        {
                            table[j].key = oldtable[i].key;
                            table[j].status = STATE_OCCUPIED;
                            break;
                        }
                    }
                }
            }
        }
        delete[] oldtable;
    }
};

int LinearProbingHashTable::PRIME[PRIME_SIZE] = {
    3,
    7,
    23,
    47,
    97,
    251,
    443,
    911,
    1471,
    2869};

int main()
{

    LinearProbingHashTable hashTable;
    hashTable.insert(1);
    hashTable.insert(2);
    hashTable.insert(3);
    hashTable.insert(4);
    hashTable.insert(5);

    std::cout << "Find 1: " << hashTable.find(1) << std::endl; // true
    std::cout << "Find 2: " << hashTable.find(2) << std::endl; // true
    std::cout << "Find 3: " << hashTable.find(3) << std::endl; // true
    std::cout << "Find 4: " << hashTable.find(4) << std::endl; // true
    std::cout << "Find 5: " << hashTable.find(5) << std::endl; // true
    hashTable.erase(2);

    std::cout << "Find 2: " << hashTable.find(2) << std::endl; // true
    return 0;
}
