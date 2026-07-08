# C/C++ Tips & Pitfalls for LeetCode

> For the leveling exam: lean on STL to save time, but hand-written classics (linked list / tree / union-find) are mandatory.

## 1. Program Skeletons

### 1.1 LeetCode style (class / method)

```cpp
#include <bits/stdc++.h>      // catch-all header, common in contests
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int> mp;
        for (int i=0;i<(int)nums.size();i++){
            int t=target-nums[i];
            if (mp.count(t)) return {mp[t], i};
            mp[nums[i]]=i;
        }
        return {};
    }
};
```

### 1.2 Local main style (read stdin, call, print)

See [docs/templates/solution.cpp](templates/solution.cpp) — copy this stub per problem, fill `Solution` only; `main()` is pre-written.

## 2. Custom Data Structures

### 2.1 Linked-list node
```cpp
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *n) : val(x), next(n) {}
};
```

### 2.2 Binary-tree node
```cpp
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

### 2.3 Union-Find
See [patterns.md §7](patterns.md). Use a global array or wrap in a struct.

## 3. STL Cheat-Sheet

| Use case | Container | Key ops |
|----------|-----------|---------|
| Hash map | `unordered_map<K,V>` | `count / [] / erase` |
| Hash set | `unordered_set<K>` | `insert / count / erase` |
| Sorted map/set | `set<K>` / `map<K,V>` | `lower_bound / upper_bound` |
| Double-ended queue | `deque<T>` | `push_back/front, pop, []` |
| Max-heap | `priority_queue<int>` | `top/push/pop` |
| Min-heap | `priority_queue<int,vector<int>,greater<int>>` | same |
| Monotonic queue | `deque<int>` (indices) | maintain manually |
| Stack | `stack<T>` | `top/push/pop` |
| Dynamic array | `vector<T>` | `push_back/size/[]/pop_back` |
| Sort | `sort(v.begin(),v.end(),cmp)` | cmp returns bool |
| Binary search | `lower_bound / upper_bound` | on sorted range |

## 4. Common Algorithm Functions

```cpp
sort(a.begin(), a.end());                      // ascending
sort(a.begin(), a.end(), greater<int>());      // descending
// custom comparator
sort(v.begin(), v.end(), [](auto& p1, auto& p2){
    return p1.first * p1.second < p2.first * p2.second;
});

int idx = lower_bound(a.begin(),a.end(),x)-a.begin();  // first position >= x
__gcd(a,b);            // gcd (C++17)
min(a,b), max(a,b), swap(a,b);
accumulate(v.begin(),v.end(),0LL);              // sum; use 0LL to avoid overflow
```

## 5. Frequent Pitfalls

### 5.1 Integer overflow
```cpp
int mid = l + (r-l)/2;          // OK — never (l+r)/2 (overflow)
long long sum = accumulate(v.begin(),v.end(),0LL);  // OK — use 0LL
```
- `int` range is about ±2.1e9. When n ≤ 1e5, sums of squares / products overflow easily — switch to `long long`.
- Mod-answer problems: apply `% MOD` after every step (add and multiply).

### 5.2 Comparing size() with int
```cpp
for (int i=0;i<nums.size();i++)   // WARNING: signed/unsigned comparison
// prefer:
for (int i=0;i<(int)nums.size();i++)   // OK
// or:
for (size_t i=0;i<nums.size();i++)
```

### 5.3 2D vector init
```cpp
vector<vector<int>> dp(m, vector<int>(n, 0));   // m x n, all zero
vector<vector<int>> g(n, vector<int>(n));       // adjacency matrix
```

### 5.4 Graph adjacency list
```cpp
int n, m;
cin >> n >> m;
vector<vector<int>> g(n);
for (int i=0;i<m;i++){
    int u,v; cin>>u>>v;
    g[u].push_back(v);
    g[v].push_back(u);  // undirected only
}
```

### 5.5 Multi-source BFS — enqueue ALL sources first
```cpp
for (int i=0;i<n;i++)
    for (int j=0;j<m;j++)
        if (grid[i][j]==2){ q.push({i,j}); visited[i][j]=true; }
```
Otherwise a single-source BFS misses the "simultaneous spread" semantics.

### 5.6 Backtracking dedup
- Permutations with duplicates: `sort` first, then `if (i>0 && a[i]==a[i-1] && !used[i-1]) continue;`
- Combinations dedup: pass `start`, loop from `start`.

### 5.7 DP rolling-array direction
```cpp
// 0/1 knapsack (1D) MUST iterate descending, else an item is reused
for (int j=W; j>=w[i]; j--)
// Complete knapsack iterates ascending
for (int j=w[i]; j<=W; j++)
```

### 5.8 Binary search boundaries
- Infinite loop usually from `mid = l+(r-l)/2` landing on `l` when `l=r-1`, while `r=mid` never moves.
- Convention: when `r=mid`, use `mid=l+(r-l)/2`; when `l=mid`, use `mid=l+(r-l+1)/2`.

## 6. Fast Input (large data in exams)

```cpp
#include <bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin>>n;
    // ...
}
```
> LeetCode online judge does not need this; add it for large local test data.

## 7. Build Commands (Makefile wraps these)

```bash
g++ -std=c++17 -O2 -Wall solution.cpp -o sol
# or:
make lc1
```

## 8. Debugging

```cpp
#define DEBUG
#ifdef DEBUG
  #define D(x)  cerr << #x << "=" << x << " "
  #define DN(x) cerr << #x << "=" << x << endl
#else
  #define D(x) ;
  #define DN(x) ;
#endif
// usage: D(a); D(b); DN(c);
```

## 9. Plain-C Fallback (if the exam restricts to C)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Linked list
typedef struct Node { int val; struct Node* next; } Node;

// Hand-rolled hash table (open addressing), hand-rolled stack/queue arrays.
// qsort comparator
int cmp(const void* a, const void* b){
    return *(int*)a - *(int*)b;
}
// qsort(arr, n, sizeof(int), cmp);
```
> In plain C, `qsort` / `bsearch` are the standard-library sort/binary-search — no need to hand-roll.
> But hash tables, dynamic strings, and priority queues must be hand-built; avoid plain C for hard problems.

## 10. Exam-Day Strategy

1. Read all problems; bucket into "can do" / "looks familiar" / "no idea".
2. Finish & submit the "can do" ones first for stable points.
3. For familiar ones, match a template ([patterns.md](patterns.md)) and scaffold.
4. For unknowns: write brute force / special cases for partial credit; save time to check the first two buckets' edge cases.
5. Last 10 minutes: check overflow, array out-of-bounds, `size()` comparisons, `long long`.
