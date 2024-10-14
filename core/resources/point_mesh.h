// one point mesh (goods for partical system)

#ifndef POINT_MESH_H
#define POINT_MESH_H

#include <utility> // std::move
#include "../math/vector3.h"
#include "../math/color.h"
#include "mesh.h"


class PointMesh: public Mesh {    
public:
    Color color = {.r = 1, .g = 1, .b = 1};
    Vector3 offset;
    float size; // won't be scaled by parents

    PointMesh(float x = 0, float y = 0, float z = 0, float size = 5): offset{.x = x, .y = y, .z = z}, size(size) {}
    PointMesh(Color color, float x = 0, float y = 0, float z = 0, float size = 5): color(std::move(color)), offset{.x = x, .y = y, .z = z}, size(size) {}
    ~PointMesh() {}

    void draw(const Transform3D &final_transform) const;
 
    // Vector2I get_global_pos() {
        
    //     Node *parent =  
    // }

private:

};

#endif