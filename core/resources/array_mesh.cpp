#include "array_mesh.h"
#include <GL/glut.h>
#include <GL/gl.h>

void ArrayMesh::draw(const Transform3D &final_transform) const {
    // draw only when has triangle
    if (points.size() < 3) {
        return ;
    }

    glBegin(GL_TRIANGLES);
    Vector3 final_point;
    glColor3f(1, 1, 1);
    for (int i = 0; i < points.size(); ++i) {
        // color & normal
        if (i < colors.size()) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
        }
        if (i < normals.size()) {
            glNormal3f(normals[i].x, normals[i].y, normals[i].z);
        }

        // vertices
        final_point = final_transform * points[i];
        glVertex3f(final_point.x, final_point.y, final_point.z);
    }
    glEnd();
}