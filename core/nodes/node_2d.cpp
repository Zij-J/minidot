#include "node_2d.h"
#include "../math/math_defs.h"
#include <math.h>

void Node2D::on_tree_entered() {
    // const_cast hacks(Godot uses too), be sure ALL Node2D aren't const!
    parent_cache = const_cast<Node2D *>(dynamic_cast<const Node2D *>(this->get_parent()));  // not direct inhertance checking(e.g. MeshInstance2D is Node2D), need dynamic_cast
    if (parent_cache != nullptr) {  // not root
        parent_cache->children_cache.push_back(this);   // `dynamic_cast` children too time consuming? how about rebuild the tree again, Node2D only tree!
    }
}


void Node2D::translate(float x, float y) {
    local_transform.origin_offset.x += x;
    local_transform.origin_offset.y += y;

    _prograte_transform_change();
}

// counter clock wise(+y up), clock wise(+y down)
void Node2D::rotate(float degree) {
    float radius = degree / 360.0f * Math_TAU;
    Vector2 rotate_x = {.x = cos(radius), .y = sin(radius)};
    Vector2 rotate_y = {.x = -sin(radius), .y = cos(radius)};
    local_transform.basis_x.transformed_by(rotate_x, rotate_y);
    local_transform.basis_y.transformed_by(rotate_x, rotate_y);

    _prograte_transform_change();
}


// traverse down, setting all dirty bits
void Node2D::_prograte_transform_change() {
    Vector<Node2D *> dfs_stack; // use pointers for C-ish fast vector
    dfs_stack.push_back(this);

    while (dfs_stack.size() != 0) {
        Node2D *now_node_2d = dfs_stack.pop_back();
        for (int i = now_node_2d->children_cache.size() - 1; i >= 0; --i) { // leaves quene
            dfs_stack.push_back(now_node_2d->children_cache[i]);
        }
        now_node_2d->is_global_transform_dirty = true;
    }
}