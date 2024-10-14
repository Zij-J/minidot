#include "point_mesh.h"
#include <GL/glut.h>
#include <GL/gl.h>

void PointMesh::draw(const Transform3D &final_transform) const {
    Vector3 final_point = final_transform * offset;

    glPointSize(size);  // usable only in GL_POINT
    glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b); //r
        glVertex3f(final_point.x, final_point.y, final_point.z);  // lower left
    glEnd();
}