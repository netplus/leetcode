"""Week 4 reviewed explanations and C++ implementations."""

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


add(704, "标准闭区间二分查找",
    "维护目标若存在必位于 [left,right] 的不变量；比较中点后可排除包含中点的一半区间。",
    ["初始化 left=0、right=n-1", "用 left+(right-left)/2 取中点", "相等返回；偏小令 left=mid+1，偏大令 right=mid-1"],
    "有序性保证 nums[mid]<target 时 mid 及其左侧都不可能是答案，另一分支对称；区间为空时说明目标不存在。",
    "闭区间循环条件是 left<=right；更新必须越过 mid 避免死循环；中点写法避免 left+right 溢出。",
    "二分的本质是在单调谓词上找边界；掌握区间定义后可扩展到左右边界和答案二分。",
r'''// ---------- Solution ----------
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = static_cast<int>(nums.size()) - 1;
        while (left <= right) {
            const int middle = left + (right - left) / 2;
            if (nums[middle] == target) return middle;
            if (nums[middle] < target) left = middle + 1;
            else right = middle - 1;
        }
        return -1;
    }
};''')


add(33, "旋转有序数组二分：每轮至少一半有序",
    "数组值互异；任取中点，左半或右半必有一侧保持升序。先识别有序侧，再判断 target 是否落在其值域内。",
    ["比较 nums[left] 与 nums[mid] 判定左侧是否有序", "target 在有序侧闭开值域内就保留该侧", "否则搜索另一侧"],
    "有序侧的端点值可准确判断目标是否位于其中；若不在即可整体排除。每轮都保留包含目标的半区，最终命中或区间为空。",
    "值互异是直接判定有序侧的关键；值域端点一边含等号、一边不含，避免重复保留 mid。",
    "对局部有序结构做二分时，先找可验证单调的一段；有重复值时往往需退化地缩边界。",
r'''// ---------- Solution ----------
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = static_cast<int>(nums.size()) - 1;
        while (left <= right) {
            int middle = left + (right - left) / 2;
            if (nums[middle] == target) return middle;

            if (nums[left] <= nums[middle]) {  // 左半段有序
                if (nums[left] <= target && target < nums[middle]) right = middle - 1;
                else left = middle + 1;
            } else {                           // 右半段有序
                if (nums[middle] < target && target <= nums[right]) left = middle + 1;
                else right = middle - 1;
            }
        }
        return -1;
    }
};''')


add(34, "边界二分：两次 lower_bound",
    "把“找任意等于 target”改成找第一个 >=target 的位置；右边界可由第一个 >=target+1（即 >target）的位置减一得到。",
    ["实现半开区间 [left,right) 的 firstAtLeast", "求 firstAtLeast(target) 并验证是否命中", "求 firstAtLeast(target+1)-1 作为末位置"],
    "单调谓词 nums[i]>=value 在边界前假、边界后真，二分返回首个真位置；所有 target 恰位于两个边界之间。",
    "target+1 可能溢出，接口用 long long value；半开区间循环 left<right；空数组和未命中返回 [-1,-1]。",
    "计数等于 lower_bound(>x)-lower_bound(>=x)；数据库范围查询与有序数组插入点同样是边界二分。",
r'''// ---------- Solution ----------
class Solution {
    static int firstAtLeast(const vector<int>& nums, long long value) {
        int left = 0, right = static_cast<int>(nums.size());
        while (left < right) {
            int middle = left + (right - left) / 2;
            if (nums[middle] < value) left = middle + 1;
            else right = middle;
        }
        return left;
    }

public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int first = firstAtLeast(nums, target);
        if (first == static_cast<int>(nums.size()) || nums[first] != target) return {-1, -1};
        int afterLast = firstAtLeast(nums, static_cast<long long>(target) + 1);
        return {first, afterLast - 1};
    }
};''')


