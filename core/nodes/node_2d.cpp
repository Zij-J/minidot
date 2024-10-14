#include "node_2d.h"

Transform2D Node2D::local_to_global_transform() const {
    Transform2D object_transform_2d = local_transform;   // traverse back, update postion to global 
    const Node2D *now_parent = this;
    while ((now_parent = dynamic_cast<const Node2D *>(now_parent->get_parent())) != nullptr) {    // not direct inhertance checking(e.g. MeshInstance2D is Node2D), need dynamic_cast
        now_parent->local_transform *= object_transform_2d;   // parent transform affect object transform!
    }

    return object_transform_2d; // copy elision do things 
}