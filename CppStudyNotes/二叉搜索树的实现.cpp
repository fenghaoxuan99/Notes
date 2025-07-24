#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <stack>
#include <queue>
template <typename T, typename Compare = std::less<T>>
class BSTree
{
public:
    BSTree() : root(nullptr) {}

    void insert(const T &value)
    {
        if (root == nullptr)
        {
            root = new Node(value);
            return;
        }
        Node *current = root;
        Node *parent = nullptr;
        while (current != nullptr)
        {
            parent = current;
            if (Compare()(value, current->data))
            {
                current = current->left;
            }
            else if (Compare()(current->data, value))
            {
                current = current->right;
            }
            else
            {
                // Value already exists in the tree, do not insert duplicates
                return;
            }
        }

        if (Compare()(value, parent->data))
        {
            parent->left = new Node(value);
        }
        else
        {
            parent->right = new Node(value);
        }
    }

    void remove(const T &value)
    {
        if (root == nullptr)
            return;
        Node *current = root;
        Node *parent = nullptr;
        while (current != nullptr && current->data != value)
        {
            parent = current;
            if (Compare()(value, current->data))
            {
                current = current->left;
            }
            else if (Compare()(current->data, value))
            {
                current = current->right;
            }
            else
            {
                // Value found, proceed to remove it
                break;
            }
        }
        if (current == nullptr)
            return; // Value not found

        if (current->left != nullptr && current->right != nullptr)
        {

            Node *pre = current->left;
            Node *parent = current;

            while (pre->right != nullptr)
            {
                parent = pre;
                pre = pre->right;
            }
            current->data = pre->data;
            current = pre;
        }

        Node *child = (current->left != nullptr) ? current->left : current->right;

        if (current == root)
        {
            root = child;
        }
        else if (parent->left == current)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }
    }

    bool find(const T &value)
    {
        Node *current = root;
        while (current != nullptr)
        {
            if (Compare()(value, current->data))
            {
                current = current->left;
            }
            else if (Compare()(current->data, value))
            {
                current = current->right;
            }
            else
            {
                return true; // Value found
            }
        }
        return false; // Value not found
    }

    // 递归前序操作
    void preOrder1(std::function<void(T)> func)
    {
        preOrder(root, func);
    }

    void preOrder2(std::function<void(T)> func) // 前序遍历非递归
    {
        if (root == nullptr)
            return;
        std::stack<Node *> stack;
        stack.push(root);
        while (!stack.empty())
        {
            Node *current = stack.top();
            stack.pop();
            func(current->data);
            if (current->right != nullptr)
                stack.push(current->right);
            if (current->left != nullptr)
                stack.push(current->left);
        }
    }

    // 递归中序操作
    void inOrder1(std::function<void(T)> func)
    {

        inOrder(root, func);
    }

    void inOrder2(std::function<void(T)> func) // 中序遍历非递归
    {

        std::stack<Node *> stack;
        Node *current = root;
        while (current != nullptr || !stack.empty())
        {
            if (current != nullptr)
            {
                stack.push(current);
                current = current->left;
            }
            else
            {
                current = stack.top();
                stack.pop();
                func(current->data);
                current = current->right;
            }
        }
    }
    // 递归后序操作
    void postOrder1(std::function<void(T)> func)
    {

        postOrder(root, func);
    }

    void postOrder2(std::function<void(T)> func) // 后序遍历非递归
    {

        if (root == nullptr)
            return;
        std::stack<Node *> stack;
        std::stack<Node *> outputStack;
        Node *current = root;

        stack.push(current);
        while (!stack.empty())
        {
            current = stack.top();
            outputStack.push(current);
            stack.pop();

            if (current->left != nullptr)
                stack.push(current->left);
            if (current->right != nullptr)
                stack.push(current->right);
        }
        while (!outputStack.empty())
        {
            current = outputStack.top();
            outputStack.pop();
            func(current->data);
        }
    }

    void levelOrder1(std::function<void(T)> func)
    {
        int height = getheight(root);
        for (int i = 0; i < height; i++)
        {
            levelOrder(root, func, i);
        }
    }

    void levelOrder2(std::function<void(T)> func)
    {
        if (root == nullptr)
            return;
        std::queue<Node *> queue;
        queue.push(root);
        while (!queue.empty())
        {
            Node *current = queue.front();
            queue.pop();
            func(current->data);
            if (current->left != nullptr)
                queue.push(current->left);
            if (current->right != nullptr)
                queue.push(current->right);
        }
    }

    void remove1(const T &value)
    {
        root = remove(root, value);
    }

    bool ischildtree(BSTree<T, Compare> &tree)
    {
        if (tree.root == nullptr)
        {
            return true;
        }
        Node *current = root;
        while (current != nullptr)
        {
            if (Compare()(tree.root->data, current->data))
            {
                current = current->left;
            }
            else if (Compare()(current->data, tree.root->data))
            {
                current = current->right;
            }
            else
            {
                break; // Value found
            }
        }
        if (current == nullptr)
            return false; // Value not found

        return childtree(current, tree.root);
    }

