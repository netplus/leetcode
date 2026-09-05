# LeetCode Mid–Senior Leveling Exam Practice Workspace (C++ · 4-Week Sprint)

> Goal: close algorithm gaps in 4 weeks for a company-internal leveling exam. Progress is organized by topic × day; all 106 formal LeetCode problems include reviewed statements, official examples, detailed Chinese pedagogy, commented reference implementations, and local multi-case verification. Week 4 additionally contains 2 timed mock-exam units (Day 27/28).

## Directory Layout

```
/nas/exam
├── README.md                 # this file: rules, workflow, conventions
├── AGENTS.md                 # agent rules: visual intuition -> formula -> steps
├── PLAN.md                   # 4-week detailed plan (LC numbers + priority + progress)
├── PROGRESS.md               # progress tracker (daily log, wrong-problem list, reviews)
├── docs/
│   ├── patterns.md           # algorithm template cheat-sheet (one memoizable snippet per topic)
│   ├── cpp-tips.md           # C/C++ tips, STL cheat-sheet, common pitfalls
│   ├── code-commenting.md     # learner-facing key implementation comment standard
│   ├── problem-review.md     # 106-problem statement audit ledger
│   ├── tricks/               # cross-problem idioms & non-obvious tricks (one file per trick)
│   │   ├── README.md         # index of tricks
│   │   └── sentinel.md       # sentinel / virtual-boundary idiom (LC-3, LC-560, LC-1248, ...)
│   └── templates/solution.cpp  # reviewed-solution structure for new problems
├── problems/                 # one directory per problem
│   ├── week1-arrays-strings/
│   ├── week2-list-tree-bsp/
│   ├── week3-graph-dp/
│   └── week4-binary-search-greedy-mock/
├── notes/                    # wrong-problem notes, solution-pattern notes
└── Makefile                  # local build + run
```

## Standard Workflow per Problem (5 steps)

1. **Read (5 min):** understand input/output, official examples, edge cases, constraints. If no idea, check the hint in `PLAN.md`.
2. **Sketch (10 min):** write 3 sentences of approach + complexity in the file header comment. If still stuck, read the matching section in `docs/patterns.md`.
3. **Implement (25 min):** first write your own `Solution`, then compare it with the reviewed implementation in `solution.cpp`. C++17 with STL; linked-list / tree / union-find structures are included locally.
4. **Verify (5 min):** `make lc<N>` (or `make w1d1`) compiles and runs, feeding `test.in`. Compare against the expected output in the file header.
5. **Review (5 min):** for problems you got stuck on or ran over time, write a note in `notes/` — what you got stuck on, what the pattern is.

> Total ~50 min per problem. Finish all P0 first, then P1.

## Reviewed Solution Contract

- Every `solution.cpp` contains a complete LeetCode-compatible `Solution` class plus a local `main()` adapter.
- Every generated header follows the learner-facing order **题目描述 → 官方题目示例 → 约束 → 解法精讲**.
- All 106 formal problems have completed the high-touch pedagogy migration defined in `AGENTS.md`: optional **前置概念 / 优化推导** appear before **图像直觉 → 一句话核心 → 公式/不变量 → 执行步骤 → 正确性解释 → 易错点 → 迁移**.
- Key implementation code follows [docs/code-commenting.md](docs/code-commenting.md): comments explain state, causal reasoning, update order, sentinels and boundary semantics rather than translating C++ line by line.
- `tools/refined_week1.py` through `tools/refined_week4.py` provide the baseline reviewed records. `tools/pedagogy_*.py` adds the high-touch learning layer; prerequisite/derivation modules add concepts and optimization bridges; `tools/code_comment*.py` owns reviewed implementation-comment overrides. `tools/refined_data.py` merges these canonical layers, and `tools/gen_all.py` renders the checked-in workspace.
- Statement metadata and official examples are maintained separately from pedagogy, so learning rewrites cannot silently alter the problem contract.
- [docs/problem-review.md](docs/problem-review.md) is the 106-problem statement audit ledger. `make verify` performs metadata/canonical audits, compiles all 106 C++17 programs and runs every multi-case judge.

## Priority & Numbering

- **P0**: must-do, high-frequency in the leveling exam. Complete all P0 before any P1.
- **P1**: optional when time is tight; at least read the approach for the marked hard ones.
- The formal corpus contains **75 P0 + 31 P1 = 106 LeetCode problems**. Day 27/28 add two timed mock-exam units, so `PLAN.md` contains 108 total plan units.
- Numbers are LeetCode problem IDs (same on the Chinese and English sites, except rare cases noted in `PLAN.md`).

## Naming & File Conventions

