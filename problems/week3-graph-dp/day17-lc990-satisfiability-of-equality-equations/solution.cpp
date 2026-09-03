// ============================================================================
// LC-990：等式方程的可满足性
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 3 周 / 第 17 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个由表示变量之间关系的字符串方程组成的数组，每个字符串方程 equations[i] 的长度为 4，并采用两种不同的形式之一："a==b" 或 "a!=b"。
// 在这里，a 和 b 是小写字母（不一定不同），表示单字母变量名。
// 只有当可以将整数分配给变量名，以便满足所有给定的方程时才返回 true，否则返回 false。
//
// 约束与要求：
//   - 1 <= equations.length <= 500
//   - equations[i].length == 4
//   - equations[i][0] 和 equations[i][3] 是小写字母
//   - equations[i][1] 要么是 '='，要么是 '!'
//   - equations[i][2] 是 '='
//
// 复杂度目标：O(n) 并查集。
//
// ----------------------------------------------------------------------------
// 解法精讲｜并查集处理等价与不等约束
// - 核心要点：
//   1. 思路起点：等式具有传递性，先把所有 a==b 合并为等价类；再检查每条 a!=b 是否错误地落在同一类。
//   2. 执行逻辑：1. 初始化 26 个字母集合；2. 第一遍只合并 ==；3. 第二遍检查 != 的两个根是否相同。
//   3. 为什么这样做：所有等式的传递闭包由并查集表示；若不等式两端同根则约束矛盾，若所有不等式跨集合，可给各集合赋不同值满足全部条件。
// - 边界与易错点：必须先统一处理等式，否则按输入顺序检查不等式可能过早得出结论；x!=x 必然失败。
// - 举一反三：等价类归并后再验证互斥约束，是符号约束、别名分析和账户身份归一化的常见套路。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n (方程数量)。
//   接下来 n 行：每行一个方程字符串。
//   输出：方程组可满足时输出 1，否则输出 0。
// test.in 的预期输出：1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    bool equationsPossible(vector<string>& equations) {
        array<int, 26> parent;
        iota(parent.begin(), parent.end(), 0);
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };
        for (const string& equation : equations) {
            if (equation[1] == '=') {
                int a = find(equation[0] - 'a');
                int b = find(equation[3] - 'a');
                parent[a] = b;
            }
        }
        for (const string& equation : equations) {
            if (equation[1] == '!' && find(equation[0] - 'a') == find(equation[3] - 'a')) {
                return false;
            }
        }
        return true;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<string> eq(n);
    for (int i = 0; i < n; ++i) cin >> eq[i];
    cout << (sol.equationsPossible(eq) ? 1 : 0) << "\n";
    return 0;
}

