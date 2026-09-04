# AGENTS.md

## Repository purpose

This repository is a LeetCode learning repository. The primary goal is not merely to keep accepted code, but to build reusable algorithmic intuition and make later review fast and reliable.

When modifying problem solutions, preserve correctness and testability, but optimize the explanation for learning first.

## Core explanation rule

For every problem, organize the main explanation in this order:

1. **建立图像直觉**：先用一个具体场景、区间图、指针图、树/图结构或状态变化，让读者先“看见”算法在做什么。
2. **再给公式/不变量**：在直觉建立后，再写数学关系、状态定义、窗口条件、递推式或不变量。
3. **最后落到执行步骤**：把公式翻译为可以直接对应代码的更新顺序，并说明为什么顺序不能交换。

Do not start a new concept with a dense formula when a simple visual model can explain the same idea first.

## Optimization derivation rule

For any problem whose primary solution is meaningfully more sophisticated than the most direct solution, **do not present the optimized algorithm as the starting point**. The explanation must show how the optimized mechanism grows out of the direct algorithm.

Use this derivation chain by default:

```text
最自然 / 最朴素的做法
        ↓
它为什么正确
        ↓
它到底慢在哪里 / 重复做了什么 / 保存了哪些以后永远没用的候选
        ↓
找出可复用的信息、可延续的状态、可淘汰的候选或可利用的单调性
        ↓
把这些观察压缩成优化机制
        ↓
再命名为哈希、前缀和、差分、Kadane、滑动窗口、双指针、单调栈……
        ↓
最后落到变量、不变量、更新顺序和具体实现技巧
```

The learner should be able to answer **“这个优化究竟省掉了哪一部分原始工作？”** before being asked to remember the final formula or template.

### Start from a real direct algorithm

The starting point should be a correct algorithm a learner could naturally write, not a deliberately absurd straw-man implementation.

When useful, show short pseudocode or a concrete execution trace. State its time/space cost and identify the exact repeated operation.

Examples:

- LC-1 Two Sum: two nested loops are repeatedly **searching for one already-determined complement**.
- LC-1109 Corporate Flight Bookings: each booking repeatedly writes the **same increment to every point in a contiguous interval**.
- LC-53 Maximum Subarray: interval enumeration keeps many candidates that, once compared at the same endpoint, are **permanently dominated by a better candidate**.

### Classify what the optimization removes

Before introducing the optimized data structure or recurrence, explicitly identify which kind of waste is being removed. Common categories include:

1. **重复查找**：同一个可计算 key 被反复线性搜索。  
   Typical optimization: build an index / hash map / lookup table.

2. **重复写入或重复计算**：一段范围内执行大量相同操作。  
   Typical optimization: record boundary events, prefix/difference information, lazy state, preprocessing.

3. **状态可延续**：中间位置没有新事件时，前一位置的有效状态可以直接继承。  
   Typical optimization: rolling state / accumulated state / sweep-line active state.

4. **候选被支配**：两个候选面对相同未来时，其中一个永远不可能反超另一个。  
   Typical optimization: dynamic-programming state compression, monotonic structures, greedy elimination.

5. **存在单调性**：一次判断可以证明整批候选都不可能成为答案。  
   Typical optimization: two pointers, sliding window, binary search, monotonic queue/stack.

6. **重复子问题**：不同搜索路径会重新计算相同状态。  
   Typical optimization: memoization / dynamic programming.

Do not force every problem into these labels, but when one of them is the real reason the optimization works, say so explicitly.

### Explain the bridge, not just the two endpoints

The most important part of the explanation is the transition from the direct algorithm to the optimized one.

Bad:

```text
暴力 O(n^2)，所以我们使用哈希表，复杂度 O(n)。
```

Preferred:

```text
固定当前 x 后，需要的另一个值已经唯一确定为 target-x。
暴力算法慢在每次都重新线性寻找这个确定值。
所以把已经看过的 value 建成 value -> index 的索引，
把“重新扫描寻找”改成一次直接查询。
```

Likewise, do not write only:

```text
区间加法使用差分：diff[L] += x, diff[R+1] -= x。
```

First explain:

```text
朴素算法会在 [L,R] 每个位置重复执行 +=x。
但这份 +x 在整个区间里状态完全相同，
所以只记录“从 L 开始生效”和“R 后结束”，
再用 running 把当前有效状态向后延续。
```

### Make state compression explicit

When an optimization keeps only a small state instead of many candidates, explain **why discarded candidates can never become useful again**.

For LC-53, for example:

