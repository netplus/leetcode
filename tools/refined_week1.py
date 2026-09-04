"""Week 1 reviewed explanations and C++ implementations."""

REFINEMENTS = {}


def add(num, pattern, model, steps, proof, pitfalls, transfer, code):
    key_points = [
        model,
        "；".join(f"{i}. {step}" for i, step in enumerate(steps, 1)) + "。",
        proof,
    ]
    REFINEMENTS[num] = {
        "pattern": pattern,
        "key_points": key_points,
        "model": model,
        "steps": steps,
        "proof": proof,
        "pitfalls": pitfalls,
        "transfer": transfer,
        "code": code,
    }


add(1, "哈希表：边扫描边查补数",
    "暴力枚举两下标是 O(n^2)；把已访问元素映射为“值 -> 下标”，当前数只需查询 target - nums[i]。",
    ["初始化空哈希表", "从左到右先查补数，命中就返回两个下标", "未命中再记录当前值"],
    "查询发生在插入当前元素之前，因此同一元素不会被复用；唯一解保证某次扫描到较晚下标时一定能找到较早下标。",
    "不能用 set，因为答案需要下标；重复值应用覆盖或保留任一下标均可；先插入再查询会在 target=2*x 时误用自己。",
    "“在线扫描 + 查询历史状态”也用于前缀和计数、Two Sum 数据流和补数配对问题。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> indexOf;
        indexOf.reserve(nums.size() * 2);

        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            const int complement = target - nums[i];
            auto it = indexOf.find(complement);  // 只在已经看过的元素中查找
            if (it != indexOf.end()) {
                return {it->second, i};
            }
            indexOf[nums[i]] = i;                // 查询后插入，避免复用 nums[i]
        }
        return {};  // 题目保证有唯一解；保留兜底使本地接口更健壮
    }
};''')


add(560, "前缀和 + 频次哈希表",
    "若当前位置前缀和为 prefix，需要寻找此前出现过多少个 prefix-k；每个这样的前缀都对应一个和为 k 的连续子数组。",
    ["先记录空前缀 0 出现一次", "累加当前前缀和并把 prefix-k 的频次加入答案", "最后再增加 prefix 的频次"],
    "区间 [j,i] 的和等于 pre[i+1]-pre[j]；所以 pre[j]=pre[i+1]-k 与目标子数组一一对应，频次累加既不漏也不重。",
    "必须保存频次而非仅判断存在；初始化 count[0]=1 才能统计从下标 0 开始的区间；含负数时普通滑动窗口不成立。",
    "凡是“连续区间 + 指定和/模数/奇偶性”，都可尝试把区间条件改写成两个前缀状态之差。",
r'''// ---------- Solution ----------
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<long long, int> frequency;
        frequency.reserve(nums.size() * 2 + 1);
        frequency[0] = 1;  // 空前缀：让从 0 开始的合法子数组也能被统计

        long long prefix = 0;
        int answer = 0;
        for (int value : nums) {
            prefix += value;
            auto it = frequency.find(prefix - k);
            if (it != frequency.end()) answer += it->second;
            ++frequency[prefix];
        }
        return answer;
    }
};''')


add(1248, "恰好型计数：atMost(k) - atMost(k-1)",
    "把奇数视为 1、偶数视为 0；“恰好 k 个奇数”可由“至多 k 个”减去“至多 k-1 个”得到。",
    ["写滑动窗口函数 atMost(limit)", "右端加入一个数并在奇数超限时收缩左端", "每个右端贡献 right-left+1，再作两次结果之差"],
    "固定右端时，窗口恢复合法后，以 right 结尾且奇数数目至多 limit 的起点正是 [left,right]；两集合做差只保留恰好 k 个。",
    "atMost(-1) 必须返回 0；答案可能由大量子数组累积，内部使用 long long；偶数不能被误当作窗口边界。",
    "恰好 K 个不同整数、恰好 K 次违规、二进制数组和为 K 都可套用 atMost 差分。",
r'''// ---------- Solution ----------
class Solution {
    static long long atMost(const vector<int>& nums, int limit) {
        if (limit < 0) return 0;
        long long count = 0;
        int left = 0;
        for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
            limit -= nums[right] & 1;
            while (limit < 0) limit += nums[left++] & 1;
            count += right - left + 1;  // 这些起点都能形成合法窗口
        }
        return count;
    }

