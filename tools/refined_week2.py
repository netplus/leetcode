"""Week 2 reviewed explanations and C++ implementations."""

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


add(206, "链表原地反转：三指针",
    "遍历时将当前节点的 next 指向已反转前缀；prev 是新链表头，current 是尚未处理部分的头。",
    ["先保存 current->next", "把 current->next 改为 prev", "prev/current 同步前进"],
    "每轮后 prev 链包含原链表前缀且方向完全反转，current 指向未处理后缀；循环结束时前缀覆盖全部节点。",
    "改指针前必须保存 next，否则会丢失后缀；空链表和单节点无需特判；返回 prev 而非原 head。",
    "链表反转是区间反转、回文链表、重排链表和 K 组翻转的基础操作。",
r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* previous = nullptr;
        ListNode* current = head;
        while (current) {
            ListNode* next = current->next;  // 改链前保存尚未处理的后缀
            current->next = previous;
            previous = current;
            current = next;
        }
        return previous;
    }
};''')


add(92, "哑节点 + 头插法反转局部区间",
    "定位到 left 前一个节点 before；保持区间首节点 first 不动，每次把 first 后面的节点摘下并插到 before 后面。",
    ["用 dummy 统一 left=1", "移动 before 到反转区间前", "执行 right-left 次摘取与头插"],
    "第 k 次头插后，前 k+1 个区间节点已按逆序位于 before 后，first 始终是这段的尾；其余链表连接保持完整。",
    "移动次数易出现 off-by-one；摘节点时要同时维护 first->next 与 moved->next；无需额外数组。",
    "哑节点统一头部修改，头插法还适用于 K 组反转和将链表区间循环右移。",
r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode dummy(0, head);
        ListNode* before = &dummy;
        for (int pos = 1; pos < left; ++pos) before = before->next;

        ListNode* first = before->next;
        for (int i = 0; i < right - left; ++i) {
            ListNode* moved = first->next;
            first->next = moved->next;   // 从区间剩余部分摘下 moved
            moved->next = before->next;
            before->next = moved;        // 插到已反转部分最前面
        }
        return dummy.next;
    }
};''')


add(21, "哑节点归并两个有序链表",
    "tail 指向结果链表末尾，每次从两个链表当前头中选择较小者接入，等价于归并排序的 merge 步骤。",
    ["建立 dummy/tail", "两表均非空时接入较小节点并推进来源指针", "把剩余非空后缀一次接到 tail"],
    "两表头是各自剩余部分的最小值，选两者较小者就是全局下一个节点；归纳可知结果始终有序且不遗漏。",
    "应复用原节点而非只复制值；相等时选任一侧都正确；哑节点避免单独处理结果头。",
    "多个有序链表、外部排序与归并排序都复用相同的“取当前最小头”不变量。",
r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode dummy;
        ListNode* tail = &dummy;
        while (list1 && list2) {
            ListNode*& chosen = (list1->val <= list2->val) ? list1 : list2;
            tail->next = chosen;
            chosen = chosen->next;
            tail = tail->next;
        }
        tail->next = list1 ? list1 : list2;
        return dummy.next;
    }
};''')


add(160, "双指针切换头部：对齐路径长度",
    "pA 走完 A 后转到 B，pB 走完 B 后转到 A；两者都走 a+b 步，长度差被第二段自动抵消。",
    ["两个指针分别从 headA/headB 出发", "到 null 时切换到另一条链表头", "首次相同节点即交点；无交点时共同为 null"],
    "若有公共尾长 c，两指针在走完各自独有段与对方独有段后同时到达公共头；若无交点，则同时走到 null。",
    "比较的是节点地址不是值；不得修改链表；循环条件 pA!=pB 已覆盖无交点情形。",
    "“走完自己的路再走对方的路”是消除长度差的技巧，也可用于比较两条路径的公共后缀。",
r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
        ListNode* a = headA;
        ListNode* b = headB;
        while (a != b) {
            a = a ? a->next : headB;
            b = b ? b->next : headA;
        }
        return a;  // 交点，或两者共同到达的 nullptr
    }
};''')


