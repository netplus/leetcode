# 哨兵 / 虚拟边界（Sentinel）

> 一句话：给"还没开始 / 不存在"的状态一个合法化的占位值，让"从起点就合法"的情况和"中途合法"的情况走**同一条代码路径**，用值代替判断。
> 朴素写法要 `if (特殊) ... else (通用) ...`；哨兵让特殊值落在通用公式里，于是 if 消失。

## 0. 它替你省掉了什么

| 题 | 哨兵 | 原本需要的 if |
|----|------|--------------|
| LC-3 无重复最长子串 | `last[c]` 初始化 `-1` | 若 c 首次出现，`last[c]+1=0=max(left,0)`，天然不回退；不用判 `last[c]==-1` |
| LC-560 和为 K 的子数组 | `frequency[0]=1` | "从下标 0 开始的子数组"等价于"减去空前缀"；不用特判 `prefix==k` |
| LC-1248 优美子数组 | `frequency[0]=1` | 同上；"整个前缀恰好含 k 个奇数"不用特判 |
| LC-209 / LC-303 / LC-304 | 前缀和数组前置 `pre[0]=0` | "从下标 0 开始的区间和"也能用 `pre[i+1]-pre[0]` 统一表达 |
| 链表 dummy 头节点 | 一个不持有数据的头节点 | 头插/尾插/中间插代码完全一样，不用特判 `head` 是否为空 |
| LC-146 LRU / LC-460 LFU | 双向链表首尾各放哨兵节点 | 插入删除永远不用判空 |
| 二分查找 `hi=n` | 虚拟右界 | 允许"插在末尾"作为合法答案，省 `lo<=n` 特判 |
| 区间 DP `dp[i][i-1]` | 空区间赋初值 | 长度 1 的 base case 不用逐个特判 |
| LC-1248 atMost 版 | `atMost(-1)` 恰好返回 0 | "恰好 k = atMost(k)-atMost(k-1)"在 k=0 时自洽 |

> 共同点：都对应一个**不存在的位置/状态**（`j=-1`、空前缀、空区间、虚拟头/尾），哨兵就是给那个虚拟位置赋一个让公式自洽的初值。

## 1. LC-3：`last[c] = -1` 为什么是神来之笔

```cpp
array<int, 256> last;
last.fill(-1);                 // 哨兵：字符"上次出现"在虚拟位置 -1
int left = 0, best = 0;
for (int right = 0; right < s.size(); ++right) {
    unsigned char c = s[right];
    left = max(left, last[c] + 1);   // 首次出现时 last[c]+1 = 0，等价于"不回退"
    best = max(best, right - left + 1);
    last[c] = right;
}
```

**朴素版需要两个分支：**

```cpp
if (last[c] == -1) {
    // 首次出现，left 不动
} else {
    left = max(left, last[c] + 1);
}
```

**哨兵版为什么对：** "首次出现"等价于"上一次出现在位置 -1"。给 `last[c]` 赋 `-1` 后，`last[c]+1 = 0`，而 `max(left, 0) = left`（left≥0 恒成立）——特殊情形悄悄并进了主公式，if 消失。

**推导路径（可复用）：**
1. 写下朴素 if-else，看特殊分支里的值能不能写成"通用值 ± 偏移"；
2. 找"虚拟位置/状态"——这题里是 `j=-1`；
3. 给虚拟位置赋一个让公式自洽的初值 → 哨兵成立。

## 2. LC-560 / LC-1248：`frequency[0] = 1` 是同一个哨兵

```cpp
unordered_map<long long, int> frequency;
frequency[0] = 1;              // 哨兵：还没看任何元素时，前缀和为 0
long long prefix = 0;
int answer = 0;
for (int value : nums) {
    prefix += value;
    auto it = frequency.find(prefix - k);   // 查"减去谁得到 k"
    if (it != frequency.end()) answer += it->second;
    ++frequency[prefix];
}
```

**为什么需要它：** 以 `i` 结尾、和为 `k` 的子数组，起点是某个 `j`，减去的是"前缀 `j-1`"。当 `j=0` 时 `j-1=-1`——"前缀 -1"就是**空前缀，它的和是 0**。`frequency[0]=1` 不是魔法，而是"给 -1 这个虚拟位置一个真实的状态值"。

**少了它会怎样：** 数组 `[1,2,3]`、`k=6`，整个数组和为 6。扫描到末尾 `prefix=6`，查 `frequency[6-6]=frequency[0]`——若没初始化，查不到，漏掉这个答案。

> LC-1248 是 LC-560 的直接变形：先把奇数映射成 1、偶数 0，再原样复用 `frequency[0]=1`。哨兵值不变，因为"还没看任何元素时累计奇数数也是 0"。

