#ifndef CAMERA_3D_H
#define CAMERA_3D_H

class Viewport; // for mutual Dependencies

#include "node_3d.h"


// look at: -z(basis_z), up: +y(basis_y)
class Camera3D: public Node3D {
public:

    Camera3D();
    Camera3D(float x, float y, float z);
    ~Camera3D();

    void on_tree_entered();

private:
    Viewport *viewport = nullptr;
    bool is_entered_tree = false;
};


#endif