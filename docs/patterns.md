# Algorithm Template Cheat-Sheet (memoize this page)

> Each snippet is the minimal, memorizable, ready-to-write version. C++.
> When solving: first identify which template the problem fits, then figure out the variation.

## 0. One-Page Exam Card (re-derive on Day 28)

- "Sorted / monotonic / optimize a value" → **Binary search**
- "Contiguous subarray / window" → **Sliding window / prefix sum**
- "All arrangements / permutations / combinations" → **Backtracking**
- "Shortest steps / levels / layers" → **BFS**
- "Connectivity / grouping / cycle detection" → **Union-Find / DFS coloring**
- "Count ways / feasibility / max-min" + overlapping subproblems → **DP**
- "Top-K / Kth largest" → **Heap / quickselect**
- "Minimize the maximum (or maximize the minimum)" → **Binary search on answer + greedy check**

---

## 1. Prefix Sum / Difference Array

```cpp
// 1D prefix sum
vector<int> pre(n+1, 0);
for (int i = 0; i < n; i++) pre[i+1] = pre[i] + a[i];
// sum of [l, r] = pre[r+1] - pre[l]

// Difference array (range-add, then prefix-sum to recover)
vector<int> d(n+1, 0);
auto add = [&](int l, int r, int v){ d[l]+=v; d[r+1]-=v; };
for (int i=0;i<n;i++) d[i+1]+=d[i]; // recover
```

## 2. Sliding Window (variable length)

```cpp
int l=0, ans=0;
for (int r=0; r<n; r++){
    add(a[r]);                  // expand right
    while (invalid()){          // shrink left while invalid
        remove(a[l]); l++;
    }
    ans = max(ans, r-l+1);      // update per problem
}
```

## 3. Two Pointers (opposite ends)

```cpp
int l=0, r=n-1;
while (l < r){
    if (check(l,r)) l++;
    else r--;
}
```

## 4. Binary Search (integer domain, leftmost value satisfying predicate)

```cpp
// Find first x satisfying P(x), where P is monotonic
int l=lo, r=hi;
while (l < r){
    int mid = l + (r-l)/2;
    if (P(mid)) r = mid;       // mid works -> search left half
    else        l = mid+1;     // mid fails -> search right half
}
return l;                      // l == r is the answer
```
> Binary search on answer: replace `P(mid)` with a verify function (greedy / simulation).

## 5. Monotonic Stack (next greater element)

```cpp
vector<int> res(n, -1);
stack<int> st;                  // indices, stack values decreasing
for (int i=0;i<n;i++){
    while (!st.empty() && a[st.top()] < a[i]){
        res[st.top()] = i;
        st.pop();
    }
    st.push(i);
}
```

## 6. Monotonic Queue (sliding window max)

```cpp
deque<int> q;                   // indices
for (int i=0;i<n;i++){
    while (!q.empty() && q.front() <= i-k) q.pop_front();
    while (!q.empty() && a[q.back()] >= a[i]) q.pop_back();
    q.push_back(i);
    if (i >= k-1) /* window [i-k+1,i] max = a[q.front()] */
}
```

## 7. Union-Find (path compression + union by rank)

```cpp
int fa[N], rk[N];
void init(int n){ for(int i=0;i<n;i++){fa[i]=i;rk[i]=0;} }
int find(int x){ return fa[x]==x ? x : fa[x]=find(fa[x]); }
bool unite(int a,int b){
    a=find(a); b=find(b);
    if (a==b) return false;     // already same set
    if (rk[a]<rk[b]) swap(a,b);
    fa[b]=a;
    if (rk[a]==rk[b]) rk[a]++;
    return true;                // merged
}
```

## 8. BFS Template (shortest steps / multi-source spread)

```cpp
queue<pair<int,int>> q;
for (auto& s : starts){ q.push(s); visited[s]=true; } // (multi-)source(s) in first
int dist=0;
while (!q.empty()){
    int sz=q.size();
    while (sz--){
        auto [x,y]=q.front(); q.pop();
        for (auto [dx,dy] : dirs){
            int nx=x+dx, ny=y+dy;
            if (valid(nx,ny) && !visited[nx][ny]){
                visited[nx][ny]=true;
                q.push({nx,ny});
            }
        }
    }
    dist++;                     // one layer per iteration
}
```

## 9. DFS Coloring (island-type problems)

```cpp
void dfs(int x,int y){
    if (!valid(x,y) || grid[x][y]!='1') return;
    grid[x][y]='2';             // mark visited by recoloring
    for (auto [dx,dy]:dirs) dfs(x+dx,y+dy);
}
```

## 10. Topological Sort (course schedule)

```cpp
vector<int> indeg(n,0);
// count indegrees while building graph
queue<int> q;
for (int i=0;i<n;i++) if (indeg[i]==0) q.push(i);
int cnt=0;
while (!q.empty()){
    int u=q.front(); q.pop(); cnt++;
    for (int v : g[u]) if (--indeg[v]==0) q.push(v);
}
// cnt==n => no cycle (can finish all courses)
```

