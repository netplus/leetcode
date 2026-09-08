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
//   4. std::cin.rdbuf()：取得 std::cin 当前使用的底层 streambuf；
//   5. std::ostringstream：把这个 streambuf 中读出的字符“写入”字符串缓冲区，
//      从而一次性得到完整 stdin 文本，因此支持跨行的 [] 输入。
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
    // ------------------------------------------------------------------------
    // 为什么这里不是：
    //
    //     std::getline(std::cin, input);
    //
    // 因为这个 getline 只读取到当前行的 '\n' 为止。
    // 对单行输入：
    //
    //     [1,-2]
    //
    // 没问题；但对多行输入：
    //
    //     [
    //         1,
    //         -2
    //     ]
    //
    // 第一次 getline 只能得到 "["，后面的内容不会进入 input。
    // ------------------------------------------------------------------------
    //
    // 这里的目标其实是：
    //
    //     “把 std::cin 中剩余的所有字符，包括换行，收集成一个 std::string。”
    //
    // 因此使用：
    //
    //     std::ostringstream buffer;
    //     buffer << std::cin.rdbuf();
    //
    // 可以把它理解为下面的数据流：
    //
    //     stdin
    //       ↓
    //     std::cin
    //       ↓
    //     std::cin.rdbuf()        // cin 背后的字符缓冲区
    //       ↓  operator<<
    //     std::ostringstream      // 接收字符，并存入自己的字符串缓冲区
    //       ↓  buffer.str()
    //     std::string
    //
    // 关键点是“流的方向”。
    //
    // std::ostringstream 中的 'o' 是 output：
    // 它是一个“输出流”。所谓输出，不是指一定输出到屏幕，而是指代码通过
    // operator<< 把数据写入这个流。ostringstream 的目标介质恰好是一块字符串缓冲区。
    //
    //     buffer << something;
    //
    // 就是在向 buffer 写数据。因此它正好适合做：
    //
    //     cin 的字符流  ->  字符串缓冲区
    //
    // ------------------------------------------------------------------------
    // 为什么不是 std::istringstream？
    //
    // std::istringstream 中的 'i' 是 input：它的典型用途正好相反——
    // 先已经有一个 std::string，再把这个字符串当作输入源，用 operator>> / getline
    // 从里面解析数据。例如：
    //
    //     std::string text = "1 2 3";
    //     std::istringstream in(text);
    //
    //     int x;
    //     in >> x;                // 从字符串流中“读”出 1
    //
    // 数据方向是：
    //
    //     std::string
    //       ↓
    //     std::istringstream
    //       ↓  operator>> / getline
    //     程序变量
    //
    // 而当前 main() 还没有完整字符串；我们正处于“把 cin 收集成字符串”的阶段，
    // 所以 istringstream 并不适合作为收集容器。
    //
    // 简单记忆：
    //
    //     ostringstream：程序向流里写，最后用 str() 取出字符串。
    //     istringstream：先给流一个字符串，再从流里读、解析。
    //     stringstream ：同时支持读和写；功能更全，但这里并不需要双向能力。
    //
    // 因此这里选择 ostringstream，是因为它精确表达了当前需求：
    // “把输入流的全部字符写入一个字符串缓冲区”。
    // ------------------------------------------------------------------------
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