add(141, "Floyd 快慢指针判环",
    "slow 每次走一步、fast 每次走两步；无环时 fast 到达空，有环时相对速度为 1，最终必在环内追上。",
    ["slow/fast 都从 head 出发", "在 fast 与 fast->next 存在时推进", "若两指针相遇返回 true，否则返回 false"],
    "进入环后可把位置看作模环长，fast 每轮相对 slow 前进一格，有限轮内必同余；无环链表则指针严格向尾部前进。",
    "循环条件必须先检查 fast 再解引用 fast->next；节点值无关；题面中的 pos 只描述测试数据，不是函数参数。",
    "速度差在周期结构中可用于找环入口、环长和重复状态，是 O(1) 空间替代 visited 集合的典型方式。",
r'''// ---------- Solution ----------
class Solution {
public:
    bool hasCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
    }
};''')


add(142, "Floyd 两阶段：判环后定位入口",
    "快慢指针相遇后，将一个指针放回头部；两者同速前进时的下一次相遇点就是环入口。",
    ["先以 1/2 倍速度寻找环内相遇点", "若 fast 到空则无环", "slow 回到 head，与 fast 同速前进直到相等"],
    "设头到入口 a、入口到相遇点 b、环剩余 c，首次相遇满足 2(a+b)=a+b+k(b+c)，可得 a 与 c 后若干整环等长，故同速相遇于入口。",
    "第二阶段不能继续让 fast 走两步；不能修改节点；无环必须返回 nullptr。",
    "Floyd 数学关系也适用于数组映射中的重复数、函数迭代周期与伪随机序列检测。",
r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* detectCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        do {
            if (!fast || !fast->next) return nullptr;
            slow = slow->next;
            fast = fast->next->next;
        } while (slow != fast);

        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};''')


add(19, "哑节点 + 固定间距快慢指针",
    "让 fast 比 slow 领先 n 个节点；fast 到末尾时，slow 正好位于倒数第 n 个节点的前驱。",
    ["dummy 指向 head，两个指针从 dummy 出发", "fast 先走 n+1 步", "随后同步前进到 fast 为空，再删除 slow->next"],
    "同步阶段始终保持 n+1 条边的间距，因此 fast 越过尾部时 slow 位于目标前一节点；哑节点使删除原头也遵循同一规则。",
    "领先 n 还是 n+1 取决于指针起点，本实现从 dummy 出发走 n+1；删除后不要再访问被删节点。",
    "固定窗口间距可找倒数第 k 个节点、中点以及流式序列中的延迟位置。",
r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;
        for (int i = 0; i <= n; ++i) fast = fast->next;
        while (fast) {
            fast = fast->next;
            slow = slow->next;
        }
        ListNode* removed = slow->next;
        slow->next = removed->next;
        delete removed;
        return dummy.next;
    }
};''')


add(234, "找中点 + 反转后半段 + 双向比较",
    "回文链表的前半段应与反转后的后半段逐项相等；用快慢指针找到后半段起点即可 O(1) 额外空间完成。",
    ["fast 两步、slow 一步找到中点", "反转 slow 开始的后半段", "比较两段，并再次反转以恢复原链表"],
    "后半段长度不大于前半段；反转后从两端向中心的对应节点变成同向排列，逐项相等当且仅当原序列回文。",
    "奇数长度时 slow 指向中间节点，多比较一次不影响；为保持调用者可观察结构，比较后恢复链表。",
    "“破坏性变换—使用—恢复”能在常量空间解决回文、重排等问题，但工程代码要明确恢复契约。",
r'''// ---------- Solution ----------
class Solution {
    static ListNode* reverse(ListNode* head) {
        ListNode* previous = nullptr;
        while (head) {
            ListNode* next = head->next;
            head->next = previous;
            previous = head;
            head = next;
        }
        return previous;
    }

public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        ListNode* reversed = reverse(slow);
        ListNode* right = reversed;
        ListNode* left = head;
        bool equal = true;
        while (right) {
            if (left->val != right->val) equal = false;
            left = left->next;
            right = right->next;
        }
        reverse(reversed);  // 恢复原链表，避免隐藏副作用
        return equal;
    }
};''')


