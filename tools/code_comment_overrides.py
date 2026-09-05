"""High-touch C++ implementation comment overrides.

These overrides keep algorithm code unchanged while adding learner-facing
comments next to the lines that carry the core invariant, state transition,
update order, or boundary semantics.

Add entries serially after reviewing each problem. `refined_data.py` merges this
layer after pedagogy overrides so `tools/gen_all.py` preserves the comments.
"""


CODE_COMMENT_OVERRIDES = {
    133: r'''// ---------- Solution ----------
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

                // 无论 neighbor 是第一次还是第 N 次被遇到，这条原图边都必须被复制。
                // 两端都通过 cloneOf 查找，保证共享邻居仍然指向同一个克隆对象。
                cloneOf[original]->neighbors.push_back(cloneOf[neighbor]);
            }
        }

        // 返回的是全新的克隆起点；它与原节点地址不同，但从该节点可到达的拓扑完全一致。
        return cloneOf[node];
    }
};''',
}
