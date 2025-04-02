// 6 surface mesh, forming a box
// adapted from https://stackoverflow.com/questions/34118650/how-to-draw-a-cube-in-opengl

#ifndef BOX_MESH_H
#define BOX_MESH_H

#include <utility> // std::move
#include "../math/vector3.h"
#include "../math/color.h"
#include "mesh.h"


class BoxMesh: public Mesh {    
public:
    Color color = {.r = 1, .g = 1, .b = 1};

    ~BoxMesh() {}

    BoxMesh(float side_length = 1.0f);
    BoxMesh(Color color, float x_length = 1.0f, float y_length = 1.0f, float z_length = 1.0f); 

    void set_side_length(float x_length, float y_length, float z_length);
    void draw(const Transform3D &final_transform) const;
 

private:
    Vector3 side_lengths;

    /* 8 point polycube, lighting result isn't satisfying because of bad normals
    // algorithm adopted from https://stackoverflow.com/questions/28375338/cube-using-single-gl-triangle-strip, speedy array, I like it
    // normal adopted from https://doc.babylonjs.com/features/featuresDeepDive/mesh/creation/custom/vertexNormals

    Vector3 cube_strip[8] = {
        Vector3{-1.f, -1.f, 1.f},    // Front-bottom-left: 0
        Vector3{1.f, -1.f, 1.f},     // Front-bottom-right: 1
        Vector3{1.f, -1.f, -1.f},    // Back-bottom-right: 2
        Vector3{1.f, 1.f, 1.f},      // Front-top-right: 3
        Vector3{1.f, 1.f, -1.f},     // Back-top-right: 4
        Vector3{-1.f, 1.f, 1.f},     // Front-top-left: 5
        Vector3{-1.f, 1.f, -1.f},    // Back-top-left: 6
        Vector3{-1.f, -1.f, -1.f},   // Back-bottom-left: 7
    },

    Vector3 cube_nomal[8] = {
        Vector3{-1.f, -1.f, 1.f},    // Front-bottom-left: 0
        Vector3{1.f, -1.f, 1.f},     // Front-bottom-right: 1
        Vector3{1.f, -1.f, -1.f},    // Back-bottom-right: 2
        Vector3{1.f, 1.f, 1.f},      // Front-top-right: 3
        Vector3{1.f, 1.f, -1.f},     // Back-top-right: 4
        Vector3{-1.f, 1.f, 1.f},     // Front-top-left: 5
        Vector3{-1.f, 1.f, -1.f},    // Back-top-left: 6
        Vector3{-1.f, -1.f, -1.f},   // Back-bottom-left: 7
    },
    */

   Vector3 cube_vertices[24] = { // Define vertices in counter-clockwise (CCW) order with normal pointing out
        // Top face (y = 1.0f)
        Vector3{1.0f, 1.0f, -1.0f},
        Vector3{-1.0f, 1.0f, -1.0f},
        Vector3{-1.0f, 1.0f, 1.0f},
        Vector3{1.0f, 1.0f, 1.0f},

        // Bottom face (y = -1.0f)
        Vector3{1.0f, -1.0f, 1.0f},
        Vector3{-1.0f, -1.0f, 1.0f},
        Vector3{-1.0f, -1.0f, -1.0f},
        Vector3{1.0f, -1.0f, -1.0f},

        // Front face  (z = 1.0f)
        Vector3{1.0f, 1.0f, 1.0f},
        Vector3{-1.0f, 1.0f, 1.0f},
        Vector3{-1.0f, -1.0f, 1.0f},
        Vector3{1.0f, -1.0f, 1.0f},

        // Back face (z = -1.0f)
        Vector3{1.0f, -1.0f, -1.0f},
        Vector3{-1.0f, -1.0f, -1.0f},
        Vector3{-1.0f, 1.0f, -1.0f},
        Vector3{1.0f, 1.0f, -1.0f},

        // Left face (x = -1.0f)
        Vector3{-1.0f, 1.0f, 1.0f},
        Vector3{-1.0f, 1.0f, -1.0f},
        Vector3{-1.0f, -1.0f, -1.0f},
        Vector3{-1.0f, -1.0f, 1.0f},

        // Right face (x = 1.0f)
        Vector3{1.0f, 1.0f, -1.0f},
        Vector3{1.0f, 1.0f, 1.0f},
        Vector3{1.0f, -1.0f, 1.0f},
        Vector3{1.0f, -1.0f, -1.0f},
    };

    Vector3 cube_nomal[6] = {
        Vector3{0.0f, 1.0f, 0.0f},  // Top face (y = 1.0f)
        Vector3{0.0f, -1.0f, 0.0f}, // Bottom face (y = -1.0f)
        Vector3{0.0f, 0.0f, 1.0f},  // Front face  (z = 1.0f)
        Vector3{0.0f, 0.0f, -1.0f}, // Back face (z = -1.0f)
        Vector3{-1.0f, 0.0f, 0.0f}, // Left face (x = -1.0f)
        Vector3{1.0f, 0.0f, 0.0f},  // Right face (x = 1.0f)
    };

    void _update_cube_strip_by_length();
};

#endif