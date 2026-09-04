"""High-touch pedagogy overrides for Week 3 / Day 21.

Day 21 deliberately connects four earlier models instead of introducing four
new templates: path-local backtracking from Day 13, prefix reachability from the
DP days, dominated-state compression from maximum-subarray style reasoning, and
Kahn topological ordering from LC-207.
"""

PEDAGOGY_WEEK3_DAY21 = {
    79: {
        "pattern": "路径回溯：把 Day 13 的选择-递归-撤销迁移到二维网格",
        "model": "状态由当前位置 (r,c) 与正在匹配的 word[index] 组成。当前格字符匹配后，它只能在这一条递归路径里被占用；临时把格子改成输入不可能出现的哨兵，相当于 used[r][c]=true，搜索四邻结束后再恢复。这里 visited 不能是全局的，因为同一个格子可以属于不同候选路径，只是不能在同一条路径中重复使用。",
        "visual": """以 word=ABCCED 为例：

A B C E
S F C S
A D E E
^
index=0

选中 A 后，只能从它的上下左右寻找 B；找到 B 后继续寻找 C。
每深入一层，都把当前格暂时封住：

A* -> B* -> C* -> C* -> E* -> D

星号表示“当前路径已经用过”。若某一步走不通，就撤销最后一个星号，再尝试上一层的其他方向。

这和 LC-46 全排列的 used 完全同构：
LC-46 的候选是数组下标；本题的候选是四邻格子。
LC-46 回退时 used[i]=false；本题回退时恢复 board[r][c]。""",
        "core": "从每个可能的首字母出发，沿四邻递归匹配下一个字符；当前路径使用过的格子临时封锁，返回上一层时恢复。",
        "formula": """search(r,c,index) 的含义：
是否存在一条从 (r,c) 开始的合法简单路径，恰好匹配 word[index..]。

进入状态的必要条件：
0 <= r < rows
0 <= c < cols
board[r][c] == word[index]

若 index+1 == word.size()：当前字符已经完成最后一个匹配，返回 true。
否则临时标记当前格，再枚举四个后继：
(r+1,c), (r-1,c), (r,c+1), (r,c-1)。

路径不变量：递归栈中已经匹配的每个格子互不重复，并且字符序列严格等于 word[0..index]。""",
        "steps": [
            "外层枚举每个格子作为 word[0] 的候选起点；search 自己负责字符不匹配时立即失败",
            "进入 search 后先检查边界和字符；若当前 index 已是最后一个字符，直接成功",
            "保存 board[r][c]，再把它改成哨兵，表示这个格子只在当前递归路径中暂时不可用",
            "依次尝试四个相邻方向去匹配 index+1；任一方向成功即可令 found=true",
            "无论 found 是否为 true，都恢复原字符后再返回，保证输入结构和兄弟搜索分支不受污染",
        ],
        "proof": "任意被算法接受的递归链，每一层都只从上一个格子的四邻进入，字符又经过 word[index] 检查；路径内已使用格被哨兵阻止再次进入，因此它一定满足题目的相邻、字符顺序和不重复约束。反过来，任意合法路径都有一个首格，外层枚举一定会尝试它；在路径的每个位置，四方向枚举一定包含合法路径的下一格，而路径中的格子此前未重复，因此不会被哨兵错误阻止，所以整条合法路径必能被枚举到。",
        "pitfalls": "不要使用永久的全局 visited：某个格子在路径 A 中失败后，仍可能在路径 B 中合法使用。恢复动作不能因为短路成功而省略，否则 exist 返回前会改变调用者的 board；当前实现先保存 found、再统一恢复。哨兵必须选择题面不会出现的字符，'\\0' 满足当前英文字母约束。",
        "transfer": "直接复用 Day 13 的回溯框架：状态选择 -> 做选择 -> 递归 -> 撤销选择。它也和 Day 15 的网格 BFS 形成对比：连通分量问题的 visited 是全局事实，而简单路径搜索的 visited 只是当前路径约束。数独、迷宫简单路径、棋盘单词搜索都应先问 visited 究竟是全局还是 path-local。",
        "memory": "路径内占用，回退时恢复；visited 的生命周期就是递归栈。",
    },

    139: {
        "pattern": "前缀可达 DP：把指数分割搜索压缩成字符串位置上的 DAG 可达性",
        "model": "把字符串的 n+1 个切分位置看成节点 0..n；若 s[i:j] 是字典词，就存在一条 i->j 的边。题目问的就是 0 能否到达 n。reachable[i] 表示前 i 个字符已经能被完整切分；只从可达起点继续接词。字典哈希和最大词长分别消除重复线性查找与明显不可能的长度候选。",
        "derivation": """最自然且正确的直接算法是 DFS：从 start 开始枚举下一段结束位置 end，只要 s[start:end] 在字典中，就递归处理 end；到达 n 时成功。它枚举所有合法切分前缀，因此不会漏解。

真正的浪费在于不同切分路径会反复到达同一个 start。例如 s 的前缀既可以按若干短词到达位置 i，也可能按一个长词到达同一 i；一旦 start 相同，后缀 s[start:] 能否拆分与之前怎样到达这里完全无关。直接 DFS 会重复计算同一“从位置 start 出发能否成功”的子问题。

把这个重复状态记下来就得到 memo；再反过来自底向前传播，就是前缀可达 DP。我们甚至可以把它看成一个隐式 DAG：位置 i 到 j 有边，当且仅当 s[i:j] 是字典词。reachable[i]=true 就是节点 i 已经从 0 可达。

此外，直接枚举每个 start 的所有 end 仍会检查大量长度明显超过所有字典词的子串；预先保存 maxLength 后，长度只需尝试 1..maxLength。字典本身放入 unordered_set，把“这个候选词是否存在”的重复线性扫描改成哈希查询。

优化链：枚举所有切分路径 -> 相同 start 的后缀被重复求解 -> 用位置状态复用结果 -> 只从可达前缀扩展 -> 哈希字典 + 最大词长限制候选，这就是前缀可达 DP。""",
        "visual": """s = "leetcode"，wordDict={"leet","code"}

切分位置：
0 1 2 3 4 5 6 7 8
|---leet---|---code---|
0 --------> 4 -------> 8

reachable[0]=true 表示空前缀已经完成。
从 0 发现 "leet"，于是 4 可达；
从 4 发现 "code"，于是 8 可达。

如果某个位置 i 不可达，即使 s[i:j] 恰好是字典词，也不能用它：因为前面的 s[0:i] 尚未形成合法切分。
这和图搜索中的 frontier 完全一样，只扩展已经真正到达的状态。""",
        "core": "把每个切分位置当成状态；只有当前前缀已经可达时，才尝试接一个字典词并把新的结束位置标成可达。",
        "formula": """reachable[i] = s[0:i) 是否能被字典词完整切分。

初始化：
reachable[0] = true

转移：
若 reachable[start] 为 true，且
1 <= length <= maxLength，
start+length <= n，
s[start:start+length] 属于字典，
则：
reachable[start+length] = true。

等价的经典写法是：
reachable[end] = OR over start<end (
    reachable[start] AND s[start:end] in dict
)

不变量：扫描到 start 时，reachable[0..start] 已准确描述这些前缀的可切分性；任何新置 true 的终点都由一个合法前缀再追加一个完整字典词得到。""",
        "steps": [
            "把 wordDict 建成 unordered_set，同时计算 maxLength，避免每次候选都线性扫描整个字典并限制无意义长度",
            "建立 n+1 个前缀状态并令 reachable[0]=true；位置 0 代表空前缀这个合法起点",
            "按 start 从左到右扫描；若 reachable[start] 为 false，说明根本没有合法切分能到达这里，直接跳过",
            "从该可达位置枚举 1..maxLength 的候选长度，检查对应 substring 是否在哈希字典中",
            "命中时把 reachable[start+length] 置 true；最终返回 reachable[n]",
        ],
        "proof": "每次转移都从一个已经能完整切分的前缀开始，并只追加一个完整字典词，因此所有被标记为 true 的状态都真实可达。对任意合法的完整切分 w1,w2,...,wk，其切分边界依次为 p0=0,p1,...,pk=n；reachable[0] 初始为真，若 pi 已被标记，算法枚举长度 |w(i+1)| 时必发现该字典词并标记 p(i+1)，归纳可得 n 最终必被标记，因此不会漏解。",
        "pitfalls": "reachable[0] 不能漏掉，否则所有切分都没有起点。字典词允许重复使用，所以这里不是 LC-416 那种‘每个物品一次’的倒序背包；同一个 word 可以从不同 start 多次形成边。substr 会产生临时字符串，约束下足够；若追求更强性能可改 trie 或 string_view 风格索引，但不能无意使用 C++20 API。",
        "transfer": "这是 Day 19/20 DP 的另一种视角：状态仍是‘前缀是否可达’，只是边由字典词决定。它也可以直接看成隐式 DAG 的 reachability；解码方法、字符串分段、最少分词数都能在同一位置图上把布尔值换成计数或最小代价。",
        "memory": "位置是节点，字典词是边；只从已经可达的切分点继续走。",
    },

    152: {
        "pattern": "双极值状态 DP：负数会翻转候选优劣，所以同时保留最大与最小后缀",
        "model": "沿用最大子数组的‘必须以当前位置结尾’状态，但加法有序性在乘法遇负数时失效：此前最大的正乘积乘负数会变成最小，而此前最小的负乘积乘负数反而可能变成最大。因此面对相同未来，不能只保留一个 best-ending-here；必须同时保留 currentMax 与 currentMin 两个不被彼此支配的极值。",
        "derivation": """一个自然正确的直接算法是枚举每个左端点，再向右不断累乘并更新全局最大值；它检查了所有连续子数组，时间 O(n^2)。

对最大子数组和 LC-53，我们曾经利用候选支配：固定同一个终点时，只保留最大的后缀和就够了，因为未来无论再加什么值，两个候选都会加同一个量，较差者永远不能反超。

乘法却破坏了这个单状态支配关系。假设以当前位置结尾的两个候选乘积是 6 和 -10。面对未来 x=-2：
6 * -2 = -12，
-10 * -2 = 20。
原来最差的 -10 反而成为最好。因此“只留最大值”会丢掉未来可能翻身的候选。

但仍不需要保留所有后缀乘积。未来只会把每个候选统一乘同一个 x：x>0 时大小顺序保持，x<0 时大小顺序完全翻转，x=0 时全部归零。因此在任何时刻，真正可能成为下一步最大或最小的只需要当前极大值与极小值；中间候选无论正乘还是负乘都不会超出这两个边界。

于是状态压缩成 currentMax/currentMin。遇到负数可以先交换两者，把‘乘负数后角色互换’显式编码，再分别决定是从 value 单独重开还是延续旧后缀。

优化链：O(n^2) 枚举所有连续区间 -> 尝试复用 LC-53 的 ending-here 压缩 -> 发现负数会让最大/最小互换 -> 中间候选仍被两个极值支配 -> 双极值 O(n) DP。""",
        "visual": """nums = [2,3,-2,4]

处理到 3：
currentMax = 6
currentMin = 3

遇到 -2 时，未来角色翻转：
最大的 6 乘 -2 -> -12
较小的 3 乘 -2 -> -6
所以负号让“大”朝“小”方向走。

更关键的例子：
候选 6 与 -10，下一项 -2：
6   -> -12
-10 ->  20

因此负乘积不是垃圾状态；它可能只差一个负数就翻成最大正乘积。
代码中的 swap(currentMax,currentMin) 就是在 value<0 时先交换‘谁有资格产生最大/最小’。""",
        "core": "固定当前终点时同时保存最大和最小乘积；正数保持二者角色，负数交换角色，0 则自然让后续状态重新开始。",
        "formula": """设上一位置：
oldMax = 以 i-1 结尾的最大乘积
oldMin = 以 i-1 结尾的最小乘积
x = nums[i]

所有以 i 结尾的候选只有三类：
x
oldMax * x
oldMin * x

所以数学上：
newMax = max(x, oldMax*x, oldMin*x)
newMin = min(x, oldMax*x, oldMin*x)

当前实现把 x<0 的情况先 swap(oldMax,oldMin)，之后可写成：
currentMax = max(x, currentMax*x)
currentMin = min(x, currentMin*x)

best = max(best, currentMax)

不变量：处理完 i 后，currentMax/currentMin 精确覆盖所有以 i 结尾非空连续子数组乘积的最大值/最小值；best 是所有已处理终点中的全局最大值。""",
        "steps": [
            "用 nums[0] 同时初始化 currentMax、currentMin 和 best，确保单元素、全负数组都不会被错误的 0 初值覆盖",
            "从第二个元素开始读取 value；若 value<0，先交换 currentMax/currentMin，因为乘负数会把旧极大候选变成极小来源、旧极小候选变成极大来源",
            "更新 currentMax=max(value,currentMax*value)，在‘从当前元素重新开始’与‘延续旧后缀’之间选择最大",
            "更新 currentMin=min(value,currentMin*value)，保留未来遇到负数时可能翻成最大值的最小后缀",
            "用 currentMax 更新历史 best；遍历结束返回 best",
        ],
        "proof": "所有以 i 结尾的连续子数组，要么只包含 nums[i]，要么由某个以 i-1 结尾的连续子数组再乘 nums[i] 得到。统一乘正数时，旧乘积集合的最大/最小仍分别产生新集合的最大/最小；统一乘负数时次序完全反转，所以旧最小产生新最大、旧最大产生新最小；乘 0 时所有延续候选都是 0。故除了旧最大和旧最小，其余中间候选永远不可能产生边界极值，可以永久丢弃。逐点维护后，best 再覆盖所有可能终点，因此没有漏掉任何连续子数组。",
        "pitfalls": "不能照搬 LC-53 只保存一个 currentMax；负数会让旧最小翻身。更新顺序若不用 swap，就必须先保存 oldMax/oldMin，否则先改 currentMax 会污染 currentMin 的计算。0 无需特判：max/min 与 value=0 会自然重置状态。best 必须初始化为 nums[0]，不能初始化为 0。",
        "transfer": "这是 LC-53 候选支配思想的升级版：面对相同未来，若运算保持顺序就只留一个极值；若运算可能反转顺序，就保留足够描述边界的多个极值状态。带符号乘积、最大绝对值路径以及某些区间变换 DP 都有类似的‘最大+最小’成对状态。",
        "memory": "加法只留最好；乘法遇负号会翻面，所以最好和最坏都要留。",
    },

    210: {
        "pattern": "LC-207 的输出型迁移：Kahn 拓扑排序把可完成性证明直接变成课程顺序",
        "model": "完全复用 LC-207：prerequisites=[a,b] 建边 b->a，indegree[a] 记录尚未完成的先修数量。队列中的课程都是当前依赖已清零、可以立即学习的 frontier。LC-207 只统计最终处理数来回答能否完成；本题在每次出队时把课程追加到 order，于是同一消除过程本身就是一份合法拓扑序。",
        "visual": """prerequisites = [[1,0],[2,0],[3,1],[3,2]]

依赖图：
0 -> 1 -> 3
 \\-> 2 -/

初始 indegree：
0:0  1:1  2:1  3:2
ready=[0]

输出 0 后，删除它的两条出边：
1、2 的入度都变 0，所以两者都可以进入 ready。
此时输出 1 还是 2 都合法，这正解释了为什么答案不唯一。

若先输出 2，再输出 1：
order = [0,2,1,3]

每个被写入 order 的节点，在写入那一刻都已经没有未完成先修。""",
        "core": "原样执行 LC-207 的入度消除，只是在每个零入度课程出队时把它记录下来；若最终记录了全部课程，这个记录顺序就是答案。",
        "formula": """对 prerequisite [a,b]：
edge: b -> a
indegree[a]++

ready = 所有 indegree[v]==0 的课程。

每次取出 course：
order.push_back(course)
对每个 dependent in next[course]：
    indegree[dependent]--
    若变成 0：加入 ready

循环不变量：
order 中每门课出现时，它的所有先修都已经出现在 order 更早的位置；ready 中所有节点当前都没有尚未输出的前驱。

结束条件：
order.size()==numCourses -> order 是一个拓扑序
否则 -> 剩余子图存在有向环，返回空数组。""",
        "steps": [
            "按照 [a,b] 表示 b 是 a 的先修，建立 b->a 邻接表并累计 indegree[a]；不要把方向写反",
            "把所有初始入度为 0 的课程放入 ready，它们不依赖任何尚未完成课程，可以作为拓扑序的起点",
            "循环弹出一个 ready 课程，立即追加到 order；这个动作就是 LC-207 中‘处理一个可完成课程’的可见输出",
            "遍历它的所有后继并递减入度；某后继恰好降到 0 时，说明它最后一个未完成先修刚被解除，将其加入 ready",
            "队列清空后比较 order.size() 与 numCourses；相等返回 order，否则返回空 vector 表示存在环",
        ],
        "proof": "一个课程只有在入度降为 0 后才会进入 ready，因此它被输出时，所有指向它的先修边都已经由更早输出的前驱删除，故 order 的每个前缀都满足先修约束。每条边只在其起点出队时处理一次，每个节点只会在入度第一次降到 0 时入队一次，不会重复。若图是 DAG，任意非空剩余 DAG 必存在零入度点，所以过程能输出全部节点；若最终仍有节点未输出，剩余子图没有零入度点，只可能包含有向环，因此不存在完成全部课程的合法顺序。",
        "pitfalls": "最常见错误仍是把 [a,b] 建成 a->b。合法拓扑序可能有很多种，不应把算法绑死到某个官方输出；本地/外部验证应检查先修相对顺序。numCourses=1 且 prerequisites 为空时应返回 [0]；存在环时返回真正的空 vector，而本地 main 才把它打印成 -1。",
        "transfer": "这是 Day 15 LC-207 的零新模板迁移：同一个 Kahn 过程，LC-207 的输出是 bool，LC-210 的输出是过程轨迹 order。构建系统、任务调度、依赖安装、编译顺序都可直接输出这条拓扑执行计划；若需要字典序最小顺序，只需把 ready 的 FIFO queue 换成小根堆，正确性不变。",
        "memory": "LC-207 会不会做完；LC-210 把‘做的顺序’顺手记下来。",
    },
}