```text
固定同一个终点时，若候选 A 的和已经大于候选 B，
以后无论再追加什么连续后缀，A 和 B 都会加上完全相同的值。
因此 B 永远不可能反超 A，可以永久丢弃。
```

Only after that observation introduce:

```text
current = 必须以当前位置结尾的最大子数组和
current = max(nums[i], current + nums[i])
```

This “same future -> dominated state can be discarded” reasoning is more reusable than memorizing a particular DP recurrence.

### Separate data, change/event, and accumulated state

When an implementation uses accumulation to make a state persist, clearly distinguish the roles of the variables.

For LC-1109-style difference/sweep implementations, use the mental model:

```text
diff[i]   = 在位置 i，当前状态要改变多少
running   = 走到当前位置时，仍然有效的所有贡献之和
answer[i] = 当前 running 对应的真实结果
```

Then explain the implementation behavior:

```text
+x = 加入一份持续状态
 0 = 没有新事件，running 自动继承旧状态
-x = 移除之前加入的那份状态
```

The important implementation insight is that `running += diff[i]` does two jobs:

1. applies new change events at `i`;
2. when `diff[i] == 0`, naturally carries the previous active state forward without repeating the original range update.

A negative difference entry is therefore usually a **state cancellation event**, not necessarily a business-level negative operation on that position.

### Derive implementation tricks after the mechanism

Implementation details should be presented only after the optimization mechanism is understood. For every non-trivial trick, state:

1. **它代表什么状态或边界**；
2. **为什么需要它**；
3. **为什么必须按这个顺序更新**；
4. **如果顺序/边界写错会发生什么**。

Examples:

- LC-1: `find()` is a pure historical query; insert the current value only **after** the query so one index cannot pair with itself.
- LC-1109: `diff[n]` may be a sentinel boundary representing “after the final real position”, not an actual answer element.
- LC-53: `current` is the best interval forced to end here; `best` is historical global optimum, so `best` must not be replaced by the latest `current`.
- Prefix-frequency problems: initialize the empty prefix before scanning when intervals starting at index 0 must be countable.

Do not summarize an implementation trick as a mnemonic until its causal explanation has appeared first.

### Name the reusable optimization pattern last

Whenever possible, let the learner first understand the mechanism in ordinary language and only then attach the standard algorithm name.

Preferred progression:

```text
大量相同区间写入
-> 只记录开始/结束变化
-> running 承接中间状态
-> 这套结构叫“差分数组 + 前缀恢复”
```

or:

```text
同一终点只保留不会被其他候选支配的最优状态
-> 每一步只需“接上还是重开”
-> 这就是 Kadane / 一维状态压缩 DP
```

The algorithm name is useful for retrieval; the mechanism is what makes it reconstructible.

## Preferred teaching structure

The `解法精讲` section should normally contain the following layers.

### 1. 图像直觉

Use a small concrete example and an ASCII diagram where useful.

The diagram must expose the key relationship rather than decorate the text. Examples:

- prefix sum: show `current prefix`, `old prefix`, and the interval between them;
- sliding window: show `left`, `right`, the legal window, and what changes when one side moves;
- two pointers: show why moving one pointer can discard a whole class of impossible answers;
- monotonic stack: show which unresolved elements remain on the stack and what an incoming element resolves;
- binary search: show the monotonic answer/search space, not just array indices;
- dynamic programming: show what one state represents before writing the transition;
- tree/graph traversal: show the frontier / recursion responsibility / visited invariant.

### 2. 一句话核心

After the picture, summarize the algorithm in one plain-language sentence.

The sentence should answer: **“每一步真正是在问什么 / 维护什么？”**

Examples:

- prefix sum: “我现在累计到 `prefix`，历史上有多少次累计到 `prefix-k`？”
- sliding window: “右端扩张获得新信息，条件失效时左端只向右移动直到恢复合法。”

### 3. 公式或不变量

Only after the intuition is clear, write the exact relation.

Explain every symbol from the picture. Avoid introducing abstract indices solely for formalism.

For example, prefer:

```text
当前累计 - 过去累计 = 中间连续区间的和
prefix - oldPrefix = k
oldPrefix = prefix - k
```

before a more formal `pre[i+1] - pre[j] = k` notation.

### 4. 执行步骤

Write the algorithm in the same order as the implementation.

Prefer short memorable sequences such as:

```text
先算现在 -> 再查过去 -> 最后把现在加入历史
```

Then map each step to the corresponding code operation.

### 5. 为什么不会漏 / 不会重

Use intuitive causality first. Formal proof is optional unless the problem genuinely needs it.

