"""Write the two mock-exam packages (Day 27 & Day 28 of Week 4).

Each is a MOCK.md describing a 90-minute, 3-problem exam. No solution.cpp.
The user writes their solutions as exam1_a.cpp / exam1_b.cpp / exam1_c.cpp etc.
in the same directory and times themselves.
"""
import os


MOCK1 = """# Mock Exam 1 — 90 minutes

**Date:** ____________   **Start:** ______   **End:** ______   **Score:** __/3

Rules: no references, no AI assistance, time yourself 90 minutes. After the
timer, review wrong answers and log them in `notes/` and `PROGRESS.md`.

Write each solution as `exam1_a.cpp`, `exam1_b.cpp`, `exam1_c.cpp` in THIS
directory. Add a `main()` that reads stdin and prints stdout, like the other
problems. Run with `g++ -std=c++17 -O2 exam1_a.cpp -o a && ./a < a.in`.

---

## Problem A — 3Sum Closest (array / two-pointer, medium)

Given an integer array nums of length n and an integer target, find three
integers in nums whose sum is closest to target. Return the sum. Assume each
input has exactly one solution.

Constraints: 3 <= n <= 1000, -1000 <= nums[i] <= 1000, -1e4 <= target <= 1e4.

Example:
  Input:  nums = [-1,2,1,-4], target = 1
  Output: 2    (sum = -1 + 2 + 1 = 2)

I/O for your a.in: line 1 = n target; line 2 = n ints. Print the closest sum.

---

## Problem B — Copy List with Random Pointer (linked list, hand-written, medium)

A linked list of length n is given such that each node contains an additional
random pointer, which could point to any node in the list, or null. Construct a
deep copy of the list and return the head of the copied list.

Define:
  struct Node { int val; Node* next; Node* random; Node(int v): val(v), next(nullptr), random(nullptr) {} };

Constraints: 0 <= n <= 1000, -1e4 <= val <= 1e4, random is null or a valid index.

Example: list [[7,null],[13,0],[11,4],[10,2],[1,0]] means node0 val=7 random=null;
node1 val=13 random->node0; node2 val=11 random->node4; etc. The copy must have
the same structure and random links.

I/O for your b.in: line 1 = n; next n lines "val randomIndex" (randomIndex = -1 for
null). Print the copied list as n lines "val randomIndex" (the copied random's
index, or -1).

---

## Problem C — House Robber III (tree DP, medium)

The thief has found himself a new place for his thievery. There is only one
entrance to the area, called "root". Besides the root, each house has one and
only one parent house. After a tour, the smart thief realizes that all houses in
this place form a binary tree. If two directly-linked houses are broken into on
the same night, the alarm will alert the police. Given the root of the binary
tree, return the maximum amount the thief can rob without alerting the police.

Define the TreeNode struct as in the other problems.

Constraints: number of nodes in [1, 1e4], 0 <= Node.val <= 1e4.

Example:
  Input (level order): 3 2 3 -1 3 -1 1
  Output: 7    (rob node 3 (root) + node 3 (level 2 right) + node 1? -> 3+3+1=7)

I/O for your c.in: line 1 = n; line 2 = n level-order values (-1 = null). Print
the maximum amount.

---

## Self-review after the timer

- [ ] Did you finish all 3 within 90 min?
- [ ] Which problem ate the most time?
- [ ] Any off-by-one / overflow / null-pointer bugs?
- [ ] Log wrong answers to `notes/` and update `PROGRESS.md` mock-exam row.
"""

MOCK2 = """# Mock Exam 2 — 90 minutes (+ Final Review)

**Date:** ____________   **Start:** ______   **End:** ______   **Score:** __/3

Rules: same as Mock 1 — no references, 90 minutes, write solutions as
`exam2_a.cpp`, `exam2_b.cpp`, `exam2_c.cpp` in THIS directory.

---

## Problem A — Rotting Oranges variant (graph / BFS, medium)

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

## Problem B — Koko Eating Bananas variant (binary search on answer, medium)

Koko can eat k bananas/hour. Each hour she picks one pile and eats min(k, pile)
bananas (rest of pile stays). Return the minimum integer k such that she eats all
piles within h hours.

Constraints: 1 <= piles.length <= 1e4, piles.length <= h <= 1e9, 1 <= piles[i] <= 1e9.

Example:
  Input: piles = [3,6,7,11], h = 8
  Output: 4

I/O for your b.in: line 1 = n h; line 2 = n ints. Print the minimum k.

---

## Problem C — Word Break (DP, medium)

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
"""


def write_mocks(root):
    base = os.path.join(root, "problems", "week4-binary-search-greedy-mock")
    for day, md in [("day27-mock-exam-1", MOCK1), ("day28-mock-exam-2", MOCK2)]:
        d = os.path.join(base, day)
        os.makedirs(d, exist_ok=True)
        with open(os.path.join(d, "MOCK.md"), "w") as f:
            f.write(md)
