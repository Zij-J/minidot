// for windows transform (from camera if has)

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "node.h"
#include "../math/transform_2d.h"
#include "../math/transform_3d.h"
#include "../config/project_setting.h"


class Viewport: public Node {    
public:

    Viewport() {}   
    ~Viewport() {}

    const Transform2D &get_canva_transform() const {
        return canva_transform;
    }

    // default: orthogonal projection, whole design width/height
    const Transform3D &get_view_transform() const {
        return view_transform;
    }
    
private:    

    Transform2D canva_transform = {
        .basis_x = {.x = 2.0f / static_cast<float>(ProjectSetting::get_singleton().design_width), .y = 0},
        .basis_y = {.x = 0, .y = -2.0f / static_cast<float>(ProjectSetting::get_singleton().design_height)}, // y is flipped!
        .origin_offset = {.x = -1, .y = 1},
    };

    Transform3D view_transform = {
        .basis_x = {.x = 2.0f / static_cast<float>(ProjectSetting::get_singleton().design_width), .y = 0, .z = 0},
        .basis_y = {.x = 0, .y = -2.0f / static_cast<float>(ProjectSetting::get_singleton().design_height), .z = 0}, // y is flipped!
        .basis_z = {.x = 0, .y = 0, .z = 1},
        .origin_offset = {.x = -1, .y = 1, .z = 0},
    };
};


#endif