"""Reviewed C++ key-code comments for Week 2 / Day 12.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK2_DAY12 = {
    20: r'''// ---------- Solution ----------
class Solution {
public:
    bool isValid(string s) {
        // openings 保存“已经开启、但尚未闭合”的左括号；
        // 栈顶就是最近开启的结构，也是下一只右括号唯一允许关闭的对象。
        stack<char> openings;

        for (char c : s) {
            if (c == '(' || c == '[' || c == '{') {
                // 左括号开启新的嵌套上下文，先欠下一笔等待未来关闭的账。
                openings.push(c);
                continue;
            }

            // 当前字符既然是右括号，就必须关闭一个已有上下文；
            // 若栈已空，说明没有任何左括号可与它配对，立即非法。
            if (openings.empty()) return false;

            // 把当前右括号映射成“它唯一允许匹配的左括号类型”。
            const char expected = (c == ')') ? '(' : (c == ']') ? '[' : '{';

            // 合法嵌套要求最近开启的结构最先关闭；
            // 即使栈更深处存在同类型左括号，也不能越过错误的栈顶去匹配它。
            if (openings.top() != expected) return false;
            openings.pop();
        }

        // 扫描结束后仍有左括号留栈，说明这些上下文从未被关闭，同样非法。
        return openings.empty();
    }
};''',
}
