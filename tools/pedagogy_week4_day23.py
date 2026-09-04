"""High-touch pedagogy overrides for Week 4 / Day 23.

Day 23 extends Day 22 from binary-searching positions to binary-searching
answers, then closes with the related but distinct partition-search model of
LC-4.
"""

PEDAGOGY_WEEK4_DAY23 = {
    1011: {
        "pattern": "答案二分：把最小运载能力改写成第一个可行容量",
        "model": "延续 Day 22 / LC-34 的 first-true 思维，只是搜索轴不再是数组下标，而是船的容量。给定 capacity 后，按原顺序尽量往当天装包裹可以计算完成所需的最少天数；capacity 越大，需要的天数只会不增，因此容量轴呈现 false...false | true...true。",
        "derivation": """一个真实且正确的直接做法是从最小可能容量 max(weights) 开始逐个尝试 capacity=maxWeight,maxWeight+1,...，对每个容量都顺序模拟装船；第一次能在 days 天内完成的容量就是答案。因为容量按从小到大枚举，所以它一定不会漏掉更小答案。问题是容量范围最坏可以延伸到 sum(weights)，每个候选又要 O(n) 模拟，总工作可能接近 O(n*S)。

真正可以省掉的是对大量连续容量的逐个尝试。固定一个 capacity 后，若它已经能在 days 天内完成，那么任何更大的容量也一定能完成；若它不能完成，那么任何更小的容量也不可能突然变得可行。于是所有候选容量天然分成：
false false ... false | true true ... true。

这正是 Day 22 / LC-34 的边界二分，只不过“下标 i 是否满足 nums[i]>=target”换成了“容量 C 是否能按时运完”。因此不再线性枚举容量，而是二分寻找第一个 true。优化类型：利用可行性的单调性，一次判定淘汰整段答案候选。""",
        "visual": """weights = [3,2,2,4,1,4], days = 3

容量下界 = 4（至少装得下最重包裹）
容量上界 = 16（一天全部运完）

看两个候选：
capacity = 5
Day1: 3,2
Day2: 2
Day3: 4,1
Day4: 4        -> 4 天，不可行

capacity = 6
Day1: 3,2
Day2: 2,4
Day3: 1,4      -> 3 天，可行

容量轴可以画成：
4   5 | 6   7   8 ... 16
F   F | T   T   T ... T
        ^
        要找第一个 true

二分的对象不是 weights 的位置，而是“答案可能是多少”。""",
        "core": "先写出 capacity -> 是否能在 days 天内完成的单调判定，再在 [max(weights),sum(weights)] 中二分第一个可行容量。",
        "formula": """搜索区间：
left  = max(weights)
right = sum(weights)

给定 capacity，顺序扫描：
used = 1
load = 0
若 load + weight > capacity：
    used += 1
    load = 0
随后 load += weight

feasible(capacity) := daysNeeded(capacity) <= days

单调性：
C1 <= C2 => daysNeeded(C1) >= daysNeeded(C2)
所以 feasible(C) 为 F...F | T...T。

边界二分：
if feasible(mid): right = mid
else:             left = mid + 1
结束 left==right，即最小可行容量。""",
        "steps": [
            "先用最重单件确定 left=max(weights)，用总重量确定 right=sum(weights)，保证答案一定在闭区间内",
            "实现 daysNeeded(capacity)：严格按原顺序累加，当下一件放不下时才开启新一天",
            "每轮取 middle；若 daysNeeded(middle)<=days，middle 已可行但可能不是最小值，所以保留 middle 并令 right=middle",
            "若 middle 不可行，则所有更小容量也不可行，令 left=middle+1 一次丢掉整段候选",
            "left==right 时返回该容量；它既可行，又因为左侧全被证明不可行而是最小值",
        ],
        "proof": "对固定 capacity，每天尽可能晚地换天会得到最少天数：如果当前包裹还能放进今天却提前换天，只会减少今天已装重量，并不会给未来创造比‘继续装’更多容量，所以不可能减少总天数。于是 daysNeeded 是该容量下真实的最少天数。容量增大只会让原先能同一天装下的组合继续装得下，因此所需天数单调不增，可行集合必是连续后缀。边界二分始终保留第一个可行容量，最终不会漏掉更小答案。",
        "pitfalls": "包裹顺序不能重排，这也是 check() 必须顺序扫描的原因。left 不能从 1 开始，因为任何小于最重包裹的容量都必然无效；right=sum(weights) 一定可行。总重量和中点用 long long，避免累加溢出。可行时必须 right=middle 而不是 middle-1，因为 middle 本身可能就是第一个 true。",
        "transfer": "LC-410 与本题几乎同构：‘一天’换成‘一个连续子数组’，capacity 换成‘允许的最大段和’，check 仍然是按顺序尽量装满。LC-875 同样寻找答案轴上的第一个 true，但 check 改为按速度计算总耗时。以后看到‘最小化最大容量/速度/阈值’，先尝试把优化问题改成单调的可行性判定。",
        "memory": "先写 check(C)，再看是否 F...F|T...T；答案二分找第一个 true。",
    },

    410: {
        "pattern": "答案二分 + 贪心判定：把最小最大段和转成最小可行上限",
        "model": "这题直接复用 LC-1011：数组元素顺序不能改变，把每个连续子数组看成一‘船’。猜一个最大允许段和 limit 后，顺序尽量把元素留在当前段，就能得到满足该上限所需的最少段数。limit 越大，所需段数越少，因此二分第一个能用至多 k 段完成的 limit。",
        "derivation": """最直接的正确做法是枚举 k-1 个切分位置。数组有 n-1 个缝隙，从中选择 k-1 个后就得到一种完整分割；计算每种分割的最大段和，再取最小值。它不会漏答案，但组合数量 C(n-1,k-1) 会迅速爆炸。

与其问‘最佳切点到底在哪里’，换一个更容易回答的问题：如果我先猜答案上限 limit，能不能把数组切成 k 段，使每段和都 <= limit？因为 nums 全部非负，给定 limit 后有一个确定的最省段策略：当前元素只要还能放进本段就继续放，只有再放就超限时才切段。提前切只会让当前段更空，不可能让最终使用段数更少。

于是 check(limit) 可以 O(n) 得到最少需要多少段。limit 越大，限制越宽松，所需段数只会不增；可行 limit 再次形成 F...F | T...T。这样就从‘枚举组合切点’转成了‘二分答案 + 线性贪心判定’。优化类型：用单调决策问题替代巨大方案枚举。""",
        "visual": """nums = [7,2,5,10,8], k = 2

猜 limit = 17：
[7,2,5] = 14
[10]    = 10
[8]     = 8
需要 3 段 -> 不可行

猜 limit = 18：
[7,2,5] = 14
[10,8]  = 18
需要 2 段 -> 可行

答案轴：
max(nums)=10 ........ 17 | 18 19 ... sum=32
          false .... F | T  T  ... T
                       ^ 第一个可行上限

它和 LC-1011 的图完全相同：
包裹/天数  <->  数组/分段数。""",
        "core": "不要直接搜索切点；先猜最大段和 limit，用贪心算满足该上限至少需要几段，再二分第一个 groupsNeeded(limit)<=k 的 limit。",
        "formula": """搜索范围：
left  = max(nums)
right = sum(nums)

给定 limit：
groups = 1
current = 0
若 current + value > limit：
    groups += 1
    current = 0
随后 current += value

groupsNeeded(limit) = 在每段和 <= limit 时最少需要的段数。

limit1 <= limit2
=> groupsNeeded(limit1) >= groupsNeeded(limit2)

feasible(limit) := groupsNeeded(limit) <= k
=> F...F | T...T。

二分 first true。""",
        "steps": [
            "用最大单元素作为答案下界、总和作为上界；任何更小上限装不下最大元素，总和则一定能作为单段上限",
            "给定 limit 顺序扫描 nums，当前元素能留在本段就继续累加，只有加入后会超过 limit 才切出新段",
            "若 groupsNeeded(middle)<=k，说明 middle 足够大，保留 middle 并继续向左寻找更小可行上限",
            "若需要超过 k 段，middle 以及所有更小上限都不可能，令 left=middle+1",
            "区间收敛后返回 left；若贪心得到少于 k 段，可继续把已有非空段拆开直到恰好 k 段，最大段和不会增加",
        ],
        "proof": "固定 limit 时，贪心总是在‘不得不切’的最晚位置切段。任何合法方案在贪心第一段结束位置之前切，只会让第一段更短，并不能把更多元素塞进后面的固定上限，因此不可能使用更少段；对剩余后缀重复这一论证，贪心得到最少段数。由于 nums 非负，提高 limit 不会使任何原本合法的段失效，所以最少段数单调不增。若最少段数 <= k，因为 k<=n，可以继续拆分非空段而不增加任何段和，故存在恰好 k 段的合法方案。",
        "pitfalls": "题目要求恰好 k 个非空连续子数组，但 check 用 <=k 是正确的，原因必须建立在‘可继续拆段且 nums 非负’上。不要对数组排序，连续性和原顺序是问题结构。累加和、limit、中点都用 long long；当前实现最终结果再安全转回 int。",
        "transfer": "这是 LC-1011 的同构题：shipWithinDays 的 capacity 就是本题 limit，daysNeeded 就是 groupsNeeded。今后遇到‘把连续任务切成至多 k 组，最小化最大组负载’，可以先尝试直接复用这个模型；若元素允许负数，贪心最晚切段与单调性都要重新证明，不能机械套用。",
        "memory": "不找切点，先猜上限；给定上限最晚切段，二分第一个能在 k 段内完成的值。",
    },

    875: {
        "pattern": "答案二分：把最小吃速转成速度轴上的第一个可行点",
        "model": "继续复用 LC-1011 的答案二分框架。未知答案是整数速度 speed；给定 speed 后，每堆香蕉独占若干小时，耗时为 ceil(pile/speed)，把各堆耗时相加即可判断能否在 h 小时内完成。speed 越大，总耗时只会下降，因此寻找第一个可行速度。",
        "derivation": """一个自然且正确的做法是按 speed=1,2,3,... 逐个尝试，计算每个速度需要的总小时数，第一次满足 hours<=h 就返回。因为速度按升序枚举，所以得到的一定是最小可行速度；但 piles[i] 可以到 10^9，逐个试速度会浪费巨大范围。

关键观察是：每堆所需小时 ceil(pile/speed) 随 speed 增大单调不增，因此总小时数也单调不增。如果某个 speed 已能在 h 小时内完成，那么任何更快速度都可行；如果它超时，那么更慢速度一定也超时。

所以速度候选再次形成 false...false | true...true。和 LC-1011 相比，二分边界模板完全不用改，只需替换 check(speed) 的业务含义。优化类型：利用答案可行性的单调性，跳过大量无需逐个测试的速度。""",
        "visual": """piles = [3,6,7,11], h = 8

speed = 3：
ceil(3/3)+ceil(6/3)+ceil(7/3)+ceil(11/3)
= 1+2+3+4 = 10 小时 -> 不可行

speed = 4：
1+2+2+3 = 8 小时 -> 可行

速度轴：
1  2  3 | 4  5 ... 11
F  F  F | T  T ... T
          ^
          最小可行速度

这里不能把所有香蕉总数直接除以 speed：
每小时只能选择一堆，剩余不足 speed 的那一小时也不能转去吃另一堆。
所以每一堆必须分别向上取整。""",
        "core": "把 speed 当作答案候选，使用 sum(ceil(pile/speed))<=h 做单调 check，然后二分速度轴上的第一个 true。",
        "formula": """搜索区间：
left = 1
right = max(piles)

hours(speed) = Σ ceil(pile / speed)
             = Σ (pile + speed - 1) / speed

feasible(speed) := hours(speed) <= h

若 s1 <= s2：
ceil(pile/s1) >= ceil(pile/s2)
=> hours(s1) >= hours(s2)
=> feasible 为 F...F | T...T。

if feasible(mid): right = mid
else:             left = mid + 1。""",
        "steps": [
            "速度至少为 1，最大只需 max(piles)：达到最大堆大小后，每堆最多一小时，而题目保证 h>=piles.length，因此上界一定可行",
            "实现 canFinish(speed)，对每一堆分别计算向上取整小时数并累加到 long long hours",
            "若累计 hours 已经超过 h，可以立即返回 false；后续只会继续增加，没有必要再扫描",
            "middle 可行时保留它并令 right=middle；不可行时把 middle 及其左侧全部淘汰，令 left=middle+1",
            "left==right 时返回最小可行整数速度",
        ],
        "proof": "对每一堆，速度越大，完成该堆需要的整小时数不会增加；逐堆相加后总耗时同样单调不增，因此可行速度一定构成连续后缀。二分只在已证明不可行时删除左段，在已知可行时保留 middle 继续寻找更小值，所以收敛点正是第一个可行速度。每堆单独向上取整准确反映‘一小时只处理一堆且剩余时间不能转移’的题意。",
        "pitfalls": "最常见错误是用 ceil(sum(piles)/speed)，这错误地允许一小时在多堆之间转移剩余吞吐量；必须逐堆取整后相加。表达式先把 pile 转成 long long，避免 pile+speed-1 的 int 溢出风险。可行时 right=middle，不能跳过 middle。",
        "transfer": "和 LC-1011/410 一样，真正可复用的是‘候选答案 -> O(n) check -> 单调真假边界’三件套，而不是某个业务公式。运输容量、加工速度、最小阈值、最大允许负载都可以先尝试在答案轴上建立单调谓词。",
        "memory": "答案不是数组下标也能二分：只要 speed 越大越容易，就找速度轴第一个 true。",
    },

    4: {
        "pattern": "有序数组分割二分：搜索一条让左右数量平衡且值域合法的切线",
        "model": "LC-4 仍然使用二分，但不是 LC-1011 的答案空间模型。把两个有序数组各切一刀，使左侧总元素个数固定为 (m+n+1)/2。只要同时满足 left1<=right2 和 left2<=right1，两个数组的左半合在一起就恰好是整体较小的一半，中位数只由四个切口边界值决定。给定 cut1 后 cut2 被总左长度唯一确定，因此只需在较短数组的 cut1 上二分。",
        "derivation": """最自然且正确的算法是像归并排序那样合并两个有序数组，再直接读取中间一个或两个元素，时间 O(m+n)、额外空间 O(m+n)；即使不真正保存结果，只用双指针走到中间，也仍需要 O(m+n) 的最坏时间。

这里浪费的是：为了得到中位数，我们处理了大量远离中间边界的元素，而真正决定答案的只有‘整体左半最大的数’和‘整体右半最小的数’。由于两个输入内部已经有序，可以不构造完整归并序列，而是直接寻找一条分割线：左侧元素数量固定，且左边所有值都不大于右边所有值。

若 nums1 切出 cut1 个元素，那么 nums2 必须切出 leftSize-cut1 个，第二个切点无需搜索。于是只剩一个变量 cut1。若 left1>right2，说明 nums1 左边拿多了，cut1 必须左移；若 left2>right1，说明 nums1 左边拿少了，cut1 必须右移。这种方向性让 cut1 可以二分。优化类型：利用两个数组内部有序性，把完整归并压缩成对一个分割边界的搜索。""",
        "visual": """先保证 nums1 是较短数组。
例如 nums1=[2]，nums2=[1,3]，总长度 3：
leftSize = (1+2+1)/2 = 2

尝试：
nums1: [2] | +∞
nums2: [1] | [3]
        ----+----
左侧共有 2 个元素。

四个边界：
left1=2   right1=+∞
left2=1   right2=3

交叉检查：
left1 <= right2   => 2 <= 3
left2 <= right1   => 1 <= +∞
都成立。

因此整体排序后的左半最大值就是 max(2,1)=2。
总长度为奇数，中位数直接是 2。

若出现 left1 > right2：
nums1 左边拿得太多 -> cut1 左移。
若出现 left2 > right1：
nums1 左边拿得太少 -> cut1 右移。""",
        "core": "固定整体左半应有多少元素，只二分较短数组的切点；另一个切点随之确定，直到两侧数量平衡且两个交叉边界都满足左<=右。",
        "formula": """令 m<=n：
leftSize = (m+n+1)/2
cut1 ∈ [0,m]
cut2 = leftSize - cut1

边界值：
left1  = cut1==0 ? -∞ : nums1[cut1-1]
right1 = cut1==m ? +∞ : nums1[cut1]
left2  = cut2==0 ? -∞ : nums2[cut2-1]
right2 = cut2==n ? +∞ : nums2[cut2]

合法分割：
left1 <= right2 && left2 <= right1

若 left1 > right2：cut1 太大，right=cut1-1
否则若 left2 > right1：cut1 太小，left=cut1+1

合法时：
奇数总长：median = max(left1,left2)
偶数总长：median = (max(left1,left2)+min(right1,right2))/2。""",
        "steps": [
            "若 nums1 比 nums2 长，交换参数角色，始终在较短数组上二分，使复杂度为 O(log min(m,n))，并保证由 cut1 推出的 cut2 落在合法范围",
            "计算固定的 leftSize=(m+n+1)/2，在 cut1 的闭区间 [0,m] 上二分；每个 cut1 唯一确定 cut2",
            "用 INT_MIN/INT_MAX 表示切口落在数组端点时不存在的左/右边界，这样无需为四种空侧分别写分支",
            "若两个交叉条件都成立，分割线已经对应整体有序序列的中间边界，按总长度奇偶直接返回中位数",
            "若 left1>right2，nums1 左侧取多了就左移 cut1；否则必是 left2>right1，nums1 左侧取少了就右移 cut1",
        ],
        "proof": "每个数组内部有序，因此若 left1<=right2 且 left2<=right1，则 nums1 左半的所有元素都<=nums2右半，nums2左半的所有元素也都<=nums1右半；再加上各自内部顺序，可知合并后的所有左侧元素都不大于所有右侧元素。leftSize 又保证左侧数量恰好是整体的一半（奇数时多一个），所以中位数只可能是左侧最大值或左右边界均值。若 left1>right2，增加 cut1 只会让 nums1 左边界不减、nums2 的 cut2 更小，因此方向只会更错，必须左移；另一种冲突对称地必须右移，所以二分不会丢失合法切点。",
        "pitfalls": "这题不是‘答案二分’，不要看到 Day 23 就强行套 feasible(value)。必须先让 nums1 成为较短数组；否则 cut2 可能越界。leftSize 用 (m+n+1)/2 可以统一奇偶，并让奇数时多出的元素放在左侧。端点哨兵只是比较用的虚拟值，不是真实数据。偶数中位数求和前转 long long，避免两个 int 边界相加溢出。",
        "transfer": "该分割模型可以推广到两个有序数组的第 k 小：把 leftSize 从中位数所需数量改成 k，再寻找满足交叉边界的切点。它与 Day 22/23 其它二分题共享的不是业务模板，而是更底层原则——找到一个可单调纠正的边界变量，每次比较证明搜索方向。",
        "memory": "不合并整表，只找中间切线：左边数量固定，交叉边界都满足左<=右。",
    },
}
