#!/usr/bin/env bash
# Multi-case judge for one LeetCode problem.
#   tools/judge.sh <NUM>          judge LC-<NUM> against all cases in cases/
#   tools/judge.sh <NUM> -v       verbose: show input/expected/actual for failing cases
#
# Cases live in the problem's cases/ dir as: 1.in 1.out 2.in 2.out ...
# Each .in is a complete stdin for main(); each .out is the expected stdout.
#
# Normalization (optional): if the problem dir contains a `.judge` file, its first
# non-empty token selects how actual vs expected are normalized before comparing:
#   exact        (default) byte-compare (trailing whitespace ignored)
#   lineset      sort all output lines (dedup) then compare — for "any order"
#                problems like permutations/subsets/anagrams/coord lists
#   lineset-si   like lineset, but also sort tokens WITHIN each line first — for
#                "any order, multi-value lines" like LC-347 top-K-frequent
#
# Exit 0 = all pass; 1 = >=1 failure; 2 = usage/not-found.
set -u
NUM="$1"; VERBOSE=0
[ "${2:-}" = "-v" ] && VERBOSE=1
if [ -z "$NUM" ]; then echo "Usage: tools/judge.sh <NUM> [-v]" >&2; exit 2; fi
NUM="${NUM#0}"; [ -z "$NUM" ] && NUM=0

# Locate repo root (this script lives in <root>/tools/). Run from there so the
# relative `problems/...` globs below resolve regardless of the caller's CWD.
SELF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$SELF_DIR/.."                         # tools/ -> repo root
cd "$ROOT"

F=$(ls problems/*/day*-lc${NUM}-*/solution.* 2>/dev/null | sort | head -n 1)
if [ -z "$F" ]; then echo "No problem found for LC-${NUM}" >&2; exit 2; fi
DIR=$(dirname "$F"); EXT="${F##*.}"; BIN=/tmp/judge_lc${NUM}

# Normalization mode.
MODE="exact"
if [ -f "$DIR/.judge" ]; then
  MODE=$(head -n1 "$DIR/.judge" | awk '{print $1}')
  case "$MODE" in
    exact|lineset|lineset-si) : ;;
    *) echo "Unknown .judge mode: $MODE (expected exact|lineset|lineset-si)" >&2; MODE="exact" ;;
  esac
fi

# Optional speedup: use a shared precompiled <bits/stdc++.h> if present.
PCHFLAGS=""
if [ -f "$ROOT/.build/all.h.gch" ]; then PCHFLAGS="-include $ROOT/.build/all.h -I $ROOT/.build"; fi

# Compile once.
if [ "$EXT" = "cpp" ]; then
  if ! g++ -std=c++17 -O2 -Wall -Wextra $PCHFLAGS "$F" -o "$BIN" 2>/tmp/jc_err; then
    echo "COMPILE ERROR:"; cat /tmp/jc_err; exit 1
  fi
elif [ "$EXT" = "c" ]; then
  if ! gcc -std=c11 -O2 -Wall -Wextra "$F" -o "$BIN" 2>/tmp/jc_err; then
    echo "COMPILE ERROR:"; cat /tmp/jc_err; exit 1
  fi
else echo "Unsupported extension: $EXT" >&2; exit 2; fi

CASEDIR="$DIR/cases"
if [ ! -d "$CASEDIR" ] || [ -z "$(ls -A "$CASEDIR"/*.in 2>/dev/null)" ]; then
  echo "LC-${NUM}: no cases/ found — falling back to test.in vs file-header expectation."
  [ -f "$DIR/test.in" ] && echo "  output: $("$BIN" < "$DIR/test.in")"
  exit 0
fi

normalize() {  # reads stdin, prints normalized form to stdout
  local txt
  txt=$(cat)
  case "$1" in
    exact)
      printf '%s' "$txt" | sed -e 's/[[:space:]]*$//' | sed -e :a -e '/^$/{$d;N;ba}'
      ;;
    lineset)
      printf '%s' "$txt" | sed -e 's/[[:space:]]*$//' | grep -v '^$' | sort
      ;;
    lineset-si)
      printf '%s' "$txt" | sed -e 's/[[:space:]]*$//' | grep -v '^$' \
        | while IFS= read -r line; do
            printf '%s\n' "$(printf '%s' "$line" | tr ' ' '\n' | grep -v '^$' | sort | tr '\n' ' ' | sed 's/ *$//')"
          done | sort
      ;;
  esac
}

CASES=$(ls "$CASEDIR"/*.in 2>/dev/null | sed -E 's#.*/([0-9]+)\.in$#\1#' | sort -n)
PASS=0; FAIL=0; FAILED_CASES=""
for c in $CASES; do
  IN="$CASEDIR/$c.in"; EXP="$CASEDIR/$c.out"
  [ -f "$EXP" ] || EXP=/dev/null
  ACT=$("$BIN" < "$IN" 2>/tmp/jc_run_err) ; RC=$?
  EXPN=$(normalize "$MODE" < "$EXP")
  ACTN=$(printf '%s' "$ACT" | normalize "$MODE")
  if [ "$RC" -ne 0 ]; then
    echo "  case $c: RUNTIME ERROR (rc=$RC)"; FAIL=$((FAIL+1)); FAILED_CASES="$FAILED_CASES $c"
    [ "$VERBOSE" = 1 ] && { echo "    --- input ---"; cat "$IN"; echo "    --- stderr ---"; cat /tmp/jc_run_err; }
  elif [ "$EXPN" = "$ACTN" ]; then
    echo "  case $c: PASS"; PASS=$((PASS+1))
  else
    echo "  case $c: FAIL"; FAIL=$((FAIL+1)); FAILED_CASES="$FAILED_CASES $c"
    [ "$VERBOSE" = 1 ] && {
      echo "    --- input ---"; cat "$IN"
      echo "    --- expected (raw) ---"; cat "$EXP"
      echo "    --- actual (raw) ---"; printf '%s\n' "$ACT"
    }
  fi
done
echo "LC-${NUM}: $PASS passed, $FAIL failed${FAILED_CASES:+ (failed:$FAILED_CASES)} [mode=$MODE]"
[ "$FAIL" -eq 0 ]
