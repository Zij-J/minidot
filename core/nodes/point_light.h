#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

class ServerDrawingObject; // for mutual Dependencies (forward declaration)

#include "node_3d.h"
#include "../math/color.h"


class PointLight: public Node3D {
public:

    PointLight();
    PointLight(float x, float y, float z, Color color, 
                float ambient = 0.3, float diffuse = 1, float specular = 1);
    ~PointLight();

    void on_tree_entered();

    void draw_light();

private:
    static int total_light_count;

    float ambient;
    float diffuse;
    float specular;
    Color color = {.r = 1, .g = 1, .b = 1};
    int light_index = -1;

    const ServerDrawingObject *server_light; // RenderServerDrawingObject
};


#endif