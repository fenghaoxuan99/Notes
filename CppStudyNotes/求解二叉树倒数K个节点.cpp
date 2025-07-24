#include <iostream>

using namespace std;

// 二叉树节点定义
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution
{
public:
    TreeNode *findKthFromEnd(TreeNode *root, int k)
    {
        int count = 0;
        TreeNode *result = nullptr;
        reverseInorder(root, k, count, result);
        return result;
    }

private:
    void reverseInorder(TreeNode *node, int k, int &count, TreeNode *&result)
    {
        if (node == nullptr || count >= k)
        {
            return;
        }

        // 先遍历右子树
        reverseInorder(node->right, k, count, result);

        // 访问当前节点
        count++;
        if (count == k)
        {
            result = node;
            return; // 提前终止遍历
        }

        // 再遍历左子树
        reverseInorder(node->left, k, count, result);
    }
};

// 辅助函数：创建测试用的二叉树
TreeNode *createTestTree()
{
    TreeNode *root = new TreeNode(5);
    root->left = new TreeNode(3);
    root->right = new TreeNode(7);
    root->left->left = new TreeNode(2);
    root->left->right = new TreeNode(4);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(8);
    return root;
}

// 辅助函数：打印中序遍历结果（用于验证）
void printInorder(TreeNode *root)
{
    if (!root)
        return;
    printInorder(root->left);
    cout << root->val << " ";
    printInorder(root->right);
}

int main()
{
    TreeNode *root = createTestTree();

    cout << "中序遍历结果: ";
    printInorder(root);
    cout << endl;

    Solution solution;
    int k = 3;
    TreeNode *kthNode = solution.findKthFromEnd(root, k);

    if (kthNode)
    {
        cout << "中序遍历倒数第" << k << "个节点是: " << kthNode->val << endl;
    }
    else
    {
        cout << "无效的k值或空树" << endl;
    }

    return 0;
}
