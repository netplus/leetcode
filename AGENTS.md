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
5. keep claimed time/space complexity consistent with the actual primary implementation.

## Repository-wide optimization workflow

When asked to optimize existing cases/problems, work incrementally rather than rewriting everything mechanically.

For each problem:

1. read the existing `solution.cpp` and tests;
2. identify the key abstraction barrier that makes the current explanation hard to understand;
3. rewrite using **图像直觉 -> 一句话核心 -> 公式/不变量 -> 执行步骤 -> 正确性直觉 -> 易错点 -> 迁移**;
4. change implementation only when doing so materially improves clarity/correctness or matches the intended primary solution;
5. keep each change focused and traceable.

## Writing style

- Chinese is the primary explanatory language; keep standard algorithm/API identifiers in English.
- Prefer concrete examples before abstraction.
- Prefer short equations tied directly to the diagram.
- Avoid compressed phrases such as “显然”, “同理”, or “直接可得” when they hide the key reasoning step.
- Avoid presenting memorized templates without explaining the invariant that makes the template applicable.
- The target is: after several days, the reader should be able to reconstruct the algorithm from the picture and one-sentence core idea rather than memorize the code.
