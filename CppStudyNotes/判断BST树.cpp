#include <iostream>
#include <climits>

template <typename T, typename Compare = std::less<T>>
class BSTChecker
{
public:
    struct Node
    {
        T data;
        Node *left;
        Node *right;
        Node(T val) : data(val), left(nullptr), right(nullptr) {}
    };

    bool isBST(Node *root)
    {
        Node *prev = nullptr; // 用于记录中序遍历的前驱节点
        return isBSTUtil(root, prev);
    }

private:
    Compare comp; // 比较函数对象（默认std::less<T>）

    bool isBSTUtil(Node *node, Node *&prev)
    {
        if (node == nullptr)
        {
            return true; // 空树是BST
        }

        // 1. 递归检查左子树
        if (!isBSTUtil(node->left, prev))
        {
            return false;
        }

        // 2. 检查当前节点是否大于前驱节点
        if (prev != nullptr && !comp(prev->data, node->data))
        {
            return false; // 当前节点 <= 前驱节点，违反BST性质
        }
        prev = node; // 更新前驱节点为当前节点

        // 3. 递归检查右子树
        return isBSTUtil(node->right, prev);
    }
};
