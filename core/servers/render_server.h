// draw out mesh_instance_2d & pictures (we need `root` as input, It's easier to remeber that by singleton then static class)

#ifndef RENDER_SERVER_H
#define RENDER_SERVER_H

#include "../resources/texture.h"
#include "../resources/mesh.h"
#include "../nodes/node_3d.h"
#include "../templates/ref.h"

class RenderServer {    
public:
    struct DrawingMesh {
        Ref<Mesh> mesh;
        Node3D *containing_node;
        DrawingMesh *before;
        DrawingMesh *next;
    };

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

    // void new_texture_instance(Texture *texture, Transform2D *global_position, const Texture *before) { // if has no before, pass nullptr 
    //     DrawingTexture *new_texture = new DrawingTexture;
    //     new_texture->global_position = global_position;
    //     new_texture->texture = texture;

    //     // add as next of `before` 
    //     DrawingTexture *now_texture = texture_list;
    //     while (now_texture->texture != before) { // if nullptr, will stop at head node
    //         now_texture = now_texture->next;
    //     }
    //     new_texture->next = now_texture->next;
    //     now_texture->next = new_texture;
    // }

    const DrawingMesh *new_mesh_instance_2d(Ref<Mesh> mesh, Node3D *containing_node);
    void delete_mesh_instance(const DrawingMesh *mesh);

    void redraw();

private:

    // struct DrawingTexture {
    //     Texture *texture;
    //     Node2D *containing_node;
    //     DrawingTexture *next;
    // };


    static RenderServer *singleton;
    static bool is_deleting;

    Viewport *root = nullptr; 
    // DrawingTexture *texture_list = new DrawingTexture;  // head noded list
    DrawingMesh *mesh_list_2d = new DrawingMesh;           // all drawables in 2D
    DrawingMesh *mesh_list_3d = new DrawingMesh;           // all drawables in 3D

    RenderServer(Viewport *root): root(root) {
        // texture_list->next = nullptr;
        mesh_list_2d->next = nullptr;
        mesh_list_3d->next = nullptr;
    }
};


#endif