## 11. Backtracking Template (permutation style)

```cpp
vector<int> path; vector<bool> used(n,false);
void dfs(){
    if (path.size()==n){ ans.push_back(path); return; }
    for (int i=0;i<n;i++){
        if (used[i]) continue;
        used[i]=true; path.push_back(i);
        dfs();
        path.pop_back(); used[i]=false;
    }
}
```
> Subsets: use a `start` parameter to avoid going back. Combinations: `start` + pruning.

## 12. Binary Tree DFS (post-order, aggregate via return value)

```cpp
int ans=0;
int dfs(TreeNode* r){
    if (!r) return 0;
    int L=dfs(r->left), R=dfs(r->right);
    ans=max(ans, L+R+1);        // diameter-style: update answer
    return max(L,R)+1;          // return subtree height
}
```

## 13. Binary Tree BFS (level order)

```cpp
queue<TreeNode*> q; q.push(root);
while (!q.empty()){
    int sz=q.size();
    while (sz--){
        auto* n=q.front(); q.pop();
        if (n->left) q.push(n->left);
        if (n->right) q.push(n->right);
    }
}
```

## 14. DP — 0/1 Knapsack

```cpp
// n items, capacity W, maximize value (or feasibility)
vector<int> dp(W+1, 0);            // 1D rolling array
for (int i=0;i<n;i++){
    for (int j=W; j>=w[i]; j--){  // REVERSE order for 1D!
        dp[j] = max(dp[j], dp[j-w[i]] + v[i]);
    }
}
// Complete knapsack: iterate j forward (ascending)
```

## 15. DP — 2D Subsequence (LCS / edit distance skeleton)

```cpp
vector<vector<int>> dp(m+1, vector<int>(n+1,0));
for (int i=1;i<=m;i++)
  for (int j=1;j<=n;j++){
      if (a[i-1]==b[j-1]) dp[i][j]=dp[i-1][j-1]+1;
      else dp[i][j]=max(dp[i-1][j], dp[i][j-1]);
  }
```

## 16. Heap / TopK

```cpp
// Kth largest -> maintain min-heap of size K
priority_queue<int, vector<int>, greater<int>> pq;  // min-heap
for (int x : a){
    pq.push(x);
    if ((int)pq.size()>k) pq.pop();   // keep only K largest
}
return pq.top();
// Median: max-heap (left half) + min-heap (right half), keep balanced
```

## 17. KMP (hand-written next array)

```cpp
vector<int> buildNext(string& p){
    int m=p.size();
    vector<int> nxt(m,0);
    for (int i=1,j=0;i<m;i++){
        while (j>0 && p[i]!=p[j]) j=nxt[j-1];
        if (p[i]==p[j]) j++;
        nxt[i]=j;
    }
    return nxt;
}
int kmp(string& s, string& p){
    auto nxt=buildNext(p);
    for (int i=0,j=0;i<(int)s.size();i++){
        while (j>0 && s[i]!=p[j]) j=nxt[j-1];
        if (s[i]==p[j]) j++;
        if (j==(int)p.size()) return i-j+1;
    }
    return -1;
}
```

## 18. Quickselect (Kth largest, average O(n))

```cpp
int quickSelect(vector<int>& a, int l, int r, int k){
    // k: 0-based kth smallest
    int p=a[l], i=l, j=r;
    while (i<j){
        while (i<j && a[j]>=p) j--; a[i]=a[j];
        while (i<j && a[i]<=p) i++; a[j]=a[i];
    }
    a[i]=p;
    if (i==k) return a[i];
    return i<k ? quickSelect(a,i+1,r,k) : quickSelect(a,l,i-1,k);
}
// Kth largest = quickSelect(..., n-k)
```

## 19. Linked-List Idioms

```cpp
// Reverse range [a, b)
ListNode* rev(ListNode* a, ListNode* b){
    ListNode *pre=nullptr,*cur=a;
    while (cur!=b){
        auto nx=cur->next;
        cur->next=pre;
        pre=cur; cur=nx;
    }
    return pre;  // new head
}
// Find midpoint: slow/fast pointers; slow stops at end of first half.
// Find cycle entry: after fast/slow meet, walk one ptr from head; re-meet point is the entry.
// Nth from end: fast goes N steps first, then both at same speed.
```

## 20. Palindrome (expand from center)

```cpp
pair<int,int> expand(string& s, int l, int r){
    while (l>=0 && r<(int)s.size() && s[l]==s[r]){ l--; r++; }
    return {l+1, r-1};   // longest palindrome [l+1, r-1]
}
```

---

## Complexity Cheat-Sheet

| Input size n | Acceptable complexity |
|--------------|------------------------|
| n ≤ 10       | O(n!)                  |
| n ≤ 20       | O(2^n)                 |
| n ≤ 100      | O(n^3)                 |
| n ≤ 1000     | O(n^2)                 |
| n ≤ 1e5      | O(n log n)             |
| n ≤ 1e6      | O(n)                   |

> In a timed exam, read n's range first, back out the required complexity, then pick the algorithm class — the single most useful pre-check.
