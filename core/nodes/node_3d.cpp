#include "node_3D.h"
#include "../math/math_defs.h"
#include <math.h>


void Node3D::on_tree_entered() {
    // const_cast hacks(Godot uses too), be sure ALL Node3D aren't const!
    parent_cache = const_cast<Node3D *>(dynamic_cast<const Node3D *>(this->get_parent()));  // not direct inhertance checking(e.g. MeshInstance3D is Node3D), need dynamic_cast
    if (parent_cache != nullptr) {  // not root
        parent_cache->children_cache.push_back(this);   // `dynamic_cast` children too time consuming? how about rebuild the tree again, Node3D only tree!
    }
}


void Node3D::translate(float x, float y, float z) {
    local_transform.origin_offset.x += x;
    local_transform.origin_offset.y += y;
    local_transform.origin_offset.z += z;

    _prograte_transform_change();
}


// counter clock wise in x axis
void Node3D::rotate_x(float degree) {
    float radius = degree / 360.0f * Math_TAU;
    Vector3 rotate_x = {.x = 1.0f, 0.0f, 0.0f};
    Vector3 rotate_y = {.x = 0, .y = cos(radius), .z = sin(radius)};
    Vector3 rotate_z = {.x = 0, .y = -sin(radius), .z = cos(radius)};
    local_transform.basis_y.transformed_by(rotate_x, rotate_y, rotate_z);
    local_transform.basis_z.transformed_by(rotate_x, rotate_y, rotate_z);

    _prograte_transform_change();
}
// counter clock wise in z axis
void Node3D::rotate_y(float degree) {
    float radius = degree / 360.0f * Math_TAU;
    Vector3 rotate_x = {.x = cos(radius), .y = 0.0f, .z = -sin(radius)};
    Vector3 rotate_y = {.x = 0.0f, 1.0f, 0.0f};
    Vector3 rotate_z = {.x = sin(radius), .y = 0.0f, .z = cos(radius)};
    local_transform.basis_x.transformed_by(rotate_x, rotate_y, rotate_z);
    local_transform.basis_z.transformed_by(rotate_x, rotate_y, rotate_z);

    _prograte_transform_change();
}
// counter clock wise in z axis
void Node3D::rotate_z(float degree) {
    float radius = degree / 360.0f * Math_TAU;
    Vector3 rotate_x = {.x = cos(radius), .y = sin(radius), .z = 0.0f};
    Vector3 rotate_y = {.x = -sin(radius), .y = cos(radius), .z = 0.0f};
    Vector3 rotate_z = {.x = 0.0f, 0.0f, 1.0f};
    local_transform.basis_x.transformed_by(rotate_x, rotate_y, rotate_z);
    local_transform.basis_y.transformed_by(rotate_x, rotate_y, rotate_z);

    _prograte_transform_change();
}

void Node3D::scale(float times) {
    local_transform.basis_x *= times;
    local_transform.basis_y *= times;
    local_transform.basis_z *= times;

    _prograte_transform_change();
}

// traverse down, setting all dirty bits
void Node3D::_prograte_transform_change() {
    Vector<Node3D *> dfs_stack; // use pointers for C-ish fast vector
    dfs_stack.push_back(this);

    while (dfs_stack.size() != 0) {
        Node3D *now_node_3d = dfs_stack.pop_back();
        for (int i = now_node_3d->children_cache.size() - 1; i >= 0; --i) { // leaves quene
            dfs_stack.push_back(now_node_3d->children_cache[i]);
        }
        now_node_3d->is_global_transform_dirty = true;
    }
}