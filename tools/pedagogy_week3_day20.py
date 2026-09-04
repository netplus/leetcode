"""High-touch pedagogy overrides for Week 3 / Day 20.

Day 20 turns the DP state-design ideas from Days 18-19 into knapsack and
interval-DP models. The thread is not a collection of templates: first decide
what one state means, then ask whether an input item may be used once or many
times, what value the state aggregates (reachability/count/minimum), and whether
changing the order of the decision can make subproblems independent.
"""

PEDAGOGY_WEEK3_DAY20 = {
    416: {
        "pattern": "0/1 背包可达性：把等分问题改写成每个数最多选一次的目标和",
        "model": "先用总和把“两组都要管”降成“只找一组”：若总和为偶数，只要存在一个下标子集恰好凑出 total/2，剩余元素自然组成另一半。随后把每个数看成一次性的选择，reachable[s] 只回答处理过的元素能否凑出 s；一维压缩时容量必须倒序，才能让当前 value 在这一轮至多贡献一次。",
        "derivation": """一个自然且正确的直接算法是对每个 nums[i] 做“选 / 不选”DFS，并维护当前子集和。走到末尾时检查是否等于 total/2；它枚举了所有下标子集，所以一定不会漏解。

问题在于不同选择路径会不断遇到相同的子问题。例如已经处理到下标 i、当前还需要凑 remaining 时，之前具体选了哪些元素并不再重要；未来能否成功只取决于“还剩哪些下标”和 remaining。直接 DFS 会把这些相同状态重复计算，最坏需要枚举 2^n 个子集。

先做一个更重要的建模化简。若两组和相等，总和 total 必须为偶数，并且其中任意一组的和都必须是 target=total/2。于是问题从“两组如何分配”变成：每个元素最多使用一次，是否能凑出 target。

可以用二维状态 reachable[i][s] 表示“只使用前 i 个元素能否凑出 s”。处理 value 时只有两类来源：不选它，继承 reachable[i-1][s]；选它，从 reachable[i-1][s-value] 转移。每个 (i,s) 只计算一次，就得到 O(n*target) DP。

再压成一维时，关键不只是省空间，而是保持‘读取上一层’的语义。若 s 从小到大更新，刚写出的 reachable[s-value] 可能已经使用了当前 value，随后又被本轮再次读取，相当于同一个下标重复使用。倒序从 target 到 value 更新，读取的 reachable[s-value] 仍是加入当前 value 之前的旧状态，因此天然保持 0/1 约束。

优化链：选/不选枚举所有子集 -> 相同 (处理位置, 剩余目标) 被重复求解 -> 等分化为 target=total/2 -> 每个状态只算一次 -> 观察只依赖上一层 -> 一维倒序 0/1 背包。""",
        "visual": """nums = [1,5,11,5]，total=22，所以只需问：能否选出和为 11 的一组？

目标容量：0 1 2 ... 10 11
初始：    T F F ...  F  F

处理 value=1 后：
0 和 1 可达。

处理 value=5 时，想象把“旧可达集合”整体向右平移 5：
旧：{0,1}
加 5：{5,6}
合并：{0,1,5,6}

这里每个 value 对应的是一个具体数组下标，只允许做一次平移。

为什么一维必须倒序？假设这一轮 value=5：
若先把 reachable[5] 由 reachable[0] 设为 true，随后正序走到 10，又可能读取刚刚变 true 的 reachable[5]，于是同一个 5 被用了两次。
倒序时先算 10、再算 5，算 10 时 reachable[5] 仍是上一轮状态，不会偷用当前元素两次。""",
        "core": "先把等分改写成“是否存在和为 total/2 的下标子集”；每个元素只能选一次，所以一维 reachable 必须倒序更新，保证本轮只读上一层状态。",
        "formula": """若 total 为奇数：答案必为 false。
否则 target = total / 2。

二维语义：
reachable[i][s] = 只使用前 i 个元素，是否能凑出和 s。

处理 value=nums[i-1]：
reachable[i][s]
= reachable[i-1][s]
  OR (s>=value AND reachable[i-1][s-value])

一维初始化：
reachable[0] = true
其余为 false。

一维转移，容量倒序：
for value in nums:
    for s = target ... value:
        reachable[s] = reachable[s] || reachable[s-value]

循环不变量：开始处理某个 value 前，reachable[s] 精确表示“仅使用此前元素是否能凑出 s”；本轮倒序结束后，它精确加入了“当前 value 选 0 次或 1 次”的所有可能。""",
        "steps": [
            "用 accumulate 计算 total；若 total 为奇数立即返回 false，因为两个整数子集不可能各占一半",
            "令 target=total/2，并建立 target+1 个布尔可达状态；reachable[0]=true 表示什么都不选可以凑出 0",
            "逐个处理 value；它对应一个具体数组下标，因此这一轮只能选择 0 次或 1 次",
            "容量 sum 从 target 倒序走到 value，并执行 reachable[sum] = reachable[sum] || reachable[sum-value]",
            "所有元素处理完后返回 reachable[target]；若它为真，未被该子集选中的剩余元素总和也必为 target",
        ],
        "proof": "对每个 value，任何使用已处理元素凑出 sum 的方案只有两类：不使用当前 value，此时旧 reachable[sum] 已完整覆盖；使用当前 value 一次，去掉它后必须由此前元素凑出 sum-value，因此由旧 reachable[sum-value] 完整覆盖。两类并集就是全部方案。倒序保证读取 sum-value 时该状态尚未被当前 value 更新，所以第二类确实只使用当前下标一次。最后若 target 可达，其补集总和为 total-target=target；反之任何等分方案都必包含一个和为 target 的子集，因此不会漏解。",
        "pitfalls": "最关键的错误是容量正序：那会把 0/1 背包悄悄改成可重复使用当前元素的完全背包。这里 nums 全为正数，target 范围可控；vector<char> 只是紧凑地保存布尔状态，不改变语义。总和奇偶判断必须在建 target 状态前完成。",
        "transfer": "这是 Day 18“选 / 不选”决策 DP 的集合版本，也是 Day 19 状态压缩思想的直接延伸。下一题 LC-494 会完整复用同一套 0/1 背包和倒序规则，只把 reachable 的值域从“是否可达”改成“有多少种下标子集”；LC-322 再通过遍历语义展示‘每件物品一次’与‘可无限复用’的根本区别。",
        "memory": "先砍一半：total/2；再问可达；元素只能一次，所以容量倒着走。",
    },

    494: {
        "pattern": "0/1 背包计数：先用代数把正负号选择变成 LC-416 的子集和",
        "model": "不要直接为 '+'/'-' 另背一套 DP。把加正号的下标集合记为 P、加负号的记为 N，联立 P-N=target 与 P+N=total，可得 P=(total+target)/2。于是每个符号表达式与一个和为 positiveSum 的下标子集一一对应；LC-416 的布尔 reachable 只需升级成 ways[s] 计数即可。",
        "derivation": """最自然的正确算法是 DFS：来到 nums[i] 时分别尝试 +nums[i] 和 -nums[i]，处理完全部元素后若累计和等于 target 就给答案加一。每个表达式对应一条唯一根到叶路径，因此算法完全正确，但会枚举 2^n 种符号选择。

可以先做记忆化，把状态压成 (i,currentSum)，因为走到同一 i 且累计和相同时，后面的选择完全相同。这已经消除了重复子问题。但这题还有更强的结构：所有 nums[i] 都是非负数，正负号实际上是在把下标分成两个集合。

设加正号的元素和为 P，加负号的元素和为 N：
P - N = target
P + N = total
两式相加得到 2P = total + target，因此 P=(total+target)/2。

所以只要 |target|<=total 且 total+target 为偶数，原问题就等价为：从每个下标最多选一次，有多少个下标子集的元素和恰好为 positiveSum。这正是 LC-416 的 0/1 背包状态图，只是 LC-416 做 OR，而这里做加法计数。

于是 ways[s] 表示使用已处理下标凑出 s 的方案数。处理 value 时倒序执行 ways[s]+=ways[s-value]。倒序仍负责‘每个下标只使用一次’；加法则把不同子集的方案数汇总起来。

优化链：2^n 正负 DFS -> 重复的 (i,sum) -> 用 P/N 方程把符号问题转成子集和 -> 复用 LC-416 的 0/1 背包 -> 布尔 OR 改成方案数相加。""",
        "visual": """nums=[1,1,1,1,1], target=3，total=5。

P - N = 3
P + N = 5
----------
2P = 8
P = 4

所以不必继续想“每个位置放 + 还是 -”，只需问：有多少个下标子集的和为 4？
五个 1 中任意挑四个，共 5 种；这正对应官方答案 5。

把 LC-416 的状态做一个替换：

LC-416：reachable[s] = 能 / 不能
LC-494：ways[s]      = 有多少种

状态转移拓扑完全一样：
旧 ways[s]          -> 不选当前 value
旧 ways[s-value]    -> 选当前 value
两类方案互斥，所以数量直接相加。

value=0 时也没有例外：
ways[s] += ways[s]
会把每个已有方案翻倍，恰好对应这个 0 可以写成 +0 或 -0，两种表达式不同。""",
        "core": "先用 P-N=target、P+N=total 把符号选择变成和为 (total+target)/2 的下标子集计数，再原样复用 LC-416 的 0/1 倒序状态。",
        "formula": """设：
P = 加 '+' 的元素和
N = 加 '-' 的元素和
total = P + N

要求：
P - N = target
因此：
2P = total + target
positiveSum = (total + target) / 2

无解条件：
abs(target) > total
或 total + target 为奇数。

状态：
ways[s] = 使用已经处理的下标，和恰好为 s 的子集数量。

初始化：
ways[0] = 1

0/1 倒序转移：
for value in nums:
    for s = positiveSum ... value:
        ways[s] += ways[s-value]

不变量：每轮开始前 ways 只统计此前下标；倒序结束后，所有方案恰好把当前下标分成“不选”和“选一次”两类。""",
        "steps": [
            "计算 total；若 abs(target)>total，目标绝对值已经超出所有数字可提供的总量，直接返回 0",
            "检查 total+target 的奇偶；若为奇数，positiveSum 不是整数，不可能存在对应符号划分",
            "计算 positiveSum=(total+target)/2，建立 ways 数组并令 ways[0]=1，代表空子集这一种基础方案",
            "逐个处理 value，并和 LC-416 一样让容量从 positiveSum 倒序到 value，保证每个数组下标只参与一次",
            "执行 ways[sum]+=ways[sum-value]；最终 ways[positiveSum] 就是表达式数量",
        ],
        "proof": "每个完整符号表达式把所有下标唯一划分为正号集合和负号集合；由两条方程可知它满足目标和，当且仅当正号下标集合的元素和为 positiveSum。反过来，任意这样的下标子集都唯一决定哪些位置放 '+'，其余位置放 '-'，因此二者是一一对应。0 虽然数值不改变集合和，但不同下标是否进入正号集合仍对应 +0/-0 的不同表达式，DP 会正确计数。倒序转移对每个下标只选 0/1 次，所以不会把同一下标重复计入。",
        "pitfalls": "必须判断 abs(target)>total，而不是只判断 target>total，因为 target 可以为负。还必须检查 total+target 的奇偶。ways 保存的是方案数而不是可达性；遇到 0 时翻倍是正确行为，不能去重。当前实现用 long long 保存中间计数，再按题目返回 int。",
        "transfer": "这是 LC-416 最直接的模型迁移：状态空间、倒序方向、每个元素一次的约束全部不变，只把布尔半环的 OR 改成整数加法。看到‘正负号 / 两组分配’时，先列总和与差值方程，常常可以先降维成子集和，再决定是求可达、计数还是最值。",
        "memory": "先列两式求 P；然后把 LC-416 的 true/false 换成 ways 数量，倒序规则一字不改。",
    },

    322: {
        "pattern": "完全背包最优化：从重复 remainder 子问题到金额 DP，并看清为何允许复用硬币",
        "model": "把 amount 看成状态而不是一棵组合搜索树。dp[a] 表示凑出金额 a 的最少硬币数；任何最优方案都有一枚‘最后使用的硬币’ coin，拿掉它后剩余部分必须是金额 a-coin 的最优方案。因为每种硬币数量无限，当前状态可以反复读取已经由同一面额参与得到的更小金额，这正是完全背包与 LC-416/494 的 0/1 约束差异。",
        "derivation": """一个自然且正确的直接算法是 DFS：还剩 remainder 时，尝试每一种 coin<=remainder，递归求 remainder-coin 的最少硬币数，再取所有选择中的最小值。它枚举了所有可能的硬币序列，因此能找到最优答案。

浪费在于同一个 remainder 会从许多不同路径反复出现。例如 coins=[1,2,5] 时，金额 6 可以通过先选 1 再到 5，也可以先选 2 再经过其他金额；一旦递归落到 remainder=4，从那里到 0 的最少硬币数只由 4 决定，与此前选择顺序无关。直接 DFS 会重复求这些 remainder。

于是定义 dp[a]=凑出金额 a 的最少硬币数，让每个金额只求一次。固定一个目标金额 a，任何非空最优方案都可以按最后一枚硬币 coin 分类；拿掉它后得到金额 a-coin。如果那部分不是 dp[a-coin] 所代表的最优方案，用更优前缀替换后会让原方案更优，产生矛盾。因此：
dp[a] = min(dp[a-coin] + 1)。

这与 LC-416/494 有一个值得刻意比较的地方：那里每个数组下标只能使用一次，所以压缩后要避免同一轮读取刚更新的状态；这里每种硬币数量无限，读取由同一个 coin 反复参与得到的较小金额完全合法。当前实现按 amount 从小到大、每个 amount 枚举所有 coin，本质上就是按状态拓扑递增求解；它自然允许 coin 被使用任意次。

优化链：枚举硬币序列 -> 相同 remainder 被重复求解 -> 每个金额只保留一个最少硬币摘要 -> 枚举最后一枚硬币 -> 自底向上完全背包 / 金额 DP。""",
        "visual": """coins=[1,2,5], amount=11。

把每个金额画成一个节点：
0 -> 1 -> 2 -> ... -> 11
每枚 coin 都提供一条“从 a-coin 跳到 a、代价 +1”的边。

例如计算 dp[6]：

最后用 1：dp[5] + 1
最后用 2：dp[4] + 1
最后用 5：dp[1] + 1

只要 dp[1]、dp[4]、dp[5] 已经是最优，dp[6] 取三者最小即可。

这里和 LC-416 的方向差异要从语义理解：
LC-416：一个 nums 下标只能拿一次，不能让当前 value 在本轮反复喂给自己。
LC-322：硬币面额只是‘类型’，同一面额本来就可以无限拿，因此从小金额继续扩到大金额完全合法。

当前代码按 current=1..amount 递增，所以 dp[current-coin] 一定已经求好。""",
        "core": "每个金额只保留“凑成它的最少硬币数”；枚举最优方案的最后一枚硬币，从已经求好的更小金额转移，而硬币无限供应允许同一面额反复参与。",
        "formula": """状态：
dp[a] = 凑出金额 a 所需的最少硬币数。

基础：
dp[0] = 0
其余先设 INF = amount + 1。

转移：
dp[a] = min(dp[a], dp[a-coin] + 1), 其中 coin<=a。

当前实现顺序：
for a = 1 ... amount:
    for coin in coins:
        if coin <= a:
            dp[a] = min(dp[a], dp[a-coin] + 1)

不变量：开始计算金额 a 时，所有 dp[0..a-1] 都已经是最终最优值；因此枚举最后一枚硬币后得到的 dp[a] 也是最终最优。

若 dp[amount] 仍为 amount+1，则无解。""",
        "steps": [
            "建立 amount+1 个状态，把 dp[0] 设为 0，其余设为 amount+1；由于最小面额至少为 1，这个值严格大于任何可能的有效最少枚数，可安全充当 INF",
            "让 current 从 1 递增到 amount，保证所有 current-coin 都是已经求好的更小状态",
            "枚举每个 coin；只有 coin<=current 时，它才可能作为组成 current 的最后一枚硬币",
            "用 dp[current-coin]+1 尝试改善 dp[current]；这允许同一面额在不同层次重复出现，因为题目明确硬币数量无限",
            "最终若 dp[amount] 仍为 INF 返回 -1，否则返回该最小值；amount=0 会自然返回 dp[0]=0",
        ],
        "proof": "取任意金额 a 的最优方案，并看它最后一枚硬币 coin。删除 coin 后，剩余硬币必然组成 a-coin；如果该剩余方案不是最少枚数 dp[a-coin]，就可以换成更少硬币的方案，再把 coin 放回，从而得到比原最优方案更好的解，矛盾。因此某个最优答案一定出现在 dp[a-coin]+1 的枚举中。反过来，每个被枚举的 coin 与一个可达的 a-coin 方案组合后都确实组成 a，所以转移不会制造非法解。",
        "pitfalls": "不要把 LC-416 的‘容量必须倒序’机械搬过来：那条规则来自“每个输入元素最多一次”，而这里同一面额可无限使用。当前金额优先的双循环求的是最少枚数，顺序不会改变最优值；若将来改成方案计数，则‘硬币在外还是金额在外’会影响统计组合还是排列，不能照抄。amount=0 应返回 0，无解要把 INF 转成 -1。",
        "transfer": "和 LC-416/494 对照学习最有效：三题都在一个一维容量轴上做 DP，但 416 的值是 bool、494 的值是 count、322 的值是 min；同时 416/494 的输入下标只能用一次，而 322 的硬币类型可无限复用。先问‘状态值聚合什么’和‘物品能用几次’，再决定 OR/+ /min 以及遍历方向。",
        "memory": "金额递增，枚举最后一枚 coin；无限硬币允许重复读更小金额，不要把 0/1 背包的倒序口诀硬套进来。",
    },

    312: {
        "pattern": "区间 DP：把难确定的“先戳谁”反过来改成邻居固定的“最后戳谁”",
        "model": "这题的障碍不是普通的重复子问题，而是戳破一个气球会改变后续邻居，导致按‘第一步’切分时左右部分仍相互影响。关键重构是对固定边界 (left,right) 枚举区间内最后被戳的 k：当 k 最后留下时，区间内部其余气球已经消失，它的邻居必然正是 left 和 right，于是左区间 (left,k) 与右区间 (k,right) 已经彼此独立，可以做区间 DP。",
        "derivation": """最直接且正确的方法是枚举戳气球的顺序：当前剩哪些气球，就尝试戳其中任意一个，按它此刻的左右邻居计算收益，再递归处理剩余气球。所有 n! 种顺序都被覆盖，因此当然能得到最大值。

仅仅说‘它是指数/阶乘复杂度’还不够。真正难以复用状态的原因是：如果按第一枚被戳的 k 来划分，k 消失以后，它左右两侧的气球会重新成为邻居。之后左侧的操作收益可能依赖右侧还剩谁，右侧也可能依赖左侧，所以‘先戳 k’并不能把问题切成两个独立子问题。

反过来问：在某个边界区间内，哪一个气球最后被戳？假设最后是 k。当它被戳时，(left,right) 内除 k 外的气球已经全部消失，因此 k 当时的两个邻居确定就是边界 left 和 right，最后一步收益固定为 values[left]*values[k]*values[right]。

而在 k 被留下直到最后之前，左侧 (left,k) 内部如何全部戳完，不会再依赖右侧内部；右侧 (k,right) 同理。于是原本动态变化的邻居关系被‘最后操作’固定住，产生真正独立的两个子问题。

给原数组两端补虚拟气球 1 后，定义 dp[left][right] 为开区间 (left,right) 全部戳完的最大收益。枚举最后的 k：
dp[left][right] = max(dp[left][k] + values[left]*values[k]*values[right] + dp[k][right])。
因为大区间依赖两个更短区间，所以按区间宽度从小到大计算。

优化链：枚举 n! 个戳破顺序 -> 发现按“第一步”无法独立切分 -> 改问“区间内最后一步是谁” -> 最后一步邻居被边界固定 -> 左右子区间独立 -> O(n^3) 区间 DP。""",
        "visual": """给 nums=[3,1,5,8] 两端补 1：

index:   0  1  2  3  4  5
values: [1, 3, 1, 5, 8, 1]

考虑开区间 (left,right)=(0,4)，也就是内部 [3,1,5]。
不要问“先戳谁”，而问“谁最后留下”。

若最后留下 k=3（值 5）：

left=0       k=3       right=4
 value 1      5          8
    |---------|----------|
     (0,3)       (3,4)

等轮到 5 最后被戳时，区间内其他气球已经没了，它的邻居一定是边界值 1 和 8，所以最后收益固定为 1*5*8。
此前左边怎么戳，只由 (0,3) 决定；右边怎么戳，只由 (3,4) 决定。

因此候选值就是：
dp[0][3] + 1*5*8 + dp[3][4]。

把 k 在 (left,right) 内全部枚举，就覆盖了所有可能的“最后一步”。""",
        "core": "动态邻居让“先戳谁”无法切分；改为枚举区间内最后戳的气球后，它的邻居固定为两端边界，左右区间因此真正独立。",
        "formula": """补边界：
values = [1] + nums + [1]

状态采用开区间：
dp[left][right] = 把 (left,right) 内所有气球戳完能得到的最大硬币数。

空区间：
right = left + 1 时，dp[left][right] = 0。

若 k 是 (left,right) 内最后被戳的气球：
candidate = dp[left][k]
          + values[left] * values[k] * values[right]
          + dp[k][right]

因此：
dp[left][right]
= max over left<k<right(candidate)

依赖不变量：计算宽度 width=right-left 的区间时，所有 dp[left][k] 与 dp[k][right] 的宽度都严格小于 width，因此已经计算完成。""",
        "steps": [
            "构造 values，在原 nums 两端各补一个值为 1 的虚拟边界；它们从不被戳，只用来统一边界乘法",
            "创建二维 dp，默认 0；开区间没有内部气球时收益本来就是 0，所以无需额外初始化",
            "让 width 从 2 递增；width=2 是第一个恰好含一个内部气球的非空开区间",
            "固定 left 并令 right=left+width，枚举每个 last∈(left,right) 作为该区间最后被戳的气球",
            "用已经完成的 dp[left][last]、dp[last][right] 加上最后一步的固定收益 values[left]*values[last]*values[right] 更新最大值",
            "最终 dp[0][n-1] 表示两个虚拟边界之间，也就是整个原数组全部戳完的最大收益",
        ],
        "proof": "任意一种把 (left,right) 内气球全部戳完的顺序都有唯一的最后一个气球 k，因此所有合法顺序会被按 last=k 唯一分类，不会漏也不会重复。固定 k 后，在最后一步发生前，(left,k) 和 (k,right) 内的气球都必须已经全部消失；由于 k 一直保留，它把左右两侧隔开，所以两边内部操作可以分别达到各自最优 dp[left][k] 与 dp[k][right]，最后再获得固定边界乘积。反过来，任取左右两个合法最优顺序并在最后戳 k，都构成一个合法的 (left,right) 顺序，因此转移枚举的每个候选都可实现。",
        "pitfalls": "状态是开区间，不要把 left/right 自己当成需要被戳的内部气球；width 必须由小到大，否则依赖的子区间还没求好。最容易误入歧途的是按‘第一枚’强行切左右，因为第一枚消失后两侧会重新相邻，并不独立。两端补 1 只是题面越界邻居规则的统一编码，不是额外可选择的真实气球。",
        "transfer": "这是 Day 19 二维 DP 的另一种坐标含义：LC-62/64 的两个维度是网格位置，LC-1143 是两个前缀长度，而这里是区间左右边界。更重要的迁移是‘改变决策时间方向来制造独立子问题’：矩阵链乘、石子合并、多边形三角剖分等区间 DP 都常枚举最后合并 / 最后操作 / 分割点，而不是执着于真实执行顺序的第一步。",
        "memory": "第一步邻居会变，切不开；最后一步邻居固定。补 1、用开区间、枚举 last、短区间先算。",
    },
}
