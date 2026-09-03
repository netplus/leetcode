"""Problem metadata for all 4 weeks. Called by gen_all.py via build(append)."""
from gen_all import LISTNODE, TREENODE

# Reusable complete main() harnesses. Each returns/prints in a fixed style.
# Placeholders __CALL__ / __READ__ are substituted per-problem below via build().


def M_read_ints_print_int(call: str) -> str:
    """Read n then n ints into a vector<int>, call `call`, print int result."""
    return (
        "int main() {\n"
        "    ios::sync_with_stdio(false);\n"
        "    cin.tie(nullptr);\n"
        "    Solution sol;\n"
        "    int n;\n"
        "    if (!(cin >> n)) return 0;\n"
        "    vector<int> a(n);\n"
        "    for (int i = 0; i < n; ++i) cin >> a[i];\n"
        f"    cout << {call} << \"\\n\";\n"
        "    return 0;\n"
        "}\n"
    )


def M_read_int_target_print_int(call: str) -> str:
    """Read n and k, then n ints; call `call`; print int."""
    return (
        "int main() {\n"
        "    ios::sync_with_stdio(false);\n"
        "    cin.tie(nullptr);\n"
        "    Solution sol;\n"
        "    int n, k;\n"
        "    if (!(cin >> n >> k)) return 0;\n"
        "    vector<int> a(n);\n"
        "    for (int i = 0; i < n; ++i) cin >> a[i];\n"
        f"    cout << {call} << \"\\n\";\n"
        "    return 0;\n"
        "}\n"
    )


def M_read_ints_print_vecint(call: str) -> str:
    """Read n then n ints; call `call`; print space-separated ints."""
    return (
        "int main() {\n"
        "    ios::sync_with_stdio(false);\n"
        "    cin.tie(nullptr);\n"
        "    Solution sol;\n"
        "    int n;\n"
        "    if (!(cin >> n)) return 0;\n"
        "    vector<int> a(n);\n"
        "    for (int i = 0; i < n; ++i) cin >> a[i];\n"
        f"    auto ans = {call};\n"
        "    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << \" \\n\"[i + 1 == ans.size()];\n"
        "    return 0;\n"
        "}\n"
    )


def M_read_n_target_print_vecint(call: str) -> str:
    return (
        "int main() {\n"
        "    ios::sync_with_stdio(false);\n"
        "    cin.tie(nullptr);\n"
        "    Solution sol;\n"
        "    int n, target;\n"
        "    if (!(cin >> n >> target)) return 0;\n"
        "    vector<int> a(n);\n"
        "    for (int i = 0; i < n; ++i) cin >> a[i];\n"
        f"    auto ans = {call};\n"
        "    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << \" \\n\"[i + 1 == ans.size()];\n"
        "    return 0;\n"
        "}\n"
    )


def M_read_string_print_int(call: str) -> str:
    return (
        "int main() {\n"
        "    ios::sync_with_stdio(false);\n"
        "    cin.tie(nullptr);\n"
        "    Solution sol;\n"
        "    string s;\n"
        "    if (!(cin >> s)) return 0;\n"
        f"    cout << {call} << \"\\n\";\n"
        "    return 0;\n"
        "}\n"
    )


def M_read_string_print_string(call: str) -> str:
    return (
        "int main() {\n"
        "    ios::sync_with_stdio(false);\n"
        "    cin.tie(nullptr);\n"
        "    Solution sol;\n"
        "    string s;\n"
        "    if (!(cin >> s)) return 0;\n"
        f"    cout << {call} << \"\\n\";\n"
        "    return 0;\n"
        "}\n"
    )


def build(P):
    _week1(P)
    _week2(P)
    _week3(P)
    _week4(P)


# ===========================================================================
# WEEK 1 — Arrays / Strings / Hashing / Two Pointers
# ===========================================================================
def _week1(P):
    # ---- Day 1: Hashing + Prefix Sum ----
    P(num=1, week=1, day=1, name="two-sum", title="Two Sum", diff="Easy", prio="P0",
      desc="""// Given an array of integers nums and an integer target, return indices of the
// two numbers such that they add up to target.
// Each input has exactly one solution, and you may not use the same element twice.
// You can return the answer in any order.""",
      constraints="""//   - 2 <= nums.length <= 1e4
//   - -1e9 <= nums[i] <= 1e9
//   - -1e9 <= target <= 1e9
//   - exactly one valid answer exists""",
      iofmt="""//   Line 1: n target
//   Line 2: n space-separated integers
//   Print the two indices (space-separated, ascending).""",
      expected="0 1",
      goal="O(n) time, O(n) space.",
      sig="vector<int> twoSum(vector<int>& nums, int target)", retdefault="{}",
      main=M_read_n_target_print_vecint("sol.twoSum(a, target)"),
      testin="4 9\n2 7 11 15\n")

    P(num=560, week=1, day=1, name="subarray-sum-equals-k", title="Subarray Sum Equals K", diff="Medium", prio="P0",
      desc="""// Given an array of integers nums and an integer k, return the total number of
// non-empty contiguous subarrays whose sum equals k.""",
      constraints="""//   - 1 <= nums.length <= 2e4
//   - -1000 <= nums[i] <= 1000
//   - -1e7 <= k <= 1e7""",
      iofmt="""//   Line 1: n k
//   Line 2: n space-separated integers
//   Print the count of subarrays summing to k.""",
      expected="2", goal="O(n) time, O(n) space.",
      sig="int subarraySum(vector<int>& nums, int k)", retdefault="0",
      main=M_read_int_target_print_int("sol.subarraySum(a, k)"),
      testin="3 2\n1 1 1\n")

    P(num=1248, week=1, day=1, name="count-number-of-nice-subarrays", title="Count Number of Nice Subarrays", diff="Medium", prio="P1",
      desc="""// Given an array nums of integers and an integer k, return the number of
// "nice" subarrays (continuous subarrays with exactly k odd numbers).""",
      constraints="""//   - 1 <= nums.length <= 5e4
//   - 1 <= k <= nums.length
//   - 1 <= nums[i] <= 1e5""",
      iofmt="""//   Line 1: n k
//   Line 2: n space-separated integers
//   Print the number of nice subarrays.""",
      expected="2", goal="O(n) time, O(1) extra space.",
      sig="int numberOfSubarrays(vector<int>& nums, int k)", retdefault="0",
      main=M_read_int_target_print_int("sol.numberOfSubarrays(a, k)"),
      testin="5 3\n1 1 2 1 1\n")

    P(num=53, week=1, day=1, name="maximum-subarray", title="Maximum Subarray", diff="Medium", prio="P0",
      desc="""// Given an integer array nums, find the subarray with the largest sum, and
// return its sum.""",
      constraints="""//   - 1 <= nums.length <= 1e5
//   - -1e4 <= nums[i] <= 1e4""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum subarray sum.""",
      expected="6", goal="O(n) time, O(1) space (Kadane).",
      sig="int maxSubArray(vector<int>& nums)", retdefault="0",
      main=M_read_ints_print_int("sol.maxSubArray(a)"),
      testin="9\n-2 1 -3 4 -1 2 1 -5 4\n")

    # ---- Day 2: Two Pointers ----
    P(num=167, week=1, day=2, name="two-sum-ii-input-array-is-sorted", title="Two Sum II - Input Array Is Sorted", diff="Medium", prio="P0",
      desc="""// Given a 1-indexed array of integers numbers that is already sorted in
// non-decreasing order, find two numbers that add up to a specific target.
// Return the indices (1-indexed), index1 < index2. Exactly one solution exists;
// do not reuse one element, and use only constant extra space.""",
      constraints="""//   - 2 <= numbers.length <= 3e4
//   - -1000 <= numbers[i], target <= 1000
//   - numbers sorted non-decreasing
//   - exactly one valid answer exists""",
      iofmt="""//   Line 1: n target
//   Line 2: n space-separated integers
//   Print two 1-indexed indices (space-separated, ascending).""",
      expected="1 2", goal="O(n) time, O(1) space.",
      sig="vector<int> twoSum(vector<int>& numbers, int target)", retdefault="{}",
      main=M_read_n_target_print_vecint("sol.twoSum(a, target)"),
      testin="4 9\n2 7 11 15\n")

    P(num=15, week=1, day=2, name="3sum", title="3Sum", diff="Medium", prio="P0",
      desc="""// Given an integer array nums, return all unique triplets
// [nums[i], nums[j], nums[k]] such that i != j != k and they sum to 0.
// The solution set must not contain duplicate triplets.""",
      constraints="""//   - 3 <= nums.length <= 3000
//   - -1e5 <= nums[i] <= 1e5""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print each triplet "a b c" on its own line; triplets sorted, no duplicates.""",
      expected="-1 -1 2 | -1 0 1", goal="O(n^2) time.",
      sig="vector<vector<int>> threeSum(vector<int>& nums)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto res = sol.threeSum(a);
    for (auto& t : res) cout << t[0] << " " << t[1] << " " << t[2] << "\\n";
    return 0;
}
""",
      testin="6\n-1 0 1 2 -1 -4\n")

    P(num=11, week=1, day=2, name="container-with-most-water", title="Container With Most Water", diff="Medium", prio="P0",
      desc="""// You are given an integer array height of length n. There are n vertical lines
// drawn such that the two endpoints of the i-th line are (i, 0) and (i, height[i]).
// Find two lines that together with the x-axis form a container holding the most
// water. Return the maximum amount of water a container can store.
// Notice that you may not slant the container.""",
      constraints="""//   - n == height.length
//   - 2 <= n <= 1e5
//   - 0 <= height[i] <= 1e4""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum area.""",
      expected="49", goal="O(n) time, O(1) space.",
      sig="int maxArea(vector<int>& height)", retdefault="0",
      main=M_read_ints_print_int("sol.maxArea(a)"),
      testin="9\n1 8 6 2 5 4 8 3 7\n")

    P(num=283, week=1, day=2, name="move-zeroes", title="Move Zeroes", diff="Easy", prio="P1",
      desc="""// Given an integer array nums, move all 0s to the end while maintaining the
// relative order of the non-zero elements. Do this in-place.""",
      constraints="""//   - 1 <= nums.length <= 1e4
//   - -2^31 <= nums[i] <= 2^31 - 1""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the array after moving zeroes (space-separated).""",
      expected="1 3 12 0 0", goal="O(n) time, O(1) space.",
      sig="void moveZeroes(vector<int>& nums)", retdefault="",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sol.moveZeroes(a);
    for (int i = 0; i < n; ++i) cout << a[i] << " \\n"[i + 1 == n];
    return 0;
}
""",
      testin="5\n0 1 0 3 12\n")

    # ---- Day 3: Sliding Window ----
    P(num=209, week=1, day=3, name="minimum-size-subarray-sum", title="Minimum Size Subarray Sum", diff="Medium", prio="P0",
      desc="""// Given an array of positive integers nums and a positive integer target,
// return the minimal length of a contiguous subarray whose sum is >= target.
// If none, return 0.""",
      constraints="""//   - 1 <= target <= 1e9
//   - 1 <= nums.length <= 1e5
//   - 1 <= nums[i] <= 1e4""",
      iofmt="""//   Line 1: n target
//   Line 2: n space-separated integers
//   Print the minimal length.""",
      expected="2", goal="O(n) time, O(1) space.",
      sig="int minSubArrayLen(int target, vector<int>& nums)", retdefault="0",
      main=M_read_int_target_print_int("sol.minSubArrayLen(k, a)"),
      testin="6 7\n2 3 1 2 4 3\n")

    P(num=3, week=1, day=3, name="longest-substring-without-repeating-characters", title="Longest Substring Without Repeating Characters", diff="Medium", prio="P0",
      desc="""// Given a string s, find the length of the longest substring without
// repeating characters.""",
      constraints="""//   - 0 <= s.length <= 1e5
//   - s consists of English letters, digits, symbols and spaces.""",
      iofmt="""//   Line 1: the string s
//   Print the length.""",
      expected="3", goal="O(n) time, O(min(alphabet,n)) space.",
      sig="int lengthOfLongestSubstring(string s)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s;
    getline(cin, s);
    cout << sol.lengthOfLongestSubstring(s) << "\\n";
    return 0;
}
""",
      testin="abcabcbb\n")

    P(num=76, week=1, day=3, name="minimum-window-substring", title="Minimum Window Substring", diff="Hard", prio="P0",
      desc="""// Given two strings s and t, return the minimum window substring of s such
// that every character in t (including duplicates) is in the window. If none,
// return the empty string.""",
      constraints="""//   - 1 <= s.length, t.length <= 1e5
//   - s and t consist of uppercase/lowercase English letters.""",
      iofmt="""//   Line 1: s
//   Line 2: t
//   Print the minimum window substring.""",
      expected="BANC", goal="O(|s| + |t|) time.",
      sig="string minWindow(string s, string t)", retdefault='""',
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s, t; if (!(cin >> s >> t)) return 0;
    cout << sol.minWindow(s, t) << "\\n";
    return 0;
}
""",
      testin="ADOBECODEBANC\nABC\n")

    P(num=438, week=1, day=3, name="find-all-anagrams-in-a-string", title="Find All Anagrams in a String", diff="Medium", prio="P1",
      desc="""// Given two strings s and p, return an array of all start indices of p's