public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        return static_cast<int>(atMost(nums, k) - atMost(nums, k - 1));
    }
};''')


add(53, "Kadane 动态规划：保留最优后缀",
    "令 current 表示“必须以当前位置结尾”的最大子数组和；它只能选择单独从当前数开始，或接在前一个最优后缀之后。",
    ["用首元素初始化 current 与 best", "更新 current=max(nums[i], current+nums[i])", "用 current 更新全局 best"],
    "所有以 i 结尾的连续子数组，要么只含 nums[i]，要么由某个以 i-1 结尾的子数组延伸；保留其中最大者并对所有终点取最大即为答案。",
    "不能把初值设为 0，否则全负数组会错误地选择空子数组；题目要求非空连续子数组。",
    "该“必须以 i 结尾”的状态定义可迁移到最大乘积子数组、最长递增连续段以及带一次操作的子数组 DP。",
r'''// ---------- Solution ----------
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int current = nums[0];
        int best = nums[0];
        for (int i = 1; i < static_cast<int>(nums.size()); ++i) {
            // 旧后缀为负时会拖累当前元素，此时从当前位置重新开始。
            current = max(nums[i], current + nums[i]);
            best = max(best, current);
        }
        return best;
    }
};''')


add(167, "有序数组的相向双指针",
    "数组有序，左右端点之和相对目标的大小能唯一决定应排除哪一端，而无需哈希表。",
    ["left/right 指向两端", "和偏小就增大 left，和偏大就减小 right", "相等时返回 1-based 下标"],
    "若当前和偏小，固定 left 与任何更靠左/不大于 right 的元素都不可能达到目标，所以 left 可安全排除；偏大时对 right 对称成立。",
    "返回值是 1-based；不能复用同一位置，所以循环条件是 left<right；题目明确要求常量额外空间。",
    "排序后两数之和、三数之和的内层、盛水容器和回文检查都依赖“单调性允许排除一端”。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int left = 0, right = static_cast<int>(numbers.size()) - 1;
        while (left < right) {
            const long long sum = static_cast<long long>(numbers[left]) + numbers[right];
            if (sum == target) return {left + 1, right + 1};
            if (sum < target) ++left;   // 需要更大的和
            else --right;               // 需要更小的和
        }
        return {};
    }
};''')