add(144, "迭代 DFS：栈模拟先序遍历",
    "先序顺序是根、左、右；栈后进先出，因此访问根后应先压右孩子、再压左孩子。",
    ["空树直接返回", "根节点入栈", "循环弹出并记录，再按右后左顺序压入非空孩子"],
    "每次弹出的节点都是递归先序下一次会进入的节点；逆序压栈保证左子树整体先于右子树处理。",
    "压栈顺序最易写反；只压非空节点；迭代版显式空间仍为树高到节点数之间。",
    "递归遍历都能用显式栈模拟；调整访问时机和压栈策略即可得到中序、后序及带状态 DFS。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> order;
        if (!root) return order;
        stack<TreeNode*> pending;
        pending.push(root);
        while (!pending.empty()) {
            TreeNode* node = pending.top();
            pending.pop();
            order.push_back(node->val);
            if (node->right) pending.push(node->right);
            if (node->left) pending.push(node->left);
        }
        return order;
    }
};''')


add(94, "迭代 DFS：沿左链压栈",
    "中序顺序是左、根、右；不断深入左子树，将尚未访问根的节点保存在栈中，回退时访问并转向右子树。",
    ["current 从 root 开始", "current 非空时沿左链压栈", "到空后弹栈访问，再令 current 指向右孩子"],
    "栈顶始终是左子树已处理、但自身尚未处理的最近祖先；访问后进入其右子树，完全复现递归中序。",
    "外循环条件要包含 current 或栈非空；不能在压栈时记录值，否则会变成先序。",
    "BST 的中序有序性质可用于验证 BST、找第 k 小和迭代器设计。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> order;
        stack<TreeNode*> ancestors;
        TreeNode* current = root;
        while (current || !ancestors.empty()) {
            while (current) {
                ancestors.push(current);
                current = current->left;
            }
            current = ancestors.top();
            ancestors.pop();
            order.push_back(current->val);
            current = current->right;
        }
        return order;
    }
};''')


add(145, "迭代后序：根右左再反转",
    "后序是左、右、根；先用栈产生根、右、左顺序，最后整体反转即可，避免复杂的访问状态标记。",
    ["根入栈", "弹出时记录，先压左再压右以得到根右左", "反转记录数组"],
    "第一次遍历对每棵子树都输出根右左，反转全序列后节点相对关系变为左右根，正是后序。",
    "压栈顺序与先序不同；反转的是结果而非输入树；空树直接得到空数组。",
    "若不能保存完整结果，可用 visited 标志或 lastVisited 指针实现真正的单栈后序。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> order;
        if (!root) return order;
        stack<TreeNode*> pending;
        pending.push(root);
        while (!pending.empty()) {
            TreeNode* node = pending.top();
            pending.pop();
            order.push_back(node->val);
            if (node->left) pending.push(node->left);
            if (node->right) pending.push(node->right);
        }
        reverse(order.begin(), order.end());
        return order;
    }
};''')


add(102, "队列 BFS：按当前层大小分组",
    "队列按距离根的层次顺序保存节点；进入一层时记录 queue.size()，只处理这批节点，新加入的孩子留给下一层。",
    ["根非空时入队", "每轮读取当前队列大小并创建层结果", "弹出固定数量节点，同时压入左右孩子"],
    "一轮开始时队列恰含当前层全部节点；固定次数处理后，这些节点全部出队且其非空孩子恰构成下一层。",
    "不能在本层循环中使用不断变化的 queue.size() 作上界；空树返回空二维数组。",
    "多源 BFS、最短无权路径和逐层统计都依赖队列中“同一距离层”的边界。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> levels;
        if (!root) return levels;
        queue<TreeNode*> pending;
        pending.push(root);
        while (!pending.empty()) {
            const int width = static_cast<int>(pending.size());
            vector<int> level;
            level.reserve(width);
            for (int i = 0; i < width; ++i) {
                TreeNode* node = pending.front();
                pending.pop();
                level.push_back(node->val);
                if (node->left) pending.push(node->left);
                if (node->right) pending.push(node->right);
            }
            levels.push_back(move(level));
        }
        return levels;
    }
};''')


add(104, "树形递归：深度由子树深度合成",
    "空树深度为 0；非空节点最大深度等于左右子树较大深度加当前这一层。",
    ["递归处理左子树", "递归处理右子树", "返回 1+max(left,right)"],
    "按树高归纳：叶子的两棵空子树深度均为 0，返回 1；若子树结果正确，根到最深叶路径必经过较深子树。",
    "空树是递归出口；极端链状树递归深度为 n，若运行环境栈较小可改 BFS。",
    "树形 DP 通常都先定义“子问题返回给父节点什么”，再由左右返回值合成当前状态。",
r'''// ---------- Solution ----------
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        return 1 + max(maxDepth(root->left), maxDepth(root->right));
    }
};''')


