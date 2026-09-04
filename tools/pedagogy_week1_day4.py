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
        "model": "把最终每个航班的座位数看成一条从左到右的折线。差分数组不保存折线的高度，而保存相邻航班之间高度改变了多少。对一个连续区间整体加 seats 时，区间内部相邻两点都同时加 seats，所以它们之间的差完全不变；真正发生变化的只有进入区间的左边界和离开区间的右边界。",
        "visual": """先不要背“左端 +x、右端后一位 -x”，从差分数组的定义把它推出来。

第一步：只看一条预订 [2,4,10]。
它希望给 2、3、4 号航班各增加 10：

航班：          1    2    3    4    5
增加量 A：      0   10   10   10    0

现在不记录 A 本身，而只记录“当前位置相对前一个位置改变了多少”：

diff[1] = A[1]             = 0
diff[2] = A[2] - A[1]      = +10   <- 从这里开始多 10
diff[3] = A[3] - A[2]      = 0
diff[4] = A[4] - A[3]      = 0
diff[5] = A[5] - A[4]      = -10   <- 从这里开始不再多 10

所以图上真正留下的只有两个“事件”：

航班：           1    2    3    4    5
                 |   +10             -10
                 |    |                |
diff：           0   10    0    0   -10
前缀累加：       0   10   10   10     0

这已经解释了为什么最后必须做前缀和：
diff 只告诉我们“这里发生了多少变化”，running 才表示“走到这里时当前总影响是多少”。

第二步：为什么区间内部完全不用改？
假设原值是 A，对闭区间 [L,R] 整体加 x。
区间内部任意相邻位置 i-1、i 都同时加 x：

(A[i] + x) - (A[i-1] + x)
= A[i] - A[i-1]

两个 x 抵消了，因此 L+1..R 的 diff 全都不变。
只有两个地方没有成对抵消：

进入区间：diff[L]     += x
离开区间：diff[R + 1] -= x

所以“改两个端点”不是技巧口诀，而是差分定义直接推出的结果。

第三步：把本题三条预订叠起来：
bookings = [[1,2,10], [2,3,20], [2,5,25]]

每条预订只留下开始/结束两个事件：
[1,2,10] -> 1 号开始 +10，3 号开始 -10
[2,3,20] -> 2 号开始 +20，4 号开始 -20
[2,5,25] -> 2 号开始 +25，6 号开始 -25

把同一位置事件相加：
航班/边界：      1    2     3     4    5     6(哨兵)
diff：          10   45   -10   -20   0    -25

从左到右累计，只取前 n 个位置：
running：       10   55    45    25   25

这就是每个航班最终的预订座位数。""",
        "core": "差分记录的不是“这个航班有多少座位”，而是“从这个航班开始，总座位数要改变多少”；区间内部的 +x 会在相邻差值里两两抵消，因此一次区间更新最终只剩“开始 +x、结束后一位 -x”两个事件。",
        "formula": """先看数学定义。若 answer 使用 0-based 下标：

diff[0] = answer[0]
diff[i] = answer[i] - answer[i-1]   (i > 0)

于是前缀和正好是逆运算：
answer[i] = diff[0] + diff[1] + ... + diff[i]

若对 0-based 闭区间 [L,R] 整体加 x：
diff[L]     += x
diff[R + 1] -= x

本题输入却是 1-based 的 [first,last]。
映射到代码：

L = first - 1
R = last - 1
R + 1 = last

所以代码恰好写成：
first = booking[0] - 1
afterLast = booking[1]
diff[first]     += seats
diff[afterLast] -= seats

注意 afterLast 这个名字很重要：
它不是“last 的数组下标”，而是“last 对应的 0-based 下标再向右一格”。

扫描时维护不变量：
running = diff[0] + ... + diff[i]
        = 当前仍处于生效区间内的所有 seats 之和
answer[i] = running""",
        "steps": [
            "先把题目识别成“很多次连续区间统一 +seats，最后一次性询问所有点”的离线区间更新问题",
            "建立长度 n+1 的 diff；第 n 个额外位置专门容纳 last=n 时的“结束后一位”事件",
            "处理每条 booking：把 1-based first 转成 first-1，在这里执行 +seats",
            "把原题 last 直接作为 0-based 的 afterLast，在这里执行 -seats；不要再额外 +1",
            "所有 booking 处理完后，从 i=0 到 n-1 累加 running += diff[i]",
            "把当前 running 写入 answer[i]；它就是此时仍然生效的所有预订贡献之和",
        ],
        "proof": "对任意一条 booking=[first,last,seats]，它在 first 对应的数组位置放入 +seats，因此从这里开始 running 会多出 seats；直到 last 对应的位置为止都没有遇到它的 -seats，所以这份贡献持续存在。扫描到 last 后面的第一个位置时才遇到 -seats，于是贡献恰好失效。换句话说，这条 booking 对所有满足 first <= 航班号 <= last 的位置贡献一次 seats，对区间外贡献 0。每条 booking 都独立满足这一性质，而加法可以线性叠加，因此把所有边界事件先相加再统一做一次前缀和，和逐条真正修改整个区间得到的结果完全相同。",
        "pitfalls": "最容易错的是三种下标混在一起：题面 [first,last] 是 1-based 闭区间；vector 的实际航班位置是 [first-1,last-1]；关闭事件在 one-past-right，也就是下标 last。因此代码是 diff[first-1]+=seats、diff[last]-=seats。使用 n+1 后，last==n 时 diff[n] 合法且只作哨兵，不需要 if 分支。不要忘记最终前缀累加，否则 diff 只是变化事件而不是答案。按官方约束，单个航班最多累计 2*10^4 * 10^4 = 2*10^8 个座位，32 位 int 足够。",
        "transfer": "差分与前缀和是一对逆操作：前缀和把“每点值”变成“累计值”，差分把“每点值”变成“相邻变化”。看到“很多次区间加，最后统一得到每个点”时优先考虑差分；若更新和查询在线交错，就需要 Fenwick Tree / Segment Tree；若更新对象变成矩形，可继续扩展到二维差分。公交上下客、会议室人数变化、区间覆盖次数、扫描线事件都可以用同一个“进入 +x、离开 -x”模型理解。",
        "memory": "先问 diff 记录什么：相邻位置的变化。闭区间 [L,R] 加 x -> L 开门 +x，R+1 关门 -x -> 最后一路前缀累加。",
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