add(15, "排序 + 枚举锚点 + 双指针去重",
    "排序后固定最左元素 nums[i]，剩余两数之和具有单调性，可在线性时间内找齐；三层枚举由此降为 O(n^2)。",
    ["排序并枚举 i，跳过重复锚点", "在 i 右侧用 left/right 搜索 -nums[i]", "命中后同时越过两侧重复值"],
    "对每个 i，双指针依据和的大小排除不可能端点，遍历了全部可行二元组；三处去重保证每个值三元组只输出一次。",
    "条件应是三个下标两两不同；排序会修改输入；求和用 long long 防止扩展约束下溢出；nums[i]>0 时可提前结束。",
    "k-Sum 通常递归固定一个数，最终降到有序 Two Sum；去重位置与本题完全同源。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> answer;
        const int n = static_cast<int>(nums.size());

        for (int i = 0; i < n - 2; ++i) {
            if (i > 0 && nums[i] == nums[i - 1]) continue;  // 锚点去重
            if (nums[i] > 0) break;

            int left = i + 1, right = n - 1;
            while (left < right) {
                const long long sum = static_cast<long long>(nums[i]) + nums[left] + nums[right];
                if (sum < 0) {
                    ++left;
                } else if (sum > 0) {
                    --right;
                } else {
                    answer.push_back({nums[i], nums[left], nums[right]});
                    const int lv = nums[left], rv = nums[right];
                    while (left < right && nums[left] == lv) ++left;
                    while (left < right && nums[right] == rv) --right;
                }
            }
        }
        return answer;
    }
};''')


add(11, "相向双指针：移动短板",
    "容器面积由宽度乘以两端较短高度决定；宽度每步必减，只有移动短板才可能提高有效高度。",
    ["左右指针置于两端", "计算当前面积并更新最大值", "移动高度较小的一端；相等时移动任一端"],
    "若左边更短，保留左边并移动右边只会让宽度变小且高度上限仍不超过左边，因此不可能更优；排除左端是安全的，右端同理。",
    "面积计算要先转 long long；不要凭较高板移动；两板间宽度是 right-left。",
    "当目标由两个端点中的瓶颈决定时，可寻找类似的支配关系，例如接雨水双指针和有序配对。",
r'''// ---------- Solution ----------
class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0, right = static_cast<int>(height.size()) - 1;
        long long best = 0;
        while (left < right) {
            const int limitingHeight = min(height[left], height[right]);
            best = max(best, 1LL * limitingHeight * (right - left));
            if (height[left] <= height[right]) ++left;  // 只有短板变化才可能改善
            else --right;
        }
        return static_cast<int>(best);
    }
};''')


add(283, "快慢指针：稳定压缩",
    "write 指向下一个非零元素应写入的位置，read 扫描全部元素；交换能把非零项稳定地压到前缀。",
    ["write 从 0 开始", "read 遇非零就与 write 交换并递增 write", "扫描结束后零自然位于后缀"],
    "第 read 次迭代后，[0,write) 恰为原数组已扫描部分的全部非零元素且相对顺序不变；交换出去的只能是零或元素自身。",
    "必须原地操作且保持非零相对顺序；write==read 时可跳过交换，但直接 swap 同样正确。",
    "稳定删除指定值、数组去重、按条件压缩都可抽象成 read 负责筛选、write 负责落位。",
r'''// ---------- Solution ----------
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int write = 0;
        for (int read = 0; read < static_cast<int>(nums.size()); ++read) {
            if (nums[read] != 0) {
                swap(nums[write], nums[read]);
                ++write;
            }
        }
    }
};''')


add(209, "正数数组上的可变滑动窗口",
    "所有元素为正使窗口和随右端扩张单调不减、随左端收缩单调不增，因此可用双指针一次扫描。",
    ["右指针逐个加入元素", "当 sum>=target 时反复记录长度并移动左端", "返回最短长度，未命中返回 0"],
    "对固定 right，收缩循环结束前枚举了所有满足目标的左边界，最后一个合法窗口最短；每个下标至多进出窗口一次。",
    "正数条件是关键，含负数时收缩不再单调；答案初值用 n+1 哨兵；和用 long long 更稳健。",
    "滑动窗口适合“连续区间 + 扩张/收缩具有单调性”；含负数的最短和问题要考虑前缀和与单调队列。",
r'''// ---------- Solution ----------
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int left = 0;
        int best = static_cast<int>(nums.size()) + 1;
        long long sum = 0;
        for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
            sum += nums[right];
            while (sum >= target) {
                best = min(best, right - left + 1);
                sum -= nums[left++];
            }
        }
        return best == static_cast<int>(nums.size()) + 1 ? 0 : best;
    }
};''')


add(3, "滑动窗口 + 字符最后位置",
    "窗口 [left,right] 始终无重复；看到字符 c 时，left 直接跳到 c 上次位置后一格，但不能向左回退。",
    ["把 256 个字节字符的最后位置初始化为 -1", "更新 left=max(left,last[c]+1)", "记录窗口长度并更新 last[c]"],
    "更新后 c 在窗口内至多出现一次，其他字符未改变；left 单调递增，因此窗口始终合法，并保留每个 right 下最长的合法后缀。",
    "字符串可能为空或含空格；char 可能有符号，索引数组时转 unsigned char；left 必须取 max 防止回退。",
    "需要维护“最近一次冲突位置”的问题，如至多一次重复、替换后最长字符段，都可由该窗口状态扩展。",
r'''// ---------- Solution ----------
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        array<int, 256> last;
        last.fill(-1);
        int left = 0, best = 0;
        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            const unsigned char c = static_cast<unsigned char>(s[right]);
            left = max(left, last[c] + 1);  // 冲突字符不在当前窗口时不能回退
            best = max(best, right - left + 1);
            last[c] = right;
        }
        return best;
    }
};''')


add(76, "最小覆盖滑动窗口",
    "need[c] 保存目标频次，missing 保存窗口尚缺的字符总数（包含重复）；missing==0 时窗口覆盖 t，可以尝试收缩。",
    ["统计 t 并令 missing=|t|", "扩张右端：若该字符仍有需求则 missing--，随后 need[c]--", "覆盖后反复收缩并更新最短答案"],
    "need 允许为负，表示窗口内冗余；missing 为 0 当且仅当所有需求均满足。每次收缩到再次缺字符前，会考察该右端对应的最短合法窗口。",
    "t 中重复字符必须按次数满足；无解返回空串；更新答案应发生在移除左字符之前；字符数组索引用 unsigned char。",
    "“满足配额后求最短”统一使用扩右至可行、缩左至刚好失效；反向可得到至多/至少型窗口计数。",
r'''// ---------- Solution ----------
class Solution {
public:
    string minWindow(string s, string t) {
        if (t.size() > s.size()) return "";
        array<int, 256> need{};
        for (unsigned char c : t) ++need[c];

        int missing = static_cast<int>(t.size());
        int left = 0, bestStart = 0, bestLength = INT_MAX;
        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            unsigned char rc = static_cast<unsigned char>(s[right]);
            if (need[rc] > 0) --missing;
            --need[rc];

            while (missing == 0) {
                if (right - left + 1 < bestLength) {
                    bestStart = left;
                    bestLength = right - left + 1;
                }
                unsigned char lc = static_cast<unsigned char>(s[left++]);
                ++need[lc];
                if (need[lc] > 0) ++missing;  // 刚移除了一个必需字符
            }
        }
        return bestLength == INT_MAX ? "" : s.substr(bestStart, bestLength);
    }
};''')


add(438, "定长滑动窗口 + 频次差",
    "异位词长度必等于 p；用 need 表示窗口相对 p 仍缺的字符数，用 missing 表示总缺口。",
    ["统计 p 的频次", "加入右字符并更新 missing", "窗口超过 |p| 时移除左字符；长度相等且 missing=0 就记录起点"],
    "窗口始终保持长度不超过 |p|；当长度恰等且总缺口为 0，等长性保证既无缺少也无多余，故频次向量完全相同。",
    "p 比 s 长时自然无答案；先加入再裁剪；输出起点按扫描顺序天然递增。",
    "固定长度的排列匹配、DNA 模式计数和滚动校验通常都采用“进一个、出一个”的窗口更新。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        array<int, 26> need{};
        for (char c : p) ++need[c - 'a'];

        vector<int> answer;
        int missing = static_cast<int>(p.size());
        int left = 0;
        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            int in = s[right] - 'a';
            if (need[in] > 0) --missing;
            --need[in];

            if (right - left + 1 > static_cast<int>(p.size())) {
                int out = s[left++] - 'a';
                ++need[out];
                if (need[out] > 0) ++missing;
            }
            if (right - left + 1 == static_cast<int>(p.size()) && missing == 0) {
                answer.push_back(left);
            }
        }
        return answer;
    }
};''')


