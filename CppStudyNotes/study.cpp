
#include <iostream>
#include <vector>
#include <algorithm>

template <typename... Args>
std::string concat(Args... args)
{
    std::string result;
    (void)std::initializer_list<int>{(result += args, 0)...};
    return result;
}

template <typename... Funcs>
int chain_call(int value, Funcs... funcs)
{
    // 使用递归展开参数包
    int result = value;
    // 使用逗号运算符和初始化列表展开
    (void)((result = funcs(result), 0) + ...);
    return result;
}
int main()
{
    // 使用可变参数模板进行字符串连接
    std::string result = concat("Hello, ", "world!", " How are you?");
    std::cout << "Concatenated string: " << result << std::endl;

    // 使用可变参数模板进行函数链式调用
    auto add = [](int x)
    { return x + 1; };
    auto multiply = [](int x)
    { return x * 2; };
    int value = 5;
    int final_result = chain_call(value, add, multiply);
    std::cout << "Final result after chain calls: " << final_result << std::endl;
    return 0;
}