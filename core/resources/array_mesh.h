// many points mesh (need >= 3 to work)

#ifndef ARRAY_MESH_H
#define ARRAY_MESH_H

#include "../math/vector3.h"
#include "../math/color.h"
#include "../templates/vector.h"
#include "mesh.h"


class ArrayMesh: public Mesh {    
public:

    Vector<Color> colors;
    Vector<Vector3> points;

    ArrayMesh() {} // must push >= 3 points after construct, otherwise won't be drawn
    ~ArrayMesh() {}

    void draw(const Transform3D &final_transform) const;


private:

};

#endif