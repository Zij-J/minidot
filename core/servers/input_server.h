// handling input (we need `root` as input, It's easier to remeber that by singleton then static class)

#ifndef INPUT_SERVER_H
#define INPUT_SERVER_H

#include "../nodes/mesh_instance_2d.h"

class InputServer {    
public:
    static InputServer &initialize(Viewport *root) {
        if (singleton == nullptr) {
            singleton = new InputServer(root);
        }
        return *singleton;  // returning instance for auto destructor call of gloabl instance
    }

    static InputServer &get_singleton() {
        ErrorHandler_FATAL_IF(singleton == nullptr, __FILE__, __LINE__);
        return *singleton;
    }

    ~InputServer() {
        if (!is_deleting) {
            is_deleting = true;
            delete singleton; 
        }
    }


private:
    static InputServer *singleton;
    static bool is_deleting;
    Viewport *root;

    InputServer(Viewport *root): root(root) {}
};


#endif