// anagrams in s. The answer may be returned in any order.""",
      constraints="""//   - 1 <= s.length, p.length <= 3e4
//   - s and p consist of lowercase English letters.""",
      iofmt="""//   Line 1: s
//   Line 2: p
//   Print the start indices (space-separated, ascending).""",
      expected="0 6", goal="O(|s|) time.",
      sig="vector<int> findAnagrams(string s, string p)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s, p; if (!(cin >> s >> p)) return 0;
    auto ans = sol.findAnagrams(s, p);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \\n"[i + 1 == ans.size()];
    return 0;
}
""",
      testin="cbaebabacd\nabc\n")

    # ---- Day 4: Prefix Sum / Difference Array ----
    P(num=303, week=1, day=4, name="range-sum-query-immutable", title="Range Sum Query - Immutable", diff="Easy", prio="P0",
      desc="""// Given an integer array nums, handle multiple queries: sumRange(left, right)
// returns the sum of elements nums[left..right] inclusive.
// (LC class NumArray; here modeled as Solution with build() + sumRange().)""",
      constraints="""//   - 1 <= nums.length <= 1e4
//   - -1e5 <= nums[i] <= 1e5
//   - 0 <= left <= right < nums.length
//   - at most 1e4 calls""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Line 3: q (number of queries)
//   Next q lines: left right
//   Print each sum on its own line.""",
      expected="1 | -1 | -3", goal="O(n) build, O(1) query.",
      sig="int sumRange(int left, int right)", retdefault="0",
      extra="""    void build(vector<int>& nums) {
        // TODO: precompute prefix sums into a member variable.
    }
""",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sol.build(a);
    int q; cin >> q;
    while (q--) {
        int l, r; cin >> l >> r;
        cout << sol.sumRange(l, r) << "\\n";
    }
    return 0;
}
""",
      testin="6\n-2 0 3 -5 2 -1\n3\n0 2\n2 5\n0 5\n")

    P(num=304, week=1, day=4, name="range-sum-query-2d-immutable", title="Range Sum Query 2D - Immutable", diff="Medium", prio="P0",
      desc="""// Given a 2D matrix, handle queries sumRegion(row1,col1,row2,col2) returning
// the sum of elements inside that rectangle (inclusive).
// (LC class NumMatrix; here modeled as Solution with build() + sumRegion().)""",
      constraints="""//   - m == matrix.length, n == matrix[i].length
//   - 1 <= m, n <= 200
//   - -1e4 <= matrix[i][j] <= 1e4
//   - 0 <= row1 <= row2 < m, 0 <= col1 <= col2 < n
//   - at most 1e4 calls will be made to sumRegion""",
      iofmt="""//   Line 1: m n
//   Next m lines: n space-separated integers
//   Line: q (number of queries)
//   Next q lines: row1 col1 row2 col2
//   Print each region sum on its own line.""",
      expected="8", goal="O(mn) build, O(1) query.",
      sig="int sumRegion(int row1, int col1, int row2, int col2)", retdefault="0",
      extra="""    void build(vector<vector<int>>& matrix) {
        // TODO: precompute 2D prefix sums into a member variable.
    }
""",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<int>> mat(m, vector<int>(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) cin >> mat[i][j];
    sol.build(mat);
    int q; cin >> q;
    while (q--) {
        int r1, c1, r2, c2; cin >> r1 >> c1 >> r2 >> c2;
        cout << sol.sumRegion(r1, c1, r2, c2) << "\\n";
    }
    return 0;
}
""",
      testin="5 5\n3 0 1 4 2\n5 6 3 2 1\n1 2 0 1 5\n4 1 0 1 7\n1 0 3 0 5\n1\n2 1 4 3\n")

    P(num=1109, week=1, day=4, name="corporate-flight-bookings", title="Corporate Flight Bookings", diff="Medium", prio="P0",
      desc="""// There are n flights labeled 1..n. bookings[i] = [first, last, seats] books
// that many seats on each flight from first to last inclusive. Return an array
// of length n where answer[i] is the total seats booked on flight i+1.""",
      constraints="""//   - 1 <= n <= 2e4
//   - 1 <= bookings.length <= 2e4
//   - bookings[i].length == 3
//   - 1 <= first <= last <= n
//   - 1 <= seats <= 1e4""",
      iofmt="""//   Line 1: n (flights) m (bookings)
//   Next m lines: first last seats
//   Print n space-separated integers (seats per flight, 1-indexed).""",
      expected="10 55 45 25 25 0 0 0 0 0", goal="O(n + m) with difference array.",
      sig="vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, m; if (!(cin >> n >> m)) return 0;
    vector<vector<int>> bk(m, vector<int>(3));
    for (int i = 0; i < m; ++i) cin >> bk[i][0] >> bk[i][1] >> bk[i][2];
    auto ans = sol.corpFlightBookings(bk, n);
    for (int i = 0; i < n; ++i) cout << ans[i] << " \\n"[i + 1 == n];
    return 0;
}
""",
      testin="10 3\n1 2 10\n2 3 20\n2 5 25\n")

    P(num=238, week=1, day=4, name="product-of-array-except-self", title="Product of Array Except Self", diff="Medium", prio="P1",
      desc="""// Given an integer array nums, return an array answer such that answer[i]
// equals the product of all elements of nums except nums[i]. O(n), no division.""",
      constraints="""//   - 2 <= nums.length <= 1e5
//   - -30 <= nums[i] <= 30
//   - product of any prefix/suffix fits in a 32-bit int""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print n space-separated integers (the products).""",
      expected="24 12 8 6", goal="O(n) time, O(1) extra space (output excluded).",
      sig="vector<int> productExceptSelf(vector<int>& nums)", retdefault="{}",
      main=M_read_ints_print_vecint("sol.productExceptSelf(a)"),
      testin="4\n1 2 3 4\n")

    # ---- Day 5: Strings ----
    P(num=5, week=1, day=5, name="longest-palindromic-substring", title="Longest Palindromic Substring", diff="Medium", prio="P0",
      desc="""// Given a string s, return the longest palindromic substring in s.""",
      constraints="""//   - 1 <= s.length <= 1000
//   - s consists of digits and English letters.""",
      iofmt="""//   Line 1: the string s
//   Print the longest palindromic substring.""",
      expected="bab", goal="O(n^2) (expand) or O(n) (Manacher).",
      sig="string longestPalindrome(string s)", retdefault='""',
      main=M_read_string_print_string("sol.longestPalindrome(s)"),
      testin="babad\n")

    P(num=14, week=1, day=5, name="longest-common-prefix", title="Longest Common Prefix", diff="Easy", prio="P0",
      desc="""// Find the longest common prefix string amongst an array of strings. If none,
// return the empty string.""",
      constraints="""//   - 1 <= strs.length <= 200
//   - 0 <= strs[i].length <= 200
//   - strs[i] consists of lowercase English letters.""",
      iofmt="""//   Line 1: n (number of strings)
//   Next n lines: one string each
//   Print the longest common prefix.""",
      expected="fl", goal="O(S) where S = total characters.",
      sig="string longestCommonPrefix(vector<string>& strs)", retdefault='""',
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<string> strs(n);
    string discard; getline(cin, discard);
    for (int i = 0; i < n; ++i) getline(cin, strs[i]);
    cout << sol.longestCommonPrefix(strs) << "\\n";
    return 0;
}
""",
      testin="3\nflower\nflow\nflight\n")

    P(num=151, week=1, day=5, name="reverse-words-in-a-string", title="Reverse Words in a String", diff="Medium", prio="P1",
      desc="""// Given an input string s, reverse the order of the words. A word is a
// sequence of non-space characters. Return a string of the words in reverse
// order separated by a single space, with no leading/trailing spaces.""",
      constraints="""//   - 1 <= s.length <= 1e4
//   - s contains English letters, digits, and spaces
//   - s contains at least one word""",
      iofmt="""//   Line 1: the whole string s (may have leading/trailing/multiple spaces)
//   Print the reversed, single-space-joined words.""",
      expected="blue is sky the", goal="O(n) time, O(1) extra (in-place).",
      sig="string reverseWords(string s)", retdefault='""',
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s;
    getline(cin, s);
    cout << sol.reverseWords(s) << "\\n";
    return 0;
}
""",
      testin="  the sky   is blue  \n")

    P(num=28, week=1, day=5, name="find-the-index-of-the-first-occurrence-in-a-string", title="Find the Index of the First Occurrence in a String", diff="Easy", prio="P1",
      desc="""// Given two strings haystack and needle, return the index of the first
// occurrence of needle in haystack, or -1 if needle is not part of haystack.""",
      constraints="""//   - 1 <= haystack.length, needle.length <= 1e4
//   - haystack and needle consist of lowercase English letters.""",
      iofmt="""//   Line 1: haystack
//   Line 2: needle
//   Print the index (0-based) or -1.""",
      expected="0", goal="O(n+m) (KMP) or O(n*m) naive.",
      sig="int strStr(string haystack, string needle)", retdefault="-1",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string h, n; if (!(cin >> h >> n)) return 0;
    cout << sol.strStr(h, n) << "\\n";
    return 0;
}
""",
      testin="sadbutsad\nsad\n")

    # ---- Day 6: Intervals / Sort Variants ----
    P(num=56, week=1, day=6, name="merge-intervals", title="Merge Intervals", diff="Medium", prio="P0",
      desc="""// Given an array of intervals where intervals[i] = [start, end], merge all
// overlapping intervals, and return an array of the non-overlapping intervals
// covering all input intervals.""",
      constraints="""//   - 1 <= intervals.length <= 1e4
//   - intervals[i].length == 2
//   - 0 <= start <= end <= 1e4""",
      iofmt="""//   Line 1: m (number of intervals)
//   Next m lines: start end
//   Print merged intervals one per line (start end), sorted by start.""",
      expected="1 6 | 8 10 | 15 18", goal="O(m log m) time.",
      sig="vector<vector<int>> merge(vector<vector<int>>& intervals)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m; if (!(cin >> m)) return 0;
    vector<vector<int>> iv(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> iv[i][0] >> iv[i][1];
    auto res = sol.merge(iv);
    for (auto& x : res) cout << x[0] << " " << x[1] << "\\n";
    return 0;
}
""",
      testin="4\n1 3\n2 6\n8 10\n15 18\n")

    P(num=57, week=1, day=6, name="insert-interval", title="Insert Interval", diff="Medium", prio="P1",
      desc="""// Given non-overlapping intervals sorted by start and a new interval, insert it
// and merge as needed so the result remains sorted and non-overlapping. Closed
// intervals sharing an endpoint count as overlapping.""",
      constraints="""//   - 0 <= intervals.length <= 1e4
//   - intervals[i].length == 2, newInterval.length == 2
//   - intervals sorted ascending by start, non-overlapping
//   - 0 <= start <= end <= 1e5""",
      iofmt="""//   Line 1: m (number of intervals)
//   Next m lines: start end
//   Last line: start end (the new interval)
//   Print merged intervals one per line (start end).""",
      expected="1 2 | 3 10", goal="O(n) time.",
      sig="vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m; if (!(cin >> m)) return 0;
    vector<vector<int>> iv(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> iv[i][0] >> iv[i][1];
    vector<int> ni(2); cin >> ni[0] >> ni[1];
    auto res = sol.insert(iv, ni);
    for (auto& x : res) cout << x[0] << " " << x[1] << "\\n";
    return 0;
}
""",
      testin="4\n1 2\n3 5\n6 7\n8 10\n4 8\n")

    P(num=179, week=1, day=6, name="largest-number", title="Largest Number", diff="Medium", prio="P1",
      desc="""// Given a list of non-negative integers nums, arrange them such that they
// form the largest number and return it as a string.""",
      constraints="""//   - 1 <= nums.length <= 100
//   - 0 <= nums[i] <= 1e9""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the largest number string.""",
      expected="210", goal="O(n log n * L).",
      sig="string largestNumber(vector<int>& nums)", retdefault='""',
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.largestNumber(a) << "\\n";
    return 0;
}
""",
      testin="2\n10 2\n")

    P(num=252, week=1, day=6, name="meeting-rooms", title="Meeting Rooms", diff="Easy", prio="P1",
      desc="""// Given an array of meeting time intervals where intervals[i] = [start, end],
// determine if a person could attend all meetings (no overlaps).""",
      constraints="""//   - 0 <= intervals.length <= 1e4
//   - intervals[i].length == 2
//   - 0 <= start < end <= 1e6""",
      iofmt="""//   Line 1: m (number of intervals)
//   Next m lines: start end
//   Print 1 if can attend all (no overlap), else 0.""",
      expected="0", goal="O(m log m) time.",
      sig="bool canAttendMeetings(vector<vector<int>>& intervals)", retdefault="false",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m; if (!(cin >> m)) return 0;
    vector<vector<int>> iv(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> iv[i][0] >> iv[i][1];
    cout << (sol.canAttendMeetings(iv) ? 1 : 0) << "\\n";
    return 0;
}
""",
      testin="2\n0 30\n5 10\n")

    # ---- Day 7: Week 1 Review ----
    P(num=239, week=1, day=7, name="sliding-window-maximum", title="Sliding Window Maximum", diff="Hard", prio="P0",
      desc="""// Return the max value in each sliding window of size k moving from left to
// right across nums. Return an array of the maxes.""",
      constraints="""//   - 1 <= nums.length <= 1e5
//   - -1e4 <= nums[i] <= 1e4
//   - 1 <= k <= nums.length""",
      iofmt="""//   Line 1: n k
//   Line 2: n space-separated integers
//   Print (n-k+1) space-separated maxes.""",
      expected="3 3 5 5 6 7", goal="O(n) time (monotonic deque).",
      sig="vector<int> maxSlidingWindow(vector<int>& nums, int k)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.maxSlidingWindow(a, k);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \\n"[i + 1 == ans.size()];
    return 0;
}
""",
      testin="8 3\n1 3 -1 -3 5 3 6 7\n")

    P(num=41, week=1, day=7, name="first-missing-positive", title="First Missing Positive", diff="Hard", prio="P0",
      desc="""// Given an unsorted integer array nums, return the smallest positive integer
