# 4-Week Practice Plan (C/C++ · Leveling Exam Sprint)

> Pace: 2–4 problems per day, ~50 min each. **Finish all P0 first**, then P1.
> Numbers are LeetCode IDs. 🚩 = high-frequency in past leveling / written exams.

---

## Week 1: Arrays / Strings / Hashing / Two Pointers

> Goal: master "scan-style" algorithms — prefix sum, difference array, sliding window, two pointers, hash lookup. The most common category in the written exam; lock it down this week.

### Day 1 — Hashing + Prefix Sum
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 1 | Two Sum | P0 🚩 | hash `target-nums[i]` |
| 560 | Subarray Sum Equals K | P0 🚩 | prefix sum + hash count |
| 1248 | Count Number of Nice Subarrays | P1 | like 560, odd/even prefix |
| 53 | Maximum Subarray | P0 🚩 | Kadane / prefix-sum min |

### Day 2 — Two Pointers (opposite + fast/slow)
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 167 | Two Sum II | P0 | sorted → opposite pointers |
| 15 | 3Sum | P0 🚩 | sort + two pointers, dedup |
| 11 | Container With Most Water | P0 🚩 | opposite pointers, move short board |
| 283 | Move Zeroes | P1 | fast/slow pointers |

### Day 3 — Sliding Window
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 209 | Minimum Size Subarray Sum | P0 | variable window |
| 3 | Longest Substring Without Repeating Characters | P0 🚩 | hash positions + left bound |
| 76 | Minimum Window Substring | P0 🚩 | count hash + need/have |
| 438 | Find All Anagrams in a String | P1 | fixed-length window |

### Day 4 — Prefix Sum / Difference Array
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 303 | Range Sum Query | P0 | 1D prefix sum |
| 304 | Range Sum Query 2D | P0 | 2D prefix sum |
| 1109 | Corporate Flight Bookings | P0 🚩 | difference array |
| 238 | Product of Array Except Self | P1 | prefix/suffix product |

### Day 5 — Strings
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 5 | Longest Palindromic Substring | P0 🚩 | expand from center / Manacher |
| 14 | Longest Common Prefix | P0 | vertical scan |
| 151 | Reverse Words in a String | P1 | two-pointer reverse |
| 28 | Find the Index of the First Occurrence | P1 | KMP (hand-written next) |

### Day 6 — Intervals / Sort Variants
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 56 | Merge Intervals | P0 🚩 | sort + merge |
| 57 | Insert Interval | P1 | simulate three cases |
| 179 | Largest Number | P1 | custom compare `a+b>b+a` |
| 252 | Meeting Rooms | P1 | sort endpoints |

### Day 7 — Week 1 Review
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 239 | Sliding Window Maximum | P0 | monotonic deque |
| 41 | First Missing Positive | P0 | in-place hash, O(n)/O(1) |
| 31 | Next Permutation | P1 | two-pointer in-place |
| 128 | Longest Consecutive Sequence | P0 🚩 | hash set, O(n) |

---

## Week 2: Linked Lists / Trees / BFS-DFS / Stacks & Queues

> Goal: hand-written data structures are "free points" in the written exam — list reversal, tree traversal, stack applications must be flawless.

### Day 8 — Linked List Basics
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 206 | Reverse Linked List | P0 🚩 | iterative + recursive |
| 92 | Reverse Linked List II | P0 | range reverse + head-insert |
| 21 | Merge Two Sorted Lists | P0 | dummy node |
| 160 | Intersection of Two Linked Lists | P1 | two pointers, two passes |

### Day 9 — Linked List Advanced
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 141 | Linked List Cycle | P0 🚩 | fast/slow |
| 142 | Linked List Cycle II | P0 🚩 | fast/slow + find entry |
| 19 | Remove Nth Node From End | P0 | fast/slow + dummy |
| 234 | Palindrome Linked List | P1 | find midpoint + reverse second half |

### Day 10 — Binary Tree Traversal
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 144/94/145 | Preorder / Inorder / Postorder | P0 | recursive + iterative (stack) |
| 102 | Level Order Traversal | P0 🚩 | BFS queue |
| 104 | Maximum Depth | P0 | DFS / BFS |
| 105 | Construct from Preorder + Inorder | P0 🚩 | recursive divide + hash |

### Day 11 — Binary Tree Properties
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 226 | Invert Binary Tree | P0 | recursive swap |
| 236 | Lowest Common Ancestor | P0 🚩 | post-order DFS |
| 543 | Diameter of Binary Tree | P0 | post-order height, update answer |
| 98 | Validate BST | P1 | inorder sorted / range recursion |

### Day 12 — Stacks / Queues / Monotonic Stack
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 20 | Valid Parentheses | P0 🚩 | stack |
| 155 | Min Stack | P0 | auxiliary stack |
| 739 | Daily Temperatures | P0 🚩 | monotonic decreasing stack |
| 84 | Largest Rectangle in Histogram | P1 | monotonic stack |