add(153, "旋转数组最小值二分：与右端比较",
    "在闭区间 [left,right] 内，若 nums[mid]>nums[right]，最小值必在 mid 右侧；否则 mid 可能就是最小值，应保留。",
    ["left/right 包住全部候选", "比较 middle 与 right", "大于则 left=middle+1，否则 right=middle；相遇即答案"],
    "右端属于旋转后段；mid 大于右端说明 mid 尚在前段，断点在右侧；否则 mid 已在后段且其左侧仍可能含最小值。",
    "更新 right=middle 而不是 middle-1，因为 middle 可能是答案；题目元素互异；循环用 left<right。",
    "旋转边界、峰值和第一个坏版本都可视为在真假两段之间寻找转折点。",
r'''// ---------- Solution ----------
class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0, right = static_cast<int>(nums.size()) - 1;
        while (left < right) {
            int middle = left + (right - left) / 2;
            if (nums[middle] > nums[right]) left = middle + 1;
            else right = middle;
        }
        return nums[left];
    }
};''')


add(410, "答案二分 + 贪心可行性检查",
    "目标最大子数组和越大，所需分段数单调不增；给定上限 limit，按顺序尽量装满每段能得到最少段数。",
    ["搜索下界=max(nums)、上界=sum(nums)", "check(limit) 贪心统计需要几段", "段数<=k 说明可行并收缩右界，否则提高左界"],
    "正数条件下，当前元素放不进本段时切段是必要且最晚的，因此得到给定上限的最少段数；可行谓词随 limit 单调。",
    "子数组必须连续且非空；上下界和中点用 long long；允许最多 k 段等价于能通过继续拆分得到恰好 k 段。",
    "最小化最大负载、工人分配、磁带切分和运载容量都常用“猜答案 + 贪心判定”。",
r'''// ---------- Solution ----------
class Solution {
public:
    int splitArray(vector<int>& nums, int k) {
        long long left = *max_element(nums.begin(), nums.end());
        long long right = accumulate(nums.begin(), nums.end(), 0LL);

        auto groupsNeeded = [&](long long limit) {
            int groups = 1;
            long long current = 0;
            for (int value : nums) {
                if (current + value > limit) {
                    ++groups;
                    current = 0;
                }
                current += value;
            }
            return groups;
        };

        while (left < right) {
            long long middle = left + (right - left) / 2;
            if (groupsNeeded(middle) <= k) right = middle;
            else left = middle + 1;
        }
        return static_cast<int>(left);
    }
};''')


add(875, "答案二分：最小可行速度",
    "速度 speed 越大，总耗时 sum(ceil(pile/speed)) 越小；“能否在 h 小时内吃完”是单调谓词。",
    ["速度范围设为 [1,maxPile]", "计算中点速度所需总小时并可提前截断", "可行时保留中点收缩右界，不可行时抬高左界"],
    "二分始终保留最小可行速度；可行集合是连续后缀，left==right 时唯一边界就是答案。",
    "向上取整用 (pile+speed-1)/speed；小时总数用 long long；每小时只能处理一堆，所以各堆分别取整。",
    "凡是资源速率增大使完成时间单调下降的问题，都可对速率做最小可行值二分。",
r'''// ---------- Solution ----------
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int left = 1, right = *max_element(piles.begin(), piles.end());
        auto canFinish = [&](int speed) {
            long long hours = 0;
            for (int pile : piles) {
                hours += (static_cast<long long>(pile) + speed - 1) / speed;
                if (hours > h) return false;
            }
            return true;
        };
        while (left < right) {
            int middle = left + (right - left) / 2;
            if (canFinish(middle)) right = middle;
            else left = middle + 1;
        }
        return left;
    }
};''')


add(1011, "答案二分：按原顺序装载的最小容量",
    "容量越大，按给定顺序运完所需天数单调不增；给定容量时，每天尽可能多装是使用天数最少的贪心方案。",
    ["容量下界=max(weights)，上界=sum(weights)", "顺序累加，超容量就开启新一天", "需要天数<=days 时缩小容量，否则增大"],
    "正重量下延迟换天不会增加未来负担，因此贪心得到给定容量的最少天数；可行容量构成连续后缀，二分找到首个可行值。",
    "包裹顺序不能改变；单件重量决定容量下界；总和与中点用 long long 防溢出。",
    "它与分割数组最大和本质相同：把连续序列切成有限段并最小化最大段负载。",
r'''// ---------- Solution ----------
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int days) {
        long long left = *max_element(weights.begin(), weights.end());
        long long right = accumulate(weights.begin(), weights.end(), 0LL);
        auto daysNeeded = [&](long long capacity) {
            int used = 1;
            long long load = 0;
            for (int weight : weights) {
                if (load + weight > capacity) {
                    ++used;
                    load = 0;
                }
                load += weight;
            }
            return used;
        };
        while (left < right) {
            long long middle = left + (right - left) / 2;
            if (daysNeeded(middle) <= days) right = middle;
            else left = middle + 1;
        }
        return static_cast<int>(left);
    }
};''')