// not present in nums. O(n) time, O(1) auxiliary space.""",
      constraints="""//   - 1 <= nums.length <= 1e5
//   - -2^31 <= nums[i] <= 2^31 - 1""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the smallest missing positive integer.""",
      expected="3", goal="O(n) time, O(1) space.",
      sig="int firstMissingPositive(vector<int>& nums)", retdefault="1",
      main=M_read_ints_print_int("sol.firstMissingPositive(a)"),
      testin="3\n1 2 0\n")

    P(num=31, week=1, day=7, name="next-permutation", title="Next Permutation", diff="Medium", prio="P1",
      desc="""// Rearrange nums into the next lexicographically greater permutation in-place.
// If no greater permutation exists, rearrange into the lowest possible order.""",
      constraints="""//   - 1 <= nums.length <= 100
//   - 0 <= nums[i] <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the next permutation (space-separated).""",
      expected="1 3 2", goal="O(n) time, O(1) space.",
      sig="void nextPermutation(vector<int>& nums)", retdefault="",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sol.nextPermutation(a);
    for (int i = 0; i < n; ++i) cout << a[i] << " \\n"[i + 1 == n];
    return 0;
}
""",
      testin="3\n1 2 3\n")

    P(num=128, week=1, day=7, name="longest-consecutive-sequence", title="Longest Consecutive Sequence", diff="Medium", prio="P0",
      desc="""// Given an unsorted array of integers nums, return the length of the longest
// consecutive elements sequence. O(n) time.""",
      constraints="""//   - 0 <= nums.length <= 1e5
//   - -1e9 <= nums[i] <= 1e9""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the longest consecutive sequence length.""",
      expected="4", goal="O(n) time, O(n) space.",
      sig="int longestConsecutive(vector<int>& nums)", retdefault="0",
      main=M_read_ints_print_int("sol.longestConsecutive(a)"),
      testin="6\n100 4 200 1 3 2\n")


# ===========================================================================
# WEEK 2 — Linked Lists / Trees / BFS-DFS / Stacks
# ===========================================================================

# Shared helper-source block (goes before main) for list problems.
LIST_HELPERS = """[[maybe_unused]] static ListNode* buildList(const vector<int>& a) {
    ListNode dummy; ListNode* tail = &dummy;
    for (int v : a) { tail->next = new ListNode(v); tail = tail->next; }
    return dummy.next;
}
[[maybe_unused]] static void printList(ListNode* h) {
    bool first = true;
    for (ListNode* c = h; c; c = c->next) { if (!first) cout << " "; cout << c->val; first = false; }
    cout << "\\n";
}
"""

TREE_HELPERS = """[[maybe_unused]] static TreeNode* buildTreeLO(const vector<long long>& a) {
    if (a.empty() || a[0] == -1) return nullptr;
    TreeNode* root = new TreeNode((int)a[0]);
    queue<TreeNode*> q; q.push(root);
    size_t i = 1;
    while (!q.empty() && i < a.size()) {
        TreeNode* cur = q.front(); q.pop();
        if (i < a.size() && a[i] != -1) { cur->left = new TreeNode((int)a[i]); q.push(cur->left); }
        i++;
        if (i < a.size() && a[i] != -1) { cur->right = new TreeNode((int)a[i]); q.push(cur->right); }
        i++;
    }
    return root;
}
[[maybe_unused]] static void printTree(TreeNode* root) {
    // Level-order with -1 for null; trailing nulls trimmed.
    if (!root) { cout << "\\n"; return; }
    queue<TreeNode*> q; q.push(root);
    vector<long long> out;
    while (!q.empty()) {
        auto* n = q.front(); q.pop();
        if (n) { out.push_back(n->val); q.push(n->left); q.push(n->right); }
        else out.push_back(-1);
    }
    while (!out.empty() && out.back() == -1) out.pop_back();
    for (size_t i = 0; i < out.size(); ++i) cout << out[i] << " \\n"[i + 1 == out.size()];
}
"""


def _week2(P):
    # ---- Day 8: Linked List Basics ----
    P(num=206, week=2, day=8, name="reverse-linked-list", title="Reverse Linked List", diff="Easy", prio="P0",
      desc="""// Given the head of a singly linked list, reverse the list, and return the
// reversed list.""",
      constraints="""//   - number of nodes in [0, 5000]
//   - -5000 <= Node.val <= 5000""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated node values
//   Print the reversed list values space-separated.""",
      expected="5 4 3 2 1", goal="O(n) time, O(1) space (iterative).",
      sig="ListNode* reverseList(ListNode* head)", retdefault="nullptr",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    printList(sol.reverseList(head));
    return 0;
}
""",
      testin="5\n1 2 3 4 5\n")

    P(num=92, week=2, day=8, name="reverse-linked-list-ii", title="Reverse Linked List II", diff="Medium", prio="P0",
      desc="""// Given the head of a singly linked list and two integers left and right
// (1-indexed), reverse the nodes from position left to position right, and
// return the reversed list.""",
      constraints="""//   - number of nodes == n
//   - 1 <= n <= 500
//   - 1 <= left <= right <= n
//   - -500 <= Node.val <= 500""",
      iofmt="""//   Line 1: n left right
//   Line 2: n space-separated node values
//   Print the resulting list values space-separated.""",
      expected="1 4 3 2 5", goal="O(n) time, O(1) space.",
      sig="ListNode* reverseBetween(ListNode* head, int left, int right)", retdefault="nullptr",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, left, right; if (!(cin >> n >> left >> right)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    printList(sol.reverseBetween(head, left, right));
    return 0;
}
""",
      testin="5 2 4\n1 2 3 4 5\n")

    P(num=21, week=2, day=8, name="merge-two-sorted-lists", title="Merge Two Sorted Lists", diff="Easy", prio="P0",
      desc="""// Merge two sorted linked lists by splicing their existing nodes together,
// and return the head of the resulting sorted list.""",
      constraints="""//   - number of nodes in both lists in [0, 50]
//   - -100 <= Node.val <= 100
//   - both lists sorted in non-decreasing order""",
      iofmt="""//   Line 1: n (length of list1)
//   Line 2: n values of list1
//   Line 3: m (length of list2)
//   Line 4: m values of list2
//   Print the merged list values space-separated.""",
      expected="1 1 2 3 4 4", goal="O(n+m) time, O(1) space.",
      sig="ListNode* mergeTwoLists(ListNode* list1, ListNode* list2)", retdefault="nullptr",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    int m; cin >> m;
    vector<int> b(m);
    for (int i = 0; i < m; ++i) cin >> b[i];
    printList(sol.mergeTwoLists(buildList(a), buildList(b)));
    return 0;
}
""",
      testin="3\n1 2 4\n3\n1 3 4\n")

    P(num=160, week=2, day=8, name="intersection-of-two-linked-lists", title="Intersection of Two Linked Lists", diff="Easy", prio="P1",
      desc="""// Given the heads of two singly linked-lists, return the node at which the two
// lists intersect. If no intersection, return null.
// The full structure contains no cycle, and the linked lists must retain their
// original structure after the function returns.""",
      constraints="""//   - number of nodes in listA in [1, 3e4]
//   - number of nodes in listB in [1, 3e4]
//   - 1 <= Node.val <= 1e5
//   - intersectVal is 0 or the value of the intersected node""",
      iofmt="""//   Line 1: na (len listA) nb (len listB) (skipA skipB ignored here)
//   Line 2: na values of listA (with shared suffix appended)
//   Line 3: nb values of listB (same shared suffix)
//   Line 4: interLen (number of shared nodes)
//   Build: last interLen nodes are shared. Print the intersected node value, or -1.""",
      expected="8", goal="O(n+m) time, O(1) space.",
      sig="ListNode *getIntersectionNode(ListNode *headA, ListNode *headB)", retdefault="nullptr",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int na, nb; if (!(cin >> na >> nb)) return 0;
    vector<int> a(na), b(nb);
    for (int i = 0; i < na; ++i) cin >> a[i];
    for (int i = 0; i < nb; ++i) cin >> b[i];
    int interLen; cin >> interLen;
    // Build lists; last interLen nodes are shared.
    ListNode* headA = buildList(a);
    ListNode* headB = buildList(b);
    if (interLen > 0) {
        // walk A to the start of the shared suffix and attach a shared tail from B
        int skipA = na - interLen, skipB = nb - interLen;
        // rebuild with shared tail
        vector<int> shared(a.begin() + skipA, a.end());
        ListNode dA, *tA = &dA;
        for (int i = 0; i < skipA; ++i) { tA->next = new ListNode(a[i]); tA = tA->next; }
        ListNode* sharedHead = buildList(shared);
        tA->next = sharedHead;
        ListNode dB, *tB = &dB;
        for (int i = 0; i < skipB; ++i) { tB->next = new ListNode(b[i]); tB = tB->next; }
        tB->next = sharedHead;
        headA = dA.next; headB = dB.next;
    }
    ListNode* ans = sol.getIntersectionNode(headA, headB);
    cout << (ans ? ans->val : -1) << "\\n";
    return 0;
}
""",
      testin="5 6\n4 1 8 4 5\n5 6 1 8 4 5\n3\n")

    # ---- Day 9: Linked List Advanced ----
    P(num=141, week=2, day=9, name="linked-list-cycle", title="Linked List Cycle", diff="Easy", prio="P0",
      desc="""// Given head, determine if the linked list has a cycle in it. Return true if
// there is a cycle, else false.""",
      constraints="""//   - number of nodes in [0, 1e4]
//   - -1e5 <= Node.val <= 1e5
//   - pos is -1 or a valid index (cycle entry)""",
      iofmt="""//   Line 1: n pos
//   Line 2: n values
//   pos = index where tail connects (-1 = no cycle)
//   Print 1 if cycle, else 0.""",
      expected="1", goal="O(n) time, O(1) space (fast/slow).",
      sig="bool hasCycle(ListNode *head)", retdefault="false",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, pos; if (!(cin >> n >> pos)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    if (pos >= 0) {
        ListNode* tail = head; while (tail->next) tail = tail->next;
        ListNode* p = head; for (int i = 0; i < pos; ++i) p = p->next;
        tail->next = p;
    }
    cout << (sol.hasCycle(head) ? 1 : 0) << "\\n";
    return 0;
}
""",
      testin="4 1\n3 2 0 -4\n")

    P(num=142, week=2, day=9, name="linked-list-cycle-ii", title="Linked List Cycle II", diff="Medium", prio="P0",
      desc="""// Given head, return the node where the cycle begins. If no cycle, return null.
// Do not modify the linked list.""",
      constraints="""//   - number of nodes in [0, 1e4]
//   - -1e5 <= Node.val <= 1e5
//   - pos is -1 or a valid index""",
      iofmt="""//   Line 1: n pos
//   Line 2: n values
//   Print the value of the cycle-entry node, or -1.""",
      expected="2", goal="O(n) time, O(1) space.",
      sig="ListNode *detectCycle(ListNode *head)", retdefault="nullptr",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, pos; if (!(cin >> n >> pos)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    if (pos >= 0) {
        ListNode* tail = head; while (tail->next) tail = tail->next;
        ListNode* p = head; for (int i = 0; i < pos; ++i) p = p->next;
        tail->next = p;
    }
    ListNode* ans = sol.detectCycle(head);
    cout << (ans ? ans->val : -1) << "\\n";
    return 0;
}
""",
      testin="4 1\n3 2 0 -4\n")

    P(num=19, week=2, day=9, name="remove-nth-node-from-end-of-list", title="Remove Nth Node From End of List", diff="Medium", prio="P0",
      desc="""// Given the head of a linked list, remove the nth node from the end of the
// list and return its head.""",
      constraints="""//   - number of nodes == sz
//   - 1 <= sz <= 30
//   - 0 <= Node.val <= 100
//   - 1 <= n <= sz""",
      iofmt="""//   Line 1: n (length) k (nth from end to remove)
//   Line 2: n values
//   Print the resulting list values space-separated.""",
      expected="1 2 3 5", goal="O(n) time, O(1) space.",
      sig="ListNode* removeNthFromEnd(ListNode* head, int n)", retdefault="nullptr",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    printList(sol.removeNthFromEnd(buildList(a), k));
    return 0;
}
""",
      testin="5 2\n1 2 3 4 5\n")

    P(num=234, week=2, day=9, name="palindrome-linked-list", title="Palindrome Linked List", diff="Easy", prio="P1",
      desc="""// Given the head of a singly linked list, return true if it is a palindrome.""",
      constraints="""//   - number of nodes in [1, 1e5]
//   - 0 <= Node.val <= 9""",
      iofmt="""//   Line 1: n
//   Line 2: n values
//   Print 1 if palindrome, else 0.""",
      expected="1", goal="O(n) time, O(1) space (follow-up).",
      sig="bool isPalindrome(ListNode* head)", retdefault="false",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << (sol.isPalindrome(buildList(a)) ? 1 : 0) << "\\n";
    return 0;
}
""",
      testin="4\n1 2 2 1\n")

    # ---- Day 10: Binary Tree Traversal ----
    P(num=144, week=2, day=10, name="binary-tree-preorder-traversal", title="Binary Tree Preorder Traversal", diff="Easy", prio="P0",
      desc="""// Given the root of a binary tree, return the preorder traversal of its nodes'
