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
        "model": "先从最直观的模拟出发：建立 answer[1..n]，每条 booking=[first,last,seats] 就把 first..last 每个航班都 +=seats。差分没有改变这件事的含义，它只是把一段连续、完全相同的重复写入压缩成两个“状态变化事件”。真正让这种压缩成立的是最后那次从左到右的累积：running 会继承上一个位置的值，因此一次 +seats 可以自动持续影响后续位置，直到遇到对应的 -seats 才把这份状态撤销。",
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

可以。我们只记录“状态变化”：

位置：      1    2    3    4    5    6
事件：      0   +25   0    0    0   -25

这里真正的实现技巧，不只是“记两个端点”，而是利用 running 的累积性让状态自动延续。
扫描规则只有一句：

running += diff[i]

逐位置看：
位置 1：diff=0    -> running = 0
位置 2：diff=+25  -> running = 25    <- 开启一份 +25 状态
位置 3：diff=0    -> running = 25    <- 没有新事件，状态自动继承
位置 4：diff=0    -> running = 25    <- 继续继承
位置 5：diff=0    -> running = 25    <- 继续继承
位置 6：diff=-25  -> running = 0     <- 撤销之前那份 +25 状态

注意位置 3、4、5 并没有再次执行 +25。
它们之所以仍得到 25，是因为 running 是累积状态：

running[i] = running[i-1] + diff[i]

当 diff[i] == 0 时：
running[i] = running[i-1]

也就是说，“没有新变化”本身就意味着“沿用之前的状态”。
这正是差分能把区间内成千上万次重复 +=seats 压缩掉的原因。

所以可以把三者分工记成：

diff[i]   ：当前位置发生的状态变化量
running   ：扫描到当前位置时，当前仍然生效的总状态
answer[i] ：把当前状态落到这个具体航班上的结果

这样再看右侧的 -25 就非常自然：
它不是“6 号航班减 25”，而是一个状态变化事件。
之前 running 里一直带着 +25；从位置 6 开始不该再带了，于是执行：

running = running + (-25)
        = 25 - 25
        = 0

也就是把之前加入 running 的那份状态移除。

如果没有这个 -25：
事件：      0   +25   0    0    0    0    0 ...
running：   0    25  25   25   25   25   25 ...

因为累积值会继续继承，+25 就会错误地永久延续下去。
因此 +25 和 -25 必须成对理解：

+25：把一份贡献加入 running，使它从这里开始持续生效
-25：把同一份贡献从 running 中移除，使它从这里开始停止生效

再看大区间 booking=[2,10000,25]。
朴素算法要对 2..10000 每个航班执行一次 +=25。
差分却只做：

2 号对应位置：              +25   <- 将 +25 加入 running
10000 号之后的边界：         -25   <- 将这份 +25 从 running 中移除

中间 3..10000 根本不需要继续记录事件，因为每走一步都会自然继承 running=25。
这就是“用累积值延续可加状态”的核心技巧。

如果题目恰好 n=10000，那么根本不存在 10001 号航班。
代码里的额外位置只是 diff 的哨兵槽位，用来承接“10000 号航班之后结束”这个事件；最终 answer 只读取 1..10000，不会输出这个哨兵位置。

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

最后做一次前缀和，就是把这些“状态变化事件”重新展开成最初那个逐航班累加的结果。""",
        "core": "差分真正的实现技巧是“事件 + 累积状态”：diff 只记录状态什么时候改变，running 负责把当前状态自动延续到后续位置。左端 +seats 是把一份贡献加入 running；中间 diff=0 时 running 原样继承，所以无需重复 +=seats；右端后一位 -seats 是把同一份贡献从 running 中移除。",
        "formula": """最关键的不变量不是先背 diff[L]+=x、diff[R+1]-=x，而是：

diff[i] = 位置 i 发生的“状态变化量”
running[i] = running[i-1] + diff[i]
answer[i] = running[i]

因此：

若 diff[i] == 0：
running[i] = running[i-1]

这就是状态能够跨位置持续的原因。

对一条 0-based 区间 [L,R] 增加 seats：

diff[L]     += seats      // 把 seats 加入持续状态
diff[R + 1] -= seats      // 把同一份 seats 从持续状态移除

于是扫描时：

位置 < L：running 不包含这份 seats
L <= 位置 <= R：running 一直包含这份 seats
位置 >= R+1：running 已经减掉这份 seats

本题输入是 1-based 的 [first,last]：
L = first - 1
R = last - 1
R + 1 = last

代码因此是：
first = booking[0] - 1
afterLast = booking[1]
diff[first]     += seats
diff[afterLast] -= seats

特别注意：当 last == n 时，afterLast == n 是 vector 中额外的哨兵下标，不对应任何真实航班。它只表示“最后一个航班之后，这份持续状态结束”。""",
        "steps": [
            "先写出朴素模型：answer[flight-1]+=seats，flight 从 first 走到 last；确认题目本质就是连续区间逐点加",
            "观察重复工作：同一条 booking 在整个区间内不断执行完全相同的 +=seats",
            "把 diff 看成事件表，而不是答案表：它只记录当前位置要让当前状态改变多少",
            "在 first 对应位置放 +seats，把这份贡献加入 running；从这里开始状态生效",
            "中间位置如果 diff=0，就执行 running+=0；由于 running 保留前值，这份贡献自然持续，不需要再次 +=seats",
            "在 last 后面的边界放 -seats，把之前加入 running 的同一份贡献移除；从这里开始状态失效",
            "多条 booking 的状态可以同时存在，running 始终等于当前位置所有仍处于 active 状态的 seats 之和",
            "最后把每个真实航班位置的 running 写进 answer；额外 diff 槽位只用于承接结束事件",
        ],
        "proof": "单看一条 booking=[first,last,seats]：在 first 前，running 中没有这份 seats；first 位置的 +seats 把它加入 running。之后只要没有对应的 -seats，哪怕每个中间位置的 diff 都是 0，running[i]=running[i-1]+0 仍会把这份 seats 原样带到下一个位置，因此 first..last 每个航班都会得到它。走过 last 后，-seats 把完全相同的贡献从 running 中移除，后续位置便不再包含它。多条 booking 只是在 running 中同时加入多份独立状态，结束时各自减掉自己的贡献，所以可以线性叠加。",
        "pitfalls": "不要把 diff 当成每个航班最终的座位数，也不要把 -seats 理解成业务上的负座位。diff 是状态变化量，running 才是当前状态。这个技巧成立的关键正是 running 会继承前值：diff=0 不是“当前位置没有座位”，而是“当前位置没有新的状态变化”。另外要区分题面 1-based 航班号和 vector 0-based 下标：真实区间 [first,last] 对应 [first-1,last-1]，关闭事件下标恰好是 last。",
        "transfer": "这其实是一类通用的“事件驱动 + 累积状态”技巧：公交上车时人数加入当前人数、下车后移除；会议开始时 active meeting +1、结束时 -1；扫描线进入区间时加入贡献、离开时删除贡献。共同模式都是：只记录状态变化点，让一个累积变量在没有事件的区间里自动保持状态。若还需要在线查询或更复杂的状态删除，才进一步升级到 Fenwick Tree、Segment Tree 或更一般的扫描线数据结构。",
        "memory": "diff 记变化，running 记状态。+x 把状态加进来；中间靠 running 自动延续；-x 把同一状态拿出去。真正省掉区间循环的是 running 的继承性。",
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
