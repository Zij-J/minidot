// just a base class for meshes(containing points)

#ifndef MESH_H
#define MESH_H

#include "../math/transform_3d.h"
#include <GL/glut.h>
#include <GL/gl.h>

class Mesh {    
public:

    Mesh() {}
    virtual ~Mesh() {}

    virtual void draw(const Transform3D &final_transform) const = 0; // viewport + object transform done
};

#endif