add(4, "较短数组上二分分割线",
    "把两个有序数组分别切成左、右两部分，使左边总元素数固定且左侧最大值<=右侧最小值；中位数只由四个边界值决定。",
    ["确保 nums1 是较短数组", "二分 cut1，并由总左长度推出 cut2", "若分割合法计算中位数，否则按边界大小移动 cut1"],
    "合法分割保证合并序列所有左元素不大于所有右元素，且数量平衡，所以奇数时左侧最大是中位数，偶数时两侧边界均值是中位数。",
    "空侧用 INT_MIN/INT_MAX 哨兵；奇偶统一用 (m+n+1)/2；平均前转 long long 避免整数加法溢出。",
    "两个有序集合的第 k 小也可通过分割数量和交叉边界条件求解，是本题的更一般形式。",
r'''// ---------- Solution ----------
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        if (nums1.size() > nums2.size()) return findMedianSortedArrays(nums2, nums1);
        const int m = static_cast<int>(nums1.size());
        const int n = static_cast<int>(nums2.size());
        const int leftSize = (m + n + 1) / 2;
        int left = 0, right = m;

        while (left <= right) {
            const int cut1 = left + (right - left) / 2;
            const int cut2 = leftSize - cut1;
            const int left1 = cut1 == 0 ? INT_MIN : nums1[cut1 - 1];
            const int right1 = cut1 == m ? INT_MAX : nums1[cut1];
            const int left2 = cut2 == 0 ? INT_MIN : nums2[cut2 - 1];
            const int right2 = cut2 == n ? INT_MAX : nums2[cut2];

            if (left1 <= right2 && left2 <= right1) {
                const int leftMax = max(left1, left2);
                if ((m + n) % 2 == 1) return leftMax;
                const int rightMin = min(right1, right2);
                return (static_cast<long long>(leftMax) + rightMin) / 2.0;
            }
            if (left1 > right2) right = cut1 - 1;
            else left = cut1 + 1;
        }
        return 0.0;  // 两个输入有序且总长度非零时不会到达
    }
};''')


add(455, "排序 + 双指针贪心匹配",
    "按需求和饼干尺寸升序；每块饼干若能满足当前最容易满足的孩子就分配，否则只能跳过该饼干。",
    ["排序 g 与 s", "child/cookie 从最小端开始", "饼干足够则两者都前进，否则只前进 cookie"],
    "最小饼干若连当前最小需求都不满足，则不能满足任何剩余孩子；若能满足，把它给当前孩子不会挤占更大饼干，交换论证不劣。",
    "每个孩子最多一块、每块饼干最多给一人；排序会修改输入；任一数组为空自然返回 0。",
    "有序资源匹配中优先满足最小需求，是区间调度、船救人等贪心交换论证的常见形态。",
r'''// ---------- Solution ----------
class Solution {
public:
    int findContentChildren(vector<int>& g, vector<int>& s) {
        sort(g.begin(), g.end());
        sort(s.begin(), s.end());
        int child = 0, cookie = 0;
        while (child < static_cast<int>(g.size()) && cookie < static_cast<int>(s.size())) {
            if (s[cookie] >= g[child]) ++child;
            ++cookie;
        }
        return child;
    }
};''')


