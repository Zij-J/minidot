// node with a texture picture, anchored in middle

#ifndef SPRITE_2D_H
#define SPRITE_2D_H

#include "node_2d.h"
#include "../resources/texture.h"

class Sprite2D: public Node2D {
public:
    Texture *texture;

    Sprite2D(int x, int y, Texture *texture): Node2D(x, y), texture(texture) {}
    ~Sprite2D() {}

private:
};



#endif