add(303, "一维前缀和",
    "prefix[i] 定义为前 i 个元素之和，并在开头放一个 0；任意闭区间 [left,right] 可由两个前缀相减得到。",
    ["build 时构造长度 n+1 的 prefix", "查询返回 prefix[right+1]-prefix[left]", "不修改原数组"],
    "prefix[right+1] 包含 0..right，prefix[left] 包含 0..left-1，相减后恰好只剩目标闭区间。",
    "最常见错误是 right 少加 1；对象必须先 build；若约束扩大可将接口也改为 long long。",
    "静态区间和、频次累计、差分还原以及二维前缀和，本质都是用可逆的累计状态消去重复计算。",
r'''// ---------- Solution ----------
class Solution {
    vector<long long> prefix;

public:
    void build(vector<int>& nums) {
        prefix.assign(nums.size() + 1, 0);
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
    }

    int sumRange(int left, int right) {
        return static_cast<int>(prefix[right + 1] - prefix[left]);
    }
};''')


add(304, "二维前缀和 + 容斥",
    "prefix[r][c] 表示原矩阵左上角 [0,r)×[0,c) 的和；目标矩形用“大矩形减上减左加回重叠”得到。",
    ["构造带额外首行首列的 prefix", "逐格使用上+左-左上+当前值", "查询按四个角做容斥"],
    "上方与左方区域各减一次，而它们的交集被减了两次，必须加回；半开坐标让边界无需分支。",
    "row2/col2 是闭区间坐标，所以查询中要加 1；漏掉加回左上角会得到系统性偏小结果。",
    "二维区域计数、图像积分图和固定矩形卷积都可用二维前缀；更高维仍是同样的容斥思想。",
r'''// ---------- Solution ----------
class Solution {
    vector<vector<long long>> prefix;

public:
    void build(vector<vector<int>>& matrix) {
        const int rows = static_cast<int>(matrix.size());
        const int cols = static_cast<int>(matrix[0].size());
        prefix.assign(rows + 1, vector<long long>(cols + 1, 0));
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                prefix[r + 1][c + 1] = matrix[r][c] + prefix[r][c + 1]
                                     + prefix[r + 1][c] - prefix[r][c];
            }
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        long long sum = prefix[row2 + 1][col2 + 1] - prefix[row1][col2 + 1]
                      - prefix[row2 + 1][col1] + prefix[row1][col1];
        return static_cast<int>(sum);
    }
};''')


