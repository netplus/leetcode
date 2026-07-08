#!/usr/bin/env bash
# Build & run a LeetCode problem by its number, feeding its test.in.
#   tools/run_lc.sh <NUM>    e.g. tools/run_lc.sh 560
set -e
NUM="$1"
if [ -z "$NUM" ]; then echo "Usage: tools/run_lc.sh <NUM>" >&2; exit 2; fi

# Strip leading zeros just in case.
NUM="${NUM#0}"; [ -z "$NUM" ] && NUM=0

F=$(ls problems/*/day*-lc${NUM}-*/solution.* 2>/dev/null | sort | head -n 1)
if [ -z "$F" ]; then echo "No problem found for LC-${NUM}" >&2; exit 1; fi

DIR=$(dirname "$F")
EXT="${F##*.}"
BIN=/tmp/run_lc${NUM}

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
