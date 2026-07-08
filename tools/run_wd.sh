#!/usr/bin/env bash
# Build & run the i-th problem for a given week/day (1-based index, default 1).
#   tools/run_wd.sh <week> <day> [index]    e.g. tools/run_wd.sh 1 1, or 1 10 3
set -e
W="$1"; D="$2"; IDX="${3:-1}"
if [ -z "$W" ] || [ -z "$D" ]; then echo "Usage: tools/run_wd.sh <week> <day> [index]" >&2; exit 2; fi

mapfile -t FILES < <(ls problems/week${W}-*/day${D}-lc*/solution.* 2>/dev/null | sort)
F="${FILES[$((IDX-1))]:-}"
if [ -z "$F" ]; then echo "No solution found under problems/week${W}-*/day${D}-lc* (index ${IDX})" >&2; exit 1; fi

DIR=$(dirname "$F")
EXT="${F##*.}"
BIN=/tmp/run_w${W}d${D}_${IDX}

if [ "$EXT" = "cpp" ]; then g++ -std=c++17 -O2 -Wall -Wextra "$F" -o "$BIN"
elif [ "$EXT" = "c" ];   then gcc -std=c11   -O2 -Wall -Wextra "$F" -o "$BIN"
else echo "Unsupported extension: $EXT" >&2; exit 1; fi

echo "=== Run: $F ==="
if [ -f "$DIR/test.in" ]; then
  echo "--- stdin (test.in) ---"; cat "$DIR/test.in"
  echo "--- stdout ---"; "$BIN" < "$DIR/test.in"
else
  echo "(no test.in)"; "$BIN"
fi