add(1109, "差分数组：区间更新转端点更新",
    "对闭区间 [first,last] 增加 seats，只需在差分数组 first-1 处加、last 处减，最后一次前缀和恢复每个航班的总座位。",
    ["建立长度 n+1 的 diff", "每条预订更新两个边界", "从左到右累加 diff 生成答案"],
    "端点标记使 seats 对 first..last 的累计贡献保持有效，并从 last+1 起被抵消；所有预订线性叠加后仍成立。",
    "题面航班是 1-based，数组是 0-based；使用 n+1 哨兵可无分支写结束标记；不是对每个区间逐项更新。",
    "批量区间加法、公交载客、覆盖次数和扫描线事件都可先记录变化点，再通过累计恢复状态。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
        vector<int> diff(n + 1, 0);
        for (const auto& booking : bookings) {
            const int first = booking[0] - 1;
            const int afterLast = booking[1];
            diff[first] += booking[2];
            diff[afterLast] -= booking[2];
        }
        vector<int> answer(n);
        int running = 0;
        for (int i = 0; i < n; ++i) {
            running += diff[i];
            answer[i] = running;
        }
        return answer;
    }
};''')


add(238, "左右乘积：复用输出数组",
    "answer[i] 先保存 i 左侧所有元素的乘积，再从右向左乘上 i 右侧乘积；全程不需要除法。",
    ["左到右写入左侧乘积并更新 prefix", "rightProduct 从 1 开始右到左扫描", "把右侧乘积乘入答案后再纳入 nums[i]"],
    "第一次扫描后 answer[i]=乘积(0..i-1)；第二次到达 i 时 rightProduct=乘积(i+1..n-1)，两者相乘正好排除 nums[i]。",
    "更新 rightProduct 的顺序不能提前；0 的个数无需特殊判断；额外空间统计通常不含返回数组。",
    "前后缀分解适用于除自身聚合、左右最大值、分割点条件和树上的换根 DP。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        vector<int> answer(nums.size(), 1);
        // 实现技巧（滚动暂存）：answer[i] 先暂存"i 之前所有前缀元素的乘积"，
        // 写完后把 nums[i] 乘进 leftProduct，这份更新后的暂存值就是下一轮
        // answer[i+1] 需要的前缀乘积——暂存值就地滚动，不需要额外数组。
        int leftProduct = 1;
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            answer[i] = leftProduct;
            leftProduct *= nums[i];
        }

        // 右往左同理：rightProduct 暂存右侧后缀乘积，先乘入 answer[i]，
        // 再把 nums[i] 纳入，滚动成下一轮的后缀乘积。
        int rightProduct = 1;
        for (int i = static_cast<int>(nums.size()) - 1; i >= 0; --i) {
            answer[i] *= rightProduct;
            rightProduct *= nums[i];
        }
        return answer;
    }
};''')


add(5, "中心扩展：统一奇偶回文",
    "每个回文都有唯一中心：奇数长度中心是字符，偶数长度中心是字符间隙；枚举中心并向两侧扩展即可覆盖全部候选。",
    ["对每个 i 分别扩展 (i,i) 和 (i,i+1)", "字符相等时持续扩大", "仅在更长时更新起点和长度"],
    "任意最长回文必在其中心对应的那次扩展中被完整发现；扩展停止恰是再扩大已不可能保持回文。",
    "偶数中心不能遗漏；同长度不更新可稳定保留最先发现者；空串虽不在官方约束内仍自然返回空。",
    "中心扩展还可统计所有回文子串；若规模更大，可进一步学习 Manacher 的对称半径复用。",
r'''// ---------- Solution ----------
class Solution {
public:
    string longestPalindrome(string s) {
        int bestStart = 0, bestLength = 0;
        auto expand = [&](int left, int right) {
            while (left >= 0 && right < static_cast<int>(s.size()) && s[left] == s[right]) {
                --left;
                ++right;
            }
            const int length = right - left - 1;
            if (length > bestLength) {
                bestLength = length;
                bestStart = left + 1;
            }
        };

        for (int center = 0; center < static_cast<int>(s.size()); ++center) {
            expand(center, center);       // 奇数长度
            expand(center, center + 1);   // 偶数长度
        }
        return s.substr(bestStart, bestLength);
    }
};''')