Explain the invariant that makes an entire class of candidates safe to include or discard.

### 6. 边界与易错点

Call out only mistakes that are likely in this problem, including:

- initialization sentinels such as `count[0] = 1`;
- update order;
- duplicate-frequency counting versus existence testing;
- integer overflow;
- index / closed-open interval conventions;
- side effects of C++ APIs;
- assumptions required by sliding windows or greedy logic.

### 7. 举一反三

Relate the problem to an already learned reusable pattern and state what changes and what stays invariant.

## Pattern reuse has priority

When a new problem can be naturally transformed into a previously learned problem, make that transformation the primary explanation unless the repository explicitly targets a stricter complexity bound.

Examples:

- “恰好有 K 个奇数” -> odd=`1`, even=`0` -> “连续子数组和为 K” -> reuse LC-560 prefix-sum reasoning.
- binary array counting problems -> consider prefix sums before introducing a more specialized window identity.

A more space-efficient or mathematically clever solution may be retained as an **进阶解法**, but should not replace a substantially clearer primary solution merely because it uses less auxiliary space.

If the stated complexity target conflicts with the clearer teaching solution, document both:

1. **主解法（优先理解）**
2. **进阶优化（满足更严格空间/常数要求）**

and clearly state the trade-off.

## C++ semantic clarity

Code should reflect the explanation and avoid hidden side effects when they obscure intent.

For associative containers:

- use `find()` / `contains()` for a pure existence/query operation when accidental insertion is undesirable;
- use `operator[]` when insertion/default construction is part of the intended state update, e.g. `++frequency[prefix]`.

Example:

```cpp
auto it = frequency.find(prefix - k);
if (it != frequency.end()) {
    answer += it->second;
}
++frequency[prefix];
```

This makes “查过去” and “记录现在” visibly different operations.

## Comments in solution.cpp

The large header comment is part of the learning material and must be maintained with the code.

Do not merely paraphrase each source line. Comments should explain:

- the state represented by a variable;
- why an update is needed;
- why the update happens in this order;
- which invariant makes the step correct.

For non-trivial problems, prefer a small ASCII diagram in the header when it materially reduces abstraction.

## Official statement fidelity

Problem statement facts and pedagogy are separate concerns. Do not invent, simplify away, or casually rewrite facts about the legal input domain or API contract merely to make an explanation shorter.

The learner-facing statement source is:

1. `tools/chinese_problem_info.json`: refreshable LeetCode CN statement/constraint cache;
2. `tools/statement_overrides.py`: persistent reviewed fixes where the Chinese cache is less explicit than the official English statement or contains extraction defects;
3. `tools/statement_metadata.py`: the effective statement metadata consumed by `gen_all.py`.

`tools/official/lc<N>.txt` is the English official snapshot used for cross-language semantic review. When Chinese and English official wording differ in precision, preserve the stricter meaning when it affects the legal input set, return contract, or an algorithmic precondition. For example, distinguish “小写英文字母” from an ambiguous unrestricted “小写字母” when the official English statement says `lowercase English letters`.

A learner-facing problem statement must explicitly preserve official facts that affect:

- length/value ranges and character sets;
- sortedness, uniqueness, positivity/non-negativity, connectivity, reachability, or similar structural assumptions;
- whether answers are unique or arbitrary ordering is allowed;
- whether elements/nodes/words may be reused or input may be modified;
- return-value semantics and special no-solution behavior;
- any other guarantee required for the chosen algorithm to be valid.

Do not edit only `problems/**/solution.cpp` to fix a statement. Put persistent corrections in the statement metadata layer and keep generated output synchronized. Refreshing `chinese_problem_info.json` must not erase a reviewed correction.

After statement changes, `python3 tools/check_statement_fidelity.py` must pass for all 106 generated problems; it is part of `make verify-meta`. Use `tools/compare_official.py` for side-by-side review of the **effective learner-facing metadata** against the cached official English statement.

## Generated repository source of truth

`problems/**/solution.cpp` is generated learning output, not the only source of truth. Do not make a pedagogy or implementation change only in a generated `solution.cpp`, because `python3 tools/gen_all.py` may overwrite it.

The generation layers are:

1. `tools/refined_week1.py` ... `tools/refined_week4.py`: baseline reviewed explanations and implementations for all problems;
2. `tools/pedagogy_overrides.py` plus modular `tools/pedagogy_week*.py`: only problems that have received an individual high-touch learning rewrite;
3. `tools/refined_data.py`: merges all baseline and pedagogy layers, then renders either the enhanced or legacy explanation;
4. `tools/statement_metadata.py`: supplies the effective official learner-facing statement metadata;
5. `tools/gen_all.py`: emits `problems/**/solution.cpp`.

