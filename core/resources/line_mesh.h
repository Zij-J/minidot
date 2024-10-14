// 2 point mesh (Line2D of Godot, but can (maybe) be in 3D)

#ifndef LINE_MESH_H
#define LINE_MESH_H

#include "../config/project_setting.h"
#include "../math/vector3.h"
#include "mesh.h"
#include <GL/glut.h>
#include <GL/gl.h>

class LineMesh: public Mesh {    
public:

    LineMesh(Vector3 point0, Vector3 point1): point0(std::move(point0)), point1(std::move(point1)) {}
    ~LineMesh() {}

    void draw(const Transform3D &final_transform) const {
        Vector3 final_point0 = final_transform * point0;
        Vector3 final_point1 = final_transform * point1;

        glLineWidth(5.0);
        glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0); //r
            glVertex3f(final_point0.x, final_point0.y, final_point0.z);  // lower left
            glVertex3f(final_point1.x, final_point1.y, final_point1.z);  // lower left
        glEnd();
    }
 
    // Vector2I get_global_pos() {
        
    //     Node *parent =  
    // }

private:

    Vector3 point0;
    Vector3 point1;
};

#endif