# AGENTS.md

## Repository purpose

This repository is a LeetCode learning repository. The goal is not merely to keep accepted code, but to build **reconstructible algorithmic intuition**: after forgetting a template, the learner should still be able to derive the solution again from the problem.

Correctness, testability, statement fidelity, and generated-file consistency remain mandatory, but learning order has priority when choosing how to explain a correct solution.

## Core explanation rule: problem reasoning before algorithm naming

Do not start a problem explanation from a named algorithm, a code template, or a dense formula.

For a non-trivial problem, use this reasoning direction by default:

```text
题目本身的直觉 / 几何或状态图像
        ↓
符合直觉的一般性逻辑
        ↓
最自然、正确的一般性实现
        ↓
这个实现的问题、限制、重复工作或复杂度瓶颈
        ↓
从具体问题中总结可复用的一般规律
        ↓
在第一次真正需要时补充背景知识 / 术语
        ↓
把一般规律映射到某个专项算法
        ↓
核心算法机制
        ↓
公式 / 不变量 / 边界语义
        ↓
与代码一致的执行步骤
        ↓
正确性、边界、易错点、迁移
```

The learner should understand **why an algorithm is needed before the algorithm name appears**.

Bad order:

```text
这题使用单调栈。
维护一个递增栈……
```

Preferred order:

```text
每根柱作为最低高度时，需要反复寻找左右第一个更矮位置
-> 一般实现会重复扫描相同区域
-> 从左到右时，尚未遇到右侧更矮柱的历史位置仍是“未决边界”
-> 更矮元素到来时可以一次结算多个历史位置
-> 这类“未决状态 + 未来事件批量结算”的结构可以用单调栈实现
```

Algorithm names are retrieval labels. The causal mechanism is the learning target.

## Prefer geometry and diagrams whenever relationships are spatial

If a problem contains intervals, boundaries, widths, areas, pointer movement, tree shape, graph frontier, matrix regions, stack states, or other spatial relationships, **prefer a diagram before abstract notation**.

Use ASCII diagrams in `solution.cpp` comments and Markdown ASCII / Mermaid diagrams in docs where useful.

A diagram must expose a relationship that later reasoning reuses. It should not be decorative.

Examples:

- histogram: show bars, blocking boundaries, and the actual usable interval;
- prefix sum: show current prefix, historical prefix, and the interval between them;
- sliding window: show left/right, the current legal window, and what changes when a boundary moves;
- two pointers: show which candidate region one pointer movement permanently discards;
- monotonic stack: show unresolved historical states and which current event resolves them;
- binary search: show the monotonic search/answer space;
- DP: show what one state represents before writing a transition;
- tree/graph: show recursion responsibility, frontier, visited region, or parent/child dependency.

Whenever a formula such as `right-left-1` can be derived from a visible interval, derive it from the picture instead of presenting it as a mnemonic.

## Official examples come before the learning derivation

Every generated problem should present statement-level examples before constraints and before `解法精讲`.

Learner-facing order:

```text
题目描述
    ↓
题目示例：Input / Output / Explanation
    ↓
约束与要求
    ↓
解法精讲
```

Official examples explain **what the problem asks**. Custom pedagogy examples later explain **why the solution works**. Do not mix these two roles.

Preserve official Input / Output / Explanation when available. Prefer all reasonably sized official examples, especially examples that expose boundary behavior.

Statement examples belong to the statement metadata layer, not pedagogy overrides.

## General solution before optimization

When the primary solution is meaningfully more sophisticated than the most natural correct solution, first state a real direct algorithm that a learner could reasonably write.

The direct algorithm must be correct, not a straw man.

Explain:

1. what it does;
2. why it matches the problem definition;
3. its time/space cost;
4. the exact work that becomes wasteful at scale.

Typical waste classes include:

- repeated lookup;
- repeated range writes / repeated calculation;
- overlapping state that could be carried forward;
- dominated candidates that can never become useful again;
- monotonicity that lets one comparison discard a whole class of candidates;
- repeated subproblems.

The learner should be able to answer:

> **“这个优化究竟省掉了哪一部分原始工作？”**

before seeing the optimized template.

## Abstract the reusable rule before naming the algorithm

After identifying the direct solution's limitation, summarize the general law in ordinary language.

Examples:

```text
固定当前 x 后，需要的另一个值已经唯一确定为 target-x；
慢点是每次都重新线性查找这个确定值。
-> 把历史 value 建索引。
-> 这之后才叫“哈希表 Two Sum”。
```

```text
相邻窗口只改变一个进入元素和一个离开元素；
旧窗口的大部分状态仍然有效。
-> 维护可增量更新的窗口状态。
-> 这之后才叫“滑动窗口”。
```

```text
历史位置的右侧阻挡边界还没有出现；
未来更矮元素到来时答案永久确定，并可能一次结算多个历史位置。
-> 只保存未决历史状态。
-> 这之后才叫“单调栈”。
```

Do not jump directly from “O(n^2) is slow” to an algorithm name.

## Define prerequisite concepts at first use, not mechanically at the top

