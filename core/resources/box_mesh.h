// 8 point mesh, forming a box
// algorithm adopted from: https://stackoverflow.com/questions/28375338/cube-using-single-gl-triangle-strip, speedy array, I like it

#ifndef BOX_MESH_H
#define BOX_MESH_H

#include <utility> // std::move
#include "../math/vector3.h"
#include "../math/color.h"
#include "mesh.h"


class BoxMesh: public Mesh {    
public:
    Color color = {.r = 1, .g = 1, .b = 1};

    ~BoxMesh() {}

    BoxMesh(float side_length = 1.0f);
    BoxMesh(Color color, float x_length = 1.0f, float y_length = 1.0f, float z_length = 1.0f); 

    void set_side_length(float x_length, float y_length, float z_length);
    void draw(const Transform3D &final_transform) const;
 

private:
    Vector3 side_lengths;

    Vector3 cube_strip[8] = {
        Vector3{-1.f, -1.f, 1.f},    // Front-bottom-left: 0
        Vector3{1.f, -1.f, 1.f},     // Front-bottom-right: 1
        Vector3{1.f, -1.f, -1.f},    // Back-bottom-right: 2
        Vector3{1.f, 1.f, 1.f},      // Front-top-right: 3
        Vector3{1.f, 1.f, -1.f},     // Back-top-right: 4
        Vector3{-1.f, 1.f, 1.f},     // Front-top-left: 5
        Vector3{-1.f, 1.f, -1.f},    // Back-top-left: 6
        Vector3{-1.f, -1.f, -1.f},   // Back-bottom-left: 7
    };

    void _update_cube_strip_by_length();
};

#endif