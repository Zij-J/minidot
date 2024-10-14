#include "line_mesh.h"


void LineMesh::draw(const Transform3D &final_transform) const {
    Vector3 final_point0 = final_transform * point0;
    Vector3 final_point1 = final_transform * point1;

    glLineWidth(width);
    glBegin(GL_LINES);
        glColor3f(color0.r, color0.g, color0.b);
        glVertex3f(final_point0.x, final_point0.y, final_point0.z);  // lower left
        glColor3f(color1.r, color1.g, color1.b);
        glVertex3f(final_point1.x, final_point1.y, final_point1.z);  // lower left
    glEnd();
}