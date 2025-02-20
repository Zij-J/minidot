#include "box_mesh.h"


BoxMesh::BoxMesh(float side_length): side_lengths{.x = side_length, .y = side_length, .z = side_length} {
    _update_cube_strip_by_length();
}

BoxMesh::BoxMesh(Color color, float x_length, float y_length, float z_length)
: color(std::move(color)), 
side_lengths{.x = x_length, .y = y_length, .z = z_length} {
    _update_cube_strip_by_length();
}

void BoxMesh::set_side_length(float x_length, float y_length, float z_length) {
    side_lengths = (Vector3){.x = x_length, .y = y_length, .z = z_length};
    _update_cube_strip_by_length();
}

void BoxMesh::draw(const Transform3D &final_transform) const {

    Vector3 final_cube_strip[8];
    for (int i = 0; i < 8; ++i) {
        final_cube_strip[i] = final_transform * cube_strip[i];
    }

    // Vector3 cube_strip[14] = {
    //     Vector3{-1.f, 1.f, 1.f},     // Front-top-left: 5
    //     Vector3{1.f, 1.f, 1.f},      // Front-top-right: 3
    //     Vector3{-1.f, -1.f, 1.f},    // Front-bottom-left: 0
    //     Vector3{1.f, -1.f, 1.f},     // Front-bottom-right: 1
    //     Vector3{1.f, -1.f, -1.f},    // Back-bottom-right: 2
    //     Vector3{1.f, 1.f, 1.f},      // Front-top-right: 3
    //     Vector3{1.f, 1.f, -1.f},     // Back-top-right: 4
    //     Vector3{-1.f, 1.f, 1.f},     // Front-top-left: 5
    //     Vector3{-1.f, 1.f, -1.f},    // Back-top-left: 6
    //     Vector3{-1.f, -1.f, 1.f},    // Front-bottom-left: 0
    //     Vector3{-1.f, -1.f, -1.f},   // Back-bottom-left: 7
    //     Vector3{1.f, -1.f, -1.f},    // Back-bottom-right: 2
    //     Vector3{-1.f, 1.f, -1.f},    // Back-top-left: 6
    //     Vector3{1.f, 1.f, -1.f}      // Back-top-right: 4
    // };


    // render: first normal & color then vertices 
    glBegin(GL_TRIANGLE_STRIP);  
        glColor3f(color.r, color.g, color.b);
        
        glNormal3f(cube_nomal[5].x, cube_nomal[5].y, cube_nomal[5].z); 
        glVertex3f(final_cube_strip[5].x, final_cube_strip[5].y, final_cube_strip[5].z); 
        glNormal3f(cube_nomal[3].x, cube_nomal[3].y, cube_nomal[3].z); 
        glVertex3f(final_cube_strip[3].x, final_cube_strip[3].y, final_cube_strip[3].z); 
        for (int i = 0; i < 7; ++i) {
            glNormal3f(cube_nomal[i].x, cube_nomal[i].y, cube_nomal[i].z); 
            glVertex3f(final_cube_strip[i].x, final_cube_strip[i].y, final_cube_strip[i].z); 
        }
        glNormal3f(cube_nomal[0].x, cube_nomal[0].y, cube_nomal[0].z); 
        glVertex3f(final_cube_strip[0].x, final_cube_strip[0].y, final_cube_strip[0].z); 
        glNormal3f(cube_nomal[7].x, cube_nomal[7].y, cube_nomal[7].z); 
        glVertex3f(final_cube_strip[7].x, final_cube_strip[7].y, final_cube_strip[7].z); 
        glNormal3f(cube_nomal[1].x, cube_nomal[2].y, cube_nomal[2].z); 
        glVertex3f(final_cube_strip[1].x, final_cube_strip[2].y, final_cube_strip[2].z); 
        glNormal3f(cube_nomal[6].x, cube_nomal[6].y, cube_nomal[6].z); 
        glVertex3f(final_cube_strip[6].x, final_cube_strip[6].y, final_cube_strip[6].z); 
        glNormal3f(cube_nomal[4].x, cube_nomal[4].y, cube_nomal[4].z); 
        glVertex3f(final_cube_strip[4].x, final_cube_strip[4].y, final_cube_strip[4].z); 

        
    glEnd();
}



void BoxMesh::_update_cube_strip_by_length() {
    for (int i = 0; i < 8; ++i) {
        cube_strip[i].x *= side_lengths.x / 2.0f;
        cube_strip[i].y *= side_lengths.y / 2.0f;
        cube_strip[i].z *= side_lengths.z / 2.0f;

        cube_nomal[i].x *= side_lengths.x; // normal need to update too, do normalize in shaders
        cube_nomal[i].y *= side_lengths.y;
        cube_nomal[i].z *= side_lengths.z;
    }
}