add(14, "纵向扫描：逐列验证公共前缀",
    "公共前缀的第 i 个字符必须存在于每个字符串且都等于第一个字符串的第 i 个字符。",
    ["用第一个字符串作为候选上界", "逐字符与其余字符串同列比较", "首次越界或不等时返回此前前缀"],
    "首次失败位置之后不可能属于公共前缀；失败前所有列都经全部字符串验证，因此返回长度恰好最大。",
    "空字符串会在第 0 列立刻失败；不能只比较相邻两串而忽略最短长度；官方保证数组非空。",
    "多个对象的共同前缀/共同路径可逐段收缩候选；排序后也可只比较字典序首尾两个字符串。",
r'''// ---------- Solution ----------
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        const string& first = strs[0];
        for (int i = 0; i < static_cast<int>(first.size()); ++i) {
            for (int j = 1; j < static_cast<int>(strs.size()); ++j) {
                if (i == static_cast<int>(strs[j].size()) || strs[j][i] != first[i]) {
                    return first.substr(0, i);
                }
            }
        }
        return first;
    }
};''')


add(151, "整体反转 + 原地压缩 + 单词复原",
    "整体反转可把单词顺序颠倒，但也会反转单词内部；随后边扫描边压缩空格，并逐个反转单词恢复其字符顺序。",
    ["反转整个字符串", "read/write 双指针原地跳过多余空格并复制单词", "每复制完一个单词就反转该段，最后截断字符串"],
    "整体反转后词序已正确；压缩阶段仅删除冗余空格，段内再反转恢复每个原单词，因此结果恰是逆序单词且仅用单空格分隔。",
    "本地输入必须整行读取；write 始终不超过 read，覆盖不会破坏未读字符；全空格输入会截断为空串。",
    "“整体变换后局部修复”也用于数组块旋转、按单词翻转和无需额外缓冲区的重排。",
r'''// ---------- Solution ----------
class Solution {
public:
    string reverseWords(string s) {
        reverse(s.begin(), s.end());
        int read = 0, write = 0;
        const int n = static_cast<int>(s.size());
        while (read < n) {
            while (read < n && s[read] == ' ') ++read;
            if (read == n) break;
            if (write > 0) s[write++] = ' ';

            const int wordStart = write;
            while (read < n && s[read] != ' ') s[write++] = s[read++];
            reverse(s.begin() + wordStart, s.begin() + write);
        }
        s.resize(write);
        return s;
    }
};''')


add(28, "KMP：失配时复用已匹配前缀",
    "lps[i] 表示模式串前缀 p[0..i] 的最长真前后缀长度；文本失配时跳到这一边界继续，不回退文本指针。",
    ["预处理 needle 的 lps 数组", "双指针扫描 haystack 与 needle", "失配按 lps 回退模式指针，完整匹配时返回起点"],
    "回退后的前缀等于刚匹配后缀，之前字符无需重验；每次比较要么推进文本，要么缩短模式边界，总复杂度线性。",
    "构造 lps 时失配也要链式回退；真前缀不能等于整个串；接口约束 needle 非空，但代码兼容空串返回 0。",
    "KMP 的失配函数也用于字符串周期、重复子串、流式匹配和多个模式拼接后的边界计算。",
r'''// ---------- Solution ----------
class Solution {
public:
    int strStr(string haystack, string needle) {
        if (needle.empty()) return 0;
        vector<int> lps(needle.size(), 0);
        for (int i = 1, len = 0; i < static_cast<int>(needle.size());) {
            if (needle[i] == needle[len]) {
                lps[i++] = ++len;
            } else if (len > 0) {
                len = lps[len - 1];  // 尝试更短的、仍可能成立的边界
            } else {
                lps[i++] = 0;
            }
        }

        for (int i = 0, j = 0; i < static_cast<int>(haystack.size());) {
            if (haystack[i] == needle[j]) {
                ++i;
                if (++j == static_cast<int>(needle.size())) return i - j;
            } else if (j > 0) {
                j = lps[j - 1];
            } else {
                ++i;
            }
        }
        return -1;
    }
};''')


