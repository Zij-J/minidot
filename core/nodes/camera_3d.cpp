#include "camera_3d.h"
#include "viewport.h"
#include "../error/error_handler.h"
#include <typeinfo> // `typeid` keyword for C# `is` like behavior

Camera3D::Camera3D() {
    tree_entered.add_listener<Camera3D, on_tree_entered>(this);
}
Camera3D::Camera3D(float x, float y, float z)
: Node3D(x, y, z) {
    tree_entered.add_listener<Camera3D, on_tree_entered>(this);
}

Camera3D::~Camera3D() {
    if (is_entered_tree) {
        viewport->remove_camera_3d(this);
    }
}

void Camera3D::on_tree_entered() {
    is_entered_tree = true;

    const Node *now_parent = parent_cache; // get closest viewport, register to it
    while (now_parent != nullptr) {
        if (typeid(*now_parent).hash_code() == typeid(Viewport).hash_code()) {
            viewport = const_cast<Viewport *>(static_cast<const Viewport *>(now_parent)); // hack: const cast, Viewport must not be const!
            viewport->register_camera_3d(this);
            break;
        }
        now_parent = now_parent->get_parent();
    }

    ErrorHandler_FATAL_IF(viewport == nullptr, __FILE__, __LINE__); // MUST has viewport to be added in tree
}