add(55, "贪心维护最远可达位置",
    "扫描到位置 i 时，只要 i 不超过当前 farthest，它就是可达的，并可把最远范围扩展到 i+nums[i]。",
    ["farthest 从 0 开始", "若 i>farthest 立即失败", "更新 farthest=max(farthest,i+nums[i])，覆盖末尾即可成功"],
    "不变量是 [0,farthest] 中每个位置可由某条路径到达；从任一可达 i 跳跃会扩充这一连续可达前缀，遇到间隙则任何更后位置都不可达。",
    "无需关心具体路径；i+nums[i] 可用 long long 计算以适应更大约束；长度 1 直接可达。",
    "区间覆盖型跳跃问题常把许多具体方案压缩成“当前层能到的最远边界”。",
r'''// ---------- Solution ----------
class Solution {
public:
    bool canJump(vector<int>& nums) {
        long long farthest = 0;
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            if (i > farthest) return false;
            farthest = max(farthest, static_cast<long long>(i) + nums[i]);
            if (farthest >= static_cast<int>(nums.size()) - 1) return true;
        }
        return true;
    }
};''')


add(45, "贪心分层：把一次跳跃看作 BFS 一层",
    "currentEnd 是当前跳数能覆盖的最远边界，farthest 是扫描这一层所有位置后下一跳能达到的最远点。",
    ["只扫描到 n-2", "持续更新 farthest", "到达 currentEnd 时必须增加一次跳跃，并把边界推进到 farthest"],
    "在增加下一跳前已枚举当前层所有可达起点，因此 farthest 是使用再一跳可达的最大范围；按层推进首次覆盖终点使用的跳数最少。",
    "题目保证终点可达；长度 1 答案为 0；循环不处理终点可避免到终点后多计一次。",
    "这种边界推进就是不显式存队列的区间 BFS，可用于最少覆盖段数和最少加油次数。",
r'''// ---------- Solution ----------
class Solution {
public:
    int jump(vector<int>& nums) {
        int jumps = 0, currentEnd = 0, farthest = 0;
        for (int i = 0; i + 1 < static_cast<int>(nums.size()); ++i) {
            farthest = max(farthest, i + nums[i]);
            if (i == currentEnd) {
                ++jumps;
                currentEnd = farthest;
            }
        }
        return jumps;
    }
};''')


add(134, "贪心重置候选起点 + 全局可行性",
    "总油量小于总消耗时无解；否则扫描净收益，若从当前 start 到 i 的油箱变负，则这一段内任何位置都不可能作为起点，候选重置为 i+1。",
    ["累计 total 与 currentTank", "currentTank<0 时重置 start 和 currentTank", "扫描后 total>=0 返回 start，否则 -1"],
    "若 start..i 总和为负，则其中任一更晚起点拥有的初始累计不可能优于 start 在到达它时的非负余量，因此都无法跨过 i；可一次排除整段。",
    "局部重置不能替代全局 total 检查；答案按题意唯一；净收益计算为 gas[i]-cost[i]。",
    "环形可行起点、前缀亏损重置和最大子数组都有“负前缀只会拖累后续”的共同思想。",
r'''// ---------- Solution ----------
class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int total = 0, currentTank = 0, start = 0;
        for (int i = 0; i < static_cast<int>(gas.size()); ++i) {
            const int gain = gas[i] - cost[i];
            total += gain;
            currentTank += gain;
            if (currentTank < 0) {
                start = i + 1;
                currentTank = 0;
            }
        }
        return total >= 0 ? start : -1;
    }
};''')


add(215, "选择算法：nth_element 定位第 k 大",
    "第 k 大在升序下标 n-k；nth_element 以平均 O(n) 的选择算法把该位置放成排序后应有的元素，无需完整排序。",
    ["计算 targetIndex=n-k", "调用 nth_element 使目标位置就位", "返回 nums[targetIndex]"],
    "算法结束后目标左侧元素不大于它、右侧不小于它，目标值与完整排序同位置一致，因此就是第 k 大（重复值按出现次数计）。",
    "题目问排序序列中的第 k 个而非第 k 个不同值；nth_element 会修改输入；若面试要求手写可实现随机 Quickselect。",
    "只需顺序统计量而非完整有序时，应优先选择 Quickselect；流式或数据量远大于 k 时用大小为 k 的堆。",
r'''// ---------- Solution ----------
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        const int targetIndex = static_cast<int>(nums.size()) - k;
        nth_element(nums.begin(), nums.begin() + targetIndex, nums.end());
        return nums[targetIndex];
    }
};''')