Keep pedagogy override modules small enough to review comfortably. New high-touch entries may be split by week or learning batch; `refined_data.py` must explicitly merge them and reject duplicate problem IDs.

For an individually optimized problem, update the canonical learning data first, then keep the generated `solution.cpp` synchronized. A future `gen_all.py` run must preserve the improved explanation and, where applicable, the improved primary implementation.

Do **not** bulk-convert untouched problems merely to satisfy a format. The repository is intentionally migrated problem by problem: an untouched problem keeps its reviewed legacy explanation until it has actually been read and optimized.

## Solution selection

Before rewriting a problem explanation:

1. identify the simplest correct reusable pattern;
2. check whether the current implementation matches that pattern;
3. if current code is correct but pedagogically advanced, prefer either:
   - changing the primary implementation to the clearer solution, or
   - retaining it as an explicitly labeled advanced alternative;
4. do not change working code solely for stylistic novelty.

## Validation

After modifying a problem:

1. preserve the public LeetCode method signature;
2. preserve or update the local test adapter consistently;
3. run/reason through all existing `cases/*.in` and expected outputs when execution is available;
4. add edge cases when an explanation exposes a previously uncovered boundary;
5. keep claimed time/space complexity consistent with the actual primary implementation;
6. ensure `python3 tools/gen_all.py` would not erase the learning rewrite;
7. ensure statement facts still match `statement_metadata.py` and the official review baseline.

## Repository-wide optimization workflow

When asked to optimize existing cases/problems, work incrementally rather than rewriting everything mechanically.

A repository-wide scan may identify and prioritize many candidates, but **pedagogy optimization itself must be strictly per-problem and serial**. Do not batch-rewrite multiple problems just because they share a pattern or can be changed by the same mechanical transformation.

For every problem, complete one independent review cycle before touching the next problem:

1. read that problem's current canonical explanation, generated `solution.cpp`, implementation, and tests;
2. reconstruct the most direct / brute-force intuition a learner would naturally start from;
3. identify exactly what repeated work, unnecessary state, dominated candidate, state persistence, or structural difficulty motivates the optimized algorithm;
4. derive the optimized mechanism from that bottleneck instead of presenting the optimized template as a fact;
5. explicitly state **what original work the optimization has removed** and why the removed work/candidates can never affect correctness;
6. map the mechanism to concrete implementation order, naming, sentinels, indices, container semantics, accumulated state, or pointer-update tricks;
7. summarize reusable implementation techniques only after they have been justified in this concrete problem;
8. update and validate only this problem's canonical data and generated output;
9. record the result, then move to the next problem.

Even when two neighboring problems reuse the same pattern, the second problem must still receive its own concrete analysis. Knowledge reuse should appear as an explicit migration/analogy, not as copied boilerplate.

For each problem:

1. read the existing `solution.cpp` and tests;
2. identify the key abstraction barrier that makes the current explanation hard to understand;
3. rewrite using **朴素直觉 -> 性能/结构瓶颈 -> 优化机制 -> 图像直觉 -> 一句话核心 -> 公式/不变量 -> 执行步骤 -> 实现技巧 -> 正确性直觉 -> 易错点 -> 迁移** when the problem has a meaningful optimization step;
4. for naturally direct problems with no meaningful optimization gap, do not fabricate a brute-force story merely to satisfy the sequence;
5. change implementation only when doing so materially improves clarity/correctness or matches the intended primary solution;
6. update the canonical generation layer before or together with the generated file;
7. keep each change focused and traceable.

## Writing style

- Chinese is the primary explanatory language; keep standard algorithm/API identifiers in English.
- Prefer concrete examples before abstraction.
- Prefer short equations tied directly to the diagram.
- Avoid compressed phrases such as “显然”, “同理”, or “直接可得” when they hide the key reasoning step.
- Avoid presenting memorized templates without explaining the invariant that makes the template applicable.
- Prefer saying exactly what work/state/candidates were eliminated over merely saying that the optimized algorithm is “more efficient”.
- When a variable carries state across positions, explain both **how the state changes** and **why it persists when there is no new event**.
- When candidates are discarded, explain why they face the same future or why monotonicity makes them permanently impossible.
- The target is: after several days, the reader should be able to reconstruct the algorithm from the direct idea, the bottleneck, and the optimization mechanism rather than memorize the code.
