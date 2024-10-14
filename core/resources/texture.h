// for loading images

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../thirdparty/stb/stb_image.h"
#include "../math/transform_2d.h"
#include "../math/vector2i.h"
#include <GL/glut.h>
#include <GL/gl.h>

class Texture {    
public:

    ~Texture() {}

    Texture(const char *image_file_path, int width, int height);
    void draw(const Transform2D &final_transform); // viewport + object transform done, not const for "dirty" mechanism

    void calculate_vertices();

private:
    Vector2i size;
    unsigned int texture = 0;

    bool is_vertices_dirty = true;
    Vector2 vertices[4];    // from left-up, clock-wise
};

#endif