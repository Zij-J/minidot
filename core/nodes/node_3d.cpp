#include "node_3D.h"


void Node3D::set_local_transform(const Transform3D &setting_local_transform) {
    local_transform = setting_local_transform;
    _prograte_transform_change();
}


// traverse down, setting all dirty bits
void Node3D::_prograte_transform_change() {
    Vector<Node3D *> dfs_stack; // use pointers for C-ish fast vector
    dfs_stack.push_back(this);

    while (dfs_stack.size() != 0) {
        Node3D *now_node_3D = dfs_stack.pop_back();
        for (int i = children_cache.size() - 1; i >= 0; --i) { // leaves quene
            dfs_stack.push_back(children_cache[i]);
        }
        now_node_3D->is_global_transform_dirty = true;
    }
}