add(105, "前序定根 + 中序切分",
    "前序首元素确定当前根；根在中序中的位置把节点集合唯一切成左、右子树，子树规模又确定前序片段。",
    ["预建中序值到下标的哈希表", "按前序顺序取根并在中序区间切分", "递归构建左区间和右区间"],
    "节点值唯一时，中序切点唯一；前序在根后先列完整左子树再列右子树，递归按中序区间大小消费前序元素，故结构唯一还原。",
    "必须先构建左子树再右子树以匹配前序游标；空区间返回 nullptr；成员游标每次调用前要重置。",
    "任意两种包含中序的遍历通常可重建树；后序则从末尾取根并优先构建右子树。",
r'''// ---------- Solution ----------
class Solution {
    unordered_map<int, int> inorderIndex;
    int preorderIndex = 0;

    TreeNode* build(const vector<int>& preorder, int left, int right) {
        if (left > right) return nullptr;
        const int rootValue = preorder[preorderIndex++];
        TreeNode* root = new TreeNode(rootValue);
        const int middle = inorderIndex[rootValue];
        root->left = build(preorder, left, middle - 1);
        root->right = build(preorder, middle + 1, right);
        return root;
    }

public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        inorderIndex.clear();
        preorderIndex = 0;
        for (int i = 0; i < static_cast<int>(inorder.size()); ++i) {
            inorderIndex[inorder[i]] = i;
        }
        return build(preorder, 0, static_cast<int>(inorder.size()) - 1);
    }
};''')


add(226, "递归交换左右子树",
    "翻转一棵树等价于先翻转左右子树，再交换它们在当前节点下的位置。",
    ["空节点直接返回", "递归得到翻转后的左右子树", "交换两个孩子并返回根"],
    "按树高归纳，子树递归后内部已镜像；交换两棵镜像子树的位置后，当前整棵树正是原树的镜像。",
    "需要保存或直接 swap 两个指针，避免覆盖其中一个；返回原根指针而非新建整棵树。",
    "对称树判断、树结构变换和 AST 重写都可采用“先处理子树，再组合当前节点”的后序框架。",
r'''// ---------- Solution ----------
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        TreeNode* left = invertTree(root->left);
        TreeNode* right = invertTree(root->right);
        root->left = right;
        root->right = left;
        return root;
    }
};''')


add(236, "后序 DFS：让命中信息向上汇聚",
    "递归返回值表示当前子树是否找到 p/q，以及能代表该命中的节点；若左右两侧都非空，当前根就是最近公共祖先。",
    ["空节点或当前命中 p/q 时直接返回", "递归查询左右子树", "两边都命中返回 root，否则返回非空一边"],
    "若 p/q 分居左右，当前节点是最低的汇合点；若同在一侧，该侧递归已返回其最近公共祖先；若当前就是其中一个，它作为祖先直接上报。",
    "比较节点地址而非值；题目保证 p、q 都存在且不同；函数返回的是节点指针。",
    "“子树返回证据，首次汇合处作决策”也用于目录公共祖先、组织树权限汇聚和树上目标集合覆盖。",
r'''// ---------- Solution ----------
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root || root == p || root == q) return root;
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        if (left && right) return root;
        return left ? left : right;
    }
};''')


add(543, "后序树形 DP：高度与直径分工",
    "递归函数向父节点返回单边最大高度；当前节点可作为路径最高点，用 leftHeight+rightHeight 更新全局直径。",
    ["空节点高度返回 0", "后序取得左右高度并更新直径", "向上返回 1+较大子树高度"],
    "任意简单路径都有唯一最高节点；当递归到它时，左右最大向下路径之和覆盖该路径的最优选择，因此全局最大不会遗漏。",
    "直径按边数计，所以节点高度相加无需再加 1；返回给父节点的路径不能同时走左右两边。",
    "树上任意到任意路径常用“当前可闭合双臂、向父只能上报单臂”的二元角色设计。",
r'''// ---------- Solution ----------
class Solution {
    int diameter = 0;

    int height(TreeNode* node) {
        if (!node) return 0;
        const int leftHeight = height(node->left);
        const int rightHeight = height(node->right);
        diameter = max(diameter, leftHeight + rightHeight);
        return 1 + max(leftHeight, rightHeight);
    }

public:
    int diameterOfBinaryTree(TreeNode* root) {
        diameter = 0;
        height(root);
        return diameter;
    }
};''')