private:
    struct Node
    {
        T data;
        Node *left;
        Node *right;
        Node(T value = T()) : data(value), left(nullptr), right(nullptr) {}
    };
    Node *root;

    Node *getLCA(Node *node, const T &value1, const T &value2)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        if (Compare()(value1, node->data) && Compare()(value2, node->data))
        {
            return getLCA(node->left, value1, value2);
        }
        if (Compare()(node->data, value1) && Compare()(node->data, value2))
        {
            return getLCA(node->right, value1, value2);
        }
        return node; // This is the LCA
    }

    void mirror(Node *node)
    {
        if (node == nullptr)
            return;
        std::swap(node->left, node->right);
        mirror(node->left);
        mirror(node->right);
    }
    // 判断是否对称
    bool isSymmetric(Node *left, Node *right)
    {
        if (left == nullptr && right == nullptr)
            return true;
        if (left == nullptr || right == nullptr)
            return false;
        return (left->data == right->data) && isSymmetric(left->left, right->right) && isSymmetric(left->right, right->left);
    }

    bool childtree(Node *father, Node *child)
    {
        if (father == nullptr && child == nullptr)
        {
            return true;
        }
        if (father == nullptr || child == nullptr)
        {
            return false;
        }
        if (father->data != child->data)
        {
            return false;
        }

        return (childtree(father->left, child->left) && childtree(father->right, child->right));
    }

    Node *insert1(Node *node, const T &value)
    {
        if (node == nullptr)
        {
            return new Node(value);
        }
        if (Compare()(value, node->data))
        {
            node->left = insert1(node->left, value);
        }
        else if (Compare()(node->data, value))
        {
            node->right = insert1(node->right, value);
        }
        return node;
    }
    // 递归前序操作
    void preOrder(Node *node, std::function<void(T)> func)
    {
        if (node == nullptr)
            return;
        func(node->data);
        preOrder(node->left, func);
        preOrder(node->right, func);
    }
    // 递归中序操作
    void inOrder(Node *node, std::function<void(T)> func)
    {
        if (node == nullptr)
            return;
        inOrder(node->left, func);
        func(node->data);
        inOrder(node->right, func);
    }
    // 递归后序操作
    void postOrder(Node *node, std::function<void(T)> func)
    {
        if (node == nullptr)
            return;
        postOrder(node->left, func);
        postOrder(node->right, func);
        func(node->data);
    }

    // 计算树的高度
    int getheight(Node *node)
    {
        if (node == nullptr)
            return 0;
        return std::max(getheight(node->left), getheight(node->right)) + 1;
    }
    // 计算节点数目
    int getnumber(Node *node)
    {
        if (node == nullptr)
            return 0;
        return getnumber(node->left) + getnumber(node->right) + 1;
    }

    // 层序遍历
    void levelOrder(Node *node, std::function<void(T)> func, int i)
    {
        if (node == nullptr)
            return;
        if (i == 0)
        {
            func(node->data);
        }
        levelOrder(node->left, func, i - 1);
        levelOrder(node->right, func, i - 1);
    }

    Node *remove(Node *node, const T &value)
    {
        if (node == nullptr)
            return nullptr;
        if (Compare()(value, node->data))
        {
            node->left = remove(node->left, value);
        }
        else if (Compare()(node->data, value))
        {
            node->right = remove(node->right, value);
        }
        else
        {
            if (node->left != nullptr && node->right != nullptr)
            {
                Node *pre = node->left;
                while (pre->right != nullptr)
                {
                    pre = pre->right;
                }
                node->data = pre->data;
                node->left = remove(node->left, pre->data);
            }
            else
            {
                Node *child = (node->left != nullptr) ? node->left : node->right;
                delete node;
                return child;
            }
        }
        return node;
    }

    bool isbstree(Node *node, Node *pre)
    {
        if (node == nullptr)
            return true;
        if (pre != nullptr && Compare()(node->data, pre->data))
            return false;
        return isbstree(node->left, pre) && isbstree(node->right, node);
    }

    int isbalance(Node *node, bool &flag)
    {
        if (node == nullptr || !flag)
            return 0;

        int left = isbalance(node->left, flag);
        int right = isbalance(node->right, flag);

        if (abs(left - right) > 1)
        {
            flag = false;
        }

        return std::max(left, right) + 1;
    }
};

int main()
{
    BSTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);

    tree.remove1(3);

    auto print = [](int node)
    { std::cout << node << " "; };

    std::cout << "Pre-order: ";
    tree.preOrder2(print);
    std::cout << std::endl;
    std::cout << "In-order: ";
    tree.inOrder1(print);
    std::cout << std::endl;
    tree.inOrder2(print);
    std::cout << std::endl;
    std::cout << "Post-order: ";
    tree.postOrder1(print);
    std::cout << std::endl;
    tree.postOrder2(print);
    std::cout << std::endl;

    std::cout << "Level-order: ";
    tree.levelOrder1(print);
    std::cout << std::endl;
    tree.levelOrder2(print);
    std::cout << std::endl;

    return 0;
}
