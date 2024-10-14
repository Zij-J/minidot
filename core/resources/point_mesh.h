// one point mesh (goods for partical system)

#ifndef POINT_MESH_H
#define POINT_MESH_H

#include "../config/project_setting.h"
#include "../math/vector3.h"
#include "mesh.h"
#include <GL/glut.h>
#include <GL/gl.h>

class PointMesh: public Mesh {    
public:

    PointMesh(float x = 0, float y = 0, float z = 0): point{.x = x, .y = y, .z = z} {}
    ~PointMesh() {}

    void draw(const Transform3D &final_transform) const {
        Vector3 final_point = final_transform * point;

        glPointSize(5.0);  // 在畫 GL_POINT 才有效
        glBegin(GL_POINTS);
            glColor3f(1.0, 0.0, 0.0); //r
            glVertex3f(final_point.x, final_point.y, final_point.z);  // lower left
        glEnd();
    }
 
    // Vector2I get_global_pos() {
        
    //     Node *parent =  
    // }

private:

    Vector3 point;
};

#endif