add(98, "递归上下界：验证整棵子树的取值范围",
    "每个节点不仅要和父节点比较，还必须满足所有祖先传下来的开区间 (lower,upper)。",
    ["根从无限上下界开始", "检查 lower<value<upper", "左子树收紧上界为 value，右子树收紧下界"],
    "祖先限制随递归完整传递，节点通过检查当且仅当它满足路径上所有 BST 约束；左右子树递归成立则整棵树成立。",
    "BST 是严格不等，重复值非法；int 极值不能用 int 哨兵，使用 long long；只比较直接孩子会漏掉跨层违例。",
    "把全局约束沿树边逐步收紧，适用于范围树、表达式合法性和带上下文的递归验证。",
r'''// ---------- Solution ----------
class Solution {
    bool validate(TreeNode* node, long long lower, long long upper) {
        if (!node) return true;
        if (node->val <= lower || node->val >= upper) return false;
        return validate(node->left, lower, node->val)
            && validate(node->right, node->val, upper);
    }

public:
    bool isValidBST(TreeNode* root) {
        return validate(root, LLONG_MIN, LLONG_MAX);
    }
};''')


add(20, "栈匹配嵌套括号",
    "左括号开启一个尚未完成的结构，右括号必须关闭最近开启且同类型的结构，正符合栈的后进先出。",
    ["左括号压栈", "右括号检查栈非空且栈顶类型匹配后弹出", "扫描完要求栈为空"],
    "每个右括号只可能与最近未闭合左括号配对；局部匹配并弹栈维持剩余前缀的待闭合序列，最终为空即全部合法。",
    "只有类型相同还不够，顺序也必须正确；遇右括号时先判空；扫描结束有残余左括号同样非法。",
    "编译器语法检查、标签嵌套、路径回退和表达式求值都利用栈保存未完成上下文。",
r'''// ---------- Solution ----------
class Solution {
public:
    bool isValid(string s) {
        stack<char> openings;
        for (char c : s) {
            if (c == '(' || c == '[' || c == '{') {
                openings.push(c);
                continue;
            }
            if (openings.empty()) return false;
            const char expected = (c == ')') ? '(' : (c == ']') ? '[' : '{';
            if (openings.top() != expected) return false;
            openings.pop();
        }
        return openings.empty();
    }
};''')


add(155, "同步辅助栈：每层携带当前最小值",
    "每次 push 同时保存该位置为止的最小值，使 top 与 getMin 都只读取栈顶，不需要扫描。",
    ["push(value,min(value,旧最小值))", "pop 同时移除值与对应最小状态", "top/getMin 分别读取 pair 两个字段"],
    "栈中每个 pair 的 second 等于从底到该层所有值的最小值；入栈递推保持不变量，出栈自动恢复上一层状态。",
    "重复最小值必须逐层保存，否则弹出一个后会丢失最小信息；题目保证空栈上不会调用查询/弹出。",
    "用栈顶携带聚合状态可实现 MaxStack、括号深度、区间 gcd 栈和支持回滚的数据结构。",
r'''// ---------- Solution ----------
class Solution {
    stack<pair<int, int>> values;  // {实际值, 压入该值后的最小值}

public:
    void push(int val) {
        const int minimum = values.empty() ? val : min(val, values.top().second);
        values.push({val, minimum});
    }

    void pop() { values.pop(); }
    int top() { return values.top().first; }
    int getMin() { return values.top().second; }
};''')


