// node with 2D position(relative to parent), handle transforms

#ifndef NODE_2D_H
#define NODE_2D_H

#include "node.h"
#include "viewport.h"
#include "../math/vector2i.h"
#include "../math/transform_2d.h"

class Node2D: public Node {    
public:
    Node2D(float x = 0, float y = 0): 
    local_transform{.basis_x = {.x = 1, .y = 0}, .basis_y = {.x = 0, .y = 1}, .origin_offset = {.x = x, .y = y}},
    global_transform(local_to_global_transform()) {
        tree_entered.add_listener<Node2D, on_tree_entered>(this);
    }
    ~Node2D() {}

    const Transform2D &get_object_transform() {
        if (is_global_transform_dirty) {
            global_transform = local_to_global_transform();
            is_global_transform_dirty = false;
        }
        return global_transform;
    }

    void on_tree_entered() {
        is_global_transform_dirty = true;
    }

private:
    bool is_global_transform_dirty = false;
    Transform2D local_transform;
    Transform2D global_transform;

    Transform2D local_to_global_transform() const;
};


#endif