# Test-case authoring spec — read BEFORE writing any cases/

## Goal
Every problem dir gets a `cases/` subdirectory with **4–8 cases** as `N.in` + `N.out`
pairs (N = 1, 2, 3, ...). The judge (`tools/judge.sh <NUM>`) compiles the user's
`solution.cpp` once and diffs each case's actual stdout against `N.out`.

## File format
- `N.in` — the **complete stdin** that the problem's `main()` reads. Look at the
  stub's "Local I/O format" comment AND its `main()` to see exactly what to feed.
- `N.out` — the **exact expected stdout**, one value per line as `main()` prints.
  - Trailing whitespace per line is ignored by the judge, but write clean output.
  - If expected is empty (e.g. returns `""`), write a single `\n` (the harness
    prints `"\n"`). If the harness prints nothing, leave `N.out` empty.
- Number cases **1, 2, 3, ...** (sorts naturally by the judge).

## Case selection (cover these dimensions; aim 5–6 per problem)
1. **Official LeetCode Example(s)** — 1–3 cases. Their answers are authoritative.
2. **Minimum input** — n=1, single element, length-1 string, 1×1 grid, etc.
3. **Boundary at constraint extremes** — empty result, all-same values,
   already-sorted / reverse-sorted / all-duplicates.
4. **Tricky / easy-to-misjudge** — empty-result cases (return `""`, `[]`, `false`,
   `-1`, `0`), ties (pick leftmost), duplicate values, negative numbers, overflow
   triggers (large sums → forces `long long`).
5. **Edge of "no solution"** — when the problem allows a no-answer case.

> Do NOT write a reference solution. Compute expected outputs by hand or from the
> official examples. If you can't determine an expected output with certainty,
> skip that case rather than guess a wrong `.out`.

## Verify your cases
After writing a problem's cases, run the judge against the **empty stub**:
```bash
bash tools/judge.sh <NUM>
```
The empty stub returns the default (`{}`, `0`, `""`, `nullptr`, `false`). So:
- Cases whose expected output equals the stub's default will spuriously "PASS".
- All other cases should FAIL (wrong output) — that's correct, the stub isn't
  implemented.
If a case RUNTIME-ERRORs on the empty stub (e.g. empty-stub returns `{}` then
`main()` does `ans[0]`), that's expected and fine — it just means the harness
indexes the result. Note it but don't "fix" the case.

## Conventions
- One problem per `cases/` dir. Never put cases in the parent.
- Keep `test.in` (the single Example-1 file) — it's still used by `make lc<N>` /
  `make w<N>d<M>` quick-run. Make `cases/1.in` == `test.in` (same Example 1) so
  they agree.
- Filenames: `1.in`, `1.out`, `2.in`, `2.out`, ... zero-padded only if >9 cases.

## Multi-answer problems (any output order) — add a `.judge` file
Some LeetCode problems accept the answer in ANY order (permutations, subsets,
combinations, anagram indices, coordinate lists, top-K sets). The default judge
compares stdout byte-for-byte, so two correct-but-differently-ordered outputs
would be a false FAIL. For these, drop a `.judge` file in the problem dir whose
first token selects a normalization mode:

- **`lineset`** — sort all output lines (dedup) before comparing. Use when each
  line is one complete answer and order across lines doesn't matter.
  Examples: LC-46 (permutations), LC-78 (subsets), LC-39 (combination sum),
  LC-22 (parentheses), LC-15 (3sum triplets), LC-438 (anagram start indices),
  LC-417 (water-flow coordinates).
- **`lineset-si`** — like lineset, but also sort the tokens WITHIN each line first.
  Use when a single line carries multiple values whose order doesn't matter
  (e.g. a set printed space-separated). Example: LC-347 (top-K frequent, where
  the K values may be printed in any order).
- **`exact`** — (default) byte compare, trailing whitespace ignored. No `.judge`
  file needed.

To apply: `echo lineset > problems/<dir>/.judge`. The judge reads this file at
run time and reports the mode used: `LC-46: 5 passed, 0 failed [mode=lineset]`.

When writing cases for a multi-answer problem, still write ONE canonical ordering
in the `.out` (e.g. lexicographic). The normalizer makes any valid ordering match.
