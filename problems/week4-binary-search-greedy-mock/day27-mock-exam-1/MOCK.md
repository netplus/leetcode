# Mock Exam 1 — 90 minutes

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
