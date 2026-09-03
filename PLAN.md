# 4-Week Practice Plan (C/C++ · Leveling Exam Sprint)

> Pace: 2–4 problems per day, ~50 min each. **Finish all P0 first**, then P1.
> Numbers are LeetCode IDs. 🚩 = high-frequency in past leveling / written exams.

---

## Week 1: Arrays / Strings / Hashing / Two Pointers

> Goal: master "scan-style" algorithms — prefix sum, difference array, sliding window, two pointers, hash lookup. The most common category in the written exam; lock it down this week.

### Day 1 — Hashing + Prefix Sum
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 1 | 两数之和 | P0 🚩 | hash `target-nums[i]` |
| 560 | 和为 K 的子数组 | P0 🚩 | prefix sum + hash count |
| 1248 | 统计「优美子数组」 | P1 | like 560, odd/even prefix |
| 53 | 最大子数组和 | P0 🚩 | Kadane / prefix-sum min |

### Day 2 — Two Pointers (opposite + fast/slow)
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 167 | 两数之和 II - 输入有序数组 | P0 | sorted → opposite pointers |
| 15 | 三数之和 | P0 🚩 | sort + two pointers, dedup |
| 11 | 盛最多水的容器 | P0 🚩 | opposite pointers, move short board |
| 283 | 移动零 | P1 | fast/slow pointers |

### Day 3 — Sliding Window
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 209 | 长度最小的子数组 | P0 | variable window |
| 3 | 无重复字符的最长子串 | P0 🚩 | hash positions + left bound |
| 76 | 最小覆盖子串 | P0 🚩 | count hash + need/have |
| 438 | 找到字符串中所有字母异位词 | P1 | fixed-length window |

### Day 4 — Prefix Sum / Difference Array
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 303 | 区域和检索 - 数组不可变 | P0 | 1D prefix sum |
| 304 | 二维区域和检索 - 矩阵不可变 | P0 | 2D prefix sum |
| 1109 | 航班预订统计 | P0 🚩 | difference array |
| 238 | 除自身以外数组的乘积 | P1 | prefix/suffix product |

### Day 5 — Strings
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 5 | 最长回文子串 | P0 🚩 | expand from center / Manacher |
| 14 | 最长公共前缀 | P0 | vertical scan |
| 151 | 反转字符串中的单词 | P1 | two-pointer reverse |
| 28 | 找出字符串中第一个匹配项的下标 | P1 | KMP (hand-written next) |

### Day 6 — Intervals / Sort Variants
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 56 | 合并区间 | P0 🚩 | sort + merge |
| 57 | 插入区间 | P1 | simulate three cases |
| 179 | 最大数 | P1 | custom compare `a+b>b+a` |
| 252 | 会议室 | P1 | sort endpoints |

### Day 7 — Week 1 Review
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 239 | 滑动窗口最大值 | P0 | monotonic deque |
| 41 | 缺失的第一个正数 | P0 | in-place hash, O(n)/O(1) |
| 31 | 下一个排列 | P1 | two-pointer in-place |
| 128 | 最长连续序列 | P0 🚩 | hash set, O(n) |

---

## Week 2: Linked Lists / Trees / BFS-DFS / Stacks & Queues

> Goal: hand-written data structures are "free points" in the written exam — list reversal, tree traversal, stack applications must be flawless.

### Day 8 — Linked List Basics
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 206 | 反转链表 | P0 🚩 | iterative + recursive |
| 92 | 反转链表 II | P0 | range reverse + head-insert |
| 21 | 合并两个有序链表 | P0 | dummy node |
| 160 | 相交链表 | P1 | two pointers, two passes |

### Day 9 — Linked List Advanced
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 141 | 环形链表 | P0 🚩 | fast/slow |
| 142 | 环形链表 II | P0 🚩 | fast/slow + find entry |
| 19 | 删除链表的倒数第 N 个结点 | P0 | fast/slow + dummy |
| 234 | 回文链表 | P1 | find midpoint + reverse second half |

### Day 10 — Binary Tree Traversal
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 144/94/145 | 二叉树的前序遍历 / 二叉树的中序遍历 / 二叉树的后序遍历 | P0 | recursive + iterative (stack) |
| 102 | 二叉树的层序遍历 | P0 🚩 | BFS queue |
| 104 | 二叉树的最大深度 | P0 | DFS / BFS |
| 105 | 从前序与中序遍历序列构造二叉树 | P0 🚩 | recursive divide + hash |

### Day 11 — Binary Tree Properties
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 226 | 翻转二叉树 | P0 | recursive swap |
| 236 | 二叉树的最近公共祖先 | P0 🚩 | post-order DFS |
| 543 | 二叉树的直径 | P0 | post-order height, update answer |
| 98 | 验证二叉搜索树 | P1 | inorder sorted / range recursion |

### Day 12 — Stacks / Queues / Monotonic Stack
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 20 | 有效的括号 | P0 🚩 | stack |
| 155 | 最小栈 | P0 | auxiliary stack |
| 739 | 每日温度 | P0 🚩 | monotonic decreasing stack |
| 84 | 柱状图中最大的矩形 | P1 | monotonic stack |

### Day 13 — Backtracking Intro
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 46 | 全排列 | P0 🚩 | backtracking template |
| 39 | 组合总和 | P0 🚩 | backtracking + pruning |
| 78 | 子集 | P0 | choose / not choose |
| 22 | 括号生成 | P1 | backtracking + left/right counts |

### Day 14 — Week 2 Review
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 148 | 排序链表 | P0 | merge sort, O(n log n)/O(1) |
| 143 | 重排链表 | P0 | midpoint + reverse + merge |
| 124 | 二叉树中的最大路径和 | P1 | post-order, any-to-any path |
| 199 | 二叉树的右视图 | P1 | BFS rightmost per level |