## 3. 哨兵的三大类型

### 类型 A：哨兵值让公式自洽（"首次出现"型）
用"非法值 +1 = 合法起点"消除首次判断。

- **LC-3**：`last[c]=-1`
- **LC-209 / LC-303 / LC-304**：前缀和前置 `pre[0]=0`，让"从 0 开始的区间"也走 `pre[r+1]-pre[l]`
- **二分查找 `hi=n`**：虚拟右界让"插在末尾"合法

### 类型 B：虚拟节点简化边界（链表/树/区间）
给"头之前""尾之后"一个实体节点，首尾操作和中间操作走同一套指针代码。

- **链表 dummy 头**：LC-21、LC-206、LC-92、LC-143、LC-23
- **双向链表首尾哨兵**：LC-146 LRU、LC-460 LFU——插入删除永远不判空，代码量减半
- **树状数组/线段树下标从 1**：`tree[0]` 当哨兵不参与运算（`lowbit(0)=0` 会死循环），整体偏移 1
- **区间 DP 空区间**：`dp[i][i-1]` 表空区间赋 0——LC-312 戳气球、LC-516 最长回文子序列

### 类型 C：差分消解"恰好"
"恰好 K"用"至多 K − 至多 K−1"差分，其中边界哨兵让差分自洽。

- **LC-1248 atMost 版**：`恰好 k = atMost(k) - atMost(k-1)`，`atMost(-1)` 返回 0
- **LC-992 K 个不同整数的子数组**：完全同构
- **LC-713 乘积 < K 的子数组**：`<K` 直接做，但 `≤K` 与 `<K+1` 的差分思想一致

## 4. 怎么自己"想到"哨兵

做题时套三条检查清单：

1. **写下朴素 if-else 后，问：特殊分支里的值，能不能写成"通用值 ± 偏移"？** 能 → 哨兵。
2. **找"虚拟位置/状态"：问题里是否存在 `j=-1`、空前缀、空区间、虚拟头/尾？给它们赋一个让公式成立的初值。**
3. **看到"恰好 K"先想 `至多 K − 至多 K−1`**，其中 `K−1`（尤其 `−1`）就是哨兵。

## 5. 注意事项

- **哨兵值不能和任何真实状态冲突。** LC-3 用 `-1` 因为下标从 0 开始；若题目用 `-1` 表示"已删除"等含义，就得换值（如 `-2` 或 `INT_MIN`）。
- **哨兵要和公式配套。** LC-3 的 `-1` 配 `+1` 运算；LC-560 的 `0` 配"前缀和"语义。换公式就得换哨兵值。
- **哨兵不是万能。** 它让代码更短更稳，但**不改变复杂度**。别为了用哨兵硬凑——朴素 if-else 有时更易读，哨兵的价值在"消除分支、减少 bug 面"。
- **初学者陷阱：** 哨兵让代码"看起来没处理边界"，反而最难审题。写完后必须用边界用例（空串、全相同、单元素、整个数组就是答案）验证一遍。

## 6. 关联题目索引

| 题 | 哨兵类型 | 位置 |
|----|---------|------|
| LC-3 | A | [day3-lc3](../../problems/week1-arrays-strings/day3-lc3-longest-substring-without-repeating-characters/solution.cpp) |
| LC-560 | A | [day1-lc560](../../problems/week1-arrays-strings/day1-lc560-subarray-sum-equals-k/solution.cpp) |
| LC-1248 | A/C | [day1-lc1248](../../problems/week1-arrays-strings/day1-lc1248-count-number-of-nice-subarrays/solution.cpp) |
| LC-209 | A | [day3-lc209](../../problems/week1-arrays-strings/day3-lc209-minimum-size-subarray-sum/solution.cpp) |
| LC-303 | A | [day4-lc303](../../problems/week1-arrays-strings/day4-lc303-range-sum-query-immutable/solution.cpp) |
| LC-304 | A | [day4-lc304](../../problems/week1-arrays-strings/day4-lc304-range-sum-query-2d-immutable/solution.cpp) |
| LC-21 | B | [day8-lc21](../../problems/week2-list-tree-bsp/day8-lc21-merge-two-sorted-lists/solution.cpp) |
| LC-206 | B | [day8-lc206](../../problems/week2-list-tree-bsp/day8-lc206-reverse-linked-list/solution.cpp) |
| LC-146 | B | [day25-lc295 邻近](../../problems/week4-binary-search-greedy-mock/day25-lc295-find-median-from-data-stream/solution.cpp) |
| LC-312 | B | [day20-lc312](../../problems/week3-graph-dp/day20-lc312-burst-balloons/solution.cpp) |
