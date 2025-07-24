#include <iostream>
#include <vector>
#include <unordered_map>

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
    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
    {
        // 构建中序遍历的值到索引的映射，方便快速查找
        for (int i = 0; i < inorder.size(); ++i)
        {
            in_map[inorder[i]] = i;
        }
        return build(preorder, 0, preorder.size() - 1,
                     inorder, 0, inorder.size() - 1);
    }

private:
    unordered_map<int, int> in_map; // 存储中序遍历的值和索引的映射

    TreeNode *build(vector<int> &preorder, int pre_start, int pre_end,
                    vector<int> &inorder, int in_start, int in_end)
    {
        if (pre_start > pre_end || in_start > in_end)
        {
            return nullptr;
        }

        // 前序遍历的第一个节点是根节点
        int root_val = preorder[pre_start];
        TreeNode *root = new TreeNode(root_val);

        // 在中序遍历中找到根节点的位置
        int in_root = in_map[root_val];
        int left_size = in_root - in_start; // 左子树的节点数量

        // 递归构建左子树
        // 前序遍历中左子树的范围：[pre_start + 1, pre_start + left_size]
        // 中序遍历中左子树的范围：[in_start, in_root - 1]
        root->left = build(preorder, pre_start + 1, pre_start + left_size,
                           inorder, in_start, in_root - 1);

        // 递归构建右子树
        // 前序遍历中右子树的范围：[pre_start + left_size + 1, pre_end]
        // 中序遍历中右子树的范围：[in_root + 1, in_end]
        root->right = build(preorder, pre_start + left_size + 1, pre_end,
                            inorder, in_root + 1, in_end);

        return root;
    }
};

// 辅助函数：打印二叉树的前序遍历（用于验证）
void printPreorder(TreeNode *root)
{
    if (!root)
        return;
    cout << root->val << " ";
    printPreorder(root->left);
    printPreorder(root->right);
}

// 辅助函数：打印二叉树的中序遍历（用于验证）
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
    vector<int> preorder = {3, 9, 20, 15, 7};
    vector<int> inorder = {9, 3, 15, 20, 7};

    Solution solution;
    TreeNode *root = solution.buildTree(preorder, inorder);

    cout << "重建后的前序遍历: ";
    printPreorder(root);
    cout << endl;

    cout << "重建后的中序遍历: ";
    printInorder(root);
    cout << endl;

    return 0;
}
