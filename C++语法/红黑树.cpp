#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <stack>
#include <queue>

template <typename T>
class RBTree
{
public:
    RBTree() : root(nullptr) {}
    void insert(const T &value)
    {
        Node *newNode = new Node(value);
        if (root == nullptr)
        {
            root = newNode;
            root->color = balck; // 根节点为黑色
            return;
        }

        Node *parent = nullptr;
        Node *current = root;
        while (current != nullptr)
        {
            parent = current;
            if (newNode->data < current->data)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }

        newNode->parent = parent;
        newnode->color = red; // 新插入的节点为红色
        if (newNode->data < parent->data)
        {
            parent->left = newNode;
        }
        else
        {
            parent->right = newNode;
        }

        // 插入后修正红黑树性质
        if (getcolor(parent) == red)
        {
            this->fixAfterInsert(newNode);
        }
    }

    void remove(const T &value)
    {
        if (root = nullptr)
        {
            return;
        }

        Node *current = root;

        while (current != nullptr)
        {
            if (value == current->data)
            {
                break;
            }
            else if (value < current->data)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        if (current == nullptr)
        {
            return; // 找不到要删除的节点
        }

        if (current->left != nullptr && current->right != nullptr)
        {
            Node *pre = current->left;
            while (pre->right != nullptr)
            {
                pre = pre->right;
            }
            current->data = pre->data;
            current = pre;
        }
        Node *child = current->left != nullptr ? current->left : current->right;
        if (child != nullptr)
        {
            child->parent = current->parent;
            if (current->parent == nullptr)
            {
                root = child;
            }
            else if (current == current->parent->left)
            {
                current->parent->left = child;
            }
            else
            {
                current->parent->right = child;
            }
        }

        Color originalColor = current->color;

        if (originalColor == balck)
        {
            while (child != root && getColor(child) == balck)
            {
                if (child == child->parent->left)
                {
                    Node *sibling = child->parent->right;
                    if (getColor(sibling) == red)
                    {
                        setColor(sibling, balck);
                        setColor(child->parent, red);
                        leftRotate(child->parent);
                        sibling = child->parent->right;
                    }
                    if (getColor(sibling->left) == balck && getColor(sibling->right) == balck)
                    {
                        setColor(sibling, red);
                        child = child->parent;
                    }
                    else
                    {
                        if (getColor(sibling->right) == balck)
                        {
                            setColor(sibling->left, balck);
                            setColor(sibling, red);
                            rightRotate(sibling);
                            sibling = child->parent->right;
                        }
                        setColor(sibling, getColor(child->parent));
                        setColor(child->parent, balck);
                        setColor(sibling->right, balck);
                        leftRotate(child->parent);
                        break;
                    }
                }
                else
                {
                    Node *sibling = child->parent->left;
                    if (getColor(sibling) == red)
                    {
                        setColor(sibling, balck);
                        setColor(child->parent, red);
                        rightRotate(child->parent);
                        sibling = child->parent->left;
                    }
                    if (getColor(sibling->right) == balck && getColor(sibling->left) == balck)
                    {
                        setColor(sibling, red);
                        child = child->parent;
                    }
                    else
                    {
                        if (getColor(sibling->left) == balck)
                        {
                            setColor(sibling->right, balck);
                            setColor(sibling, red);
                            leftRotate(sibling);
                            sibling = child->parent->left;
                        }
                        setColor(sibling, getColor(child->parent));
                        setColor(child->parent, balck);
                        setColor(sibling->left, balck);
                        rightRotate(child->parent);
                        break;
                    }
                }
            }
            if (child != nullptr)
            {
                setColor(child, balck);
            }
        }
    }

private:
    enum Color
    {
        red,
        balck
    };

    struct Node
    {
        Node(T value = T(), Node *left = nullptr, Node *right = nullptr, Node *parent = nullptr, Color color = balck)
            : data(value), left(left), right(right), parent(parent), color(color) {}
        T data;
        Node *left;
        Node *right;
        Node *parent;
        Color color; // 新增颜色属性
    };
    Node *root; // 根节点

    Color getColor(Node *node) const
    {
        return node == nullptr ? balck : node->color;
    }

    void setColor(Node *node, Color color)
    {
        if (node != nullptr)
        {
            node->color = color;
        }
    }

    Node *getParent(Node *node) const
    {
        return node == nullptr ? nullptr : node->parent;
    }

    Node *getleft(Node *node) const
    {
        return node == nullptr ? nullptr : node->left;
    }

    Node *getright(Node *node) const
    {
        return node == nullptr ? nullptr : node->right;
    }

    void *leftRotate(Node *father)
    {
        Node *child = father->right;
        child->parent = father->parent;
        if (father->parent == nullptr)
        {
            root = child;
        }
        else
        {
            if (father == father->parent->left)
            {
                father->parent->left = child;
            }
            else
            {
                father->parent->right = child;
            }
        }

        father->right = child->left;
        if (child->left != nullptr)
        {
            child->left->parent = father;
        }

        child->left = father;
        father->parent = child;
    }

    void rightRotate(Node *father)
    {
        Node *child = father->left;
        child->parent = father->parent;
        if (father->parent == nullptr)
        {
            root = child;
        }
        else
        {
            if (father == father->parent->left)
            {
                father->parent->left = child;
            }
            else
            {
                father->parent->right = child;
            }
        }
        father->left = child->right;
        if (child->right != nullptr)
        {
            child->right->parent = father;
        }

        child->right = father;
        father->parent = child;
    }

    void fixAfterInsert(Node *node)
    {

        while (getColor(node->parent) == red)
        {
            if (node->parent == node->parent->parent->left)
            {
                Node *uncle = node->parent->parent->right;
                if (getColor(uncle) == red)
                {
                    setColor(node->parent, balck);
                    setColor(uncle, balck);
                    setColor(node->parent->parent, red);
                    node = node->parent->parent;
                }
                else
                {
                    if (node == node->parent->right)
                    {
                        node = node->parent;
                        leftRotate(node);
                    }
                    setColor(node->parent, balck);
                    setColor(node->parent->parent, red);
                    rightRotate(node->parent->parent);
                }
            }
            else
            {
                Node *uncle = node->parent->parent->left;
                if (getColor(uncle) == red)
                {
                    setColor(node->parent, balck);
                    setColor(uncle, balck);
                    setColor(node->parent->parent, red);
                    node = node->parent->parent;
                }
                else
                {
                    if (node == node->parent->left)
                    {
                        node = node->parent;
                        rightRotate(node);
                    }
                    setColor(node->parent, balck);
                    setColor(node->parent->parent, red);
                    leftRotate(node->parent->parent);
                }
            }
        }
        setColor(root, balck); // 根节点始终为黑色
    };

    void fixAfterRemove(Node *node)
    {
        while (getColor(node) == balck)
        {
            if (node == node->parent->left)
            {
                Node *sibling = node->parent->right;
                if (getColor(sibling) == red)
                {
                    setColor(sibling, balck);
                    setColor(node->parent, red);
                    leftRotate(node->parent);
                    sibling = node->parent->right;
                }
                if (getColor(sibling->left) == balck && getColor(sibling->right) == balck)
                {
                    setColor(sibling, red);
                    node = node->parent;
                }
                else
                {
                    if (getColor(sibling->right) == balck)
                    {
                        setColor(sibling->left, balck);
                        setColor(sibling, red);
                        rightRotate(sibling);
                        sibling = node->parent->right;
                    }
                    setColor(sibling, getColor(node->parent));
                    setColor(node->parent, balck);
                    setColor(sibling->right, balck);
                    leftRotate(node->parent);
                    break;
                }
            }
            else
            {
                Node *sibling = node->parent->left;
                if (getColor(sibling) == red)
                {
                    setColor(sibling, balck);
                    setColor(node->parent, red);
                    rightRotate(node->parent);
                    sibling = node->parent->left;
                }
                if (getColor(sibling->right) == balck && getColor(sibling->left) == balck)
                {
                    setColor(sibling, red);
                    node = node->parent;
                }
                else
                {
                    if (getColor(sibling->left) == balck)
                    {
                        setColor(sibling->right, balck);
                        setColor(sibling, red);
                        leftRotate(sibling);
                        sibling = node->parent->left;
                    }
                    setColor(sibling, getColor(node->parent));
                    setColor(node->parent, balck);
                    setColor(sibling->left, balck);
                    rightRotate(node->parent);
                    break;
                }
            }
        }
        if (node != nullptr)
        {
            setColor(node, balck); // 设置为黑色
        }
    }
};

int main()
{
}
