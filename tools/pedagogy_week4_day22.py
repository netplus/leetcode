"""High-touch pedagogy overrides for Week 4 / Day 22.

Day 22 builds one reusable binary-search model in four stages: exact lookup,
boundary lookup, rotated-array search, and rotated-array boundary lookup.
"""

PEDAGOGY_WEEK4_DAY22 = {
    704: {
        "pattern": "标准二分：把一次比较升级成整段候选淘汰",
        "model": "先把二分看成候选区间收缩，而不是背 left/right 模板。数组严格升序，因此比较 nums[mid] 与 target 后，不只是知道 mid 对不对，还能一次证明 mid 左侧或右侧整段都不可能是答案。闭区间 [left,right] 始终表示‘目标若存在，只可能在这里’。",
        "derivation": """最自然且正确的算法是从左到右逐个比较 nums[i] 与 target；找到就返回 i，否则扫描结束返回 -1。它不会漏，因为每个元素都检查了，但最坏需要 O(n) 次比较。

真正可以省掉的工作来自数组已经严格升序。若 nums[mid] < target，那么 left..mid 中每个值都 <= nums[mid] < target，这整段无需再逐个检查；若 nums[mid] > target，mid..right 同理可以整体淘汰。

因此一次比较不再只排除一个元素，而是排除至少约一半候选。只要维护‘答案若存在一定仍在当前候选区间’这个不变量，每轮把不可能的一半永久丢掉，候选规模 n -> n/2 -> n/4 -> ...，最终就是 O(log n) 的二分查找。""",
        "visual": """nums = [-1,0,3,5,9,12], target = 9

left=0                right=5
[-1, 0, 3, 5, 9, 12]
         ^ mid=2, nums[mid]=3

3 < 9，而且数组升序：
[-1, 0, 3] 这一整段都不可能再出现 9。
             x x x

候选直接缩成 [3,5]，而不是只把下标 2 划掉。

二分的核心图像就是：
一次中点比较 -> 给一整段候选判死刑。""",
        "core": "始终让 [left,right] 包住所有仍可能等于 target 的位置；比较中点后，利用有序性一次淘汰不可能的一半。",
        "formula": """闭区间不变量：
target 若存在，则其下标一定属于 [left,right]。

middle = left + (right-left)/2

nums[middle] == target -> 命中
nums[middle] <  target -> [left,middle] 全部过小，left = middle+1
nums[middle] >  target -> [middle,right] 全部过大，right = middle-1

循环条件 left <= right，因为 left==right 时仍有一个真实候选没有检查。
结束时 left > right，候选集合为空。""",
        "steps": [
            "初始化 left=0、right=n-1，把整个数组定义为闭区间候选集合",
            "只在 left<=right 时循环，并用 left+(right-left)/2 计算 middle",
            "若 nums[middle]==target 立即返回 middle",
            "若中点值过小，把 left 更新为 middle+1；若过大，把 right 更新为 middle-1，更新必须越过已判定的 middle",
            "区间变空仍未命中时返回 -1",
        ],
        "proof": "若中点值小于 target，由严格升序可知所有 i<=middle 都满足 nums[i]<=nums[middle]<target，因此删除 [left,middle] 不会删掉答案；中点值大于 target 时对称。每次更新都只删除已被有序性证明不可能的元素，所以不会漏。middle 命中时立即结束；未命中时 middle 被永久移出候选区间，因此不会重复检查同一候选，区间最终必然缩空或命中。",
        "pitfalls": "先确定区间语义再写循环。闭区间对应 left<=right、right=n-1、更新 middle±1；不要把半开区间模板混进来。middle 用 left+(right-left)/2 避免 left+right 的潜在整数溢出。题目严格升序且元素唯一，所以命中任意一个就是唯一答案。",
        "transfer": "LC-34 会把‘找某个值’升级成‘找真假分界线’；LC-33/153 会说明即使整体不再有序，只要一次比较仍能证明一批候选不可能，二分思想仍可成立。之后遇到二分题，先问的不是‘mid 怎么写’，而是‘一次判断能否安全淘汰整段候选’。",
        "memory": "闭区间包住全部候选；中点比较不是排除一个点，而是证明一整段不可能。",
    },

    34: {
        "pattern": "边界二分：从找一个 target 升级为找单调谓词第一次成立的位置",
        "model": "LC-704 找到任意 target 就结束，但重复元素下我们要找的是边界。把数组位置按谓词 nums[i] >= value 分成‘假...假 | 真...真’两段，firstAtLeast(value) 专门寻找第一个真位置。target 的左边界是 firstAtLeast(target)，右边界则是 firstAtLeast(target+1)-1。",
        "derivation": """一个自然正确做法是线性扫描数组，记录第一次和最后一次遇到 target 的位置，时间 O(n)。即使先用普通二分 O(log n) 找到某个 target，再向左右扩展，极端输入全部都是 target 时扩展仍会退化到 O(n)。

浪费在于：我们已经知道数组非递减，却还在边界附近逐个确认重复值。对于固定 value，谓词 P(i)=[nums[i]>=value] 具有单调性：一旦某位置为真，右边所有位置都为真；一旦某位置为假，左边所有位置都为假。

所以问题不再是‘某个位置等不等于 target’，而是‘假区间和真区间的分界点在哪里’。二分每次可以整段排除仍为假的左半或已经为真的右半，直接找到 first true。用 value=target 找到 target 段开始，用 value=target+1 找到第一个严格大于 target 的位置，再减一得到结束位置。""",
        "visual": """nums = [5,7,7,8,8,10], target=8

对 P(i): nums[i] >= 8：
index: 0 1 2 | 3 4 5
P:     F F F | T T T
              ^ firstAtLeast(8)=3

对 nums[i] >= 9：
index: 0 1 2 3 4 | 5
P:     F F F F F | T
                  ^ firstAtLeast(9)=5

所以所有 8 恰好位于 [3, 5-1] = [3,4]。

边界二分不是‘找到 8 后继续找’，而是直接搜索 F/T 的分界线。""",
        "core": "把数值比较改写成单调真假谓词，二分寻找第一个满足 nums[i]>=value 的位置；两个分界点夹住全部 target。",
        "formula": """firstAtLeast(value)：寻找最小 i，使 nums[i] >= value；若不存在，返回 n。

使用半开区间 [left,right)：
初始 [0,n)
while left < right:
    middle = left + (right-left)/2
    nums[middle] < value -> middle 仍是假，left = middle+1
    nums[middle] >= value -> middle 已是真且可能是第一个真，right = middle
结束时 left==right，即第一个真位置。

first = firstAtLeast(target)
afterLast = firstAtLeast((long long)target + 1)
答案 = [first, afterLast-1]，前提是 first<n 且 nums[first]==target。""",
        "steps": [
            "先实现唯一的 firstAtLeast(nums,value)，明确它在半开区间 [left,right) 中寻找第一个真位置",
            "求 first=firstAtLeast(target)；若 first==n 或 nums[first]!=target，说明 target 根本不存在，返回 [-1,-1]",
            "求 afterLast=firstAtLeast((long long)target+1)，它是第一个严格大于 target 的位置",
            "返回 [first,afterLast-1]；两个边界搜索都保持 O(log n)",
        ],
        "proof": "firstAtLeast 中，若 nums[middle]<value，则 middle 及其左侧都不可能满足谓词，所以 left=middle+1 安全；否则 middle 已满足谓词，但可能正是最左边那个，故只能令 right=middle 保留它。循环结束时假区间已全部在 left 左侧、真区间从 left 开始。对 target 与 target+1 分别求界后，位于两界之间的值既 >=target 又 <target+1；对整数即恰等于 target，因此不会漏也不会多。",
        "pitfalls": "这里刻意使用半开区间 [left,right)，循环是 left<right，命中谓词时 right=middle 而不是 middle-1，因为 middle 可能就是答案。target 可能是 INT_MAX，所以不能直接用 int 的 target+1；当前实现把 value 提升为 long long。先验证 first 真正命中 target，再计算结果可正确处理空数组和不存在情况。",
        "transfer": "这是比 LC-704 更通用的二分形态：lower_bound、upper_bound、插入位置、计数区间都可视为找单调谓词的边界。Day 23 的‘答案二分’只是把下标轴换成答案值轴：仍然是寻找 false...false|true...true 的第一次成立位置。",
        "memory": "找重复值边界，不要先命中再扩展；直接二分 F...F | T...T 的分界。",
    },

    33: {
        "pattern": "旋转数组二分：整体失序时，每轮先找到仍然有序的那一半",
        "model": "旋转数组由两段升序片段拼接而成。整体看起来不再单调，但元素互异时，对任意 [left,right] 和 middle，至少有一侧 [left,middle] 或 [middle,right] 仍然严格有序。先识别这段‘可判定区间’，再用端点值判断 target 是否落在它的值域；若不在，整段就能安全淘汰。",
        "derivation": """最自然正确的方法是线性扫描 target，O(n)。普通 LC-704 二分不能直接套，因为 nums[mid]<target 并不再意味着 target 一定在右边：例如 [4,5,6,7,0,1,2] 中 middle=7，而小值都在旋转断点之后。

关键不是强求整个候选区间单调，而是问：一次比较还能否找到一块‘确定有序、因此可以整段判断’的区域？旋转后的两个升序片段只在一个断点处失序；在元素互异的前提下，中点不可能让左右两侧都跨过那个唯一断点，所以至少一半保持升序。

如果左半有序，就能用 nums[left] <= target < nums[middle] 精确判断 target 是否属于左半值域；属于则保留左半，否则整段左半可淘汰。若左半无序，则右半必有序，做对称判断。这样每轮仍能删除约一半候选，恢复 O(log n)。""",
        "visual": """nums = [4,5,6,7,0,1,2], target=0
left=0        mid=3        right=6
[4,5,6,7 | 0,1,2]
 ^       ^   ^     ^

[left..mid] = [4,5,6,7] 明确有序。
它的值域是 [4,7)，target=0 不在里面，
所以即使我们还不知道旋转点的精确位置，也能一次删掉整个左半：

[4,5,6,7] | [0,1,2]
 x x x x     ^ 继续搜索

核心不是先找 pivot，而是每轮抓住一半可证明的有序结构。""",
        "core": "每轮先判断哪一半仍有序，再判断 target 是否落在这半的值域；若不在，就整半淘汰。",
        "formula": """闭区间 [left,right] 内，先检查 nums[middle]==target。

若 nums[left] <= nums[middle]：左半有序
    nums[left] <= target < nums[middle]
        -> right = middle-1
    否则
        -> left = middle+1

否则：右半有序
    nums[middle] < target <= nums[right]
        -> left = middle+1
    否则
        -> right = middle-1

不变量：target 若存在，始终保留在 [left,right]；每轮至少删除 middle 和一侧候选。""",
        "steps": [
            "用 LC-704 相同的闭区间 left/right 和 middle；若中点直接命中，立即返回",
            "比较 nums[left] 与 nums[middle]：若前者 <= 后者，则左半确定有序，否则右半确定有序",
            "在确定有序的一半上，用端点值判断 target 是否位于其值域；位于则只保留该半，否则删除该半",
            "更新边界时始终越过 middle；候选区间为空后返回 -1",
        ],
        "proof": "元素互异且候选区间最多跨一个旋转断点，因此 middle 两侧至少一侧不跨断点、保持升序。对于这段有序区间，端点值可以准确判定 target 是否属于其值域：若属于，答案只能留在这一半；若不属于，即使 target 存在也只能在另一半。每轮只删除已被值域证明不可能的一半，所以不会漏；middle 每轮被命中或排除，区间严格缩小，因此不会重复和死循环。",
        "pitfalls": "不要根据 nums[middle] 与 target 的大小直接决定方向，那是 LC-704 的整体有序前提。`nums[left] <= nums[middle]` 中等号用于处理 left==middle。题目值互异非常关键；若允许重复，nums[left]==nums[mid]==nums[right] 时可能无法判断哪半有序，需要缩边界并可能退化到 O(n)。值域条件一端含等号、一端排除 middle，避免把 middle 重复保留。",
        "transfer": "它把二分的定义从‘数组整体有序’升级为‘每轮能证明一批候选不可能’。LC-153 会进一步去掉 target，直接把旋转断点/最小值作为边界寻找；带重复的 LC-81 则展示当判定信息不足时为什么二分会退化。",
        "memory": "整体不有序没关系：先找本轮确定有序的一半，再用值域决定保留哪边。",
    },

    153: {
        "pattern": "旋转边界二分：用右端锚点判断 middle 在断点哪一侧",
        "model": "把最小值看成旋转数组两段之间的断点。与 LC-33 每轮寻找 target 不同，这次只需判断 middle 位于‘旋转前的高值段’还是‘包含最小值的低值段’。右端 nums[right] 始终提供一个低值段锚点：nums[mid]>nums[right] 时 mid 明确在高值段，最小值只能在右边；否则 mid 已落在低值段并可能就是最小值，必须保留。",
        "derivation": """最自然正确的方法是线性扫描所有元素取最小值，O(n)。也可以扫描相邻元素寻找唯一下降点 nums[i]>nums[i+1]，其后一项就是最小值，但最坏仍需 O(n)。

这里真正可利用的是旋转结构只有一个断点。我们不必逐个寻找下降点，只需判断 middle 在断点左侧还是右侧。为什么选择 nums[right] 作为锚点？当前候选区间的 right 位于包含最小值的那一侧或恰好就是候选末端；元素互异时：
- nums[mid] > nums[right]：mid 与 right 之间跨过断点，mid 一定在高值段，最小值严格在 mid 右侧；
- nums[mid] < nums[right]：mid 到 right 已经处于同一升序段，最小值不可能在 mid 右侧更深处，mid 自己却可能就是最小值。

于是一次比较把一半位置排除，得到寻找旋转边界的 O(log n) 二分。""",
        "visual": """nums = [4,5,6,7,0,1,2]

4 5 6 7 | 0 1 2
高值段      ^ 低值段
            最小值/断点

第一轮 middle=3 -> nums[mid]=7，right=2：
7 > 2，说明 middle 还在断点左边，
最小值只能在 (middle,right]：left=middle+1。

之后若 middle 落到 0 或 1：
nums[mid] < nums[right]，
说明断点已经在 [left,middle]，middle 仍可能正是最小值，
所以只能 right=middle，不能 middle-1。""",
        "core": "把最小值当成旋转断点；用 nums[right] 作锚点判断 middle 在高值段还是低值段，并始终保留可能就是最小值的 middle。",
        "formula": """候选不变量：最小值下标始终属于闭区间 [left,right]。

while left < right:
    middle = left + (right-left)/2
    if nums[middle] > nums[right]:
        # middle 在高值段，不可能是最小值
        left = middle+1
    else:
        # middle 在低值段，可能就是最小值
        right = middle

left==right 时只剩唯一候选，返回 nums[left]。

由于元素互异，middle<right 时不会出现 nums[middle]==nums[right] 的模糊情况。""",
        "steps": [
            "初始化 left=0、right=n-1，让最小值始终被闭区间包住；这里循环只需 left<right，因为相遇时答案已经唯一",
            "计算 middle，并比较 nums[middle] 与当前 nums[right]，不要与固定的 nums[n-1] 混淆候选区间语义",
            "若 nums[middle]>nums[right]，证明断点严格在 middle 右侧，令 left=middle+1",
            "否则 middle 已在低值段且可能本身就是最小值，令 right=middle 保留它",
            "left==right 时返回 nums[left]",
        ],
        "proof": "若 nums[middle]>nums[right]，在原始严格升序数组中不可能出现左值大于右值，说明 middle 到 right 之间必跨旋转断点，因此最小值严格位于 middle 右侧，删除到 middle 安全。否则 nums[middle]<nums[right]，middle..right 本身严格升序，这段内部除 middle 外不可能有更小的旋转断点；最小值只能在 [left,middle]，且 middle 可能就是它，所以 right=middle。两种更新都保留最小值并严格缩小区间，最终唯一候选即答案。",
        "pitfalls": "第二分支必须 `right=middle`，不能 `middle-1`，因为 middle 可能正好指向最小值。循环使用 left<right 与该保留策略配套。题目元素互异使与 right 的比较没有歧义；若允许重复，等号时不能判断断点在哪边，通常只能 right--。不要先额外找 pivot 再做别的操作，本题直接把 pivot 当二分目标即可。",
        "transfer": "LC-153 是 LC-34 的‘边界二分’思想在旋转结构上的版本：不是搜索某个具体 target，而是在两个结构区段之间找转折点。LC-154 加入重复值后会暴露信息不足导致的退化；峰值、谷值、第一处违背性质的位置也常能用‘比较相邻/锚点后排除一侧’来构造二分。",
        "memory": "最小值就是断点；mid>right 去右边，否则保留 mid 去左边。",
    },
}