// values.""",
      constraints="""//   - number of nodes in [0, 100]
//   - -100 <= Node.val <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers (level order, -1 = null)
//   Print the preorder traversal space-separated.""",
      expected="1 2 3", goal="O(n) time.",
      sig="vector<int> preorderTraversal(TreeNode* root)", retdefault="{}",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.preorderTraversal(buildTreeLO(a));
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \\n"[i + 1 == ans.size()];
    return 0;
}
""",
      testin="4\n1 -1 2 3\n")

    P(num=94, week=2, day=10, name="binary-tree-inorder-traversal", title="Binary Tree Inorder Traversal", diff="Easy", prio="P0",
      desc="""// Given the root of a binary tree, return the inorder traversal of its nodes'
// values.""",
      constraints="""//   - number of nodes in [0, 100]
//   - -100 <= Node.val <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers (level order, -1 = null)
//   Print the inorder traversal space-separated.""",
      expected="1 3 2", goal="O(n) time.",
      sig="vector<int> inorderTraversal(TreeNode* root)", retdefault="{}",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.inorderTraversal(buildTreeLO(a));
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \\n"[i + 1 == ans.size()];
    return 0;
}
""",
      testin="4\n1 -1 2 3\n")

    P(num=145, week=2, day=10, name="binary-tree-postorder-traversal", title="Binary Tree Postorder Traversal", diff="Easy", prio="P0",
      desc="""// Given the root of a binary tree, return the postorder traversal of its
// nodes' values.""",
      constraints="""//   - number of nodes in [0, 100]
//   - -100 <= Node.val <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers (level order, -1 = null)
//   Print the postorder traversal space-separated.""",
      expected="3 2 1", goal="O(n) time.",
      sig="vector<int> postorderTraversal(TreeNode* root)", retdefault="{}",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.postorderTraversal(buildTreeLO(a));
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \\n"[i + 1 == ans.size()];
    return 0;
}
""",
      testin="4\n1 -1 2 3\n")

    P(num=102, week=2, day=10, name="binary-tree-level-order-traversal", title="Binary Tree Level Order Traversal", diff="Medium", prio="P0",
      desc="""// Given the root of a binary tree, return the level order traversal of its
// nodes' values (i.e., from left to right, level by level).""",
      constraints="""//   - number of nodes in [0, 2000]
//   - -1000 <= Node.val <= 1000""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers (level order, -1 = null)
//   Print one level per line, space-separated.""",
      expected="3 | 9 20 | 15 7", goal="O(n) time.",
      sig="vector<vector<int>> levelOrder(TreeNode* root)", retdefault="{}",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto res = sol.levelOrder(buildTreeLO(a));
    for (auto& lvl : res) {
        for (size_t i = 0; i < lvl.size(); ++i) cout << lvl[i] << " \\n"[i + 1 == lvl.size()];
    }
    return 0;
}
""",
      testin="7\n3 9 20 -1 -1 15 7\n")

    P(num=104, week=2, day=10, name="maximum-depth-of-binary-tree", title="Maximum Depth of Binary Tree", diff="Easy", prio="P0",
      desc="""// Given the root of a binary tree, return its maximum depth (number of nodes
// along the longest path from root to a leaf).""",
      constraints="""//   - number of nodes in [0, 1e4]
//   - -100 <= Node.val <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers (level order, -1 = null)
//   Print the maximum depth.""",
      expected="3", goal="O(n) time.",
      sig="int maxDepth(TreeNode* root)", retdefault="0",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.maxDepth(buildTreeLO(a)) << "\\n";
    return 0;
}
""",
      testin="7\n3 9 20 -1 -1 15 7\n")

    P(num=105, week=2, day=10, name="construct-binary-tree-from-preorder-and-inorder-traversal", title="Construct Binary Tree from Preorder and Inorder Traversal", diff="Medium", prio="P0",
      desc="""// Given two integer arrays preorder and inorder where preorder is the
// preorder traversal of a binary tree and inorder is the inorder traversal of
// the same tree, construct and return the binary tree.""",
      constraints="""//   - 1 <= preorder.length <= 3000
//   - inorder.length == preorder.length
//   - all values unique
//   - -3000 <= preorder[i], inorder[i] <= 3000""",
      iofmt="""//   Line 1: n
//   Line 2: n preorder values
//   Line 3: n inorder values
//   Print the constructed tree's level order (-1 = null).""",
      expected="3 9 20 -1 -1 15 7", goal="O(n) time (with hash map).",
      sig="TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder)", retdefault="nullptr",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> pre(n), in(n);
    for (int i = 0; i < n; ++i) cin >> pre[i];
    for (int i = 0; i < n; ++i) cin >> in[i];
    TreeNode* root = sol.buildTree(pre, in);
    printTree(root);
    return 0;
}
""",
      testin="5\n3 9 20 15 7\n9 3 15 20 7\n")

    # ---- Day 11: Binary Tree Properties ----
    P(num=226, week=2, day=11, name="invert-binary-tree", title="Invert Binary Tree", diff="Easy", prio="P0",
      desc="""// Given the root of a binary tree, invert the tree, and return its root.""",
      constraints="""//   - number of nodes in [0, 100]
//   - -100 <= Node.val <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers (level order, -1 = null)
//   Print the inverted tree's level order (-1 = null).""",
      expected="4 7 2 9 6 3 1", goal="O(n) time.",
      sig="TreeNode* invertTree(TreeNode* root)", retdefault="nullptr",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    printTree(sol.invertTree(buildTreeLO(a)));
    return 0;
}
""",
      testin="7\n4 2 7 1 3 6 9\n")

    P(num=236, week=2, day=11, name="lowest-common-ancestor-of-a-binary-tree", title="Lowest Common Ancestor of a Binary Tree", diff="Medium", prio="P0",
      desc="""// Given a binary tree, find the lowest common ancestor (LCA) of two given
// nodes p and q. The LCA is the lowest node that has both p and q as descendants;
// a node is allowed to be a descendant of itself.""",
      constraints="""//   - number of nodes in [2, 1e5]
//   - -1e9 <= Node.val <= 1e9
//   - all Node.val are unique
//   - p != q and both exist in the tree""",
      iofmt="""//   Line 1: n
//   Line 2: n level-order values (-1 = null)
//   Line 3: p q (values of the two nodes)
//   Print the LCA node value.""",
      expected="3", goal="O(n) time.",
      sig="TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q)", retdefault="nullptr",
      prestruct=TREENODE + TREE_HELPERS + """static TreeNode* findNode(TreeNode* root, int val) {
    if (!root) return nullptr;
    if (root->val == val) return root;
    TreeNode* l = findNode(root->left, val);
    if (l) return l;
    return findNode(root->right, val);
}
""",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    int pv, qv; cin >> pv >> qv;
    TreeNode* root = buildTreeLO(a);
    TreeNode* p = findNode(root, pv);
    TreeNode* q = findNode(root, qv);
    TreeNode* ans = sol.lowestCommonAncestor(root, p, q);
    cout << (ans ? ans->val : -1) << "\\n";
    return 0;
}
""",
      testin="9\n3 5 1 6 2 0 8 -1 -1 7 4\n5 1\n") if False else None

    P(num=236, week=2, day=11, name="lowest-common-ancestor-of-a-binary-tree", title="Lowest Common Ancestor of a Binary Tree", diff="Medium", prio="P0",
      desc="""// Given a binary tree, find the lowest common ancestor (LCA) of two given
// nodes p and q. The LCA is the lowest node that has both p and q as descendants;
// a node is allowed to be a descendant of itself.""",
      constraints="""//   - number of nodes in [2, 1e5]
//   - -1e9 <= Node.val <= 1e9
//   - all Node.val are unique
//   - p != q and both exist in the tree""",
      iofmt="""//   Line 1: n
//   Line 2: n level-order values (-1 = null)
//   Line 3: p q (values of the two nodes)
//   Print the LCA node value.""",
      expected="3", goal="O(n) time.",
      sig="TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q)", retdefault="nullptr",
      prestruct=TREENODE + TREE_HELPERS + """static TreeNode* findNode(TreeNode* root, int val) {
    if (!root) return nullptr;
    if (root->val == val) return root;
    TreeNode* l = findNode(root->left, val);
    if (l) return l;
    return findNode(root->right, val);
}
""",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    int pv, qv; cin >> pv >> qv;
    TreeNode* root = buildTreeLO(a);
    TreeNode* p = findNode(root, pv);
    TreeNode* q = findNode(root, qv);
    TreeNode* ans = sol.lowestCommonAncestor(root, p, q);
    cout << (ans ? ans->val : -1) << "\\n";
    return 0;
}
""",
      testin="7\n3 5 1 6 2 0 8\n5 1\n")

    P(num=543, week=2, day=11, name="diameter-of-binary-tree", title="Diameter of Binary Tree", diff="Easy", prio="P0",
      desc="""// Given the root of a binary tree, return the length (number of edges) of
// the diameter of the tree (longest path between any two nodes).""",
      constraints="""//   - number of nodes in [1, 1e4]
//   - -100 <= Node.val <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n level-order values (-1 = null)
//   Print the diameter (number of edges).""",
      expected="3", goal="O(n) time.",
      sig="int diameterOfBinaryTree(TreeNode* root)", retdefault="0",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.diameterOfBinaryTree(buildTreeLO(a)) << "\\n";
    return 0;
}
""",
      testin="5\n1 2 3 4 5\n")

    P(num=98, week=2, day=11, name="validate-binary-search-tree", title="Validate Binary Search Tree", diff="Medium", prio="P1",
      desc="""// Given the root of a binary tree, determine if it is a valid binary search
// tree (BST). A valid BST: a node's left subtree holds only keys strictly less
// than the node's key, its right subtree only keys strictly greater, and both
// subtrees must themselves be valid BSTs.""",
      constraints="""//   - number of nodes in [1, 1e4]
//   - -2^31 <= Node.val <= 2^31 - 1""",
      iofmt="""//   Line 1: n
//   Line 2: n level-order values (-1 = null)
//   Print 1 if valid BST, else 0.""",
      expected="0", goal="O(n) time.",
      sig="bool isValidBST(TreeNode* root)", retdefault="false",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << (sol.isValidBST(buildTreeLO(a)) ? 1 : 0) << "\\n";
    return 0;
}
""",
      testin="7\n5 1 4 -1 -1 3 6\n")

    # ---- Day 12: Stacks / Queues / Monotonic Stack ----
    P(num=20, week=2, day=12, name="valid-parentheses", title="Valid Parentheses", diff="Easy", prio="P0",
      desc="""// Given a string s containing just the characters '(', ')', '{', '}', '[' and
// ']', determine if the input string is valid (brackets correctly matched and
// closed in the correct order).""",
      constraints="""//   - 1 <= s.length <= 1e4
//   - s consists of parentheses-type characters only""",
      iofmt="""//   Line 1: the string s
//   Print 1 if valid, else 0.""",
      expected="1", goal="O(n) time, O(n) space.",
      sig="bool isValid(string s)", retdefault="false",
      main=M_read_string_print_int("(sol.isValid(s) ? 1 : 0)"),
      testin="()[]{}\n")

    P(num=155, week=2, day=12, name="min-stack", title="Min Stack", diff="Medium", prio="P0",
      desc="""// Design a stack that supports push, pop, top, and retrieving the minimum
// element in O(1) time. (LC class MinStack; here modeled as Solution.)""",
      constraints="""//   - -2^31 <= val <= 2^31 - 1
//   - methods pop/top/getMin called on non-empty stack
//   - at most 3e4 calls""",
      iofmt="""//   Line 1: q (number of operations)
//   Next q lines: op [arg]
//     op = 1 x  -> push(x)
//     op = 2    -> pop()
//     op = 3    -> top()  -> print x
//     op = 4    -> getMin() -> print min""",
      expected="-3 | 0 | -2", goal="O(1) per operation.",
      sig="void push(int val)", retdefault="",
      prestruct="""#include <climits>
""",
      extra="""    // Implement these as the MinStack operations:
    void pop() { /* TODO */ }
    int top() { /* TODO */ return 0; }
    int getMin() { /* TODO */ return 0; }
""",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int q; if (!(cin >> q)) return 0;
    while (q--) {
        int op; cin >> op;
        if (op == 1) { int x; cin >> x; sol.push(x); }
        else if (op == 2) { sol.pop(); }
        else if (op == 3) { cout << sol.top() << "\\n"; }
        else { cout << sol.getMin() << "\\n"; }
    }
    return 0;
}
""",
      testin="7\n1 -2\n1 0\n1 -3\n4\n2\n3\n4\n")

    P(num=739, week=2, day=12, name="daily-temperatures", title="Daily Temperatures", diff="Medium", prio="P0",
      desc="""// Given an array of integers temperatures representing daily temperatures,
// return an array answer such that answer[i] is the number of days after the
// ith day until a warmer temperature. If none, answer[i] == 0.""",
      constraints="""//   - 1 <= temperatures.length <= 1e5
//   - 30 <= temperatures[i] <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print n space-separated integers (days to wait).""",
      expected="1 1 4 2 1 1 0 0", goal="O(n) time (monotonic stack).",
      sig="vector<int> dailyTemperatures(vector<int>& temperatures)", retdefault="{}",
      main=M_read_ints_print_vecint("sol.dailyTemperatures(a)"),
      testin="8\n73 74 75 71 69 72 76 73\n")

    P(num=84, week=2, day=12, name="largest-rectangle-in-histogram", title="Largest Rectangle in Histogram", diff="Hard", prio="P1",
      desc="""// Given an array of integers heights representing the bar heights of a
// histogram where each bar is 1 wide, return the area of the largest rectangle
// in the histogram.""",
      constraints="""//   - 1 <= heights.length <= 1e5
//   - 0 <= heights[i] <= 1e4""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the largest rectangle area.""",
      expected="10", goal="O(n) time (monotonic stack).",
      sig="int largestRectangleArea(vector<int>& heights)", retdefault="0",
      main=M_read_ints_print_int("sol.largestRectangleArea(a)"),
      testin="6\n2 1 5 6 2 3\n")

    # ---- Day 13: Backtracking Intro ----
    P(num=46, week=2, day=13, name="permutations", title="Permutations", diff="Medium", prio="P0",
      desc="""// Given an array nums of distinct integers, return all possible permutations.
// The answer may be returned in any order.""",
      constraints="""//   - 1 <= nums.length <= 6
//   - -10 <= nums[i] <= 10
//   - all integers are unique""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print each permutation on its own line, space-separated.""",
      expected="1 2 3 | 1 3 2 | 2 1 3 | 2 3 1 | 3 1 2 | 3 2 1", goal="O(n! * n) time.",
      sig="vector<vector<int>> permute(vector<int>& nums)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto res = sol.permute(a);
    for (auto& p : res) {
        for (size_t i = 0; i < p.size(); ++i) cout << p[i] << " \\n"[i + 1 == p.size()];
    }
    return 0;
}
""",
      testin="3\n1 2 3\n")

    P(num=39, week=2, day=13, name="combination-sum", title="Combination Sum", diff="Medium", prio="P0",
      desc="""// Given an array of distinct integers candidates and a target integer target,