add(347, "频次哈希 + 大小为 k 的最小堆",
    "先统计每个值出现次数；最小堆只保留目前频次最高的 k 个元素，堆顶是候选中最弱者，便于被更强者替换。",
    ["哈希统计 value->frequency", "每项入堆，若大小超过 k 就弹出最小频次", "弹出剩余 k 项作为答案"],
    "处理任意前缀后，堆中是该前缀频次最大的至多 k 项；新项加入后删除 k+1 项中的最小者维持不变量。",
    "答案顺序任意；最小堆比较 pair 时会用值打破频次平局，不影响合法性；题目保证第 k 高频边界唯一。",
    "Top-K 的通用选择是：全排序、大小 k 堆、桶排序或 Quickselect，取决于数据规模与是否流式。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> frequency;
        for (int value : nums) ++frequency[value];

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> top;
        for (auto [value, count] : frequency) {
            top.push({count, value});
            if (static_cast<int>(top.size()) > k) top.pop();
        }
        vector<int> answer;
        while (!top.empty()) {
            answer.push_back(top.top().second);
            top.pop();
        }
        return answer;
    }
};''')


add(295, "双堆维护数据流中位数",
    "max-heap lower 保存较小一半，min-heap upper 保存较大一半；维持 lower.size() 等于 upper.size() 或多一。",
    ["新数先按与 lower.top 比较放入一侧", "从较大堆顶向另一堆搬运以恢复大小", "奇数返回 lower.top，偶数返回两堆顶均值"],
    "所有 lower 元素不大于 upper 元素，且大小平衡，所以两个堆顶正是有序序列中间的一个或两个元素。每次搬运边界元素保持顺序不变量。",
    "偶数均值相加前转 long long；findMedian 只在非空时调用；平衡方向要与“lower 可多一个”约定一致。",
    "双堆适合在线中位数；若还需删除滑窗旧值，可配合延迟删除哈希表。",
r'''// ---------- Solution ----------
class Solution {
    priority_queue<int> lower;  // 较小一半，堆顶是其中最大值
    priority_queue<int, vector<int>, greater<int>> upper;  // 较大一半

public:
    void addNum(int num) {
        if (lower.empty() || num <= lower.top()) lower.push(num);
        else upper.push(num);

        if (lower.size() > upper.size() + 1) {
            upper.push(lower.top());
            lower.pop();
        } else if (upper.size() > lower.size()) {
            lower.push(upper.top());
            upper.pop();
        }
    }

    double findMedian() {
        if (lower.size() > upper.size()) return lower.top();
        return (static_cast<long long>(lower.top()) + upper.top()) / 2.0;
    }
};''')


add(23, "多路归并：最小堆保存每条链当前头",
    "每条链表内部有序，全局下一个最小节点一定在某条链的当前头；最小堆在 k 个候选头间选择。",
    ["所有非空链表头入堆", "弹出最小节点接到结果尾", "把该节点的后继作为同链新候选入堆"],
    "堆始终包含每条尚未耗尽链的最小未输出节点，因此堆顶是所有剩余节点的全局最小值，归纳得到完整有序序列。",
    "堆比较器要比较 node->val；应复用节点且最后令 tail 正确连接；空链表数组返回 nullptr。",
    "k 路归并用于外部排序、日志时间线合并和归并多个有序迭代器，复杂度通常是 O(N log k)。",
r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto greaterNode = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(greaterNode)> smallest(greaterNode);
        for (ListNode* head : lists) if (head) smallest.push(head);

        ListNode dummy;
        ListNode* tail = &dummy;
        while (!smallest.empty()) {
            ListNode* node = smallest.top();
            smallest.pop();
            if (node->next) smallest.push(node->next);
            tail->next = node;
            tail = node;
        }
        return dummy.next;
    }
};''')


add(253, "按开始时间扫描 + 最小结束时间堆",
    "堆保存当前占用会议室的结束时间；下一会议开始前，所有已结束会议室都可释放，然后为当前会议占用一间。",
    ["按开始时间排序", "弹出所有 end<=currentStart 的结束时间", "压入当前 end，并记录堆大小最大值"],
    "处理当前会议后，堆恰表示与它时间上重叠、仍占用房间的会议；同一时刻的堆大小是所需房间数，对全程取最大即最小容量。",
    "结束等于开始时房间可复用，所以弹出条件是 <=；应弹出所有已结束项；空输入返回 0。",
    "资源复用问题常按开始事件排序，并用最小堆管理最早释放的资源，例如机器调度和连接池容量。",
r'''// ---------- Solution ----------
class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());
        priority_queue<int, vector<int>, greater<int>> endTimes;
        int best = 0;
        for (const auto& meeting : intervals) {
            while (!endTimes.empty() && endTimes.top() <= meeting[0]) endTimes.pop();
            endTimes.push(meeting[1]);
            best = max(best, static_cast<int>(endTimes.size()));
        }
        return best;
    }
};''')