add(739, "单调递减栈：等待下一个更大值",
    "栈保存尚未找到更暖日期的下标，且对应温度单调不增；当前更高温度可一次结算栈顶若干日期。",
    ["从左到右扫描日期", "当前温度更高时反复弹栈并填写距离", "把当前下标压栈等待未来结算"],
    "被弹下标之间没有更早的更高温度，否则早已被弹；当前日是它遇到的第一个严格更暖日。未弹出的日期最终答案保持 0。",
    "题目要求严格更高，相等不能弹；栈存下标才能计算天数；每个下标最多进出一次。",
    "下一个更大/更小元素、股票跨度和柱状图边界都通过单调栈批量结算待定元素。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        vector<int> answer(temperatures.size(), 0);
        stack<int> pending;
        for (int day = 0; day < static_cast<int>(temperatures.size()); ++day) {
            while (!pending.empty() && temperatures[day] > temperatures[pending.top()]) {
                const int previous = pending.top();
                pending.pop();
                answer[previous] = day - previous;
            }
            pending.push(day);
        }
        return answer;
    }
};''')


add(84, "单调递增栈：高度下降时结算矩形",
    "栈保存高度单调不降的柱下标；遇到更矮柱时，被弹柱的右边界确定，弹栈后的新栈顶确定其左边界。",
    ["扫描到 n，并在末尾使用高度 0 哨兵", "当前高度更低时反复弹出并计算宽度", "压入当前下标"],
    "柱 h 被弹时，栈顶之后到 i-1 的柱高都至少为 h，且两侧是首个更矮位置，所以此时得到以 h 为最低柱的最大矩形。",
    "宽度是 i-leftBoundary-1；末尾哨兵用于结算所有残留柱；栈存下标而不是只存高度。",
    "“找左右首个更小元素”可用于最大矩形、子数组最小值贡献和笛卡尔树构建。",
r'''// ---------- Solution ----------
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        stack<int> increasing;
        long long best = 0;
        const int n = static_cast<int>(heights.size());
        for (int i = 0; i <= n; ++i) {
            const int currentHeight = (i == n) ? 0 : heights[i];
            while (!increasing.empty() && heights[increasing.top()] > currentHeight) {
                const int height = heights[increasing.top()];
                increasing.pop();
                const int leftBoundary = increasing.empty() ? -1 : increasing.top();
                best = max(best, 1LL * height * (i - leftBoundary - 1));
            }
            increasing.push(i);
        }
        return static_cast<int>(best);
    }
};''')


add(46, "回溯：路径 + 选择集合",
    "排列的每一层选择一个尚未使用的元素放到当前位置；used 标记选择状态，path 保存当前前缀。",
    ["若 path 长度等于 n 就记录答案", "枚举所有未使用下标，做选择并递归", "递归返回后撤销 path 与 used"],
    "搜索树第 d 层枚举所有未出现在前缀中的元素，因此每条根到叶路径对应一个唯一排列，反之每个排列都有唯一选择序列。",
    "撤销顺序必须与选择对应；题目保证元素互异，若有重复值还需排序并做同层去重。",
    "回溯统一写成选择、递归、撤销；组合、子集、棋盘搜索只是在选择空间和剪枝条件上不同。",
r'''// ---------- Solution ----------
class Solution {
    vector<vector<int>> answer;
    vector<int> path;
    vector<char> used;

    void search(const vector<int>& nums) {
        if (path.size() == nums.size()) {
            answer.push_back(path);
            return;
        }
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            if (used[i]) continue;
            used[i] = true;
            path.push_back(nums[i]);
            search(nums);
            path.pop_back();       // 撤销本层选择，恢复进入递归前的状态
            used[i] = false;
        }
    }

public:
    vector<vector<int>> permute(vector<int>& nums) {
        answer.clear();
        path.clear();
        used.assign(nums.size(), false);
        search(nums);
        return answer;
    }
};''')


add(39, "组合回溯：起始下标控制去重，可重复选择",
    "候选数可无限次使用；递归参数 start 限定后续只能选择当前或更右的候选，从而消除排列顺序造成的重复。",
    ["排序以便剪枝", "从 start 枚举，选择 candidates[i] 后仍递归 i", "remaining 为 0 时记录，候选过大时停止"],
    "每个组合按非降顺序生成，因此只有一条搜索路径；所有合法多重集合都可按该顺序逐项选择而被覆盖。",
    "递归传 i 而非 i+1 才允许复用；官方候选值互异且为正，正数保证 remaining 剪枝成立。",
    "若每项只能用一次改传 i+1；若候选有重复则增加同层去重，这三者构成组合回溯常用变体。",
r'''// ---------- Solution ----------
class Solution {
    vector<vector<int>> answer;
    vector<int> path;