// return a list of all unique combinations of candidates where the chosen numbers
// sum to target. Candidates may be chosen an unlimited number of times.""",
      constraints="""//   - 1 <= candidates.length <= 30
//   - 2 <= candidate[i] <= 40
//   - all candidates distinct
//   - 1 <= target <= 40""",
      iofmt="""//   Line 1: n target
//   Line 2: n space-separated candidates
//   Print each combination on its own line, space-separated.""",
      expected="2 2 3 | 7", goal="O(2^(target/min)) time.",
      sig="vector<vector<int>> combinationSum(vector<int>& candidates, int target)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target; if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto res = sol.combinationSum(a, target);
    for (auto& c : res) {
        for (size_t i = 0; i < c.size(); ++i) cout << c[i] << " \\n"[i + 1 == c.size()];
    }
    return 0;
}
""",
      testin="4 7\n2 3 6 7\n")

    P(num=78, week=2, day=13, name="subsets", title="Subsets", diff="Medium", prio="P0",
      desc="""// Given an integer array nums of unique elements, return all possible subsets
// (the power set). The solution set must not contain duplicate subsets.""",
      constraints="""//   - 1 <= nums.length <= 10
//   - -10 <= nums[i] <= 10
//   - all integers unique""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print each subset on its own line, space-separated (empty subset = blank line).""",
      expected="<empty> | 1 | 2 | 1 2 | 3 | 1 3 | 2 3 | 1 2 3", goal="O(n * 2^n).",
      sig="vector<vector<int>> subsets(vector<int>& nums)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto res = sol.subsets(a);
    for (auto& s : res) {
        for (size_t i = 0; i < s.size(); ++i) cout << s[i] << " \\n"[i + 1 == s.size()];
        if (s.empty()) cout << "\\n";
    }
    return 0;
}
""",
      testin="3\n1 2 3\n")

    P(num=22, week=2, day=13, name="generate-parentheses", title="Generate Parentheses", diff="Medium", prio="P1",
      desc="""// Given n pairs of parentheses, write a function to generate all combinations
// of well-formed parentheses.""",
      constraints="""//   - 1 <= n <= 8""",
      iofmt="""//   Line 1: n
//   Print each combination on its own line.""",
      expected="((())) | (()()) | (())() | ()(()) | ()()()", goal="O(4^n / sqrt(n)).",
      sig="vector<string> generateParenthesis(int n)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    auto res = sol.generateParenthesis(n);
    for (auto& s : res) cout << s << "\\n";
    return 0;
}
""",
      testin="3\n")

    # ---- Day 14: Week 2 Review ----
    P(num=148, week=2, day=14, name="sort-list", title="Sort List", diff="Medium", prio="P0",
      desc="""// Given the head of a linked list, sort it in ascending order using O(n log n)
// time and O(1) extra space (follow-up).""",
      constraints="""//   - number of nodes in [0, 5e4]
//   - -1e5 <= Node.val <= 1e5""",
      iofmt="""//   Line 1: n
//   Line 2: n values
//   Print the sorted list values space-separated.""",
      expected="1 2 3 4", goal="O(n log n) time, O(1) space.",
      sig="ListNode* sortList(ListNode* head)", retdefault="nullptr",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    printList(sol.sortList(buildList(a)));
    return 0;
}
""",
      testin="4\n4 2 1 3\n")

    P(num=143, week=2, day=14, name="reorder-list", title="Reorder List", diff="Medium", prio="P0",
      desc="""// Reorder the list so that it becomes L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2 -> ...
// You may not modify the values in the list's nodes; only nodes themselves may be
// changed.""",
      constraints="""//   - number of nodes in [1, 5e4]
//   - 1 <= Node.val <= 1000""",
      iofmt="""//   Line 1: n
//   Line 2: n values
//   Print the reordered list values space-separated.""",
      expected="1 4 2 3", goal="O(n) time, O(1) space.",
      sig="void reorderList(ListNode* head)", retdefault="",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    sol.reorderList(head);
    printList(head);
    return 0;
}
""",
      testin="4\n1 2 3 4\n")

    P(num=124, week=2, day=14, name="binary-tree-maximum-path-sum", title="Binary Tree Maximum Path Sum", diff="Hard", prio="P1",
      desc="""// A path in a binary tree is a sequence of nodes where each pair of adjacent
// nodes has an edge connecting them. No node appears more than once. Find the
// maximum sum of a non-empty path. It may start/end anywhere and need not pass
// through the root.""",
      constraints="""//   - number of nodes in [1, 3e4]
//   - -1000 <= Node.val <= 1000""",
      iofmt="""//   Line 1: n
//   Line 2: n level-order values (-1 = null)
//   Print the maximum path sum.""",
      expected="6", goal="O(n) time.",
      sig="int maxPathSum(TreeNode* root)", retdefault="0",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.maxPathSum(buildTreeLO(a)) << "\\n";
    return 0;
}
""",
      testin="3\n1 2 3\n")

    P(num=199, week=2, day=14, name="binary-tree-right-side-view", title="Binary Tree Right Side View", diff="Medium", prio="P1",
      desc="""// Given the root of a binary tree, imagine yourself standing on its right side,
// return the values of the nodes you can see ordered from top to bottom.""",
      constraints="""//   - number of nodes in [0, 100]
//   - -100 <= Node.val <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n level-order values (-1 = null)
//   Print the right-side-view values space-separated.""",
      expected="1 3 4", goal="O(n) time.",
      sig="vector<int> rightSideView(TreeNode* root)", retdefault="{}",
      prestruct=TREENODE + TREE_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.rightSideView(buildTreeLO(a));
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \\n"[i + 1 == ans.size()];
    return 0;
}
""",
      testin="7\n1 2 3 -1 5 -1 4\n")


def _week3(P):
    # ---- Day 15: Graph Traversal ----
    P(num=200, week=3, day=15, name="number-of-islands", title="Number of Islands", diff="Medium", prio="P0",
      desc="""// Given an m x n 2D binary grid of '1' (land) and '0' (water), return the number
// of islands. An island is formed by connecting adjacent lands horizontally or
// vertically. All four edges are surrounded by water.""",
      constraints="""//   - m == grid.length
//   - n == grid[i].length
//   - 1 <= m, n <= 300
//   - grid[i][j] is '0' or '1'""",
      iofmt="""//   Line 1: m n
//   Next m lines: each row as a string of n chars ('0'/'1')
//   Print the number of islands.""",
      expected="1", goal="O(m*n) time, O(m*n) space (DFS/BFS).",
      sig="int numIslands(vector<vector<char>>& grid)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<char>> g(m, vector<char>(n));
    for (int i = 0; i < m; ++i) {
        string row; cin >> row;
        for (int j = 0; j < n; ++j) g[i][j] = row[j];
    }
    cout << sol.numIslands(g) << "\\n";
    return 0;
}
""",
      testin="4 5\n11110\n11010\n11000\n00000\n")

    P(num=695, week=3, day=15, name="max-area-of-island", title="Max Area of Island", diff="Medium", prio="P0",
      desc="""// Given an m x n binary matrix grid of 0 (water) and 1 (land), return the
// maximum area of an island. If no island, return 0.""",
      constraints="""//   - m == grid.length, n == grid[i].length
//   - 1 <= m, n <= 50
//   - grid[i][j] is 0 or 1""",
      iofmt="""//   Line 1: m n
//   Next m lines: n space-separated integers per row
//   Print the maximum island area.""",
      expected="6", goal="O(m*n) time.",
      sig="int maxAreaOfIsland(vector<vector<int>>& grid)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<int>> g(m, vector<int>(n));
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) cin >> g[i][j];
    cout << sol.maxAreaOfIsland(g) << "\\n";
    return 0;
}
""",
      testin="8 13\n0 0 1 0 0 0 0 1 0 0 0 0 0\n0 0 0 0 0 0 0 1 1 1 0 0 0\n0 1 1 0 1 0 0 0 0 0 0 0 0\n0 1 0 0 1 1 0 0 1 0 1 0 0\n0 1 0 0 1 1 0 0 1 1 1 0 0\n0 0 0 0 0 0 0 0 0 0 1 0 0\n0 0 0 0 0 0 0 1 1 1 0 0 0\n0 0 0 0 0 0 0 1 1 0 0 0 0\n")

    P(num=133, week=3, day=15, name="clone-graph", title="Clone Graph", diff="Medium", prio="P1",
      desc="""// Given a reference of a node in a connected undirected graph, return a deep
// copy (clone) of the graph. Each node has a val and a list of neighbors.""",
      constraints="""//   - number of nodes in [0, 100]
//   - 1 <= Node.val <= 100
//   - Node.val unique, no undirected self-loops, no repeated edges
//   - graph connected""",
      iofmt="""//   Line 1: n (number of nodes, labeled 1..n) m (number of edges)
//   Next m lines: u v (edge between u and v)
//   Print the clone's adjacency list: for each node i, its neighbors space-separated,
//   one node per line.""",
      expected="2 | 1", goal="O(V+E) time.",
      sig="Node* cloneGraph(Node* node)", retdefault="nullptr",
      prestruct="""class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() { val = 0; neighbors = vector<Node*>(); }
    Node(int _val) { val = _val; neighbors = vector<Node*>(); }
    Node(int _val, vector<Node*> _neighbors) { val = _val; neighbors = _neighbors; }
};
""",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, m; if (!(cin >> n >> m)) return 0;
    vector<Node*> nodes(n + 1);
    for (int i = 1; i <= n; ++i) nodes[i] = new Node(i);
    for (int e = 0; e < m; ++e) {
        int u, v; cin >> u >> v;
        nodes[u]->neighbors.push_back(nodes[v]);
        nodes[v]->neighbors.push_back(nodes[u]);
    }
    Node* src = (n >= 1) ? nodes[1] : nullptr;
    Node* cl = sol.cloneGraph(src);
    // Print adjacency of the clone via BFS from cl (val == 1).
    if (!cl) { for (int i = 0; i < n; ++i) cout << "\\n"; return 0; }
    unordered_map<int, Node*> seen; seen[cl->val] = cl;
    queue<Node*> q; q.push(cl);
    vector<vector<int>> adj(n + 1);
    while (!q.empty()) {
        Node* c = q.front(); q.pop();
        for (Node* nb : c->neighbors) {
            adj[c->val].push_back(nb->val);
            if (!seen.count(nb->val)) { seen[nb->val] = nb; q.push(nb); }
        }
    }
    for (int i = 1; i <= n; ++i) {
        sort(adj[i].begin(), adj[i].end());
        for (size_t j = 0; j < adj[i].size(); ++j) cout << adj[i][j] << " \\n"[j + 1 == adj[i].size()];
        if (adj[i].empty()) cout << "\\n";
    }
    return 0;
}
""",
      testin="2 1\n1 2\n")

    P(num=207, week=3, day=15, name="course-schedule", title="Course Schedule", diff="Medium", prio="P0",
      desc="""// There are numCourses courses labeled 0..numCourses-1. prerequisites[i] = [a, b]
// means you must take b before a. Return true if you can finish all courses
// (no cycle in the directed graph).""",
      constraints="""//   - 1 <= numCourses <= 2000
//   - 0 <= prerequisites.length <= 5000""",
      iofmt="""//   Line 1: numCourses m
//   Next m lines: a b (must take b before a)
//   Print 1 if all courses can be finished, else 0.""",
      expected="1", goal="O(V+E) time (topological sort / DFS).",
      sig="bool canFinish(int numCourses, vector<vector<int>>& prerequisites)", retdefault="false",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int nc, m; if (!(cin >> nc >> m)) return 0;
    vector<vector<int>> pre(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> pre[i][0] >> pre[i][1];
    cout << (sol.canFinish(nc, pre) ? 1 : 0) << "\\n";
    return 0;
}
""",
      testin="2 1\n1 0\n")

    # ---- Day 16: BFS / Shortest Path ----
    P(num=994, week=3, day=16, name="rotting-oranges", title="Rotting Oranges", diff="Medium", prio="P0",
      desc="""// Given an m x n grid with 0 (empty), 1 (fresh), 2 (rotten), every minute any
