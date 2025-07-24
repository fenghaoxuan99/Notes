#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

// 链式哈希表的实现

class HashTable
{
public:
    HashTable() : size(PRIME[0]), useBucketnumber(0), loadFactor(0.75), PRIME_INDEX(0)
    {
        if (size != PRIME[0])
        {
            while (PRIME[PRIME_INDEX] < size)
            {
                PRIME_INDEX++;
            }
            size = PRIME[PRIME_INDEX];
        }
        table.resize(size);
    }

    ~HashTable() {}

public:
    // 插入键值对
    bool insert(int key)
    {
        double loadfactor = (double)useBucketnumber / size;
        if (loadfactor > loadFactor)
        {
            expand();
        }
        int idx = key % size;
        if (table[idx].empty())
        {
            useBucketnumber++;
            table[idx].emplace_front(key);
            return true;
        }
        else
        {
            auto it = std::find(table[idx].begin(), table[idx].end(), key);
            if (it == table[idx].end())
            {
                table[idx].emplace_front(key);
            }
            else
            {
                return false; // 键已存在
            }
        }
        return true;
    }

    bool remove(int key)
    {
        int idx = key % size;
        auto it = std::find(table[idx].begin(), table[idx].end(), key);
        if (it != table[idx].end())
        {
            table[idx].erase(it);
            if (table[idx].empty())
            {
                useBucketnumber--;
            }
            return true;
        }
        return false; // 键不存在
    }

    // 查找键值对
    bool find(int key)
    {
        int idx = key % size;
        for (auto it = table[idx].begin(); it != table[idx].end(); ++it)
        {
            if (*it == key)
            {
                return true;
            }
        }
        return false;
    }

private:
    vector<list<int>> table;          // 哈希表的底层存储结构
    int size;                         // 哈希表的大小
    int useBucketnumber;              // 已使用的桶数
    double loadFactor;                // 负载因子
    static const int PRIME_SIZE = 10; // 默认容量
    static int PRIME[PRIME_SIZE];     // 素数表
    int PRIME_INDEX;                  // 素数表的索引

private:
    // 扩容哈希表
    void expand()
    {
        int oldsize = size;
        vector<list<int>> oldtable;
        oldtable.swap(table);
        size = PRIME[++PRIME_INDEX];
        table.resize(size);
        useBucketnumber = 0;
        for (int i = 0; i < oldsize; i++)
        {
            for (auto it = oldtable[i].begin(); it != oldtable[i].end(); ++it)
            {
                int idx = *it % size;
                if (table[idx].empty())
                {
                    useBucketnumber++;
                }
                table[idx].emplace_front(*it);
            }
        }
    }
};

int HashTable::PRIME[PRIME_SIZE] = {
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
    HashTable hashTable;
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
    hashTable.remove(3);
    std::cout << "Find 3 after removal: " << hashTable.find(3) << std::endl; // false
    hashTable.remove(1);
    std::cout << "Find 1 after removal: " << hashTable.find(1) << std::endl; // false

    return 0;
}
