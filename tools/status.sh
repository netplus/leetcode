#!/usr/bin/env bash
# Report per-problem status: is the Solution method still the empty stub, or
# has the user implemented it?
#
#   tools/status.sh              -> all problems, grouped by week
#   tools/status.sh week1        -> filter to a week prefix (e.g. week1, week2)
set -u
SELF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$SELF_DIR/.."
cd "$ROOT"
FILTER="${1:-}"

# A stub is "implemented" if there is any non-comment, non-blank line in the
# Solution method body AFTER the "// Your implementation here." marker (besides
# the single default 'return ...;').
is_implemented() {  # $1 = solution.cpp path  -> prints 1/0
  local f="$1"
  # Extract from the marker to the next line that is just '    }' (end of method).
  # Count real code lines in that span excluding the marker and the lone return.
  local span
  span=$(sed -n '/Your implementation here/,/^    }/p' "$f")
  # Remove the marker line and any line that is *only* a default return.
  local stripped
  stripped=$(echo "$span" \
    | grep -v 'Your implementation here' \
    | grep -vE '^\s*return\s*[^;]*;\s*$' \
    | grep -vE '^\s*$|^\s*//' \
    | grep -vE '^\s*\}\s*$')
  # If any code remains, the user added real logic.
  if echo "$stripped" | grep -qE '\S'; then echo 1; else echo 0; fi
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
