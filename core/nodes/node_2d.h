// node with 2D position(relative to parent), handle transforms

#ifndef NODE_2D_H
#define NODE_2D_H

#include "node.h"
#include "../math/vector2i.h"
#include "../math/transform_2d.h"

class Node2D: public Node {    
public:

    Node2D(float x = 0, float y = 0): local_transform((Transform2D){.basis_x = {.x = 1, .y = 0}, .basis_y = {.x = 0, .y = 1}, .origin_offset = {.x = x, .y = y}}) { // copy elision do things
        tree_entered.add_listener<Node2D, &Node2D::on_tree_entered>(this);
    }
    ~Node2D() {}
    
    // or so called `get_global_transform`
    const Transform2D &get_object_transform() {
        if (is_global_transform_dirty) {
            // Vector<Node2D *> parent_stack;
            // Node2D *now_node = this;
            // while (now_node != nullptr && now_node->is_global_transform_dirty) {   // dynamic cast, == nullptr if failed(no Node2D parent)
            //     parent_stack.push_back(now_node);
            //     now_node = now_node->parent_cache;
            // }

            // now_node = parent_stack.pop_back(); // start for root, traverse back, update postion to global 
            // now_node->global_transform = now_node->local_transform;   
            // now_node->is_global_transform_dirty = false; // root may transform too, must handle dirty
            // while (parent_stack.size() != 0) {
            //     now_node = parent_stack.pop_back();
            //     now_node->global_transform = now_node->parent_cache->global_transform * now_node->local_transform;
            //     now_node->is_global_transform_dirty = false;
            // }

            if (parent_cache == nullptr) { // readiblity increase a lot! back to iterative when not fast enough
                global_transform = local_transform;
            } else {
                global_transform = parent_cache->get_object_transform() * local_transform;
            }
            is_global_transform_dirty = false;
        }
        return global_transform;
    }

    void on_tree_entered();

    void translate(float x, float y);
    void rotate(float degree);


protected:
    Node2D *parent_cache = nullptr;
    Vector<Node2D *> children_cache;

private:
    bool is_global_transform_dirty = true;
    Transform2D local_transform;  
    Transform2D global_transform; // let it be uninit/default init, ture init only when `get_object_transform` 

    void _prograte_transform_change();
};


#endif