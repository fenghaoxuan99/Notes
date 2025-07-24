#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <stack>
#include <queue>

template <typename T>
class AVLTree
{
public:
    AVLTree() : root(nullptr) {}

    // 插入操作 ，参数value为要插入的值，返回插入后的根节点
    void insert(T value)
    {
        root = insert(root, value);
    }
    void remove(T value)
    {
        root = remove(root, value);
    }

    // 中序遍历
    void inorderTraversal(std::function<void(T)> visit)
    {
        inorderTraversal(root, visit);
    }

private:
    struct Node
    {
        T data;
        Node *left;
        Node *right;
        int height;
        Node(T value = T()) : data(value), left(nullptr), right(nullptr), height(1) {}
    };
    Node *root;

    // 获取节点的高度
    int getHeight(Node *node)
    {
        return node == nullptr ? 0 : node->height;
    }
    // 右旋转操作 ，参数y为旋转的轴，返回旋转后的根节点

    Node *rightRotate(Node *father)
    {
        Node *child = father->left;
        father->left = child->right;
        child->right = father;

        father->height = std::max(getHeight(father->left), getHeight(father->right)) + 1;
        child->height = std::max(getHeight(child->left), getHeight(child->right)) + 1;

        return child;
    }
    // 左旋转操作 ，参数y为旋转的轴，返回旋转后的根节点
    Node *leftRotate(Node *father)
    {
        Node *child = father->right;
        father->right = child->left;
        child->left = father;

        father->height = std::max(getHeight(father->left), getHeight(father->right)) + 1;
        child->height = std::max(getHeight(child->left), getHeight(child->right)) + 1;

        return child;
    }

    // 左平衡操作
    Node *leftBalance(Node *node)
    {

        node->left = leftRotate(node->left);
        return rightRotate(node); // 右旋转
    }

    // 右平衡操作
    Node *rightBalance(Node *node)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node); // 左旋转
    }

    Node *insert(Node *node, T value)
    {
        if (node == nullptr)
        {
            return new Node(value);
        }
        if (value < node->data)
        {
            node->left = insert(node->left, value);
            if (getHeight(node->left) - getHeight(node->right) > 1)
            {
                if (getHeight(node->left->left) > getHeight(node->left->right))
                {
                    node = rightRotate(node); // 右旋转
                }
                else
                {
                    node = leftBalance(node); // 左平衡
                }
            }
        }
        else if (value > node->data)
        {
            node->right = insert(node->right, value);
            if (getHeight(node->right) - getHeight(node->left) > 1)
            {
                if (getHeight(node->right->right) > getHeight(node->right->left))
                {
                    node = leftRotate(node); // 左旋转
                }
                else
                {
                    node = rightBalance(node); // 右平衡
                }
            }
        }
        else
        {
            ;
        }
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

        return node;
    }

    Node *remove(Node *node, const T &value)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (node->data > value)
        {
            node->left = remove(node->left, value);
            if (getHeight(node->right) - getHeight(node->left) > 1)
            {
                if (getHeight(node->right->right) > getHeight(node->right->left))
                {
                    node = leftRotate(node); // 左旋转
                }
                else
                {
                    node = rightBalance(node); // 右平衡
                }
            }
        }
        else if (node->data < value)
        {
            node->right = remove(node->right, value);
            if (getHeight(node->left) - getHeight(node->right) > 1)
            {
                if (getHeight(node->left->left) > getHeight(node->left->right))
                {
                    node = rightRotate(node); // 右旋转
                }
                else
                {
                    node = leftBalance(node); // 左平衡
                }
            }
        }
        else
        {
            if (node->left != nullptr && node->right != nullptr)
            {

                if (getHeight(node->left) > getHeight(node->right))
                {
                    Node *maxNode = node->left;
                    while (maxNode->right != nullptr)
                    {
                        maxNode = maxNode->right;
                    }
                    node->data = maxNode->data;
                    node->left = remove(node->left, maxNode->data);
                }
                else
                {
                    Node *minNode = node->right;
                    while (minNode->left != nullptr)
                    {
                        minNode = minNode->left;
                    }
                    node->data = minNode->data;
                    node->right = remove(node->right, minNode->data);
                }
            }
            else
            {
                Node *temp = node->left != nullptr ? node->left : node->right;
                delete node;
                return temp;
            }
        }
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

        return node;
    }

    void inorderTraversal(Node *node, std::function<void(T)> visit)
    {
        if (node == nullptr)
            return;
        inorderTraversal(node->left, visit);
        visit(node->data);
        inorderTraversal(node->right, visit);
    }
};

int main()
{
    AVLTree<int> avl;
    for (int i = 0; i < 10; i++)
    {
        avl.insert(i);
    }
    avl.remove(5);
    avl.inorderTraversal([](int value)
                         { std::cout << value << " "; });
}