add(42, "双指针接雨水：由较低侧上界确定",
    "leftMax/rightMax 分别是两侧已见最高柱；哪侧上界较小，该侧可接水量已经由它确定，不受另一侧更远柱影响。",
    ["左右指针从两端向内", "比较 leftMax 与 rightMax", "处理较小上界侧：更新最大值或累加 max-height，然后移动"],
    "当 leftMax<=rightMax，左位置右侧至少存在高度 rightMax，故水位上限由 leftMax 唯一决定；处理后排除该位置安全，另一侧对称。",
    "先更新边界再计算差可避免负数；宽度单位为 1；空/少于三柱自然接不到水。",
    "双侧边界最值常可把前后缀数组压成双指针，例如接水、容器和边界约束扫描。",
r'''// ---------- Solution ----------
class Solution {
public:
    int trap(vector<int>& height) {
        int left = 0, right = static_cast<int>(height.size()) - 1;
        int leftMax = 0, rightMax = 0;
        long long water = 0;
        while (left <= right) {
            if (leftMax <= rightMax) {
                leftMax = max(leftMax, height[left]);
                water += leftMax - height[left];
                ++left;
            } else {
                rightMax = max(rightMax, height[right]);
                water += rightMax - height[right];
                --right;
            }
        }
        return static_cast<int>(water);
    }
};''')


add(406, "高个优先排序 + 按 k 插入",
    "先处理更高的人时，当前队列中的人都会计入新人的“前方不矮人数”；把新人插到下标 k 就能立即满足其约束。",
    ["按身高降序、同高按 k 升序排序", "依次把 person 插入 answer.begin()+k", "返回构造队列"],
    "插入时前面恰有 k 个已处理且身高不低于当前者；以后插入的更矮者不影响该计数。同高按 k 升序确保插入位置有效且计数正确。",
    "排序规则两关键字都不可反；vector 中间插入使总复杂度 O(n^2)；题目保证存在合法重建。",
    "先安排对后续更“有约束力”的对象，再让弱对象填空，是重建队列和嵌套调度中的常用贪心。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
        sort(people.begin(), people.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[0] != b[0]) return a[0] > b[0];
            return a[1] < b[1];
        });
        vector<vector<int>> answer;
        for (const auto& person : people) {
            answer.insert(answer.begin() + person[1], person);
        }
        return answer;
    }
};''')


add(621, "最高频任务构造骨架",
    "频次最高任务之间需要 maxFrequency-1 个完整间隔，每段长度至少 n+1；并列最高频任务占据骨架各列，最后一组无需冷却尾巴。",
    ["统计 26 类任务频次", "求最高频 maxFrequency 及达到它的类别数 maxCount", "答案取任务总数与 (maxFrequency-1)*(n+1)+maxCount 的较大值"],
    "当空闲槽足够时骨架下界可构造达到；当其他任务填满所有槽时不存在空闲，实际长度就是任务数。两种下界取大覆盖全部情况。",
    "冷却是相同任务之间至少 n 个区间；多个最高频任务要加 maxCount 而不是 1；n=0 时公式自然退化。",
    "频次骨架适合单位时长且任务可任意重排；若任务时长/冷却不同，应改用堆加时间模拟。",
r'''// ---------- Solution ----------
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        array<int, 26> frequency{};
        for (char task : tasks) ++frequency[task - 'A'];
        const int maxFrequency = *max_element(frequency.begin(), frequency.end());
        const int maxCount = static_cast<int>(count(frequency.begin(), frequency.end(), maxFrequency));
        const int skeleton = (maxFrequency - 1) * (n + 1) + maxCount;
        return max(static_cast<int>(tasks.size()), skeleton);
    }
};''')
