#!/usr/bin/env bash
# Report per-problem status: does the generated file contain a reviewed
# implementation, or an explicit fallback stub?
#
#   tools/status.sh              -> all problems, grouped by week
#   tools/status.sh week1        -> filter to a week prefix (e.g. week1, week2)
set -u
SELF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$SELF_DIR/.."
cd "$ROOT"
FILTER="${1:-}"

# The canonical generator no longer leaves the old marker in completed files.
# A newly added, not-yet-reviewed problem uses one of these explicit markers.
is_implemented() {  # $1 = solution.cpp path  -> prints 1/0
  local f="$1"
  if grep -qE 'TODO: implement|Your implementation here|解法精讲：尚待补充' "$f"; then
    echo 0
  else
    echo 1
  fi
}

for w in problems/week*; do
  [ -d "$w" ] || continue
  case "$FILTER" in
    "") : ;;
    *) echo "$w" | grep -q "$FILTER" || continue ;;
  esac
  echo "## $(basename "$w")"
  for d in "$w"/day*-lc*/; do
    [ -d "$d" ] || continue
    f="$d/solution.cpp"
    [ -f "$f" ] || continue
    impl=$(is_implemented "$f")
    base=$(basename "$d")
    if [ "$impl" = 1 ]; then
      printf "  [DONE]   %s\n" "$base"
    else
      printf "  [stub]   %s\n" "$base"
    fi
  done
done