    void search(const vector<int>& candidates, int start, int remaining) {
        if (remaining == 0) {
            answer.push_back(path);
            return;
        }
        for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
            if (candidates[i] > remaining) break;
            path.push_back(candidates[i]);
            search(candidates, i, remaining - candidates[i]);  // i 可再次使用
            path.pop_back();
        }
    }

public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        answer.clear();
        path.clear();
        search(candidates, 0, target);
        return answer;
    }
};''')


add(78, "子集回溯：每个前缀本身就是答案",
    "按下标递增选择元素；当前 path 对应一个子集，应在进入每层时记录，然后继续尝试加入后续元素。",
    ["记录当前 path（包括空集）", "从 start 到末尾枚举下一元素", "选择后递归 i+1，再撤销"],
    "每个子集按元素原下标的递增序列唯一表示，搜索树恰好枚举所有这类序列，因此共有 2^n 个且无重复。",
    "不要只在叶子记录，否则会漏掉较短子集；递归传 i+1；输出规模本身就是 O(n·2^n)。",
    "子集是组合搜索的母题；加目标和、长度、重复元素处理或可行性剪枝即可得到多种组合问题。",
r'''// ---------- Solution ----------
class Solution {
    vector<vector<int>> answer;
    vector<int> path;

    void search(const vector<int>& nums, int start) {
        answer.push_back(path);
        for (int i = start; i < static_cast<int>(nums.size()); ++i) {
            path.push_back(nums[i]);
            search(nums, i + 1);
            path.pop_back();
        }
    }

public:
    vector<vector<int>> subsets(vector<int>& nums) {
        answer.clear();
        path.clear();
        search(nums, 0);
        return answer;
    }
};''')


add(22, "约束生成：只扩展仍可能合法的括号前缀",
    "合法前缀必须始终满足 usedClose<=usedOpen<=n；回溯只添加不破坏该条件的字符，无需生成后再过滤。",
    ["open<n 时可添加左括号", "close<open 时可添加右括号", "长度达到 2n 时记录"],
    "所有生成路径的每个前缀都合法，终点左右括号各 n 个；任意合法括号串的逐字符选择也始终满足两个条件，因此不会漏解。",
    "右括号条件必须是 close<open；到终点才记录；字符串 push 后要 pop 回退。",
    "构造型搜索应尽早编码前缀合法性；同理可生成 IP、表达式、受限序列和括号的多类型变体。",
r'''// ---------- Solution ----------
class Solution {
    vector<string> answer;
    string path;

    void search(int n, int open, int close) {
        if (static_cast<int>(path.size()) == 2 * n) {
            answer.push_back(path);
            return;
        }
        if (open < n) {
            path.push_back('(');
            search(n, open + 1, close);
            path.pop_back();
        }
        if (close < open) {
            path.push_back(')');
            search(n, open, close + 1);
            path.pop_back();
        }
    }

public:
    vector<string> generateParenthesis(int n) {
        answer.clear();
        path.clear();
        search(n, 0, 0);
        return answer;
    }
};''')


add(148, "自底向上链表归并排序",
    "归并排序适合链表，因为拆分和合并只改指针；自底向上依次合并长度 1、2、4... 的相邻段，可把额外空间降到 O(1)。",
    ["先统计链表长度", "每轮按 runSize 切出 left/right 两段", "归并后接回已排序前缀，并将 runSize 翻倍"],
    "第 k 轮开始前每个长度 runSize 的段已排序；两两归并后得到长度 2*runSize 的有序段，最终段长覆盖 n 即全局有序。",
    "split 必须断开链；merge 要返回合并段尾以便继续拼接；链表归并不需要随机访问。",
    "外部排序、大文件归并和迭代归并排序都使用“固定段长翻倍”的阶段不变量。",
r'''// ---------- Solution ----------
class Solution {
    static ListNode* split(ListNode* head, int size) {
        while (--size > 0 && head) head = head->next;
        if (!head) return nullptr;
        ListNode* second = head->next;
        head->next = nullptr;
        return second;
    }

