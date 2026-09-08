#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// ============================================================================
// 练习：把形如 "[1,-2]" 的文本输入解析为 std::vector<int>
//
// 支持示例：
//   [1,-2]
//   [1, -2, 3, 40]
//   [
//       1,
//       -2,
//       3
//   ]
//
// 练习重点：
//   1. std::getline(stream, token, ',')：按逗号切分；
//   2. trim：显式清理 token 首尾空白，不依赖 std::stoi 的前导空白处理；
//   3. std::stoi：只负责把清理后的数字字符串转换为 int；
//   4. std::cin.rdbuf()：一次读完整个标准输入，因此支持跨行的 [] 输入。
//
// 本练习假设输入格式合法，重点是字符串切分与转换，不做完整语法校验。
// ============================================================================

void trim(std::string& s)
{
    // 把普通空格、Tab、Windows/Unix 换行都视为可清理空白。
    const std::string whitespace = " \t\r\n";

    const std::size_t begin = s.find_first_not_of(whitespace);
    if (begin == std::string::npos) {
        s.clear();
        return;
    }

    const std::size_t end = s.find_last_not_of(whitespace);
    s = s.substr(begin, end - begin + 1);
}

std::vector<int> parseIntVector(std::string input)
{
    // 整体输入也可能带有首尾换行，例如从多行 stdin 读取时：
    //
    //   "\n[1, -2]\n"
    //
    // 先 trim，保证 '[' 和 ']' 位于字符串两端。
    trim(input);

    if (!input.empty() && input.front() == '[') {
        input.erase(input.begin());
    }

    if (!input.empty() && input.back() == ']') {
        input.pop_back();
    }

    std::vector<int> result;
    std::stringstream ss(input);
    std::string token;

    // getline 只负责按照 ',' 切分。
    //
    // 例如：
    //   [1,  -2,  30]
    //
    // 去掉 [] 后，getline 得到的 token 可能是：
    //   "1"
    //   "  -2"
    //   "  30"
    //
    // 多行输入时 token 还可能包含 '\n'。
    // 因此这里显式 trim，使职责保持清晰：
    //   getline -> 切分
    //   trim    -> 清理空白
    //   stoi    -> 字符串转整数
    while (std::getline(ss, token, ',')) {
        trim(token);

        if (!token.empty()) {
            result.push_back(std::stoi(token));
        }
    }

    return result;
}

int main()
{
    // 不使用 std::getline(std::cin, input)，因为它只读取一行。
    // 直接读取整个 stdin，才能同时支持：
    //
    //   [1,-2]
    //
    // 和：
    //
    //   [
    //       1,
    //       -2
    //   ]
    std::ostringstream buffer;
    buffer << std::cin.rdbuf();

    const std::vector<int> nums = parseIntVector(buffer.str());

    for (std::size_t i = 0; i < nums.size(); ++i) {
        if (i != 0) {
            std::cout << ' ';
        }
        std::cout << nums[i];
    }
    std::cout << '\n';

    return 0;
}