// fresh orange adjacent (4-dir) to a rotten one rots. Return the minimum number
// of minutes until no fresh orange remains, or -1 if impossible.""",
      constraints="""//   - m == grid.length, n == grid[i].length
//   - 1 <= m, n <= 10
//   - grid[i][j] in {0,1,2}""",
      iofmt="""//   Line 1: m n
//   Next m lines: n space-separated integers per row
//   Print the minimum minutes, or -1.""",
      expected="4", goal="O(m*n) time (multi-source BFS).",
      sig="int orangesRotting(vector<vector<int>>& grid)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<int>> g(m, vector<int>(n));
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) cin >> g[i][j];
    cout << sol.orangesRotting(g) << "\\n";
    return 0;
}
""",
      testin="3 3\n2 1 1\n1 1 0\n0 1 1\n")

    P(num=127, week=3, day=16, name="word-ladder", title="Word Ladder", diff="Hard", prio="P0",
      desc="""// Given two words beginWord and endWord and a dictionary wordList, return the
// number of words in the shortest transformation sequence from beginWord to
// endWord where each step changes exactly one letter. Every transformed word,
// including endWord, must be in wordList; beginWord need not be. Return 0 if none.""",
      constraints="""//   - 1 <= beginWord.length <= 10
//   - endWord.length == beginWord.length
//   - 1 <= wordList.length <= 5000
//   - all words unique, lowercase letters""",
      iofmt="""//   Line 1: beginWord endWord
//   Line 2: L (number of words in wordList)
//   Line 3: L space-separated words
//   Print the length of the shortest transformation sequence (number of words).""",
      expected="5", goal="O(L^2 * N) with BFS.",
      sig="int ladderLength(string beginWord, string endWord, vector<string>& wordList)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string b, e; if (!(cin >> b >> e)) return 0;
    int L; cin >> L;
    vector<string> wl(L);
    for (int i = 0; i < L; ++i) cin >> wl[i];
    cout << sol.ladderLength(b, e, wl) << "\\n";
    return 0;
}
""",
      testin="hit cog\n6\nhot dot dog lot log cog\n")

    P(num=542, week=3, day=16, name="01-matrix", title="01 Matrix", diff="Medium", prio="P1",
      desc="""// Given an m x n binary matrix, return the distance of the nearest 0 for each
// cell (distance = number of steps, 4-directional).""",
      constraints="""//   - m == mat.length, n == mat[i].length
//   - 1 <= m, n <= 1e4
//   - 1 <= m*n <= 1e4
//   - mat[i][j] is 0 or 1, and mat contains at least one 0""",
      iofmt="""//   Line 1: m n
//   Next m lines: n space-separated integers per row
//   Print the distance matrix, one row per line space-separated.""",
      expected="0 0 0 | 0 1 0 | 1 2 1", goal="O(m*n) time (multi-source BFS).",
      sig="vector<vector<int>> updateMatrix(vector<vector<int>>& mat)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<int>> g(m, vector<int>(n));
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) cin >> g[i][j];
    auto res = sol.updateMatrix(g);
    for (auto& r : res) for (size_t j = 0; j < r.size(); ++j) cout << r[j] << " \\n"[j + 1 == r.size()];
    return 0;
}
""",
      testin="3 3\n0 0 0\n0 1 0\n1 1 1\n")

    P(num=417, week=3, day=16, name="pacific-atlantic-water-flow", title="Pacific Atlantic Water Flow", diff="Medium", prio="P1",
      desc="""// Given an m x n matrix heights where heights[r][c] is the height above sea
// level, water can flow from a cell to a neighbor with height <= current. The
// Pacific touches the left/top edges, the Atlantic the right/bottom. Return a
// list of grid coordinates from which water can flow to both oceans.""",
      constraints="""//   - m == heights.length, n == heights[i].length
//   - 1 <= m, n <= 200
//   - 0 <= heights[r][c] <= 1e5""",
      iofmt="""//   Line 1: m n
//   Next m lines: n space-separated integers per row
//   Print each coordinate "r c" on its own line, sorted.""",
      expected="0 4 | 1 3 | 1 4 | 2 2 | 3 0 | 3 1 | 4 0", goal="O(m*n) time.",
      sig="vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<int>> g(m, vector<int>(n));
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) cin >> g[i][j];
    auto res = sol.pacificAtlantic(g);
    sort(res.begin(), res.end());
    for (auto& c : res) cout << c[0] << " " << c[1] << "\\n";
    return 0;
}
""",
      testin="5 5\n1 2 2 3 5\n3 2 3 4 4\n2 4 5 3 1\n6 7 1 4 5\n5 1 1 2 4\n")

    # ---- Day 17: Union-Find ----
    P(num=547, week=3, day=17, name="number-of-provinces", title="Number of Provinces", diff="Medium", prio="P0",
      desc="""// Given an n x n matrix isConnected where isConnected[i][j] = 1 if city i and j
// are directly connected, a province is a group of directly/indirectly connected
// cities. Return the total number of provinces.""",
      constraints="""//   - 1 <= n <= 200
//   - n == isConnected.length == isConnected[i].length
//   - isConnected[i][j] in {0,1}, symmetric, isConnected[i][i]==1""",
      iofmt="""//   Line 1: n
//   Next n lines: n space-separated integers per row
//   Print the number of provinces.""",
      expected="2", goal="O(n^2) time, O(n) space (union-find).",
      sig="int findCircleNum(vector<vector<int>>& isConnected)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<vector<int>> g(n, vector<int>(n));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) cin >> g[i][j];
    cout << sol.findCircleNum(g) << "\\n";
    return 0;
}
""",
      testin="3\n1 1 0\n1 1 0\n0 0 1\n")

    P(num=684, week=3, day=17, name="redundant-connection", title="Redundant Connection", diff="Medium", prio="P0",
      desc="""// A tree of n nodes labeled 1..n has n-1 edges; one extra edge is added creating
// a cycle. Return the edge that can be removed so the result is a valid tree. If
// multiple answers, return the last one in the input.""",
      constraints="""//   - number of nodes n in [3, 1000]
//   - edges.length == n
//   - edges[i].length == 2
//   - 1 <= ai < bi <= ai, all pairs unique""",
      iofmt="""//   Line 1: m (number of edges == n)
//   Next m lines: u v
//   Print the redundant edge "u v".""",
      expected="1 4", goal="O(n * alpha(n)) union-find.",
      sig="vector<int> findRedundantConnection(vector<vector<int>>& edges)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m; if (!(cin >> m)) return 0;
    vector<vector<int>> e(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> e[i][0] >> e[i][1];
    auto ans = sol.findRedundantConnection(e);
    cout << ans[0] << " " << ans[1] << "\\n";
    return 0;
}
""",
      testin="5\n1 2\n2 3\n3 4\n1 4\n1 5\n")

    P(num=990, week=3, day=17, name="satisfiability-of-equality-equations", title="Satisfiability of Equality Equations", diff="Medium", prio="P1",
      desc="""// Given an array of equations strings of form "a==b" or "a!=b" with single
// lowercase letters, return true if it is possible to assign integers to the
// variables so all equations are satisfied.""",
      constraints="""//   - 1 <= equations.length <= 500
//   - equations[i].length == 4
//   - equations[i][1] in {'=','!'}; equations[i][0],[3] lowercase letters""",
      iofmt="""//   Line 1: n (number of equations)
//   Next n lines: one equation string each
//   Print 1 if satisfiable, else 0.""",
      expected="1", goal="O(n) union-find.",
      sig="bool equationsPossible(vector<string>& equations)", retdefault="false",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<string> eq(n);
    for (int i = 0; i < n; ++i) cin >> eq[i];
    cout << (sol.equationsPossible(eq) ? 1 : 0) << "\\n";
    return 0;
}
""",
      testin="3\na==b\nb==c\na==c\n")

    P(num=685, week=3, day=17, name="redundant-connection-ii", title="Redundant Connection II", diff="Hard", prio="P1",
      desc="""// A rooted tree of n nodes labeled 1..n has one new directed edge added.
// The result may contain a cycle, a node with two parents, or both. Return the
// edge that can be removed to make a valid rooted tree; if multiple, return the
// last one in the input.""",
      constraints="""//   - 3 <= n <= 1000
//   - edges.length == n
//   - edges[i].length == 2, 1 <= u,v <= n, u != v
//   - edges[i] = [u, v] meaning u is parent of v""",
      iofmt="""//   Line 1: m (number of edges == n)
//   Next m lines: u v (u is parent of v)
//   Print the redundant edge "u v".""",
      expected="2 3", goal="O(n) union-find.",
      sig="vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m; if (!(cin >> m)) return 0;
    vector<vector<int>> e(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> e[i][0] >> e[i][1];
    auto ans = sol.findRedundantDirectedConnection(e);
    cout << ans[0] << " " << ans[1] << "\\n";
    return 0;
}
""",
      testin="4\n1 2\n1 3\n2 4\n2 3\n")

    # ---- Day 18: DP Intro (1D / Linear) ----
    P(num=70, week=3, day=18, name="climbing-stairs", title="Climbing Stairs", diff="Easy", prio="P0",
      desc="""// You are climbing a staircase taking 1 or 2 steps at a time. Return the number
// of distinct ways to reach the top (step n).""",
      constraints="""//   - 1 <= n <= 45""",
      iofmt="""//   Line 1: n
//   Print the number of ways.""",
      expected="3", goal="O(n) time, O(1) space.",
      sig="int climbStairs(int n)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    cout << sol.climbStairs(n) << "\\n";
    return 0;
}
""",
      testin="3\n")

    P(num=198, week=3, day=18, name="house-robber", title="House Robber", diff="Medium", prio="P0",
      desc="""// You are a robber along a street where nums[i] is the money in house i. You
// cannot rob adjacent houses. Return the maximum amount you can rob.""",
      constraints="""//   - 1 <= nums.length <= 100
//   - 0 <= nums[i] <= 400""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum amount.""",
      expected="4", goal="O(n) time, O(1) space.",
      sig="int rob(vector<int>& nums)", retdefault="0",
      main=M_read_ints_print_int("sol.rob(a)"),
      testin="4\n1 2 3 1\n")

    P(num=213, week=3, day=18, name="house-robber-ii", title="House Robber II", diff="Medium", prio="P0",
      desc="""// Same as House Robber but the houses are arranged in a circle (first and last
// are adjacent). Return the maximum amount you can rob without alerting police.""",
      constraints="""//   - 1 <= nums.length <= 100
//   - 0 <= nums[i] <= 1000""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum amount.""",
      expected="3", goal="O(n) time, O(1) space.",
      sig="int rob(vector<int>& nums)", retdefault="0",
      main=M_read_ints_print_int("sol.rob(a)"),
      testin="3\n2 3 2\n")

    P(num=746, week=3, day=18, name="min-cost-climbing-stairs", title="Min Cost Climbing Stairs", diff="Easy", prio="P1",
      desc="""// Given an array cost where cost[i] is the cost of step i, you can start at
// index 0 or 1, and each step you can climb 1 or 2 steps. Return the minimum cost
// to reach the top (past the last index).""",
      constraints="""//   - 2 <= cost.length <= 1000
//   - 0 <= cost[i] <= 999""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the minimum cost.""",
      expected="15", goal="O(n) time, O(1) space.",
      sig="int minCostClimbingStairs(vector<int>& cost)", retdefault="0",
      main=M_read_ints_print_int("sol.minCostClimbingStairs(a)"),
      testin="3\n10 15 20\n")

    # ---- Day 19: DP Paths / Subsequences ----
    P(num=62, week=3, day=19, name="unique-paths", title="Unique Paths", diff="Medium", prio="P0",
      desc="""// A robot at the top-left corner of an m x n grid moves only right or down.
// Return the number of possible unique paths to the bottom-right corner.""",
      constraints="""//   - 1 <= m, n <= 100
//   - answer fits in a 32-bit int (test inputs guaranteed)""",
      iofmt="""//   Line 1: m n
//   Print the number of unique paths.""",
      expected="28", goal="O(m*n) time, O(n) space.",
      sig="int uniquePaths(int m, int n)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    cout << sol.uniquePaths(m, n) << "\\n";
    return 0;
}
""",
      testin="3 7\n")

    P(num=64, week=3, day=19, name="minimum-path-sum", title="Minimum Path Sum", diff="Medium", prio="P0",
      desc="""// Given an m x n grid of non-negative numbers, find a path from top-left to
// bottom-right which minimizes the sum of numbers along the path (move only right
// or down). Return the minimum sum.""",
      constraints="""//   - m == grid.length, n == grid[i].length
//   - 1 <= m, n <= 200
//   - 0 <= grid[i][j] <= 200""",
      iofmt="""//   Line 1: m n
//   Next m lines: n space-separated integers per row
//   Print the minimum path sum.""",
      expected="7", goal="O(m*n) time, O(n) space.",
      sig="int minPathSum(vector<vector<int>>& grid)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<int>> g(m, vector<int>(n));
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) cin >> g[i][j];
    cout << sol.minPathSum(g) << "\\n";
    return 0;
}
""",
      testin="3 3\n1 3 1\n1 5 1\n4 2 1\n")

    P(num=300, week=3, day=19, name="longest-increasing-subsequence", title="Longest Increasing Subsequence", diff="Medium", prio="P0",
      desc="""// Given an integer array nums, return the length of the longest strictly