Technical vocabulary should appear **when the reasoning first needs it**.

Do not front-load a problem with definitions the learner does not yet need. The early problem model should use ordinary language whenever possible.

When a concept becomes necessary, define it before relying on it:

```text
概念名称
    ↓
一句精确定义
    ↓
最小具体例子
    ↓
必要时给一个反例 / 边界例子
    ↓
说明它怎样影响当前推导
```

Typical concepts include:

- closed/open/half-open intervals;
- strict vs non-strict ordering when duplicates matter;
- prefix/suffix and subarray/subsequence;
- tree depth vs height, node-count vs edge-count path length;
- topological order;
- connected component / cycle / frontier / invariant;
- stack LIFO, queue FIFO;
- amortized analysis.

Algorithm-specific background should normally appear **after the direct reasoning and generalization, but before the core algorithm implementation**.

## Specialized algorithm guides are required for reusable non-trivial mechanisms

`docs/patterns.md` is a review / retrieval cheat-sheet. It is not the authoritative teaching source for an algorithm family.

Reusable non-trivial algorithms should have mechanism-level guides under:

```text
docs/algorithms/<algorithm>.md
```

The index and topic-document contract are defined in [docs/algorithms/README.md](docs/algorithms/README.md).

A specialized guide should explain, in this order:

1. the general class of problems without starting from the algorithm name;
2. the natural correct general implementation;
3. the limitation of that implementation;
4. the reusable law extracted from the limitation;
5. diagrams / geometry where applicable;
6. background concepts at first use;
7. the formal algorithm and data structure;
8. variants and boundary/equality policies;
9. correctness and complexity;
10. a progression of representative problems.

Do not create empty topic documents merely to fill a directory. Create or expand one when an algorithm family starts carrying shared explanation across multiple problems.

Current algorithm-guide index:

- [专项算法文档总览](docs/algorithms/README.md)

The major reusable algorithm families used by the 106-problem plan now have
mechanism-level guides under `docs/algorithms/`. Future work should deepen those
guides rather than reintroducing algorithm theory separately in each problem.

## Problem-level mapping after the algorithm guide exists

A problem should still explain its own reasoning before pointing to the specialized guide.

Use the two layers like this:

```text
specific problem
    -> intuition / geometry
    -> natural solution
    -> limitation
    -> general rule
    -> “this rule is the monotonic-stack / two-pointer / ... pattern”
    -> refer to the topic guide for the general mechanism
    -> explain how this problem instantiates it
    -> formula / invariant / implementation
```

Do not replace a problem explanation with “see algorithm guide”. The guide owns the general theory; the problem owns the mapping from its concrete semantics to that theory.

## Preferred reasoning-first `解法精讲` structure

Problems migrated to the current standard should normally render these layers:

### 1. 题目直觉 / 图形模型

Explain what the problem means in ordinary language. Use a small concrete example and a diagram whenever spatial relationships exist.

### 2. 最自然的一般性解法

Show the correct direct reasoning and, when useful, short pseudocode or a trace.

### 3. 一般解法的问题与限制

Name the exact repeated work, scaling problem, impossible memory cost, or structural limitation.

### 4. 从具体问题抽象规律

State the reusable observation **without using a specialized algorithm as the premise**.

### 5. 必要背景知识（only when needed）

Insert definitions only when the next step needs them.

### 6. 专项算法背景

Name the algorithm family only now. Link/reference the corresponding `docs/algorithms/` guide when one exists.

### 7. 核心算法

Map the general rule onto concrete states, containers, pointers, boundaries, or transitions.

### 8. 公式 / 不变量

Derive formulas from the previous picture/state model. Explain every symbol and equality/strictness choice.

### 9. 执行步骤

Write the update order in the same sequence as the implementation.

### 10. 为什么不会漏 / 不会重

Explain the causal invariant; formal proof is optional unless needed.

### 11. 边界与易错点

Call out correctness-sensitive initialization, update order, overflow, interval conventions, duplicate ownership, sentinels, side effects, and algorithm preconditions.

### 12. 举一反三

Explain what remains invariant and what changes in the next related problem class.

Not every simple problem needs all twelve headings. The principle is the ordering boundary: **problem reasoning first, specialized algorithm second**.

## Pattern reuse has priority when it makes the problem simpler

If a new problem can naturally be transformed into an already-understood problem, use that transformation as the primary explanation unless a stricter required complexity invalidates it.

Example:

```text
“恰好 K 个奇数”
-> odd=1, even=0
-> “连续子数组和为 K”
-> reuse LC-560 prefix-frequency reasoning
```

A more clever or more space-efficient solution may be kept as `进阶解法`, but should not replace a substantially clearer primary solution merely because it has a smaller constant or less auxiliary memory.

If necessary, present both:

1. 主解法（优先理解）
2. 进阶优化（满足更严格空间 / 常数要求）

## Implementation tricks come after the mechanism

For every non-trivial implementation trick, explain:

1. what state/boundary it represents;
2. why it is needed;
3. why the update order matters;
4. what concrete bug appears if the order or boundary relation changes.