---

## Week 3: Graphs / Union-Find / Dynamic Programming

> Goal: graph BFS/DFS framework + union-find + the five DP models. This is the week that separates "mid" from "senior"; DP is 30%+ of the written exam.

### Day 15 — Graph Traversal
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 200 | 岛屿数量 | P0 🚩 | DFS/BFS coloring |
| 695 | 岛屿的最大面积 | P0 | like 200 |
| 133 | 克隆图 | P1 | BFS + hash |
| 207 | 课程表 | P0 🚩 | topological sort (indegree) |

### Day 16 — BFS / Shortest Path
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 994 | 腐烂的橘子 | P0 🚩 | multi-source BFS |
| 127 | 单词接龙 | P0 | BFS + build graph |
| 542 | 01 矩阵 | P1 | multi-source BFS |
| 417 | 太平洋大西洋水流问题 | P1 | reverse BFS from borders |

### Day 17 — Union-Find
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 547 | 省份数量 | P0 🚩 | union-find template |
| 684 | 冗余连接 | P0 | union-find cycle detection |
| 990 | 等式方程的可满足性 | P1 | union-find |
| 685 | 冗余连接 II | P1 | directed version |

### Day 18 — DP Intro (1D / Linear)
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 70 | 爬楼梯 | P0 | Fibonacci DP |
| 198 | 打家劫舍 | P0 🚩 | linear DP |
| 213 | 打家劫舍 II | P0 | circular → split into two |
| 746 | 使用最小花费爬楼梯 | P1 | linear DP |

### Day 19 — DP Paths / Subsequences
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 62 | 不同路径 | P0 | grid DP |
| 64 | 最小路径和 | P0 | grid DP |
| 300 | 最长递增子序列 | P0 🚩 | O(n²) + binary search |
| 1143 | 最长公共子序列 | P0 🚩 | 2D DP |

### Day 20 — DP Knapsack / Interval
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 416 | 分割等和子集 | P0 🚩 | 0/1 knapsack |
| 494 | 目标和 | P0 | 0/1 knapsack variant |
| 322 | 零钱兑换 | P0 🚩 | complete knapsack |
| 312 | 戳气球 | P1 🚩 | interval DP (hard) |

### Day 21 — Week 3 Review
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 210 | 课程表 II | P0 | topological sort (return order) |
| 79 | 单词搜索 | P0 | backtracking on grid |
| 139 | 单词拆分 | P0 | DP |
| 152 | 乘积最大子数组 | P1 | keep min & max |

---

## Week 4: Binary Search / Greedy / Heap / Mock Exams

> Goal: binary search on answer, greedy strategies, TopK heap; finish with timed mock exams to rehearse real exam pacing.

### Day 22 — Binary Search
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 704 | 二分查找 | P0 | template |
| 33 | 搜索旋转排序数组 | P0 🚩 | segmented binary search |
| 34 | 在排序数组中查找元素的第一个和最后一个位置 | P0 | left/right bound binary search |
| 153 | 寻找旋转排序数组中的最小值 | P1 | binary search |

### Day 23 — Binary Search on Answer
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 410 | 分割数组的最大值 | P0 🚩 | binary search + greedy check |
| 875 | 爱吃香蕉的珂珂 | P0 | binary search on answer |
| 1011 | 在 D 天内送达包裹的能力 | P0 🚩 | binary search on answer |
| 4 | 寻找两个正序数组的中位数 | P1 🚩 | binary search O(log) |

### Day 24 — Greedy
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 455 | 分发饼干 | P0 | sort + two pointers |
| 55 | 跳跃游戏 | P0 🚩 | maintain max reach |
| 45 | 跳跃游戏 II | P0 🚩 | greedy interval coverage |
| 134 | 加油站 | P1 | greedy |

### Day 25 — Heap / TopK
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 215 | 数组中的第 K 个最大元素 | P0 🚩 | min-heap size k / quickselect |
| 347 | 前 K 个高频元素 | P0 🚩 | hash + heap |
| 295 | 数据流的中位数 | P0 🚩 | two heaps |
| 23 | 合并 K 个升序链表 | P1 🚩 | min-heap |

### Day 26 — Comprehensive / Fill Gaps
| # | 中文标题 | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 253 | 会议室 II | P0 | sweep line / heap |
| 42 | 接雨水 | P0 | two pointers / monotonic stack |
| 406 | 根据身高重建队列 | P1 | greedy insert |
| 621 | 任务调度器 | P1 | greedy by count |

### Day 27 — Mock Exam 1 (90 min)
- 3 problems, timed 90 min, no references:
  - one array / two-pointer medium
  - one tree / linked-list hand-written
  - one DP medium
- See `problems/week4-binary-search-greedy-mock/day27-mock-exam-1/MOCK.md`. Wrong answers go to `notes/`.

### Day 28 — Mock Exam 2 + Final Review
- Mock Exam 2 (90 min):
  - one graph / BFS
  - one binary-search-answer / greedy
  - one medium-hard DP
- Second pass over all wrong problems from the plan.
- Compile the "Exam-Day Card" at the top of [docs/patterns.md](docs/patterns.md).

---

## Problem Count

| Week | P0 | P1 | Total |
|------|----|----|-------|
| W1 | 18 | 10 | 28 |
| W2 | 18 | 12 | 30 |
| W3 | 18 | 10 | 28 |
| W4 | 14 | 8  | 22 |
| **Total** | **68** | **40** | **108** |

> ~90 distinct problems over 4 weeks. Finishing all P0 (≈68) covers 90%+ of leveling-exam topics. (Some review days overlap with earlier weeks to reinforce patterns.)