// increasing subsequence.""",
      constraints="""//   - 1 <= nums.length <= 2500
//   - -1e4 <= nums[i] <= 1e4""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the LIS length.""",
      expected="4", goal="O(n log n) time, O(n) space.",
      sig="int lengthOfLIS(vector<int>& nums)", retdefault="0",
      main=M_read_ints_print_int("sol.lengthOfLIS(a)"),
      testin="8\n10 9 2 5 3 7 101 18\n")

    P(num=1143, week=3, day=19, name="longest-common-subsequence", title="Longest Common Subsequence", diff="Medium", prio="P0",
      desc="""// Given two strings text1 and text2, return the length of their longest common
// subsequence. If no common subsequence, return 0.""",
      constraints="""//   - 1 <= text1.length, text2.length <= 1000
//   - consist of lowercase English letters""",
      iofmt="""//   Line 1: text1
//   Line 2: text2
//   Print the LCS length.""",
      expected="3", goal="O(m*n) time, O(min(m,n)) space.",
      sig="int longestCommonSubsequence(string text1, string text2)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string a, b; if (!(cin >> a >> b)) return 0;
    cout << sol.longestCommonSubsequence(a, b) << "\\n";
    return 0;
}
""",
      testin="abcde\nace\n")

    # ---- Day 20: DP Knapsack / Interval ----
    P(num=416, week=3, day=20, name="partition-equal-subset-sum", title="Partition Equal Subset Sum", diff="Medium", prio="P0",
      desc="""// Given a non-empty array nums of positive integers, determine if the array can
// be partitioned into two subsets such that the sum of elements in both subsets is
// equal.""",
      constraints="""//   - 1 <= nums.length <= 200
//   - 1 <= nums[i] <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print 1 if partitionable into equal sums, else 0.""",
      expected="1", goal="O(n*sum) time, O(sum) space (0/1 knapsack).",
      sig="bool canPartition(vector<int>& nums)", retdefault="false",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << (sol.canPartition(a) ? 1 : 0) << "\\n";
    return 0;
}
""",
      testin="4\n1 5 11 5\n")

    P(num=494, week=3, day=20, name="target-sum", title="Target Sum", diff="Medium", prio="P0",
      desc="""// Given an array nums and a target, assign + or - to each element and concatenate
// them. Return the number of different expressions that evaluate to target.""",
      constraints="""//   - 1 <= nums.length <= 20
//   - 0 <= nums[i] <= 1000
//   - 0 <= sum(nums) <= 1000
//   - -1000 <= target <= 1000""",
      iofmt="""//   Line 1: n target
//   Line 2: n space-separated integers
//   Print the number of expressions evaluating to target.""",
      expected="5", goal="O(n*sum) time (0/1 knapsack).",
      sig="int findTargetSumWays(vector<int>& nums, int target)", retdefault="0",
      main=M_read_int_target_print_int("sol.findTargetSumWays(a, k)"),
      testin="5 3\n1 1 1 1 1\n")

    P(num=322, week=3, day=20, name="coin-change", title="Coin Change", diff="Medium", prio="P0",
      desc="""// Given an integer array coins representing denominations and an integer amount,
// return the fewest number of coins needed to make up that amount. If impossible,
// return -1. You may use each coin unlimited times.""",
      constraints="""//   - 1 <= coins.length <= 12
//   - 1 <= coins[i] <= 2^31 - 1
//   - 0 <= amount <= 1e4""",
      iofmt="""//   Line 1: n amount
//   Line 2: n space-separated coin denominations
//   Print the fewest number of coins, or -1.""",
      expected="3", goal="O(n*amount) time (complete knapsack).",
      sig="int coinChange(vector<int>& coins, int amount)", retdefault="-1",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, amount; if (!(cin >> n >> amount)) return 0;
    vector<int> c(n);
    for (int i = 0; i < n; ++i) cin >> c[i];
    cout << sol.coinChange(c, amount) << "\\n";
    return 0;
}
""",
      testin="3 11\n1 2 5\n")

    P(num=312, week=3, day=20, name="burst-balloons", title="Burst Balloons", diff="Hard", prio="P1",
      desc="""// Given n balloons indexed 0..n-1 with nums[i] painted on each, bursting balloon i
// gives nums[left]*nums[i]*nums[right] coins (left/right are adjacent at burst
// time). Return the maximum coins obtainable by bursting all balloons.""",
      constraints="""//   - n == nums.length
//   - 1 <= n <= 300
//   - 0 <= nums[i] <= 100""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum coins.""",
      expected="167", goal="O(n^3) time (interval DP).",
      sig="int maxCoins(vector<int>& nums)", retdefault="0",
      main=M_read_ints_print_int("sol.maxCoins(a)"),
      testin="4\n3 1 5 8\n")

    # ---- Day 21: Week 3 Review ----
    P(num=210, week=3, day=21, name="course-schedule-ii", title="Course Schedule II", diff="Medium", prio="P0",
      desc="""// There are numCourses courses labeled 0..numCourses-1. Each pair [a,b]
// means b must be taken before a. Return any valid ordering that finishes every
// course, or an empty array if the prerequisite graph contains a cycle.""",
      constraints="""//   - 1 <= numCourses <= 2000
//   - 0 <= prerequisites.length <= numCourses * (numCourses - 1)
//   - each pair has two distinct valid course IDs; all pairs are distinct""",
      iofmt="""//   Line 1: numCourses m
//   Next m lines: a b (b before a)
//   Print the ordering space-separated, or -1 if impossible.""",
      expected="0 1", goal="O(V+E) time (topological sort).",
      sig="vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int nc, m; if (!(cin >> nc >> m)) return 0;
    vector<vector<int>> pre(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> pre[i][0] >> pre[i][1];
    auto ans = sol.findOrder(nc, pre);
    if (ans.empty()) cout << -1 << "\\n";
    else for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \\n"[i + 1 == ans.size()];
    return 0;
}
""",
      testin="2 1\n1 0\n")

    P(num=79, week=3, day=21, name="word-search", title="Word Search", diff="Medium", prio="P0",
      desc="""// Given an m x n grid of characters and a string word, return true if word exists
// in the grid (constructed from adjacent cells, each cell used once).""",
      constraints="""//   - m == board.length, n == board[i].length
//   - 1 <= m, n <= 6
//   - 1 <= word.length <= 15
//   - board and word consist of lowercase and uppercase English letters""",
      iofmt="""//   Line 1: m n
//   Next m lines: each row as a string of n chars
//   Line: the word
//   Print 1 if the word exists, else 0.""",
      expected="1", goal="O(m*n*4^L) time (backtracking).",
      sig="bool exist(vector<vector<char>>& board, string word)", retdefault="false",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<char>> b(m, vector<char>(n));
    for (int i = 0; i < m; ++i) { string row; cin >> row; for (int j = 0; j < n; ++j) b[i][j] = row[j]; }
    string word; cin >> word;
    cout << (sol.exist(b, word) ? 1 : 0) << "\\n";
    return 0;
}
""",
      testin="3 4\nABCE\nSFCS\nADEE\nABCCED\n")

    P(num=139, week=3, day=21, name="word-break", title="Word Break", diff="Medium", prio="P0",
      desc="""// Given a string s and a dictionary of strings wordDict, return true if s can be
// segmented into a space-separated sequence of one or more dictionary words.
// The same dictionary word may be reused multiple times in the segmentation.""",
      constraints="""//   - 1 <= s.length <= 300
//   - 1 <= wordDict.length <= 1000
//   - 1 <= wordDict[i].length <= 20
//   - s and words consist of lowercase English letters""",
      iofmt="""//   Line 1: s
//   Line 2: L (number of words)
//   Line 3: L space-separated words
//   Print 1 if s can be segmented, else 0.""",
      expected="1", goal="O(n^2) or O(n*L) time (DP).",
      sig="bool wordBreak(string s, vector<string>& wordDict)", retdefault="false",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s; if (!(cin >> s)) return 0;
    int L; cin >> L;
    vector<string> w(L);
    for (int i = 0; i < L; ++i) cin >> w[i];
    cout << (sol.wordBreak(s, w) ? 1 : 0) << "\\n";
    return 0;
}
""",
      testin="leetcode\n2\nleet code\n")

    P(num=152, week=3, day=21, name="maximum-product-subarray", title="Maximum Product Subarray", diff="Medium", prio="P1",
      desc="""// Given an integer array nums, find a contiguous non-empty subarray with the
// largest product and return the product.""",
      constraints="""//   - 1 <= nums.length <= 2e4
//   - -10 <= nums[i] <= 10
//   - the product of every subarray fits in a 32-bit int""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum product.""",
      expected="6", goal="O(n) time, O(1) space.",
      sig="int maxProduct(vector<int>& nums)", retdefault="0",
      main=M_read_ints_print_int("sol.maxProduct(a)"),
      testin="4\n2 3 -2 4\n")


def _week4(P):
    # ---- Day 22: Binary Search ----
    P(num=704, week=4, day=22, name="binary-search", title="Binary Search", diff="Easy", prio="P0",
      desc="""// Given an array of integers nums sorted in ascending order and an integer target,
// write a function to search target in nums. Return its index, or -1 if not present.
// O(log n) runtime.""",
      constraints="""//   - 1 <= nums.length <= 1e4
//   - -1e4 < nums[i], target < 1e4
//   - all integers unique, nums sorted ascending""",
      iofmt="""//   Line 1: n target
//   Line 2: n space-separated integers
//   Print the index, or -1.""",
      expected="4", goal="O(log n) time, O(1) space.",
      sig="int search(vector<int>& nums, int target)", retdefault="-1",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target; if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.search(a, target) << "\\n";
    return 0;
}
""",
      testin="6 9\n-1 0 3 5 9 12\n")

    P(num=33, week=4, day=22, name="search-in-rotated-sorted-array", title="Search in Rotated Sorted Array", diff="Medium", prio="P0",
      desc="""// Given a rotated sorted array nums of distinct values and a target, return the
// index of target, or -1. O(log n) runtime.""",
      constraints="""//   - 1 <= nums.length <= 5000
//   - -1e4 <= nums[i] <= 1e4
//   - all values unique, nums rotated at some pivot""",
      iofmt="""//   Line 1: n target
//   Line 2: n space-separated integers
//   Print the index, or -1.""",
      expected="4", goal="O(log n) time.",
      sig="int search(vector<int>& nums, int target)", retdefault="-1",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target; if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.search(a, target) << "\\n";
    return 0;
}
""",
      testin="7 0\n4 5 6 7 0 1 2\n")

    P(num=34, week=4, day=22, name="find-first-and-last-position-of-element-in-sorted-array", title="Find First and Last Position of Element in Sorted Array", diff="Medium", prio="P0",
      desc="""// Given a sorted array nums (non-decreasing) and a target, find the starting
// and ending positions of target. Return [-1, -1] if not found. O(log n).""",
      constraints="""//   - 0 <= nums.length <= 1e5
//   - -1e9 <= nums[i] <= 1e9
//   - nums sorted ascending""",
      iofmt="""//   Line 1: n target
//   Line 2: n space-separated integers
//   Print "left right" (the bounds), or "-1 -1".""",
      expected="3 4", goal="O(log n) time (two binary searches).",
      sig="vector<int> searchRange(vector<int>& nums, int target)", retdefault="{-1, -1}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target; if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.searchRange(a, target);
    cout << ans[0] << " " << ans[1] << "\\n";
    return 0;
}
""",
      testin="6 8\n5 7 7 8 8 10\n")

    P(num=153, week=4, day=22, name="find-minimum-in-rotated-sorted-array", title="Find Minimum in Rotated Sorted Array", diff="Medium", prio="P1",
      desc="""// Given a rotated sorted array of distinct values, return the minimum element.
// O(log n) runtime.""",
      constraints="""//   - n == nums.length
//   - 1 <= n <= 5000
//   - -5000 <= nums[i] <= 5000
//   - all values unique""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the minimum.""",
      expected="1", goal="O(log n) time.",
      sig="int findMin(vector<int>& nums)", retdefault="0",
      main=M_read_ints_print_int("sol.findMin(a)"),
      testin="5\n3 4 5 1 2\n")

    # ---- Day 23: Binary Search on Answer ----
    P(num=410, week=4, day=23, name="split-array-largest-sum", title="Split Array Largest Sum", diff="Hard", prio="P0",
      desc="""// Given an array nums of non-negative integers and an integer k, split nums into
// k non-empty contiguous subarrays such that the largest sum among the subarrays is
// minimized. Return the minimized largest sum.""",
      constraints="""//   - 1 <= nums.length <= 1000
//   - 0 <= nums[i] <= 1e6
//   - 1 <= k <= min(50, nums.length)""",
      iofmt="""//   Line 1: n k
//   Line 2: n space-separated integers
//   Print the minimized largest sum.""",
      expected="18", goal="O(n log S) where S = sum(nums).",
      sig="int splitArray(vector<int>& nums, int k)", retdefault="0",
      main=M_read_int_target_print_int("sol.splitArray(a, k)"),
      testin="7 2\n7 2 5 10 8\n")

    P(num=875, week=4, day=23, name="koko-eating-bananas", title="Koko Eating Bananas", diff="Medium", prio="P0",
      desc="""// Koko loves to eat bananas. piles[i] is the bananas in pile i, h is the hours.
// Koko eats at speed k bananas/hour: each hour she picks one pile and eats up to k
// from it. Return the minimum integer k such that she can eat all piles within h
// hours.""",
      constraints="""//   - 1 <= piles.length <= 1e4
//   - piles.length <= h <= 1e9
//   - 1 <= piles[i] <= 1e9""",
      iofmt="""//   Line 1: n h
//   Line 2: n space-separated integers
//   Print the minimum eating speed.""",
      expected="4", goal="O(n log max(piles)).",
      sig="int minEatingSpeed(vector<int>& piles, int h)", retdefault="0",
      main=M_read_int_target_print_int("sol.minEatingSpeed(a, k)"),
      testin="4 8\n3 6 7 11\n")

    P(num=1011, week=4, day=23, name="capacity-to-ship-packages-within-d-days", title="Capacity To Ship Packages Within D Days", diff="Medium", prio="P0",
      desc="""// Packages weights[i] must be shipped in the given order within `days` days.
// Each day's total load may not exceed the fixed ship capacity. Return the least
// capacity that ships every package within the deadline.""",
      constraints="""//   - 1 <= days <= weights.length <= 5e4
//   - 1 <= weights[i] <= 500
//   - package order cannot be changed""",
      iofmt="""//   Line 1: n D
//   Line 2: n space-separated integers
//   Print the least ship capacity.""",
      expected="15", goal="O(n log S).",
      sig="int shipWithinDays(vector<int>& weights, int days)", retdefault="0",
      main=M_read_int_target_print_int("sol.shipWithinDays(a, k)"),
      testin="10 5\n1 2 3 4 5 6 7 8 9 10\n")

    P(num=4, week=4, day=23, name="median-of-two-sorted-arrays", title="Median of Two Sorted Arrays", diff="Hard", prio="P1",
      desc="""// Given two sorted arrays nums1 and nums2 of sizes m and n, return the median of
// the two sorted arrays. The overall runtime must be O(log(m+n)).""",
      constraints="""//   - 0 <= m, n <= 1000
//   - 1 <= m + n
//   - -1e6 <= nums1[i], nums2[i] <= 1e6""",
      iofmt="""//   Line 1: m n
//   Line 2: m space-separated integers (nums1)
//   Line 3: n space-separated integers (nums2)
//   Print the median (as a float, e.g. 2.0 or 2.5).""",
      expected="2.5", goal="O(log(min(m,n))) time.",
      sig="double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2)", retdefault="0.0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<int> a(m), b(n);
    for (int i = 0; i < m; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) cin >> b[i];
    cout << sol.findMedianSortedArrays(a, b) << "\\n";
    return 0;
}
""",
      testin="2 2\n1 3\n2 4\n")

    # ---- Day 24: Greedy ----
    P(num=455, week=4, day=24, name="assign-cookies", title="Assign Cookies", diff="Easy", prio="P0",
      desc="""// Assume you are an awesome parent giving cookies to children. Each child i has a