add(56, "排序后线性合并区间",
    "按起点排序后，与当前合并段重叠的区间必连续出现，只需比较其起点与当前段终点。",
    ["按起点再终点排序", "若新区间起点大于当前终点就开启新段", "否则扩展当前终点为两者最大值"],
    "排序保证后续起点不下降；一旦出现间隔，未来区间也不可能再与已封闭段相交；重叠时取最大终点覆盖两段并集。",
    "闭区间端点相等也算重叠，所以分段条件是 start>lastEnd；排序会修改输入。",
    "区间并集、日程压缩、覆盖长度和扫描线预处理都从“排序使局部关系连续”这一点出发。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());
        vector<vector<int>> answer;
        for (const auto& interval : intervals) {
            if (answer.empty() || interval[0] > answer.back()[1]) {
                answer.push_back(interval);
            } else {
                answer.back()[1] = max(answer.back()[1], interval[1]);
            }
        }
        return answer;
    }
};''')


add(57, "有序区间的三阶段扫描",
    "原区间已按起点排序且互不重叠，可分为完全在新区间左边、与新区间相交、完全在右边三段。",
    ["复制 end<newStart 的左段", "吸收所有 start<=newEnd 的重叠段并扩大新区间", "放入合并段后复制剩余右段"],
    "三类按排序顺序连续且互斥；中段全部与不断扩大的新区间相交，合并后覆盖其并集，左右段保持原有顺序。",
    "端点相接视为重叠；intervals 可为空；题目不要求原地修改，直接构造结果更清晰。",
    "已排序数据中插入并局部合并的模式，也用于事件时间线、版本范围和内存区段管理。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        vector<vector<int>> answer;
        int i = 0, n = static_cast<int>(intervals.size());

        while (i < n && intervals[i][1] < newInterval[0]) {
            answer.push_back(intervals[i++]);
        }
        while (i < n && intervals[i][0] <= newInterval[1]) {
            newInterval[0] = min(newInterval[0], intervals[i][0]);
            newInterval[1] = max(newInterval[1], intervals[i][1]);
            ++i;
        }
        answer.push_back(newInterval);
        while (i < n) answer.push_back(intervals[i++]);
        return answer;
    }
};''')


add(179, "拼接序的自定义排序",
    "对字符串 a,b，若 a+b>b+a，则 a 放前面能让整个结果更大；用该两两规则排序所有数字。",
    ["把整数转为字符串", "按 a+b>b+a 排序", "若首项为 0 则统一返回 0，否则顺序拼接"],
    "比较器直接选择 a、b 两段的更优局部顺序；该关系可证明满足排序所需的一致性，任意相邻逆序交换都会使结果变大。",
    "不能按数值或字符串字典序直接排；全零输入不能返回多个 0；结果可能超整数范围，必须使用 string。",
    "当对象拼接后的全局字典序由相邻顺序决定时，可设计 xy 与 yx 的比较器，例如最小拼接数。",
r'''// ---------- Solution ----------
class Solution {
public:
    string largestNumber(vector<int>& nums) {
        vector<string> parts;
        parts.reserve(nums.size());
        for (int value : nums) parts.push_back(to_string(value));
        sort(parts.begin(), parts.end(), [](const string& a, const string& b) {
            return a + b > b + a;
        });
        if (parts[0] == "0") return "0";
        string answer;
        for (const string& part : parts) answer += part;
        return answer;
    }
};''')


add(252, "按开始时间排序检测相邻冲突",
    "会议按开始时间排序后，若存在重叠，则当前会议必会与此前结束最晚的覆盖发生冲突；对互不重叠序列只需比较相邻项。",
    ["按起点排序", "从第二个会议起比较 start 与前一 end", "start<end 时返回 false，否则全部通过"],
    "若每个相邻会议都满足 previous.end<=current.start，传递地所有更早会议也已结束；反之相邻检测立即发现冲突。",
    "会议通常按半开区间理解，前一场结束等于后一场开始不冲突；空或单个输入返回 true。",
    "排序后检查相邻关系也用于重复区间、最小时间差和区间是否互斥；求所需会议室数则改用堆或扫描线。",
r'''// ---------- Solution ----------
class Solution {
public:
    bool canAttendMeetings(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());
        for (int i = 1; i < static_cast<int>(intervals.size()); ++i) {
            if (intervals[i][0] < intervals[i - 1][1]) return false;
        }
        return true;
    }
};''')


