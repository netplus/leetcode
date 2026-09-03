// ============================================================================
// LC-22：括号生成
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 2 周 / 第 13 天
// ----------------------------------------------------------------------------
// 题目描述：
// 数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且有效的括号组合。
//
// 约束与要求：
//   - 1 <= n <= 8
//
// 复杂度目标：O(4^n / sqrt(n))。
//
// ----------------------------------------------------------------------------
// 解法精讲｜约束生成：只扩展仍可能合法的括号前缀
// - 核心要点：
//   1. 思路起点：合法前缀必须始终满足 usedClose<=usedOpen<=n；回溯只添加不破坏该条件的字符，无需生成后再过滤。
//   2. 执行逻辑：1. open<n 时可添加左括号；2. close<open 时可添加右括号；3. 长度达到 2n 时记录。
//   3. 为什么这样做：所有生成路径的每个前缀都合法，终点左右括号各 n 个；任意合法括号串的逐字符选择也始终满足两个条件，因此不会漏解。
// - 边界与易错点：右括号条件必须是 close<open；到终点才记录；字符串 push 后要 pop 回退。
// - 举一反三：构造型搜索应尽早编码前缀合法性；同理可生成 IP、表达式、受限序列和括号的多类型变体。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   输出：所有组合，每行一个。
// test.in 的预期输出：((())) | (()()) | (())() | ()(()) | ()()()
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
    vector<string> answer;
    string path;

    void search(int n, int open, int close) {
        if (static_cast<int>(path.size()) == 2 * n) {
            answer.push_back(path);
            return;
        }
        if (open < n) {
            path.push_back('(');
            search(n, open + 1, close);
            path.pop_back();
        }
        if (close < open) {
            path.push_back(')');
            search(n, open, close + 1);
            path.pop_back();
        }
    }

public:
    vector<string> generateParenthesis(int n) {
        answer.clear();
        path.clear();
        search(n, 0, 0);
        return answer;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    auto res = sol.generateParenthesis(n);
    for (auto& s : res) cout << s << "\n";
    return 0;
}