- Directory: `problems/weekN-topic/dayN-lcNUM-short-name/`
- Files: `solution.cpp` (or `solution.c`) + `test.in`
- Every file follows [docs/templates/solution.cpp](docs/templates/solution.cpp): all learner-facing statement metadata, analysis, and key comments are in Chinese; API names and complexity notation retain their standard spellings.
- For linked-list / tree problems, the `ListNode` / `TreeNode` struct is defined above `Solution`, and `main()` builds the structure from stdin (level-order for trees, with `-1` for null).

## Local Commands

```bash
make lc1            # build & run LC-1, feeding its test.in, printing stdin + stdout
make w1d1           # build & run the single problem scheduled for week1/day1
make w2d10-3        # build & run the 3rd problem on week2/day10
make all            # compile every solution.cpp under problems/
make list           # list all compilable targets
make clean          # remove build artifacts
```

> After adding or removing problems, regenerate the convenience targets:
> `python3 tools/gen_targets.py` (the Makefile `include`s `tools/_targets.mk`).
> To regenerate all reviewed solutions: `python3 tools/gen_all.py` (re-emits every
> `solution.cpp` and `test.in`, plus the two mock packages; coverage validation
> fails if any problem lacks review data).

## Multi-case judging

Every formal problem has a `cases/` dir with 4–8 test cases (`1.in` + `1.out`, ...),
covering official examples + boundaries (empty, n=1, no-solution, overflow). When
you finish implementing a solution, judge it against all cases.

**Two ways to build & test — pick whichever:**

```bash
# (A) From inside a problem directory — each dir has its own Makefile:
cd problems/week1-arrays-strings/day1-lc1-two-sum
make            # compile -> ./sol           (alias: make build)
make test       # judge ALL cases via the central judge  (alias: make judge)
make run        # quick smoke: build + run ./sol on test.in
make clean

# (B) From the repo root — by LeetCode number:
make judge1         # judge LC-1
make judge560       # judge LC-560
bash tools/judge.sh 560 -v    # verbose: shows input/expected/actual for failing cases
```

The judge compiles your `solution.cpp` once, runs each `case`, and diffs actual
stdout vs the `.out` file. Report: `N passed, M failed (failed: ...)`. Exit code
0 = all pass. For multi-answer problems (permutations, subsets, top-K), a `.judge`
file in the dir selects a normalization mode so any valid output order passes —
see [tools/CASES_SPEC.md](tools/CASES_SPEC.md).

> Per-problem Makefiles are auto-generated by `tools/gen_makefiles.py` (re-run it
after adding problems). It only writes `Makefile`s — never touches your
`solution.cpp` / `test.in` / `cases/`.

## Batch operations (from repo root)

```bash
make judge-all      # judge every formal problem; roll-up summary ("N / 106 pass")
make judge-w1       # judge all of Week 1 (judge-w2 .. judge-w4 analogous)
make judge-d1       # judge Day 1 (judge-d2 .. judge-d28 analogous where applicable)
make status         # which problems have reviewed implementations vs fallback stubs
make pch            # build a shared precompiled header (.build/) — speeds up builds
make verify-meta    # non-mutating metadata + canonical/generated fidelity audit
make verify         # verify-meta + compile all + judge all 106 formal problems
make clean
```

`make verify-meta` now compares every checked-in `solution.cpp` and `test.in` against the canonical in-memory render and verifies both mock packages as well. This catches edits made only to generated files, including key-code-comment drift, without first modifying the working tree.

`make status` reads each `solution.cpp` and marks `[DONE]` (reviewed code present) vs
`[stub]` (an explicit not-yet-reviewed fallback). The batch judges compile + run all cases per
problem and print a summary; exit code 0 only if every problem fully passes.

> **Tip:** run `make pch` once after a fresh checkout. It precompiles
> `<bits/stdc++.h>` into `.build/all.h.gch`; `make` and `make judge*` then use it
> automatically (~10× faster compiles). `judge-all` (106 problems) drops from
> ~10 min to ~75 s.

## Plan Overview

See [PLAN.md](PLAN.md). Weekly themes:

| Week | Topic                                  | Key patterns |
|------|----------------------------------------|--------------|
| W1   | Arrays / Strings / Hashing / 2-Ptr     | prefix sum, sliding window, two pointers |
| W2   | Linked Lists / Trees / BFS-DFS / Stack | hand-written structures, backtracking |
| W3   | Graphs / Union-Find / Dynamic Prog.    | BFS/DFS, linear/grid/sequence/knapsack/interval DP |
| W4   | Binary Search / Greedy / Heap / Mock   | binary-search-on-answer, greedy, heap/selection, timed mock |

## Progress Tracking

See [PROGRESS.md](PROGRESS.md). At the end of each day, fill in: problem numbers completed, time spent, whether reviewed, wrong-problem IDs.
