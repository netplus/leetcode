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
        "visual": """差分数组不直接记录“每个位置有多少”，而只记录“从哪里开始变化、从哪里恢复”。

例如给航班 2..4 每个都加 10：

最终想增加：  [0, 10, 10, 10, 0]
位置：          1   2   3   4   5

真正需要记录的只有两个事件：
               +10         -10
                ↓            ↓
diff:          [0, 10, 0, 0, -10]

对 diff 做一次从左到右前缀累加：
0 -> 10 -> 10 -> 10 -> 0
正好恢复出区间 2..4 的持续影响。

所以差分可以看成“只记录台阶在哪里升高、在哪里降回去”。""",
        "core": "区间整体加值时，不逐点修改；只在区间开始处打开影响，在区间结束后的第一个位置关闭影响，最后前缀累加恢复真实值。",
        "formula": """原题航班编号是 1-based。
转成 0-based：
first = booking[0] - 1
afterLast = booking[1]

对区间 [first, afterLast-1] 加 seats：
diff[first] += seats
diff[afterLast] -= seats

最后：
running += diff[i]
answer[i] = running""",
        "steps": [
            "建立长度 n+1 的 diff，额外位置用于安全记录最后一个区间之后的关闭事件",
            "每条 booking 只更新两个边界：开始位置 +seats，结束后一位 -seats",
            "所有 booking 的边界事件可以直接叠加",
            "从左到右对 diff 求前缀和，得到每个航班最终座位数",
        ],
        "proof": "从 first 开始，running 多出 seats；在到达 afterLast 之前都没有相反事件，因此影响持续存在；到 afterLast 时减去 seats，影响恰好从原闭区间 last 的下一位结束。多个区间满足线性叠加。",
        "pitfalls": "题面航班是 1-based，而 vector 是 0-based；结束标记不是 last-1，而是原题编号 last 映射后的“下一位置” afterLast；不要真的对区间内每个元素逐项加。",
        "transfer": "LC-303 是“已知每个点，预处理累计值以快速查区间”；本题反过来是“已知很多区间更新，只记录边界变化，再累计恢复每个点”。差分与前缀和互为一对操作。",
        "memory": "区间开始 +x，区间结束后一位 -x，最后做前缀和。",
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
