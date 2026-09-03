"""Additional high-touch pedagogy overrides for Week 1.

This module is kept separate from the initial override set so Week 1 can continue
to be reviewed problem by problem without turning one registry file into a large
monolith.  refined_data.py merges these entries with the baseline registry.
"""

PEDAGOGY_WEEK1 = {
    209: {
        "visual": """例：nums = [2, 3, 1, 2, 4, 3], target = 7。

right 一直向右扩张：
[2, 3, 1, 2]  sum = 8  已经 >= 7
 L        R

既然已经够了，就尝试从左边“挤掉”元素，让窗口更短：
去掉 2 后：
   [3, 1, 2]  sum = 6  又不够了
    L     R

因为数组里全是正数，右边加元素只会让 sum 变大，左边删元素只会让 sum 变小。
所以窗口一旦“够了”就放心缩左边；缩到“不够”的那一刻，再继续扩右边。""",
        "core": "右边负责把窗口扩到够用，左边负责在仍然够用时尽可能压缩；正数保证这两个方向都不需要回头。",
        "formula": """windowSum = nums[left] + ... + nums[right]

所有 nums[i] > 0，因此：
right++  => windowSum 只会增大
left++   => windowSum 只会减小

当 windowSum >= target 时：
先记录当前长度，再不断 left++，直到 windowSum < target。""",
        "steps": [
            "left=0，sum=0，best 设为 n+1 哨兵",
            "right 每前进一步，把 nums[right] 加入 sum",
            "只要 sum>=target，就先更新 best，再减去 nums[left] 并 left++",
            "收缩到 sum<target 后停止，继续扩张 right",
            "扫描结束，best 未更新则返回 0",
        ],
        "proof": "固定一个 right 时，随着 left 右移，正数保证窗口和只会下降。因此 while 循环会从较长合法窗口一路缩到最短合法窗口，再多缩一步就非法；这个 right 下不可能还有更短合法起点被漏掉。left/right 都只向右移动，所以总复杂度 O(n)。",
        "pitfalls": "正数条件是整个算法成立的根基；若含负数，删掉左端后 sum 可能反而变大，窗口就失去单调性。更新 best 必须发生在移除左端之前；sum 用 long long 更稳妥。",
        "transfer": "判断能否用滑动窗口时，不要先看题目有没有“子数组”，而要先问：扩右、缩左时目标状态是否具有单调性。含负数的最短子数组和需要换前缀和/单调队列等方法。",
        "memory": "右边扩到够 -> 左边缩到刚好不够 -> 再扩右边。",
    },

    3: {
        "visual": """例：s = \"abba\"。

扫描到第二个 b（right=2）时：
 a  b  b  a
 0  1  2  3
    L  R

b 上次在 1，所以为了不重复，left 必须跳到 2。

随后扫描到最后一个 a（right=3）：
a 上次在 0，last[a]+1 = 1；但当前 left 已经是 2：
 a  b  b  a
 0  1  2  3
       L  R

如果把 left 退回 1，窗口会重新包含两个 b。
所以真正的更新不是 left=last[c]+1，而是：
left = max(left, last[c]+1)。""",
        "core": "新字符只会和它最近一次出现的位置发生冲突；若那个旧位置仍在当前窗口里，就把 left 直接跳过它，但 left 永远不能回退。",
        "formula": """窗口不变量：s[left..right] 内没有重复字符。

当前字符 c = s[right]：
left = max(left, last[c] + 1)

更新后：
当前窗口中旧的 c 已被排除；
left 单调不减；
因此 [left,right] 仍无重复。""",
        "steps": [
            "last 全部初始化为 -1，left=0，best=0",
            "right 从左到右扫描当前字符 c",
            "执行 left=max(left,last[c]+1)，必要时直接跨过最近一次冲突",
            "此时 [left,right] 是以 right 结尾的最长无重复后缀，用它更新 best",
            "最后记录 last[c]=right，供未来字符查询",
        ],
        "proof": "对固定 right，唯一新引入的重复风险来自当前字符 c，因为旧窗口在处理 right 前已经无重复。跳过 c 的最近旧位置后，新窗口重新合法；又因为 left 不回退，所以不会把过去已经排除的其他冲突重新放回来。",
        "pitfalls": "最容易错的是忘记 max(left,...) 导致 left 回退；字符串可为空；当前实现用 256 大小数组按字节记录位置，索引时转 unsigned char，避免有符号 char 造成负下标。",
        "transfer": "这是一类“遇到冲突时直接跳到冲突位置之后”的窗口，而不是一步一步 while 收缩。凡是能记录最近冲突位置的问题，都可以考虑这种跳跃式 left 更新。",
        "memory": "看到 c -> 找 c 上次在哪 -> left 只向右跳过它，绝不回头。",
    },

    438: {
        "visual": """把 p 看成一张“欠账表”。例：p = \"abc\"：

need[a]=1, need[b]=1, need[c]=1, missing=3

当窗口依次收入 c、b、a：
收入 c：欠 c 的 1 份被还掉，missing 3->2
收入 b：missing 2->1
收入 a：missing 1->0

窗口长度又恰好等于 |p|=3，于是这个窗口一定就是 p 的一个异位词。

need 可以变成负数：负数表示某字符“收多了”。窗口向右滑时始终做两件事：右边进一个，左边出一个。""",
        "core": "固定长度 |p| 的窗口维护一张字符欠账表；当总欠账 missing 为 0 时，等长窗口的字符频次就与 p 完全一致。",
        "formula": """need[c] > 0：窗口还欠 c
need[c] = 0：c 的配额刚好
need[c] < 0：c 在窗口里有富余

字符进入窗口：
if (need[in] > 0) missing--;
need[in]--;

字符离开窗口：
need[out]++;
if (need[out] > 0) missing++;

窗口长度 == |p| 且 missing == 0 => 异位词。""",
        "steps": [
            "统计 p 的字符频次到 need，并令 missing=|p|",
            "right 每次收入 s[right]：若它正在欠账则 missing--，然后 need[in]--",
            "若窗口长度超过 |p|，从 left 移出一个字符并恢复它的 need",
            "窗口长度恰为 |p| 且 missing==0 时，记录 left",
        ],
        "proof": "missing 统计的是尚未满足的字符槽位总数。固定窗口长度等于 |p| 时，如果 missing=0，说明 p 要求的每一份字符都已满足；窗口总字符数又没有额外空间，因此不可能还有某字符多出来而另一个字符不够，频次向量必然完全相同。",
        "pitfalls": "need 允许为负，不要把负数误判为错误；字符离开时要先 ++need[out]，再判断是否重新产生欠账；p 比 s 长时自然不会形成长度 |p| 的窗口。",
        "transfer": "LC-76 使用完全相同的 need/missing 欠账模型，只是 LC-438 的窗口长度固定，而 LC-76 在欠账清零后会主动收缩以求最短覆盖。先理解 LC-438，再看 LC-76 会更顺。",
        "memory": "右边进一个、左边出一个；长度固定，missing==0 就命中。",
    },

    76: {
        "visual": """仍然把 t 看成“欠账表”。例：t = \"ABC\"，初始欠 A/B/C 各 1 份，missing=3。

s = A D O B E C O D E B A N C
    L         R

right 扩到第一个 C 时，A/B/C 都已经收齐，missing=0：
窗口 \"ADOBEC\" 合法。

这时不再继续盲目扩右，而是开始移动 left，尝试把无关或富余字符挤掉。
一旦移走 A：need[A] 从 0 变回 1，missing 从 0 变 1，说明窗口刚刚失去覆盖能力。
于是停止收缩，继续向右寻找下一份 A。

整个过程就是：欠账未清 -> 扩右；欠账清零 -> 缩左；刚重新欠账 -> 再扩右。""",
        "core": "right 负责把缺的字符收齐，missing==0 后 left 负责尽量丢掉多余字符；直到刚丢掉一个必需字符，再重新扩张。",
        "formula": """need[c] > 0：当前窗口还欠 c
need[c] = 0：c 的需求刚好满足
need[c] < 0：c 有富余
missing：所有尚未满足的字符槽位总数

收入 rc：
if (need[rc] > 0) missing--;
need[rc]--;

移出 lc：
need[lc]++;
if (need[lc] > 0) missing++;  // 刚失去一份必需字符

missing == 0 <=> 当前窗口覆盖 t。""",
        "steps": [
            "统计 t 的频次到 need，missing 初始化为 |t|",
            "right 向右收入字符；若它能偿还欠账则 missing--，随后 need--",
            "当 missing==0 时，当前窗口已覆盖 t：先更新最短答案",
            "然后不断移出 left 字符；若某次移出让 need[lc]>0，则 missing++，说明窗口刚变非法，停止收缩",
            "继续扩张 right，直到再次把欠账补齐",
        ],
        "proof": "每次 right 固定后，只要窗口仍合法就持续收缩，因此会检查这个 right 对应的所有合法左边界，并在刚变非法前得到该 right 的最短合法窗口。need 的正/零/负精确区分欠缺、刚好和富余，missing=0 因而等价于全部配额满足。",
        "pitfalls": "t 中重复字符是按次数欠账，不是按字符种类；更新最优答案必须发生在移除 left 之前；need 为负表示富余是正常状态；无解时返回空串。",
        "transfer": "LC-438 与本题共用同一套字符配额账本：438 固定窗口长度，只判断是否完全匹配；76 窗口长度可变，在满足配额后继续缩左求最短。以后看到“包含某些字符及其次数”的窗口题，优先想到 need/missing。",
        "memory": "欠账没清就扩右；清零后缩左；刚重新欠一份就停。",
    },
}
