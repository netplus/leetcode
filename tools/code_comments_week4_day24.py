"""Reviewed C++ key-code comments for Week 4 / Day 24.

Entries are added only after an independent per-problem review.
"""

CODE_COMMENTS_WEEK4_DAY24 = {
    455: r'''// ---------- Solution ----------
class Solution {
public:
    int findContentChildren(vector<int>& g, vector<int>& s) {
        // 排序后，child 指向最小未满足胃口，cookie 指向最小未处理饼干；
        // 这样每一步都能判断当前最小资源是否还有未来价值。
        sort(g.begin(), g.end());
        sort(s.begin(), s.end());
        int child = 0, cookie = 0;

        while (child < static_cast<int>(g.size()) && cookie < static_cast<int>(s.size())) {
            // 当前最小饼干若够用，立刻匹配给当前最小胃口是安全的：
            // 把更大的饼干留给更难满足的孩子，不会减少任何最优方案的匹配数。
            if (s[cookie] >= g[child]) ++child;

            // 无论是否匹配，这块饼干都已经处理完：若太小，它连最小剩余胃口都满足不了，
            // 更不可能服务后面更大的胃口；若已匹配，则每块饼干也只能使用一次。
            ++cookie;
        }
        // child 每增加一次就固定了一名已满足孩子，因此它本身就是最大匹配数。
        return child;
    }
};''',

    55: r'''// ---------- Solution ----------
class Solution {
public:
    bool canJump(vector<int>& nums) {
        // farthest 压缩了所有已知路径：当前已经证明连续前缀 [0,farthest] 全部可达。
        long long farthest = 0;
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            // 只有已经落在可达前缀中的位置才有资格贡献下一跳；
            // 第一次出现 i>farthest 时形成永久缺口，后面的未达位置不可能反过来扩张前沿。
            if (i > farthest) return false;

            // 把当前可达起点 i 的最远落点并入总前沿；中间所有更短落点都已经被连续前缀包含。
            farthest = max(farthest, static_cast<long long>(i) + nums[i]);
            if (farthest >= static_cast<int>(nums.size()) - 1) return true;
        }
        return true;
    }
};''',

    45: r'''// ---------- Solution ----------
class Solution {
public:
    int jump(vector<int>& nums) {
        // currentEnd 是“用当前 jumps 次跳跃能覆盖的这一层右边界”；
        // farthest 是扫描这一整层时，下一跳最多能把边界推进到哪里。
        int jumps = 0, currentEnd = 0, farthest = 0;

        // 不扫描最后一个下标：一旦进入终点就已经完成，若在终点再次触发层尾会多计一跳。
        for (int i = 0; i + 1 < static_cast<int>(nums.size()); ++i) {
            // 层内只收集所有起点的下一跳最远覆盖，不立即决定从哪个具体位置起跳。
            farthest = max(farthest, i + nums[i]);

            // 只有扫描完当前层的全部起点（i==currentEnd）时，才真正消耗一次跳跃，
            // 并把下一层边界整体更新为这一层汇总出的 farthest。
            if (i == currentEnd) {
                ++jumps;
                currentEnd = farthest;
            }
        }
        return jumps;
    }
};''',

    134: r'''// ---------- Solution ----------
class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        // total 记录整圈净收益，决定“是否存在解”；
        // currentTank 只记录当前候选 start 从起点走到 i 的局部剩余油量。
        int total = 0, currentTank = 0, start = 0;
        for (int i = 0; i < static_cast<int>(gas.size()); ++i) {
            const int gain = gas[i] - cost[i];
            total += gain;
            currentTank += gain;

            // 若 start..i 第一次累加成负数，不只是 start 失败：
            // 因为 start 到任意中间点此前前缀都非负，从其中任何 j∈(start,i] 重新出发，
            // 到 i 的净收益只会更差，因此这整段起点都可永久排除，下一候选直接跳到 i+1。
            if (currentTank < 0) {
                start = i + 1;
                currentTank = 0;
            }
        }

        // 局部失败时不能清空 total；只有整圈总油量不少于总消耗时才可能存在起点。
        // total>=0 时，所有早于 start 的候选已经被逐段证明失败，最后留下的 start 即可走完整圈。
        return total >= 0 ? start : -1;
    }
};''',
}
