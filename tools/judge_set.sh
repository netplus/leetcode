#!/usr/bin/env bash
# Judge a SET of problems and print a roll-up summary.
#   tools/judge_set.sh <dir-glob>...   e.g. tools/judge_set.sh 'problems/week1-*'
# Each arg is a glob (quoted); every matching dir with a solution is judged.
# Exit 0 only if EVERY problem passes all its cases.
set -u
SELF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$SELF_DIR/.."
cd "$ROOT"

if [ $# -eq 0 ]; then echo "Usage: tools/judge_set.sh <dir-glob>..." >&2; exit 2; fi

# Collect problem dirs (those containing a solution.cpp or solution.c).
declare -a DIRS=()
for pat in "$@"; do
  for d in $pat; do
    [ -d "$d" ] || continue
    # only dirs that directly contain a solution file (.cpp or .c)
    if [ -f "$d/solution.cpp" ] || [ -f "$d/solution.c" ]; then
      DIRS+=("$d")
    fi
  done
done
TOTAL=${#DIRS[@]}
if [ "$TOTAL" -eq 0 ]; then echo "No problems matched." >&2; exit 2; fi

PASS=0; FAIL=0; FAIL_DETAILS=""
for d in "${DIRS[@]}"; do
  num=$(basename "$d" | sed -E 's/.*lc([0-9]+).*/\1/')
  # strip leading zeros for judge.sh
  num="${num#0}"; [ -z "$num" ] && num=0
  line=$(bash "$SELF_DIR/judge.sh" "$num" 2>/dev/null | tail -1)
  # line like: LC-XX: N passed, M failed (...) [mode=..]
  failed=$(echo "$line" | grep -oE '[0-9]+ failed' | head -1 | grep -oE '[0-9]+')
  [ -z "$failed" ] && failed=0
  if [ "$failed" -eq 0 ]; then
    PASS=$((PASS+1))
  else
    FAIL=$((FAIL+1))
    title=$(basename "$d")
    FAIL_DETAILS="$FAIL_DETAILS\n  $title: $line"
  fi
done

echo "==============================="
echo "Summary: $PASS / $TOTAL problems fully pass"
if [ "$FAIL" -gt 0 ]; then
  echo -e "Failures ($FAIL):$FAIL_DETAILS"
fi
echo "==============================="
[ "$FAIL" -eq 0 ]