    static pair<ListNode*, ListNode*> mergeRuns(ListNode* a, ListNode* b) {
        ListNode dummy;
        ListNode* tail = &dummy;
        while (a && b) {
            ListNode*& chosen = (a->val <= b->val) ? a : b;
            tail->next = chosen;
            chosen = chosen->next;
            tail = tail->next;
        }
        tail->next = a ? a : b;
        while (tail->next) tail = tail->next;
        return {dummy.next, tail};
    }

public:
    ListNode* sortList(ListNode* head) {
        int length = 0;
        for (ListNode* node = head; node; node = node->next) ++length;
        ListNode dummy(0, head);

        for (int runSize = 1; runSize < length; runSize *= 2) {
            ListNode* current = dummy.next;
            ListNode* sortedTail = &dummy;
            while (current) {
                ListNode* left = current;
                ListNode* right = split(left, runSize);
                current = split(right, runSize);
                auto [mergedHead, mergedTail] = mergeRuns(left, right);
                sortedTail->next = mergedHead;
                sortedTail = mergedTail;
            }
        }
        return dummy.next;
    }
};''')


add(143, "中点 + 反转后半段 + 交替合并",
    "目标顺序交替取原链表头、尾、次头、次尾；先把链表一分为二并反转后半段，尾部顺序就变成可顺向读取。",
    ["快慢指针找前半段尾并断链", "原地反转后半段", "从两段轮流摘一个节点交织"],
    "第一段保持 L0,L1...，反转第二段得到 Ln,Ln-1...；交替合并恰好产生题目顺序，且每个原节点使用一次。",
    "必须先断开两段以免成环；奇数长度让前半段多一个节点；函数原地修改且无返回值。",
    "拆分、局部变换、再归并是链表题常见三段式，例如回文判断、奇偶链表和拉链合并。",
r'''// ---------- Solution ----------
class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        ListNode* second = slow->next;
        slow->next = nullptr;  // 先断链，避免交织时产生环
        ListNode* reversed = nullptr;
        while (second) {
            ListNode* next = second->next;
            second->next = reversed;
            reversed = second;
            second = next;
        }

        ListNode* first = head;
        second = reversed;
        while (second) {
            ListNode* nextFirst = first->next;
            ListNode* nextSecond = second->next;
            first->next = second;
            second->next = nextFirst;
            first = nextFirst;
            second = nextSecond;
        }
    }
};''')


add(124, "树上任意路径 DP：全局双臂、向上单臂",
    "递归返回从当前节点向下选择一条分支的最大贡献；当前节点可同时连接左右正贡献形成完整路径，并更新全局答案。",
    ["空节点贡献 0", "左右贡献若为负则截断为 0", "用 node+left+right 更新答案，向父返回 node+max(left,right)"],
    "任意非空简单路径有唯一最高节点；在该节点会组合其左右最佳向下贡献。向父传播时只能选择一侧，否则路径会分叉。",
    "全局答案不能初始化为 0，否则全负树错误；路径不要求经过根；一个节点最多出现一次。",
    "直径、最大路径和、最长同值路径都共享“节点处可闭合两臂，父方向只能接一臂”的框架。",
r'''// ---------- Solution ----------
class Solution {
    int best = INT_MIN;

    int gain(TreeNode* node) {
        if (!node) return 0;
        const int left = max(0, gain(node->left));
        const int right = max(0, gain(node->right));
        best = max(best, node->val + left + right);
        return node->val + max(left, right);
    }

public:
    int maxPathSum(TreeNode* root) {
        best = INT_MIN;
        gain(root);
        return best;
    }
};''')


add(199, "层序遍历：每层最后一个节点",
    "从右侧观察时，每层最右节点可见；标准 BFS 按左后右入队，则该层处理的最后一个节点就是答案。",
    ["根入队", "固定当前层节点数逐个处理", "处理到该层最后一个节点时记录其值"],
    "BFS 每轮恰好覆盖同一深度的全部节点；按从左到右顺序处理，末节点的横向位置最右，因此逐层记录正确。",
    "空树返回空数组；层大小必须在入层时固定；也可用优先访问右子树的 DFS 首次到达每层。",
    "层视图、每层最大值、平均值和锯齿遍历都只是在 BFS 层内采用不同聚合规则。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> answer;
        if (!root) return answer;
        queue<TreeNode*> pending;
        pending.push(root);
        while (!pending.empty()) {
            const int width = static_cast<int>(pending.size());
            for (int i = 0; i < width; ++i) {
                TreeNode* node = pending.front();
                pending.pop();
                if (i == width - 1) answer.push_back(node->val);
                if (node->left) pending.push(node->left);
                if (node->right) pending.push(node->right);
            }
        }
        return answer;
    }
};''')
