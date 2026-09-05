"""Reviewed C++ key-code comments for Week 1 / Day 5.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK1_DAY5 = {
    14: r'''// ---------- Solution ----------
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        // 公共前缀不可能比任意一个字符串更长，因此先用 strs[0]
        // 作为“最长可能前缀”的上界，随后逐列让其他字符串来验收。
        const string& first = strs[0];

        for (int i = 0; i < static_cast<int>(first.size()); ++i) {
            // 进入第 i 列时，不变量是：所有字符串的 [0,i) 已经逐列验证完全相同。
            // 因此前面 i 个字符已经确定属于公共前缀；现在只需判断第 i 列能否继续延伸。
            for (int j = 1; j < static_cast<int>(strs.size()); ++j) {
                // 必须先判断 i 是否已经到达当前字符串末尾，再访问 strs[j][i]。
                // “有人没有第 i 个字符”与“第 i 个字符不同”都会立即终止前缀；
                // 一旦这里失败，任何更长前缀都不可能成立，所以可以直接返回 [0,i)。
                if (i == static_cast<int>(strs[j].size()) || strs[j][i] != first[i]) {
                    return first.substr(0, i);
                }
            }
        }

        // 若 first 的每一列都通过所有字符串验收，first 本身就是最长公共前缀。
        return first;
    }
};''',
}
