"""Composed high-touch C++ implementation comment overrides.

The historical monolithic registry is kept in code_comment_overrides_base.py.
This composition layer owns later corrections without requiring generated
solution.cpp files to be edited directly.
"""

from code_comment_overrides_base import CODE_COMMENT_OVERRIDES as _BASE_OVERRIDES


CODE_COMMENT_OVERRIDES = dict(_BASE_OVERRIDES)

# LC-133 was refined after the original monolithic registry was written.
# Keep the improved explanation canonical here so gen_all.render() reproduces
# the checked-in solution.cpp byte-for-byte instead of reporting generated drift.
CODE_COMMENT_OVERRIDES[133] = r'''// ---------- Solution ----------
class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;

        // cloneOf 同时承担两个角色：
        // 1) visited：key 已存在表示这个原节点已经被发现；
        // 2) identity map：value 是该原节点唯一对应的克隆对象。
        // 图中同一个节点可能从多条边再次到达，因此不能“每见一次就 new 一次”。
        unordered_map<Node*, Node*> cloneOf;

        // 起点必须先登记映射再入队。
        // 这样后续若沿环重新遇到起点，会直接复用这个克隆，而不会再创建第二个对象。
        cloneOf[node] = new Node(node->val);
        queue<Node*> pending;
        pending.push(node);

        while (!pending.empty()) {
            Node* original = pending.front();
            pending.pop();

            for (Node* neighbor : original->neighbors) {
                // 只有第一次发现 neighbor 时才创建克隆并安排后续遍历。
                // “先写 cloneOf，再入队”很重要：从这一行开始，neighbor 就已经被视为 visited；
                // 即使它在真正出队前又被另一条边遇到，也不会重复创建或重复入队。
                if (!cloneOf.count(neighbor)) {
                    cloneOf[neighbor] = new Node(neighbor->val);
                    pending.push(neighbor);
                }

                // 复制原图边 original—neighbor。被接入的是 cloneOf[neighbor]，它是已存在的克隆对象：
                // 执行到本行时 cloneOf.count(neighbor) 必为 1——上面 if 要么刚刚首次创建它，
                // 要么 neighbor 早已在映射里而 if 跳过。因此 push 的绝不是原指针 neighbor 本身，
                // 也不会在这里再 new。于是原图里被多条边共享的同一个 neighbor，在克隆图里始终
                // 映射到同一个克隆对象（起点同理：循环外已登记，作为 neighbor 出现时 count 同样为 1）。
                //
                // 上面 if 守“对象身份”：一个原节点只 new 一次、只入队一次，重复到达时 if 会跳过；
                // 本行守“边的复制”：original 的每条邻接项都要在克隆图里重建一次。
                // if 可跳过，但本行不可跳——否则会漏掉 neighbor 已存在时才出现的那条反向边
                // （例如处理 original=2 时复制 2—1，而 1—2 早在处理 original=1 时已复制）。
                cloneOf[original]->neighbors.push_back(cloneOf[neighbor]);
            }
        }

        // 返回的是全新的克隆起点；它与原节点地址不同，但从该节点可到达的拓扑完全一致。
        return cloneOf[node];
    }
};'''
