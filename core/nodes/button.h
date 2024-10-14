// node with a texture picture, anchored in middle

#ifndef BUTTON_H
#define BUTTON_H

#include "node_2d.h"
#include "../resources/texture.h"

class Button: public Node2D {
public:
    Button(int x, int y): Node2D(x, y) {}
    ~Button() {}

private:
};



#endif