// 2 point mesh (Line2D of Godot, but can (maybe) be in 3D)

#ifndef LINE_MESH_H
#define LINE_MESH_H

#include <utility> // std::move
#include "../math/vector3.h"
#include "../math/color.h"
#include "mesh.h"


class LineMesh: public Mesh {    
public:
    Color color0 = {.r = 1, .g = 1, .b = 1};
    Color color1 = color0; // same color for point1 if not assign color1
    Vector3 point0;
    Vector3 point1;
    float width; // sadly can't scale with parents


    LineMesh(Vector3 point0, Vector3 point1, float width = 5): point0(std::move(point0)), point1(std::move(point1)), width(width) {}
    LineMesh(Color color0, Vector3 point0, Vector3 point1, float width = 5): 
    color0(std::move(color0)),
    point0(std::move(point0)), point1(std::move(point1)),
    width(width) {} 
    LineMesh(Color color0, Color color1, Vector3 point0, Vector3 point1, float width = 5): 
    color0(std::move(color0)), color1(std::move(color1)),
    point0(std::move(point0)), point1(std::move(point1)),
    width(width) {}

    ~LineMesh() {}

    void draw(const Transform3D &final_transform) const;
 
    // Vector2I get_global_pos() {
        
    //     Node *parent =  
    // }

private:

};

#endif