// greed factor g[i]; each cookie j has size s[j]. A child is content if s[j] >= g[i].
// Maximize the number of content children; each child gets at most one cookie
// and each cookie can be assigned at most once.""",
      constraints="""//   - 1 <= g.length <= 3e4
//   - 0 <= s.length <= 3e4
//   - 1 <= g[i], s[j] <= 2^31 - 1""",
      iofmt="""//   Line 1: ng ns
//   Line 2: ng space-separated integers (g)
//   Line 3: ns space-separated integers (s)
//   Print the number of content children.""",
      expected="1", goal="O(g log g + s log s) time.",
      sig="int findContentChildren(vector<int>& g, vector<int>& s)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int ng, ns; if (!(cin >> ng >> ns)) return 0;
    vector<int> g(ng), s(ns);
    for (int i = 0; i < ng; ++i) cin >> g[i];
    for (int i = 0; i < ns; ++i) cin >> s[i];
    cout << sol.findContentChildren(g, s) << "\\n";
    return 0;
}
""",
      testin="3 2\n1 2 3\n1 1\n")

    P(num=55, week=4, day=24, name="jump-game", title="Jump Game", diff="Medium", prio="P0",
      desc="""// You are at index 0 of nums; nums[i] is the maximum jump length from i. Return
// true if you can reach the last index.""",
      constraints="""//   - 1 <= nums.length <= 1e4
//   - 0 <= nums[i] <= 1e5""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print 1 if reachable, else 0.""",
      expected="1", goal="O(n) time, O(1) space (greedy).",
      sig="bool canJump(vector<int>& nums)", retdefault="false",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << (sol.canJump(a) ? 1 : 0) << "\\n";
    return 0;
}
""",
      testin="5\n2 3 1 1 4\n")

    P(num=45, week=4, day=24, name="jump-game-ii", title="Jump Game II", diff="Medium", prio="P0",
      desc="""// Like Jump Game, but return the minimum number of jumps to reach the last index.
// It is guaranteed reachable.""",
      constraints="""//   - 1 <= nums.length <= 1e4
//   - 0 <= nums[i] <= 1000
//   - reachable to last index""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the minimum number of jumps.""",
      expected="2", goal="O(n) time, O(1) space (greedy).",
      sig="int jump(vector<int>& nums)", retdefault="0",
      main=M_read_ints_print_int("sol.jump(a)"),
      testin="5\n2 3 1 1 4\n")

    P(num=134, week=4, day=24, name="gas-station", title="Gas Station", diff="Medium", prio="P1",
      desc="""// There are n gas stations along a circular route. gas[i] is gas at station i;
// cost[i] is gas to travel from i to i+1. Return the starting gas station index if
// you can travel around once, or -1. If a solution exists, it is unique.""",
      constraints="""//   - n == gas.length == cost.length
//   - 1 <= n <= 1e5
//   - 0 <= gas[i], cost[i] <= 1e4""",
      iofmt="""//   Line 1: n
//   Line 2: n gas values
//   Line 3: n cost values
//   Print the starting index, or -1.""",
      expected="3", goal="O(n) time, O(1) space (greedy).",
      sig="int canCompleteCircuit(vector<int>& gas, vector<int>& cost)", retdefault="-1",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> gas(n), cost(n);
    for (int i = 0; i < n; ++i) cin >> gas[i];
    for (int i = 0; i < n; ++i) cin >> cost[i];
    cout << sol.canCompleteCircuit(gas, cost) << "\\n";
    return 0;
}
""",
      testin="5\n1 2 3 4 5\n3 4 5 1 2\n")

    # ---- Day 25: Heap / TopK ----
    P(num=215, week=4, day=25, name="kth-largest-element-in-an-array", title="Kth Largest Element in an Array", diff="Medium", prio="P0",
      desc="""// Given an integer array nums and an integer k, return the kth largest element in
// the array (not the kth distinct element).""",
      constraints="""//   - 1 <= k <= nums.length <= 1e5
//   - -1e4 <= nums[i] <= 1e4""",
      iofmt="""//   Line 1: n k
//   Line 2: n space-separated integers
//   Print the kth largest element.""",
      expected="5", goal="O(n) avg (quickselect) or O(n log k) (heap).",
      sig="int findKthLargest(vector<int>& nums, int k)", retdefault="0",
      main=M_read_int_target_print_int("sol.findKthLargest(a, k)"),
      testin="6 2\n3 2 1 5 6 4\n")

    P(num=347, week=4, day=25, name="top-k-frequent-elements", title="Top K Frequent Elements", diff="Medium", prio="P0",
      desc="""// Given an integer array nums and an integer k, return the k most frequent elements.
// The answer may be returned in any order.""",
      constraints="""//   - 1 <= nums.length <= 1e5
//   - -1e4 <= nums[i] <= 1e4
//   - k is in [1, number of unique elements]
//   - the set of k most frequent elements is unique""",
      iofmt="""//   Line 1: n k
//   Line 2: n space-separated integers
//   Print the k most frequent elements space-separated (any order).""",
      expected="1 2", goal="O(n log k) time.",
      sig="vector<int> topKFrequent(vector<int>& nums, int k)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.topKFrequent(a, k);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \\n"[i + 1 == ans.size()];
    return 0;
}
""",
      testin="6 2\n1 1 1 2 2 3\n")

    P(num=295, week=4, day=25, name="find-median-from-data-stream", title="Find Median from Data Stream", diff="Hard", prio="P0",
      desc="""// Implement a class that supports addNum(int num) and findMedian() in O(log n)
// and O(1) respectively. (LC class MedianFinder; here modeled as Solution.)""",
      constraints="""//   - -1e5 <= num <= 1e5
//   - at most 5e4 calls
//   - findMedian called on non-empty data structure""",
      iofmt="""//   Line 1: q (number of operations)
//   Next q lines: op [arg]
//     op = 1 x  -> addNum(x)
//     op = 2    -> findMedian() -> print median (as float)""",
      expected="1.5 | 2 | 2.5", goal="O(log n) per addNum (two heaps).",
      sig="void addNum(int num)", retdefault="",
      prestruct="#include <functional>\n",
      extra="""    // Implement these as the MedianFinder operations:
    double findMedian() { /* TODO */ return 0.0; }
""",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int q; if (!(cin >> q)) return 0;
    while (q--) {
        int op; cin >> op;
        if (op == 1) { int x; cin >> x; sol.addNum(x); }
        else cout << sol.findMedian() << "\\n";
    }
    return 0;
}
""",
      testin="7\n1 1\n1 2\n2\n1 3\n2\n1 4\n2\n")

    P(num=23, week=4, day=25, name="merge-k-sorted-lists", title="Merge k Sorted Lists", diff="Hard", prio="P1",
      desc="""// Given an array of k linked lists, each sorted in ascending order, merge them
// into one sorted linked list and return its head.""",
      constraints="""//   - k == lists.length
//   - 0 <= k <= 1e4
//   - 0 <= lists[i].length <= 500
//   - -1e4 <= Node.val <= 1e4
//   - total number of nodes across all lists <= 1e4""",
      iofmt="""//   Line 1: k
//   Then k blocks: each block is a line with li (length) then a line with li values
//   Print the merged list values space-separated.""",
      expected="1 1 2 3 4 4 5 6", goal="O(N log k) time (min-heap).",
      sig="ListNode* mergeKLists(vector<ListNode*>& lists)", retdefault="nullptr",
      prestruct=LISTNODE + LIST_HELPERS,
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int k; if (!(cin >> k)) return 0;
    vector<ListNode*> lists;
    for (int i = 0; i < k; ++i) {
        int li; cin >> li;
        vector<int> v(li);
        for (int j = 0; j < li; ++j) cin >> v[j];
        lists.push_back(buildList(v));
    }
    printList(sol.mergeKLists(lists));
    return 0;
}
""",
      testin="3\n3\n1 4 5\n3\n1 3 4\n2\n2 6\n")

    # ---- Day 26: Comprehensive / Fill Gaps ----
    P(num=253, week=4, day=26, name="meeting-rooms-ii", title="Meeting Rooms II", diff="Medium", prio="P0",
      desc="""// Given an array of meeting time intervals [start, end), find the minimum number
// of conference rooms required.""",
      constraints="""//   - 0 <= intervals.length <= 1e4
//   - intervals[i].length == 2
//   - 0 <= start < end <= 1e6""",
      iofmt="""//   Line 1: m (number of intervals)
//   Next m lines: start end
//   Print the minimum number of rooms.""",
      expected="2", goal="O(m log m) time (sweep line / heap).",
      sig="int minMeetingRooms(vector<vector<int>>& intervals)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m; if (!(cin >> m)) return 0;
    vector<vector<int>> iv(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> iv[i][0] >> iv[i][1];
    cout << sol.minMeetingRooms(iv) << "\\n";
    return 0;
}
""",
      testin="3\n0 30\n5 10\n15 20\n")

    P(num=42, week=4, day=26, name="trapping-rain-water", title="Trapping Rain Water", diff="Hard", prio="P0",
      desc="""// Given n non-negative integers representing an elevation map where the width of
// each bar is 1, compute how much water it can trap after raining.""",
      constraints="""//   - n == height.length
//   - 1 <= n <= 2e4
//   - 0 <= height[i] <= 1e5""",
      iofmt="""//   Line 1: n
//   Line 2: n space-separated integers
//   Print the total units of trapped water.""",
      expected="6", goal="O(n) time, O(1) space (two pointers).",
      sig="int trap(vector<int>& height)", retdefault="0",
      main=M_read_ints_print_int("sol.trap(a)"),
      testin="12\n0 1 0 2 1 0 1 3 2 1 2 1\n")

    P(num=406, week=4, day=26, name="queue-reconstruction-by-height", title="Queue Reconstruction by Height", diff="Medium", prio="P1",
      desc="""// Given people[i] = [h, k] meaning the person has height h and exactly k people
// in front who are >= height h. Reconstruct and return the queue.""",
      constraints="""//   - 1 <= people.length <= 2000
//   - 0 <= h <= 1e6
//   - 0 <= k < people.length""",
      iofmt="""//   Line 1: n
//   Next n lines: h k
//   Print the reconstructed queue, one "h k" per line.""",
      expected="5 0 | 7 0 | 5 2 | 6 1 | 4 4 | 7 1", goal="O(n^2) or O(n log n).",
      sig="vector<vector<int>> reconstructQueue(vector<vector<int>>& people)", retdefault="{}",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<vector<int>> p(n, vector<int>(2));
    for (int i = 0; i < n; ++i) cin >> p[i][0] >> p[i][1];
    auto res = sol.reconstructQueue(p);
    for (auto& x : res) cout << x[0] << " " << x[1] << "\\n";
    return 0;
}
""",
      testin="6\n7 0\n4 4\n7 1\n5 0\n6 1\n5 2\n")

    P(num=621, week=4, day=26, name="task-scheduler", title="Task Scheduler", diff="Medium", prio="P1",
      desc="""// Given a tasks array of characters and an integer n (cooldown), the CPU needs n
// units of idle time between two same tasks. Return the minimum units of time to
// finish all tasks.""",
      constraints="""//   - 1 <= tasks.length <= 1e4
//   - tasks[i] is an uppercase English letter
//   - 0 <= n <= 100""",
      iofmt="""//   Line 1: n (number of tasks) cooldown
//   Line 2: a string of n characters (the tasks)
//   Print the minimum units of time.""",
      expected="8", goal="O(n) time (greedy by count).",
      sig="int leastInterval(vector<char>& tasks, int n)", retdefault="0",
      main="""int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int nt, cooldown; if (!(cin >> nt >> cooldown)) return 0;
    string s; cin >> s;
    vector<char> tasks(s.begin(), s.end());
    cout << sol.leastInterval(tasks, cooldown) << "\\n";
    return 0;
}
""",
      testin="6 2\nAAABBB\n")