Examples:

- LC-1: query history before inserting current so one index cannot pair with itself;
- prefix-frequency: initialize the empty prefix when intervals starting at index 0 must count;
- difference arrays: distinguish change events from accumulated active state;
- monotonic stack: `< / <= / > / >=` is a duplicate-ownership rule, not a cosmetic comparison;
- sentinels: explain which normal event the virtual boundary is forcing.

Do not teach a trick as a mnemonic before its causal explanation.

## C++ semantic clarity

Code should visibly match the reasoning and avoid hidden side effects when they obscure intent.

For associative containers:

- use `find()` / `contains()` for pure queries when accidental insertion is undesirable;
- use `operator[]` when insertion/default construction is the intended update, e.g. `++frequency[prefix]`.

Example:

```cpp
auto it = frequency.find(prefix - k);
if (it != frequency.end()) {
    answer += it->second;
}
++frequency[prefix];
```

This keeps “query old state” and “record current state” visibly separate.

## Comments in `solution.cpp`

The large header comment is part of the learning artifact and must stay synchronized with canonical pedagogy. Detailed code-comment standards live in [docs/code-commenting.md](docs/code-commenting.md).

Key comments should explain:

- what a variable/container/pointer represents;
- why an update or elimination is valid;
- why update order matters;
- sentinel / interval / visited / strictness semantics;
- what goes wrong if a correctness-sensitive boundary changes.

Do not paraphrase obvious syntax. Ordinary I/O does not need pedagogical comments.

## Statement fidelity

Problem facts and pedagogy are separate layers. Never change legal inputs, return semantics, examples, or algorithmic preconditions just to simplify a lesson.

The learner-facing statement baseline and validation process are documented in [docs/doocs-baseline.md](docs/doocs-baseline.md).

Relevant canonical files include:

- `tools/statement_metadata.py`;
- `tools/statement_overrides.py`;
- `tools/statement_examples.py`;
- `tools/chinese_problem_info.json`;
- `tools/official/lc<N>.txt` for offline cross-checking.

Facts that must remain explicit when they affect correctness include:

- length/value ranges and character sets;
- sortedness, uniqueness, positivity/non-negativity;
- connectivity/reachability;
- reuse/modification permissions;
- return-value and no-solution behavior;
- any precondition required by the chosen algorithm.

Do not fix statement facts only in generated `solution.cpp`; update the metadata layer and keep generated output synchronized.

## Generated repository source of truth

`problems/**/solution.cpp` is generated learning output, not the only source of truth. A pedagogy, implementation, or key-comment change must not exist only in generated output because `python3 tools/gen_all.py` may overwrite it.

Current canonical pipeline:

1. `tools/refined_week1.py` ... `tools/refined_week4.py`: baseline reviewed explanation/implementation records;
2. `tools/pedagogy_overrides.py` plus modular `tools/pedagogy_week*.py`: high-touch per-problem pedagogy;
3. `tools/pedagogy_prerequisites.py`: optional prerequisite concepts;
4. `tools/pedagogy_derivations.py` + `tools/pedagogy_derivations_backfill.py`: preserved direct-optimization evidence;
5. `tools/pedagogy_reasoning_first.py`: full-plan natural-solution / limitation / generalization / algorithm-background bridge;
6. `tools/code_comment_overrides.py` plus modular `tools/code_comments_week*.py`: reviewed implementation comments;
7. `tools/refined_data.py`: merges canonical layers, validates full reasoning-first coverage, and renders pedagogy;
8. `tools/statement_metadata.py`: effective learner-facing statement metadata;
9. `tools/gen_all.py`: renders `solution.cpp`, `test.in`, and Week 4 mock packages.

The current reasoning-first renderer uses explicit fields such as:

```text
visual
general_solution
limitations
generalization
algorithm_background
core
formula
steps
```

All 106 formal problems are now required to render through this reasoning-first
structure. `validate_coverage()` treats missing fields as canonical drift. Future
edits should improve individual sections without falling back to the older format.

`python3 tools/check_generated_fidelity.py` is the non-mutating canonical/generated guard. `make verify-meta` must pass before considering a repository-wide maintenance pass complete.

## Solution selection

Before rewriting a problem:

1. identify the simplest correct reusable reasoning path;
2. check whether the current implementation matches it;
3. if the code is correct but pedagogically too advanced, either make the clearer solution primary or retain the advanced one as explicitly labeled secondary material;
4. do not change working code solely for stylistic novelty.

## Validation

After modifying a problem:

1. preserve the public LeetCode method signature;
2. preserve/update the local test adapter consistently;
3. reason through existing `cases/*.in` and expected outputs; run them when execution is available;
4. add edge cases when the new reasoning exposes an uncovered boundary;
5. keep claimed time/space complexity consistent with the actual implementation;
6. update canonical pedagogy first or together with generated output;
7. keep statement facts consistent with `statement_metadata.py` / reviewed doocs baseline;
8. run `python3 tools/check_generated_fidelity.py`;
9. run `make verify-meta`;
10. when execution is available, run `make verify` for compile + all judges.
