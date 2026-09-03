# 模拟考试 2 — 90 分钟（+最终复习）

**Date:** ____________   **Start:** ______   **End:** ______   **Score:** __/3

Rules: same as Mock 1 — no references, 90 minutes, write solutions as
`exam2_a.cpp`, `exam2_b.cpp`, `exam2_c.cpp` in THIS directory.

---

## 题目 A — 腐烂的橘子变体（图 / BFS，中等）

Given an m x n grid with 0 (empty), 1 (fresh), 2 (rotten), every minute any fresh
orange 4-adjacent to a rotten one becomes rotten. Return the minimum number of
minutes until no fresh orange remains, or -1 if impossible.

Constraints: 1 <= m, n <= 10, grid[i][j] in {0,1,2}.

Example:
  Input (m n, then m rows of n ints):
    3 3
    2 1 1
    1 1 0
    0 1 1
  Output: 4

I/O for your a.in: line 1 = m n; next m lines = n ints each. Print the minutes, or -1.

---

## 题目 B — 爱吃香蕉的珂珂变体（答案二分，中等）

Koko can eat k bananas/hour. Each hour she picks one pile and eats min(k, pile)
bananas (rest of pile stays). Return the minimum integer k such that she eats all
piles within h hours.

Constraints: 1 <= piles.length <= 1e4, piles.length <= h <= 1e9, 1 <= piles[i] <= 1e9.

Example:
  Input: piles = [3,6,7,11], h = 8
  Output: 4

I/O for your b.in: line 1 = n h; line 2 = n ints. Print the minimum k.

---

## 题目 C — 单词拆分（DP，中等）

Given a string s and a dictionary of strings wordDict, return true if s can be
segmented into a space-separated sequence of one or more dictionary words.

Constraints: 1 <= s.length <= 300, 1 <= wordDict.length <= 1000,
1 <= wordDict[i].length <= 20, lowercase English letters.

Example:
  Input: s = "leetcode", wordDict = ["leet","code"]
  Output: true (1)

I/O for your c.in: line 1 = s; line 2 = L; line 3 = L space-separated words.
Print 1 if segmentable, else 0.

---

## Final Review Checklist (after the timer)

Re-derive from memory, on paper, these high-frequency templates (see
`docs/patterns.md` to check your answers):

- [ ] Binary search template (leftmost value satisfying a monotonic predicate)
- [ ] Sliding window (variable length)
- [ ] Monotonic stack / monotonic queue
- [ ] Union-Find (path compression + union by rank) — write all 5 lines
- [ ] BFS multi-source (all sources enqueued first)
- [ ] Topological sort (indegree queue)
- [ ] Backtracking (permutation + combination + subset variants)
- [ ] 0/1 knapsack 1D (descending) vs complete knapsack (ascending)
- [ ] Two-heap median (max-heap left + min-heap right, rebalancing)
- [ ] Linked-list idioms: reverse [a,b), find midpoint, find cycle entry

Then do a SECOND PASS over every problem in your `notes/` wrong-problem list.
Mark them done (🔁) in `PROGRESS.md`.

## Confidence self-assessment (1-5): ______

Notes / gaps to cram tonight:
-
-
