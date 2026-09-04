"""High-touch pedagogy overrides for Week 1 Day 4."""

PEDAGOGY_WEEK1_DAY4 = {
    303: {
        "visual": """不要先把 prefix[i] 理解成“第 i 个元素的前缀和”，而把它看成数组边界上的累计值：

nums:       [ a0 ][ a1 ][ a2 ][ a3 ][ a4 ]
boundary:  0     1     2     3     4     5
prefix:    ^           ^                 ^
           0        prefix[2]         prefix[5]

prefix[i] 表示走到边界 i 时，已经跨过了前 i 个元素。

如果要问闭区间 [2,4]：

|------ prefix[5]：包含 a0..a4 ------|
|-- prefix[2]：包含 a0..a1 --|
                                  ^
                         相减后只剩 a2..a4

这样 right 为什么要写成 right+1 就很自然：right 是元素下标，而 prefix 存在于元素右侧的边界。""",
        "core": "先把每个数组边界的累计和存下来；任意区间就是“右边界累计 - 左边界累计”。",
        "formula": """prefix[0] = 0
prefix[i+1] = prefix[i] + nums[i]

因此 prefix[i] = nums[0] + ... + nums[i-1]

闭区间 [left,right]：
sum(left,right) = prefix[right+1] - prefix[left]""",
        "steps": [
            "建立长度 n+1 的 prefix，并令 prefix[0]=0",
            "从左到右执行 prefix[i+1]=prefix[i]+nums[i]",
            "每次查询 [left,right] 直接返回 prefix[right+1]-prefix[left]",
        ],
        "proof": "prefix[right+1] 包含 nums[0..right]，prefix[left] 包含 nums[0..left-1]；两段共同的左侧部分被相减抵消，恰好只留下 nums[left..right]。",
        "pitfalls": "最常见错误是把 right 写成 prefix[right]，本质上是混淆了“元素下标”和“边界下标”；额外的 prefix[0]=0 让 left=0 时无需特殊分支。",
        "transfer": "LC-560 其实就是在扫描过程中动态使用这些边界累计值；LC-304 把一维边界扩成二维矩形边界；LC-1109 的差分数组则可以看成前缀和的逆过程。",
        "memory": "prefix 存边界；区间和 = 右边界累计 - 左边界累计。",
    },

    304: {
        "visual": """一维前缀是“从起点到某个边界”的累计；二维前缀只是把线段换成左上角矩形。

prefix[r][c] 表示原矩阵中：
行 [0,r)，列 [0,c) 这个左上矩形的总和。

查询目标矩形时，可以先拿一个覆盖它的“大左上矩形”：

+-----------------------+
|        TOP            |
|   +-------------------|
| L |      TARGET       |
| E |                   |
| F |                   |
| T |                   |
+---+-------------------+

大矩形 - TOP - LEFT 会把左上角交叠区减两次，所以必须再加回来一次。
这就是二维容斥，不需要死记四项公式。""",
        "core": "目标子矩形 = 大左上矩形 - 上方多余部分 - 左侧多余部分 + 被重复减掉的左上交集。",
        "formula": """定义：prefix[r][c] = matrix[0..r-1][0..c-1] 的和

构造：
prefix[r+1][c+1]
= matrix[r][c]
+ prefix[r][c+1]
+ prefix[r+1][c]
- prefix[r][c]

查询闭矩形 (row1,col1) .. (row2,col2)：
ans = prefix[row2+1][col2+1]
    - prefix[row1][col2+1]
    - prefix[row2+1][col1]
    + prefix[row1][col1]""",
        "steps": [
            "建立 (rows+1)×(cols+1) 的 prefix，额外首行首列全部为 0",
            "逐格用“上 + 左 - 左上 + 当前值”构造二维累计矩形",
            "查询时先取右下边界对应的大矩形",
            "减去目标上方和左侧，再加回它们重复减掉的左上交集",
        ],
        "proof": "大左上矩形覆盖了目标区以及上方、左侧多余区域；减去上方和左侧后，二者交集被减了两次，所以加回一次。每个目标单元最终保留一次，其他单元最终系数为 0。",
        "pitfalls": "row2/col2 是原矩阵闭区间下标，因此映射到 prefix 边界时要 +1；构造式和查询式都存在“减两次、加回一次”的左上交集，漏掉它是最典型错误。",
        "transfer": "这是 LC-303 的二维版本：一维是两个边界相减，二维是四个矩形做容斥。图像积分图、二维频次查询都使用相同思想。",
        "memory": "大矩形 - 上 - 左 + 左上重叠。",
    },

    1109: {
        "model": "先从最直观的模拟出发：建立 answer[1..n]，每条 booking=[first,last,seats] 就把 first..last 每个航班都 +=seats。差分没有改变这件事的含义，它只是把一段连续、完全相同的重复写入压缩成两个事件：从 first 开始让 +seats 生效，在 last 后面的边界把这份影响撤销。最后再从左到右统一展开这些事件。",
        "visual": """第一步永远先写最直观的算法。

假设只有一条：
booking = [2,5,25]

朴素做法就是：
航班：      1    2    3    4    5    6
增加量：    0   25   25   25   25    0

也就是依次执行：
flight[2] += 25
flight[3] += 25
flight[4] += 25
flight[5] += 25

这个算法完全正确。差分只是在问：
既然 2..5 每个位置做的事情都一模一样，能不能不要立刻写 4 次？

可以。我们只记录“这份 +25 从哪里开始生效，以及从哪里开始不再生效”：

位置：      1    2    3    4    5    6
事件：      0   +25   0    0    0   -25

然后从左到右维护 running：
位置 1：running = 0
位置 2：running = 0 + 25 = 25
位置 3：running = 25 + 0 = 25
位置 4：running = 25 + 0 = 25
位置 5：running = 25 + 0 = 25
位置 6：running = 25 - 25 = 0

于是重新展开出来的增加量正好还是：
0, 25, 25, 25, 25, 0

这里最容易误解的是位置 6 上的 -25。
它绝不是说“6 号航班原本有 0 个座位，现在真的减掉 25”。
它只是一个控制 running 的结束事件：前面从位置 2 开始持续生效的 +25，到这里应该停止了，所以用 -25 把 running 从 25 恢复为 0。

如果没有这个 -25：
事件：      0   +25   0    0    0    0    0 ...
前缀累加：  0    25  25   25   25   25   25 ...

那么 +25 会错误地一直传到 6、7、8... 后面的所有位置。
所以右侧的 -25 本质上是“撤销之前开启的持续效果”，而不是对某个真实航班做一次负数更新。

再看大区间 booking=[2,10000,25]。
它表达的是：2..10000 全部 +25。
差分只记录：

2 号航班对应的位置：             +25   <- 开始生效
10000 号航班之后的边界：          -25   <- 到这里结束

如果题目恰好 n=10000，那么根本不存在 10001 号航班。
代码里的那个额外位置只是 diff 的哨兵槽位，用来承接“10000 号航班之后结束”这个事件；最终 answer 只读取 1..10000，不会输出这个哨兵位置。

因此更准确的图像不是：
“10001 号航班减 25”
而是：

2 号航班                  10000 号航班       右边界
   |-----------------------------|               |
  +25                                           -25
   |<--------- +25 持续生效 --------->|          |

第二步，再从数学上看为什么只需两个事件。
若最终每点增加量为 A，则差分记录相邻位置的变化：

diff[0] = A[0]
diff[i] = A[i] - A[i-1]

对闭区间 [L,R] 整体 +x 后，区间内部任意相邻两点都会同时 +x：

(A[i] + x) - (A[i-1] + x)
= A[i] - A[i-1]

因此内部差值完全不变，只有：
进入区间：diff[L]     += x
离开区间：diff[R + 1] -= x

最后做一次前缀和，就是把这些“开始/结束事件”重新展开成最初那个逐航班累加的结果。""",
        "core": "差分就是把朴素的“区间内每个航班都 +=seats”延迟展开：左端的 +seats 开启一份持续影响，右端后一位的 -seats 负责撤销这份影响；这个 -seats 是结束事件，不是把某个真实航班减成负数。",
        "formula": """先看朴素语义：
对 booking=[first,last,seats]，希望最终满足：
answer[first..last] 全部 += seats

差分把这段重复写入改成两个事件。
若使用 0-based 闭区间 [L,R]：

diff[L]     += seats      // 从 L 开始 running 多 seats
diff[R + 1] -= seats      // 从 R+1 开始撤销这份 seats

随后：
running += diff[i]
answer[i] = running

所以 -seats 的作用可以直接从 running 看：
在 L..R：running 包含这份 seats
到 R+1：running = running - seats
于是这条 booking 的影响从这里起归零。

本题输入是 1-based 的 [first,last]：
L = first - 1
R = last - 1
R + 1 = last

代码因此是：
first = booking[0] - 1
afterLast = booking[1]
diff[first]     += seats
diff[afterLast] -= seats

特别注意：当 last == n 时，afterLast == n 是 vector 中额外的哨兵下标，不对应任何真实航班。它只表示“最后一个航班之后，这份影响结束”。""",
        "steps": [
            "先写出朴素模型：answer[flight-1]+=seats，flight 从 first 走到 last；先确认题目本质就是连续区间逐点加",
            "观察浪费：一条 booking 在连续很多个位置重复执行完全相同的 +=seats",
            "把这些重复写入延迟：在 first 对应位置记录 +seats，表示从这里开始生效",
            "在 last 后面的边界记录 -seats，表示撤销之前那份持续影响；不要理解成某个真实航班被减 seats",
            "所有 booking 的开始/结束事件叠加到同一个 diff 中",
            "最后从左到右 running+=diff[i]；running 就是当前位置所有仍然生效的 booking 贡献之和",
            "只把前 n 个 running 写入 answer；第 n 个额外 diff 槽位只可能是结束哨兵，不属于答案",
        ],
        "proof": "拿单条 booking=[first,last,seats] 单独看：扫描到 first 前，running 中没有这份 seats；在 first 处遇到 +seats 后，running 多出 seats。first 到 last 之间没有任何事件撤销它，因此每个真实航班都得到这份 seats。走过 last 后，在 next boundary 遇到 -seats，running 恰好减回去，所以后续位置不再得到这份贡献。若 last=n，这个撤销事件虽然不会被 answer 扫描到，但把它放进 n+1 的 diff 能让区间表示保持统一且无需分支。多条 booking 只是把各自的开始/结束事件相加，线性叠加后与朴素逐航班修改完全等价。",
        "pitfalls": "最容易产生的误解是把 diff 中的负数当成“对这个航班做减法”。实际上 diff[i] 表示从位置 i 开始，running 要变化多少。特别是 last=n 时，diff[n]-=seats 位于真实 answer 之外，它只是“最后一个航班之后的结束边界”。另一个常见错误是把题面 1-based 航班号与 vector 0-based 下标混在一起：真实航班区间 [first,last] 对应数组 [first-1,last-1]，关闭事件下标恰好是 last。",
        "transfer": "差分适合“很多次区间统一加，最后统一读取每个点”。可以把它理解成事件压缩或延迟展开：公交 [上车,+人数] / [下车后,-人数]、会议开始/结束时人数变化、区间覆盖次数、扫描线都使用同一个模型。若更新和查询在线交错，通常升级到 Fenwick Tree / Segment Tree；二维矩形统一更新则可扩展到二维差分。",
        "memory": "先想朴素逐点加；差分只是把它压成两个开关：L 开启 +x，R 后面的边界用 -x 关掉。-x 是撤销持续效果，不是给那个位置算负数。",
    },

    238: {
        "visual": """对位置 i 来说，“除了自己以外”天然分成左右两块：

nums = [1, 2, 3, 4]
              ^ i=2

左边乘积：1 * 2 = 2
右边乘积：4     = 4
answer[2] = 2 * 4 = 8

不需要算全体乘积再除 nums[i]。
第一趟从左往右，把“来到 i 之前已经乘过的东西”写进 answer[i]；
第二趟从右往左，再把“来到 i 之前从右侧已经乘过的东西”乘进去。

关键点是：两趟都必须先使用累计值，再把 nums[i] 纳入累计，这样 nums[i] 自己才不会被乘进去。""",
        "core": "把除自身乘积分解成“左侧所有元素乘积 × 右侧所有元素乘积”，分别用两趟扫描维护。",
        "formula": """answer[i]
= product(nums[0..i-1]) * product(nums[i+1..n-1])

第一趟：
answer[i] = leftProduct
leftProduct *= nums[i]

第二趟：
answer[i] *= rightProduct
rightProduct *= nums[i]""",
        "steps": [
            "answer 初始化为 1，leftProduct=1",
            "左到右：先把 leftProduct 写给 answer[i]，再执行 leftProduct*=nums[i]",
            "rightProduct=1，从右到左扫描",
            "先执行 answer[i]*=rightProduct，再执行 rightProduct*=nums[i]",
            "最终每个 answer[i] 同时拥有 i 左侧和右侧的乘积",
        ],
        "proof": "第一趟到达 i 时，leftProduct 只包含 i 左边元素；第二趟到达 i 时，rightProduct 只包含 i 右边元素。两者相乘覆盖所有其他元素且恰好排除 nums[i]。",
        "pitfalls": "两趟的更新顺序都不能反：若先乘 nums[i] 再写/使用累计值，就会把自己算进去；0 不需要额外分支，这种左右分解天然处理一个或多个 0；空间复杂度通常不计返回数组 answer。",
        "transfer": "前后缀分解不只用于乘积：只要某个位置的答案能拆成“左侧聚合 ×/+/max/... 右侧聚合”，就可以考虑左右两趟扫描。",
        "memory": "先用累计值回答当前位置，再把当前位置纳入累计。左右各做一遍。",
    },
}