### Day 13 — Backtracking Intro
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 46 | Permutations | P0 🚩 | backtracking template |
| 39 | Combination Sum | P0 🚩 | backtracking + pruning |
| 78 | Subsets | P0 | choose / not choose |
| 22 | Generate Parentheses | P1 | backtracking + left/right counts |

### Day 14 — Week 2 Review
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 148 | Sort List | P0 | merge sort, O(n log n)/O(1) |
| 143 | Reorder List | P0 | midpoint + reverse + merge |
| 124 | Binary Tree Maximum Path Sum | P1 | post-order, any-to-any path |
| 199 | Binary Tree Right Side View | P1 | BFS rightmost per level |

---

## Week 3: Graphs / Union-Find / Dynamic Programming

> Goal: graph BFS/DFS framework + union-find + the five DP models. This is the week that separates "mid" from "senior"; DP is 30%+ of the written exam.

### Day 15 — Graph Traversal
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 200 | Number of Islands | P0 🚩 | DFS/BFS coloring |
| 695 | Max Area of Island | P0 | like 200 |
| 133 | Clone Graph | P1 | BFS + hash |
| 207 | Course Schedule | P0 🚩 | topological sort (indegree) |

### Day 16 — BFS / Shortest Path
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 994 | Rotting Oranges | P0 🚩 | multi-source BFS |
| 127 | Word Ladder | P0 | BFS + build graph |
| 542 | 01 Matrix | P1 | multi-source BFS |
| 417 | Pacific Atlantic Water Flow | P1 | reverse BFS from borders |

### Day 17 — Union-Find
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 547 | Number of Provinces | P0 🚩 | union-find template |
| 684 | Redundant Connection | P0 | union-find cycle detection |
| 990 | Satisfiability of Equality Equations | P1 | union-find |
| 685 | Redundant Connection II | P1 | directed version |

### Day 18 — DP Intro (1D / Linear)
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 70 | Climbing Stairs | P0 | Fibonacci DP |
| 198 | House Robber | P0 🚩 | linear DP |
| 213 | House Robber II | P0 | circular → split into two |
| 746 | Min Cost Climbing Stairs | P1 | linear DP |

### Day 19 — DP Paths / Subsequences
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 62 | Unique Paths | P0 | grid DP |
| 64 | Minimum Path Sum | P0 | grid DP |
| 300 | Longest Increasing Subsequence | P0 🚩 | O(n²) + binary search |
| 1143 | Longest Common Subsequence | P0 🚩 | 2D DP |

### Day 20 — DP Knapsack / Interval
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 416 | Partition Equal Subset Sum | P0 🚩 | 0/1 knapsack |
| 494 | Target Sum | P0 | 0/1 knapsack variant |
| 322 | Coin Change | P0 🚩 | complete knapsack |
| 312 | Burst Balloons | P1 🚩 | interval DP (hard) |

### Day 21 — Week 3 Review
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 210 | Course Schedule II | P0 | topological sort (return order) |
| 79 | Word Search | P0 | backtracking on grid |
| 139 | Word Break | P0 | DP |
| 152 | Maximum Product Subarray | P1 | keep min & max |

---

## Week 4: Binary Search / Greedy / Heap / Mock Exams

> Goal: binary search on answer, greedy strategies, TopK heap; finish with timed mock exams to rehearse real exam pacing.

### Day 22 — Binary Search
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 704 | Binary Search | P0 | template |
| 33 | Search in Rotated Sorted Array | P0 🚩 | segmented binary search |
| 34 | Find First and Last Position | P0 | left/right bound binary search |
| 153 | Find Minimum in Rotated Sorted Array | P1 | binary search |

### Day 23 — Binary Search on Answer
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 410 | Split Array Largest Sum | P0 🚩 | binary search + greedy check |
| 875 | Koko Eating Bananas | P0 | binary search on answer |
| 1011 | Capacity To Ship Packages | P0 🚩 | binary search on answer |
| 4 | Median of Two Sorted Arrays | P1 🚩 | binary search O(log) |

### Day 24 — Greedy
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 455 | Assign Cookies | P0 | sort + two pointers |
| 55 | Jump Game | P0 🚩 | maintain max reach |
| 45 | Jump Game II | P0 🚩 | greedy interval coverage |
| 134 | Gas Station | P1 | greedy |

### Day 25 — Heap / TopK
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 215 | Kth Largest Element | P0 🚩 | min-heap size k / quickselect |
| 347 | Top K Frequent Elements | P0 🚩 | hash + heap |
| 295 | Find Median from Data Stream | P0 🚩 | two heaps |
| 23 | Merge k Sorted Lists | P1 🚩 | min-heap |

### Day 26 — Comprehensive / Fill Gaps
| # | Title | Priority | Hint / Pattern |
|---|-------|----------|----------------|
| 253 | Meeting Rooms II | P0 | sweep line / heap |
| 42 | Trapping Rain Water | P0 | two pointers / monotonic stack |
| 406 | Queue Reconstruction by Height | P1 | greedy insert |
| 621 | Task Scheduler | P1 | greedy by count |

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
