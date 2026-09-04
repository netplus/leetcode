"""High-touch learning rewrites for individually reviewed problems.

The week modules remain the baseline reviewed solution registry.  This file stores
only the problems that have received the stronger learning pass defined in
AGENTS.md: visual intuition -> one-sentence core -> formula/invariant -> steps.

Keeping these overrides separate lets us improve the 106-problem workspace one
problem at a time without mechanically rewriting untouched material.  gen_all.py
consumes the merged registry through refined_data.py, so regenerated solution.cpp
files keep the learning rewrites.
"""

PEDAGOGY_OVERRIDES = {
    1: {
        "pattern": "从暴力配对到哈希索引：边扫描边查补数",
        "model": "最直接的做法是固定一个下标，再在线性候选里寻找另一个下标；但当前值 x 一旦确定，真正要找的值其实已经唯一确定为 target-x。哈希表的作用，就是给这些被反复查找的历史值建立“值 -> 下标”索引，把一次线性寻找变成平均 O(1) 查询。",
        "visual": """先从最自然的暴力想法开始。例：nums = [2, 7, 11, 15], target = 9。

固定 2 后，只能向后一个个试：

2 + 7  = 9   -> 命中
2 + 11 = 13
2 + 15 = 17

如果第一次没命中，换下一个数又要继续扫描后面的候选。
两层枚举慢的地方其实很具体：我们在反复做“到数组里找某一个确定的值”。

因为当前拿到 x 时，另一个数并不是未知的：

x + old = target
old     = target - x

所以可以反过来扫描：不再拿 x 去未来逐个试，而是给“已经看过的值”建立索引。

扫描 nums = [2, 7, 11, 15]：

历史索引 indexOf          当前 x=7
{ 2 -> 0 }                    |
      ^                       |
      +---- 查找 9-7 = 2 -----+

find(2) 直接告诉我们：值 2 以前出现过，而且下标是 0。
于是当前下标 1 和历史下标 0 就是答案。

优化的关键不是“用了 unordered_map”这件事本身，而是：
把暴力中的“反复线性找一个确定值”换成了“先建立索引，再直接查这个值”。""",
        "core": "暴力慢在每次都线性寻找一个已经确定的补数；把已扫描元素建立成“值 -> 下标”索引后，当前 x 只需去历史里直接查 target-x。",
        "formula": """当前值：x = nums[i]
需要的历史值：

x + old = target
old = target - x = complement

暴力：
对一个 x，在候选位置中逐个比较是否等于 complement
=> 一次寻找 O(n)，总共 O(n^2)

哈希索引：
indexOf[value] = 这个 value 已出现过的下标
indexOf.find(complement) = 平均 O(1)
=> 整个数组只扫描一次，平均 O(n)

扫描不变量：
处理下标 i 之前，indexOf 只保存下标 < i 的历史元素。""",
        "steps": [
            "初始化 unordered_map<int,int> indexOf；它不是只记录“出现过没有”，而是保存“值 -> 历史下标”",
            "扫描当前 nums[i]，先计算唯一需要的补数 complement = target - nums[i]",
            "用 indexOf.find(complement) 做纯查询；命中说明历史下标与当前 i 已经组成答案，立即返回",
            "只有查询未命中时，才执行 indexOf[nums[i]] = i，把当前元素加入历史，供后面的元素查询",
        ],
        "proof": "任意合法答案都有一个较早下标 i 和较晚下标 j。扫描到 j 之前，nums[i] 已经进入 indexOf；而 nums[i]=target-nums[j]，所以扫描 j 时一定能通过 complement 命中它。反过来，哈希表里只保存当前下标之前的元素，因此一次命中一定使用两个不同下标。整个过程只是把暴力枚举的配对顺序改成“较晚元素到来时寻找较早元素”，没有删掉任何合法配对。",
        "pitfalls": "最重要的是“先查后记”。例如 nums=[3,3], target=6：第一个 3 查询不到后才记录；第二个 3 再查 3，才能得到两个不同下标。若先插入当前元素再查，在第一个 3 处就可能错误地拿自己配自己。不能只用 set，因为题目要返回下标；find() 表达纯查询，operator[] 用在真正要写入 indexOf[nums[i]]=i 的地方，二者语义不要混在一起。重复值覆盖旧下标没有问题：题目保证存在唯一答案，只要保留一个合法历史下标即可。reserve() 只是减少 rehash 的性能细节，不参与正确性。",
        "transfer": "这一题最值得迁移的不是“Two Sum 就用哈希表”，而是一个更通用的优化动作：先看暴力算法在重复查什么；如果每次都在历史数据里寻找一个可直接算出的 key，就考虑为历史建立索引。LC-560 会把 key 从“元素值”升级成“前缀状态”：当前 prefix 到历史里寻找 prefix-k。实现层再记住两个技巧：纯查询用 find()/contains()，有意创建或更新状态时再用 operator[]。",
        "memory": "先从暴力问：我到底在反复找什么？算出 complement -> find 历史 -> 命中返回 -> 未命中再记录当前。",
        "code": r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> indexOf;  // 已扫描历史：value -> index
        indexOf.reserve(nums.size() * 2); // 仅减少扩容/rehash，不影响算法正确性

        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            const int complement = target - nums[i];

            // 纯查询：只问“需要的补数以前出现过吗？”，不希望缺失 key 被自动创建。
            auto it = indexOf.find(complement);
            if (it != indexOf.end()) {
                return {it->second, i};
            }

            // 查询之后才把当前元素加入历史，保证后续命中时一定使用不同下标。
            indexOf[nums[i]] = i;
        }
        return {};  // 题目保证有唯一解；保留兜底使本地接口更健壮
    }
};''',
    },

    53: {
        "visual": """走到当前位置 x 时，只比较两种连续子数组：

... [前一位置的最佳后缀] | x
          current          ^

继续接上：current + x
从这里重开：       x

旧后缀如果已经是负担，就没有必要继续带着它。""",
        "core": "走到每个位置，只决定一件事：前面的最佳后缀值得不值得继续带上。",
        "formula": """current = max(nums[i], current + nums[i])
best    = max(best, current)

current 始终表示“必须以当前位置 i 结尾”的最大子数组和；best 表示目前见过的所有 current 中最大者。""",
        "steps": [
            "用 nums[0] 初始化 current 和 best，保证子数组非空",
            "对每个 nums[i]，比较“从当前重新开始”和“接在旧 current 后面”",
            "得到新的 current 后，用它更新全局 best",
        ],
        "proof": "任意非空连续子数组一定有一个结尾位置 i。对固定 i，最优答案只可能是 nums[i] 自己，或某个以 i-1 结尾的连续后缀再接上 nums[i]；current 保留第二类里的最优者。对所有结尾位置取最大值，就是全局最大子数组和。",
        "pitfalls": "current/best 不能初始化为 0，否则全负数组会错误地选择不存在的“空子数组”；题目明确要求子数组至少含一个元素。",
        "transfer": "这题最重要的不是背 Kadane，而是学会定义“必须以当前位置结尾”的状态。最大乘积子数组、带一次删除/修改的子数组 DP 也常从这个视角出发。",
        "memory": "每个位置先选“接上还是重开”，再用这个位置的最佳答案更新全局 best。",
    },

    560: {
        "visual": """把两个前缀和画成两段累计长度：

|---------------- 当前 prefix ----------------|
|---------- 历史 prefix-k ----------|
                                      ^
                              中间连续区间的和恰好为 k

所以并不是直接枚举子数组，而是在当前累计值下寻找一个合适的历史累计值。""",
        "core": "我现在累计到了 prefix，历史上有多少次累计到了 prefix-k？",
        "formula": """当前累计 - 过去累计 = 中间连续区间的和
prefix - oldPrefix = k
oldPrefix = prefix - k""",
        "steps": [
            "frequency[0]=1，先记录“数组开始前”的空前缀",
            "扫描当前 value，执行 prefix += value，先算出“现在”累计到了多少",
            "用 find(prefix-k) 查询历史；若存在，把它的出现频次全部加入答案",
            "最后 ++frequency[prefix]，把当前 prefix 加入历史",
        ],
        "proof": "每一个以当前位置结尾、和为 k 的连续子数组，都唯一对应一个它开始位置之前的历史前缀 oldPrefix=prefix-k；同一个 prefix-k 如果出现多次，就代表多个不同起点，所以必须累加频次而不是只判断存在。",
        "pitfalls": "frequency[0]=1 才能统计从下标 0 开始的区间；哈希表必须保存频次；查询 prefix-k 使用 find()，避免 operator[] 在纯查询时插入无意义的 0；当前 prefix 必须在查询之后再加入历史；数组含负数时普通滑动窗口不成立。",
        "transfer": "凡是“连续区间 + 指定累计差”，都可以尝试把区间条件改写成“当前前缀状态 - 历史前缀状态”。LC-1248 就是先把奇数映射成 1、偶数映射成 0，再原样复用本题。",
        "memory": "先算现在 -> 再查过去 -> 最后把现在加入历史。",
        "code": r'''// ---------- Solution ----------
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<long long, int> frequency;
        frequency.reserve(nums.size() * 2 + 1);
        frequency[0] = 1;  // 空前缀：让从 0 开始的合法子数组也能被统计

        long long prefix = 0;
        int answer = 0;
        for (int value : nums) {
            prefix += value;
            auto it = frequency.find(prefix - k);  // 纯查询：不希望缺失 key 被自动插入
            if (it != frequency.end()) {
                answer += it->second;
            }
            ++frequency[prefix];                   // 记录现在：不存在时创建正是预期行为
        }
        return answer;
    }
};''',
    },

    1248: {
        "pattern": "奇偶映射 + 前缀和 + 频次哈希表（复用 LC-560）",
        "model": "先把奇数记为 1、偶数记为 0；这样“连续子数组中恰好有 k 个奇数”就直接变成“连续子数组和为 k”，可以复用 LC-560。",
        "steps": [
            "frequency[0]=1，记录数组开始前的空前缀",
            "扫描 value：奇数让 prefix 加 1，偶数让 prefix 加 0",
            "用 find(prefix-k) 查询历史频次，存在就累加到答案",
            "最后执行 ++frequency[prefix]，把当前累计奇数数加入历史",
        ],
        "proof": "prefix 表示从开头到当前位置累计遇到的奇数个数；两个 prefix 的差就是它们之间连续区间的奇数个数，因此历史 prefix-k 与当前位置一一组成恰好含 k 个奇数的子数组。",
        "pitfalls": "必须保存 prefix 的出现频次而不是只判断存在；frequency[0]=1 才能统计从下标 0 开始的答案；查询 prefix-k 用 find()，避免纯查询意外插入；偶数只是贡献 0，不是窗口边界。当前主解法 O(n) 时间、O(n) 额外空间。",
        "transfer": "这是 LC-560 的直接变形：先把题目属性映射成可累加的 0/1 状态，再统计指定前缀差；类似的“恰好 K 次满足条件”问题也可先尝试这种转换。",
        "visual": """先做一次非常简单的等价转换：

原数组： [2, 2, 1, 2, 1, 2]
奇偶值： [0, 0, 1, 0, 1, 0]

于是问题变成：上面的 0/1 数组中，有多少个连续子数组的和为 k？

|------------- 当前 prefix：累计奇数数 -------------|
|--------- 历史 prefix-k ---------|
                                      ^
                              中间恰好多出 k 个奇数

这和 LC-560 的图完全相同，只是 prefix 从“元素累计和”变成了“奇数累计个数”。""",
        "core": "先把奇数当 1、偶数当 0，然后把这题直接当成 LC-560：当前 prefix 去历史里找 prefix-k。",
        "formula": """区间内奇数个数 = 当前累计奇数数 - 过去累计奇数数
prefix - oldPrefix = k
oldPrefix = prefix - k""",
        "memory": "奇偶转 0/1 -> 先算现在 -> 查 prefix-k -> 再记录现在。",
        "advanced": "如果必须满足题头的 O(1) 额外空间目标，再学习 atMost(k)-atMost(k-1) 的滑动窗口版本；它是空间优化，不作为第一次理解本题的主线。",
        "code": r'''// ---------- Solution ----------
