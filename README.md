# LeetCode Mid–Senior Leveling Exam Practice Workspace (C++ · 4-Week Sprint)

> Goal: close algorithm gaps in 4 weeks for a company-internal leveling exam. Progress is organized by topic × day; all 106 problems include reviewed statements, detailed Chinese analysis, commented reference implementations, and local multi-case verification.

## Directory Layout

```
/nas/exam
├── README.md                 # this file: rules, workflow, conventions
├── PLAN.md                   # 4-week detailed plan (LC numbers + priority + progress)
├── PROGRESS.md               # progress tracker (daily log, wrong-problem list, reviews)
├── docs/
│   ├── patterns.md           # algorithm template cheat-sheet (one memoizable snippet per topic)
│   ├── cpp-tips.md           # C/C++ tips, STL cheat-sheet, common pitfalls
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

1. **Read (5 min):** understand input/output, edge cases, constraints. If no idea, check the hint in `PLAN.md`.
2. **Sketch (10 min):** write 3 sentences of approach + complexity in the file header comment. If still stuck, read the matching section in `docs/patterns.md`.
3. **Implement (25 min):** first write your own `Solution`, then compare it with the reviewed implementation in `solution.cpp`. C++17 with STL; linked-list / tree / union-find structures are included locally.
4. **Verify (5 min):** `make lc<N>` (or `make w1d1`) compiles and runs, feeding `test.in`. Compare against the "Expected output" in the file header.
5. **Review (5 min):** for problems you got stuck on or ran over time, write a note in `notes/` — what you got stuck on, what the pattern is.

> Total ~50 min per problem. Finish all P0 first, then P1.

## Reviewed Solution Contract

- Every `solution.cpp` contains a complete LeetCode-compatible `Solution` class plus a local `main()` adapter.
- Each header presents the title, statement, constraints, complexity goal, local I/O contract, and expected output in Chinese, then explains the core logic through its starting viewpoint, execution flow, and intuitive reason, followed by pitfalls and transfer to related patterns. English source metadata remains internal to the generator for cross-checking only.
- `tools/refined_week1.py` through `tools/refined_week4.py` are the canonical implementation sources. `python3 tools/gen_all.py` regenerates reviewed solutions; it no longer emits empty stubs.
- [docs/problem-review.md](docs/problem-review.md) is the 106-problem statement audit ledger. Local validation compiles with `g++ -std=c++17 -O2 -Wall -Wextra` and runs every `cases/*.in` fixture.

## Priority & Numbering

- **P0**: must-do, high-frequency in the leveling exam. Complete all P0 before any P1.
- **P1**: optional when time is tight; at least read the approach for the marked hard ones.
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
make w1d10-3         # build & run the 3rd problem on week1/day10 (when a day has several)
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

Every problem has a `cases/` dir with 4–8 test cases (`1.in` + `1.out`, ...),
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
> after adding problems). It only writes `Makefile`s — never touches your
> `solution.cpp` / `test.in` / `cases/`.

## Batch operations (from repo root)

```bash
make judge-all      # judge every problem; roll-up summary ("N / 106 pass")
make judge-w1       # judge all of Week 1   (w1, w2, w3, w4)
make judge-d1       # judge Day 1 across weeks (d1 .. d28)
make status         # which problems have reviewed implementations vs fallback stubs
make pch            # build a shared precompiled header (.build/) — speeds up builds
make verify         # audit ledger + compile + all multi-case judges
make clean
```

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
| W3   | Graphs / Union-Find / Dynamic Prog.    | BFS/DFS, knapsack/interval/bitmask DP |
| W4   | Binary Search / Greedy / Heap / Mock   | binary-search-on-answer, greedy, timed mock |

## Progress Tracking

See [PROGRESS.md](PROGRESS.md). At the end of each day, fill in: problem numbers completed, time spent, whether reviewed, wrong-problem IDs.
