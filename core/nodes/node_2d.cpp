#include "node_2d.h"



// traverse down, setting all dirty bits
void Node2D::_prograte_transform_change() {
    Vector<Node2D *> dfs_stack; // use pointers for C-ish fast vector
    dfs_stack.push_back(this);

    while (dfs_stack.size() != 0) {
        Node2D *now_node_2d = dfs_stack.pop_back();
        for (int i = children_cache.size() - 1; i >= 0; --i) { // leaves quene
            dfs_stack.push_back(children_cache[i]);
        }
        now_node_2d->is_global_transform_dirty = true;
    }
}