# Test-case authoring spec — read BEFORE writing any cases/

## Goal

Every formal LeetCode problem directory owns a deliberate `cases/` validation
suite. A useful suite is not measured only by case count: every case should exist
for a reason and should be able to expose a concrete class of wrong
implementation.

Repository invariants:

- **6 cases minimum** for every formal `dayN-lcM-*` problem;
- **7–8 cases** for high-value problems whose common mistakes need additional
  dedicated coverage;
- every executable case has one matching row in `cases/meta.tsv`;
- never add near-duplicate inputs merely to reach a number.

The hard case gate is:

```bash
python3 tools/check_case_coverage.py --strict
```

`make verify-meta` runs this strict gate automatically. The non-strict invocation
is retained only as a local authoring diagnostic while a single suite is being
edited.

The case-coverage gate applies to the 106 formal `dayN-lcM-*` problem units.
Week 4 mock packages are separate learning artifacts and are validated by their
own workflow rather than being counted as formal LC suites.

## File format

Each executable case is a pair:

- `N.in` — the **complete stdin** consumed by the problem's `main()`;
- `N.out` — the **exact expected stdout** produced by the harness.

Number cases `1, 2, 3, ...` with no gaps. Keep the suite at no more than 8 cases
unless the repository policy is deliberately changed.

Trailing whitespace per output line is ignored by the judge, but expected files
should still be clean. If the harness prints a newline for an empty string, the
expected output must contain that newline. If the harness prints nothing, an
empty `N.out` is valid.

## `test.in` versus `cases/`

`test.in` is a **single quick-run sample**, not the complete validation suite.
It exists so `make lc<N>` / `make w<N>d<M>` can run one representative input
without selecting a case number. Seeing only one short `test.in` therefore does
**not** mean the problem has only one test case.

The complete formal suite lives under `cases/`:

```text
cases/1.in  + cases/1.out
cases/2.in  + cases/2.out
...
cases/meta.tsv
```

Repository convention intentionally keeps:

```text
test.in == cases/1.in
```

So `test.in` is best understood as an alias/copy of the first representative
case for fast debugging. Coverage assessment, CI validation, and regression
judging must inspect/run the whole `cases/*.in` suite rather than counting lines
or scenarios in `test.in`.

## Validation dimensions

Do not mechanically assign one case to each bullet. Choose cases that target the
actual semantics and likely bugs of the problem.

1. **Official example** — retain at least the primary LeetCode example so the
   local harness stays anchored to the statement.
2. **Minimum input** — smallest legal array/string/tree/grid/query set.
3. **Semantic boundary** — first/last position, empty answer, exact equality,
   all-same values, already sorted / reverse sorted, full-range update, etc.
4. **Algorithm trap** — a case that breaks a tempting but wrong implementation:
   e.g. boolean existence instead of frequency, wrong strictness, stale deque
   entries, incorrect duplicate ownership, off-by-one range ending, or invalid
   sliding-window assumptions.
5. **Alternative result branch / no solution** — only when the problem permits
   it.
6. **Duplicates / ordering / overflow** — include when these properties matter
   to correctness.
7. **Stress-shaped small case** — a hand-checkable input that repeats the core
   state transition several times. This is more useful than a huge opaque input.

A good rule is:

> Before adding a case, finish the sentence: “This case catches an implementation
> that incorrectly ______.”

If you cannot fill that blank, the case probably adds little validation power.

## Case intent metadata

Every formal suite contains `cases/meta.tsv`. This turns the case set into a
learning asset and lets the audit verify that every executable case has an
explicit purpose.

Format, one line per case:

```text
# case<TAB>kinds<TAB>purpose
1	official	官方样例：验证题目主路径和本地 I/O 契约
2	minimum,boundary	最小合法输入：验证初始化和单元素边界
3	trap,duplicates	重复状态会产生多个答案，防止把频次错误写成 bool
```

Rules:

- column 1: positive case number;
- column 2: one or more comma-separated retrieval labels;
- column 3: concise explanation of **what bug or boundary this case validates**;
- metadata must cover every `N.in` / `N.out` pair exactly once.

Kinds are deliberately extensible. Use lowercase words or kebab-case labels such
as:

- general labels: `official`, `minimum`, `boundary`, `trap`, `no-solution`,
  `duplicates`, `ordering`, `overflow`, `stress`, `variant`;
- mechanism labels when they improve retrieval: `backtracking`, `cycle`,
  `shared-identity`, `greedy`, `deep-root`, `reuse`, etc.

Do not invent a new kind merely to paraphrase the purpose column. The kind is a
short retrieval label; **the third column is the authoritative explanation**.
The audit validates kind syntax rather than enforcing a closed vocabulary, so a
new algorithm family does not require editing the checker first.

## Do not use a hidden reference solution

Expected outputs should come from an official example, a direct derivation, or a
small hand-checkable calculation. If an expected output cannot be established
with confidence, do not add the case.

For complicated cases, it is fine to write a temporary brute-force checker while
reviewing the case, but do not make the repository's expected-output generation
depend on the production solution being tested.

## Verify your cases

Run the strict case gate:

```bash
python3 tools/check_case_coverage.py --strict
```

Run one problem's judge after changing its cases:

```bash
bash tools/judge.sh <NUM>
```

Run repository metadata verification before submitting; this includes the strict
case gate:

```bash
make verify-meta
```

And, when the implementation set is expected to compile and pass:

```bash
make verify
```

## Conventions

- One problem per `cases/` directory.
- Keep `test.in`; it is used only for the representative quick-run path, not as
  evidence of full case coverage.
- `cases/1.in` must be byte-for-byte identical to `test.in`.
- Do not duplicate an existing input under another case number.
- Keep case numbers contiguous from 1.
- `cases/meta.tsv` is canonical documentation for case intent; do not bury the
  purpose only in a PR description.

## Multi-answer problems — `.judge`

Some LeetCode problems accept answers in arbitrary order. The default judge is an
exact textual comparison, so these problems need a `.judge` file in the problem
directory.

- **`lineset`** — sort output lines before comparing. Use when each line is one
  complete answer and line order is irrelevant. Examples: LC-46, LC-78, LC-39,
  LC-22, LC-15, LC-438, LC-417.
- **`lineset-si`** — also sort tokens inside each line. Use when each line itself
  represents an unordered set, e.g. LC-347 top-K values.
- **`exact`** — default; no `.judge` file is required.

Still write one canonical ordering in every `.out`; normalization only prevents
false failures caused by another valid ordering.