class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        unordered_map<int, int> frequency;
        frequency.reserve(nums.size() * 2 + 1);
        frequency[0] = 1;  // 空前缀：还没看任何元素时，累计奇数数为 0

        int prefix = 0;
        int answer = 0;
        for (int value : nums) {
            prefix += value & 1;  // 奇数 -> 1，偶数 -> 0

            // 纯查询：历史上有多少次累计奇数数等于 prefix-k？
            auto it = frequency.find(prefix - k);
            if (it != frequency.end()) {
                answer += it->second;
            }

            // 查询完成后，再把“现在”加入历史，供后续位置使用。
            ++frequency[prefix];
        }
        return answer;
    }
};''',
    },

    167: {
        "visual": """例：numbers = [2, 7, 11, 15], target = 9。

L                              R
2        7        11          15
|                              |
+------------- 17 -------------+

17 > 9。关键不是“随便把右指针左移”，而是：
15 已经和当前最小候选 2 相加都太大了；那么 15 和 7、11 相加只会更大。
所以 right=15 这一整列候选都可以一次性排除。

如果当前和 < target，则完全对称：left 已经和当前最大候选 right 相加都太小，left 这一整行都不可能再成功。""",
        "core": "每次比较两端之和，利用有序性证明其中一端已经不可能参与任何剩余答案，然后安全丢掉这一端。",
        "formula": """sum = numbers[left] + numbers[right]

sum < target:
  对任意 j <= right，numbers[left] + numbers[j] <= sum < target
  => left 不可能参与答案，left++

sum > target:
  对任意 i >= left，numbers[i] + numbers[right] >= sum > target
  => right 不可能参与答案，right--""",
        "steps": [
            "left 指向最小候选，right 指向最大候选",
            "计算 sum=numbers[left]+numbers[right]",
            "sum<target：排除 left；sum>target：排除 right",
            "sum==target：返回题目要求的 1-based 下标",
        ],
        "proof": "每次移动指针前，都能用有序性证明被丢弃的端点不可能和当前候选区间中的任何元素组成 target，因此不会漏解；每一步至少丢弃一个端点，所以最多线性扫描一次。",
        "pitfalls": "返回的是 1-based 下标；循环必须是 left<right，不能复用同一位置；如果题目没有“有序”这个前提，就不能用上述整行/整列排除逻辑，应回到哈希表等方法。",
        "transfer": "LC-15 会直接复用这个模型：先固定一个数，把三数之和降维成“在剩余有序区间里找两数之和”。双指针的本质不是两个变量，而是单调性允许一次排除一批候选。",
        "memory": "和小 -> 左端不可能，left++；和大 -> 右端不可能，right--。",
    },

    15: {
        "visual": """排序后，不要把 3Sum 看成一个全新的问题，而是先固定一个锚点：

nums = [-4, -1, -1, 0, 1, 2]
             i   L        R
            -1  -1        2

固定 nums[i] = -1 后：
(-1) + nums[L] + nums[R] = 0
        ↓
nums[L] + nums[R] = 1

于是剩余部分就是一题 LC-167：在有序区间里找“两数之和 = -nums[i]”。
找到 [-1,-1,2] 后，L/R 要跨过相同值；下一个 i 如果仍是 -1，也要直接跳过，否则会重复输出同一个值三元组。""",
        "core": "先排序并固定第一个数，把 3Sum 降维成很多次有序 Two Sum；去重发生在“锚点”和“命中后的左右值”两个层次。",
        "formula": """nums[i] + nums[left] + nums[right] = 0

固定 nums[i] 后：
nums[left] + nums[right] = -nums[i]

这正是 LC-167 的双指针子问题。""",
        "steps": [
            "先排序，让剩余两数具有可利用的单调性",
            "枚举锚点 i；若 nums[i] 与前一个锚点相同则跳过",
            "在 [i+1,n-1] 上运行相向双指针，目标为 -nums[i]",
            "命中后记录三元组，并让 left/right 分别跨过当前重复值",
            "若 nums[i]>0，可直接结束：后面三个数都不会再凑出 0",
        ],
        "proof": "对每个不同的锚点值，剩余区间通过 LC-167 的排除规则完整扫描所有可能的两数组合，因此不会漏；锚点去重避免相同第一个值重复开局，命中后左右去重避免同一锚点下重复产生相同三元组。",
        "pitfalls": "去重比较的是值，不是下标；锚点去重要在进入双指针前做，命中后的 left/right 去重也不能省；排序会修改输入；求和使用 long long 更稳妥。",
        "transfer": "这是 k-Sum 的基本降维方法：固定一个数，把 k-Sum 变成 (k-1)-Sum，最终落到有序 Two Sum。学习顺序应是 LC-167 -> LC-15，而不是分别背两套模板。",
        "memory": "排序 -> 固定一个 -> 剩下变 Two Sum -> 命中后跳重复。",
    },

    11: {
        "visual": """把左右两条线当成水桶的两块板：

height[left] = 3                 height[right] = 8
      |                               |
      |~~~~~~~~ 水面只能到 3 ~~~~~~~~|
      |                               |
      L-------------------------------R

面积的高度被短板 3 卡死。
如果保留左边这块短板，只把 right 往左移动：宽度一定变小，而水面上限仍然不可能超过 3。
所以所有“继续使用这块左短板”的更窄容器都不可能比当前更大，可以一次性丢掉 left。

想让面积还有变大的可能，只能尝试换掉短板，看能不能找到更高的板。""",
        "core": "宽度每次都会缩小，因此只有换掉当前短板，才有机会提高决定面积的有效高度。",
        "formula": """area = (right-left) * min(height[left], height[right])

若 height[left] <= height[right]：
对任意 right' < right，
min(height[left], height[right']) <= height[left]
且 right'-left < right-left
=> area(left,right') <= 当前 area
=> left 可以安全排除""",
        "steps": [
            "left/right 从最宽的两端开始",
            "计算当前面积并更新 best",
            "比较两端高度，移动较短的一端；相等时移动任意一端",
            "重复直到 left==right",
        ],
        "proof": "当左边是短板时，所有保留 left、只缩短右边界的候选都被当前组合支配：宽度更小，且高度上限不会超过 height[left]。因此丢弃 left 不会错过更优解；右边较短时完全对称。",
        "pitfalls": "不要移动较高的一端，那无法突破当前短板上限；宽度是 right-left；面积乘法最好先提升到 long long；这题虽然也是相向双指针，但依据是“瓶颈支配关系”，不同于 LC-167 的有序和单调性。",
        "transfer": "看到“两个端点决定答案，且其中较弱的一端形成上限”时，可以尝试寻找类似的支配关系。关键是先证明丢掉哪一端安全，再写双指针。",
        "memory": "先算面积；谁是短板，就丢谁。",
    },

    283: {
        "visual": """把数组想成“已压紧的非零区 + 中间零洞 + 尚未扫描区”：

nums = [1, 0, 0, 3, 12]
        [ 已压紧 ] [零洞] [未扫描]
             1      0 0    3 12
                    ^      ^
                  write   read

read 找到 3 时，把它交换到 write 指向的第一个零洞：
[1, 3, 0, 0, 12]
       ^     ^
     write  read继续

write 永远指向“下一个非零数应该落下的位置”；read 负责向前寻找下一个非零数。""",
        "core": "read 负责找非零元素，write 负责把它们按原顺序压紧到数组前缀；两者之间留下的自然都是零。",
        "formula": """扫描到 read 之前始终维护：
[0, write)   = 已扫描部分的所有非零元素，且保持原相对顺序
[write, read)= 0

若 nums[read] != 0：
swap(nums[write], nums[read]);
write++""",
        "steps": [
            "write=0，表示第一个非零元素应落到下标 0",
            "read 从左到右扫描全部元素",
            "遇到 0：read 继续，write 不动",
            "遇到非零：与 nums[write] 交换，然后 write++",
            "扫描结束后，[0,write) 是稳定的非零前缀，剩余位置全是 0",
        ],
        "proof": "write 只在遇到非零时递增，所以写入前缀的非零元素顺序与 read 的发现顺序完全一致；而 write 到 read 之间的位置只可能是之前留下或交换过来的 0，因此交换不会把尚未处理的非零元素弄乱。",
        "pitfalls": "题目要求保持非零元素相对顺序，因此不能用普通 partition；write==read 时 swap 自己仍然正确，只是可省略；这里的“快慢指针”本质是稳定过滤/压缩，而不是相向排除。",
        "transfer": "删除指定值、数组原地去重、按条件保留元素，都可以先问：read 负责筛选什么，write 负责把保留下来的元素写到哪里。",
        "memory": "read 找要保留的，write 指向它该落的位置。",
    },
}