add(239, "单调队列：保存仍可能成为最大值的下标",
    "双端队列中的下标对应值严格递减；新值进入时，队尾所有不大于它的值永远不可能再当最大值，可删除。",
    ["移除已滑出窗口的队首", "从队尾删除不大于当前值的下标，再压入当前下标", "窗口形成后输出队首值"],
    "队首始终是窗口内最大候选；被队尾淘汰的元素更小且更早过期，不可能优于新元素。每个下标最多入队出队各一次。",
    "队列必须存下标才能判断过期；重复值用 <= 淘汰旧值可让新值存活更久；输出从 i>=k-1 开始。",
    "单调队列适合滑窗最值、带宽限制的 DP 优化，以及前缀和上寻找最优候选。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        deque<int> candidates;
        vector<int> answer;
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            if (!candidates.empty() && candidates.front() <= i - k) {
                candidates.pop_front();
            }
            while (!candidates.empty() && nums[candidates.back()] <= nums[i]) {
                candidates.pop_back();
            }
            candidates.push_back(i);
            if (i >= k - 1) answer.push_back(nums[candidates.front()]);
        }
        return answer;
    }
};''')


add(41, "原地哈希：把值 x 放到下标 x-1",
    "长度为 n 的数组若包含 1..n，则答案是 n+1；否则最小缺失正数一定在 1..n，可用数组位置充当哈希桶。",
    ["遍历每个位置并循环交换合法但未归位的值", "交换条件同时限制范围并防重复死循环", "再次扫描首个 nums[i]!=i+1 的位置"],
    "交换循环结束后，每个出现过的 x∈[1,n] 都被放到 x-1；因此从左到右首次不匹配的位置正是最小缺失正数。",
    "重复值时必须检查目标位置是否已有同值；0、负数和大于 n 的值忽略；不能另开集合，否则不满足 O(1) 辅助空间。",
    "数组值域与下标可一一映射时，可原地做桶定位，例如找重复数、缺失数和首次缺席编号。",
r'''// ---------- Solution ----------
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        for (int i = 0; i < n; ++i) {
            while (nums[i] >= 1 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
                swap(nums[i], nums[nums[i] - 1]);
            }
        }
        for (int i = 0; i < n; ++i) {
            if (nums[i] != i + 1) return i + 1;
        }
        return n + 1;
    }
};''')


add(31, "字典序下一排列：最小幅度增大",
    "从右侧找最长非递增后缀；其前一位 pivot 是最右侧还能增大的位置。用后缀中最小的较大值替换，再把后缀变为最小升序。",
    ["从右向左找 nums[i]<nums[i+1]", "若存在 pivot，从右找首个大于它的值并交换", "反转 pivot 之后的非递增后缀"],
    "选择最右 pivot 保证更高位不变；右侧首个较大值是可用的最小增量；反转后缀使剩余部分最小，所以得到紧邻的下一字典序。",
    "完全非递增时不存在更大排列，整段反转成最小序；比较必须严格以正确处理重复值；要求原地常量空间。",
    "排列前驱、下一个更大数字和组合枚举都可从“确定最右可变位，再让后缀极值化”推导。",
r'''// ---------- Solution ----------
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int pivot = static_cast<int>(nums.size()) - 2;
        while (pivot >= 0 && nums[pivot] >= nums[pivot + 1]) --pivot;
        if (pivot >= 0) {
            int successor = static_cast<int>(nums.size()) - 1;
            while (nums[successor] <= nums[pivot]) --successor;
            swap(nums[pivot], nums[successor]);
        }
        reverse(nums.begin() + pivot + 1, nums.end());
    }
};''')


add(128, "哈希集合 + 只从序列起点扩展",
    "把所有值放入集合；只有 x-1 不存在时，x 才是某条连续序列的起点，从它向上计数。",
    ["建立去重集合", "跳过存在前驱的值", "从每个起点连续查询 x+1 并更新最大长度"],
    "每条连续序列只从最小值启动一次，且其中每个数只在这次扩展中被访问，因此总工作量是 O(n) 而非表面的双层 O(n^2)。",
    "重复值由 set 消除；空数组答案为 0；若值域允许 INT 边界，递增前要避免溢出。",
    "哈希集合中“只从无前驱节点启动”也用于链式编号、连续日期段和隐式图连通分量扫描。",
r'''// ---------- Solution ----------
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> values(nums.begin(), nums.end());
        int best = 0;
        for (int x : values) {
            if (x != INT_MIN && values.count(x - 1)) continue;
            int length = 1;
            int current = x;
            while (current != INT_MAX && values.count(current + 1)) {
                ++current;
                ++length;
            }
            best = max(best, length);
        }
        return best;
    }
};''')
