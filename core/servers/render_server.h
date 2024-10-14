// draw out mesh_instance_2d & pictures (we need `root` as input, It's easier to remeber that by singleton then static class)

#ifndef RENDER_SERVER_H
#define RENDER_SERVER_H

#include "../nodes/mesh_instance_2d.h"

class RenderServer {    
public:
    static RenderServer &initialize(Viewport *root) {
        if (singleton == nullptr) {
            singleton = new RenderServer(root);
        }
        return *singleton;  // returning instance for auto destructor call of gloabl instance
    }

    static RenderServer &get_singleton() {
        ErrorHandler_FATAL_IF(singleton == nullptr, __FILE__, __LINE__);
        return *singleton;
    }

    ~RenderServer() {
        if (!is_deleting) {
            is_deleting = true;
            delete singleton; 
        }
    }

    void redraw();

private:
    static RenderServer *singleton;
    static bool is_deleting;
    Viewport *root;

    RenderServer(Viewport *root): root(root) {}
};


#endif