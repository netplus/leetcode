// ============================================================================
// LC-1011：在 D 天内送达包裹的能力
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 23 天
// ----------------------------------------------------------------------------
// 题目描述：
// 传送带上的包裹必须在 days 天内从一个港口运送到另一个港口。
// 传送带上的第 i 个包裹的重量为 weights[i]。
// 每一天，我们都会按给出重量（weights）的顺序往传送带上装载包裹。
// 我们装载的重量不会超过船的最大运载重量。
// 返回能在 days 天内将传送带上的所有包裹送达的船的最低运载能力。
//
// 约束与要求：
//   - 1 <= days <= weights.length <= 5 * 10^4
//   - 1 <= weights[i] <= 500
//
// 复杂度目标：O(n log S)。
//
// ----------------------------------------------------------------------------
// 解法精讲｜答案二分：按原顺序装载的最小容量
// - 核心要点：
//   1. 思路起点：容量越大，按给定顺序运完所需天数单调不增；给定容量时，每天尽可能多装是使用天数最少的贪心方案。
//   2. 执行逻辑：1. 容量下界=max(weights)，上界=sum(weights)；2. 顺序累加，超容量就开启新一天；3. 需要天数<=days 时缩小容量，否则增大。
//   3. 为什么这样做：正重量下延迟换天不会增加未来负担，因此贪心得到给定容量的最少天数；可行容量构成连续后缀，二分找到首个可行值。
// - 边界与易错点：包裹顺序不能改变；单件重量决定容量下界；总和与中点用 long long 防溢出。
// - 举一反三：它与分割数组最大和本质相同：把连续序列切成有限段并最小化最大段负载。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n D。
//   第 2 行：n 个以空格分隔的整数。
//   输出：运载能力的最小值。
// test.in 的预期输出：15
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int days) {
        long long left = *max_element(weights.begin(), weights.end());
        long long right = accumulate(weights.begin(), weights.end(), 0LL);
        auto daysNeeded = [&](long long capacity) {
            int used = 1;
            long long load = 0;
            for (int weight : weights) {
                if (load + weight > capacity) {
                    ++used;
                    load = 0;
                }
                load += weight;
            }
            return used;
        };
        while (left < right) {
            long long middle = left + (right - left) / 2;
            if (daysNeeded(middle) <= days) right = middle;
            else left = middle + 1;
        }
        return static_cast<int>(left);
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.shipWithinDays(a, k) << "\n";
    return 0;
}

