// draw out mesh_instance_2d & pictures (we need `root` as input, It's easier to remeber that by singleton then static class)

#ifndef RENDER_SERVER_H
#define RENDER_SERVER_H

#include "../resources/texture.h"
#include "../resources/mesh.h"
#include "../nodes/point_light.h"
#include "../nodes/node_2d.h"
#include "../nodes/node_3d.h"
#include "../nodes/viewport.h"
#include "../templates/ref.h"


// used in RenderServer, can't be nested class because of mutual inclusion of PointLight/RenderServer
// nested class simply can't forward declaration 
class ServerDrawingObject {
public:
    ServerDrawingObject *before = nullptr;
    ServerDrawingObject *next = nullptr;

    ServerDrawingObject() {}
    virtual ~ServerDrawingObject() {}

    virtual void draw_by(const Viewport &drawing_viewport) = 0;
};


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

    const ServerDrawingObject *new_mesh_instance_2d(Ref<Mesh> mesh, Node2D *containing_node);
    const ServerDrawingObject *new_mesh_instance_3d(Ref<Mesh> mesh, Node3D *containing_node);
    const ServerDrawingObject *new_light_3d(PointLight *light);
    void delete_drawing_object(const ServerDrawingObject *mesh);

    void redraw();

private:

    // struct DrawingTexture {
    //     Texture *texture;
    //     Node2D *containing_node;
    //     DrawingTexture *next;
    // };
    class DrawingMesh2D: public ServerDrawingObject {
    public:
        Ref<Mesh> mesh;
        Node2D *containing_node;
        
        DrawingMesh2D(): containing_node(nullptr) {} // empty consturct
        DrawingMesh2D(Ref<Mesh> mesh, Node2D *containing_node): mesh(mesh), containing_node(containing_node) {}
        ~DrawingMesh2D() {}

        void draw_by(const Viewport &drawing_viewport);
    };
    class DrawingMesh3D: public ServerDrawingObject {
    public:
        Ref<Mesh> mesh;
        Node3D *containing_node;
        
        DrawingMesh3D(): containing_node(nullptr) {} // empty consturct
        DrawingMesh3D(Ref<Mesh> mesh, Node3D *containing_node): mesh(mesh), containing_node(containing_node) {}
        ~DrawingMesh3D() {}

        void draw_by(const Viewport &drawing_viewport);
    };
    class DrawingLight3D: public ServerDrawingObject { // light follow data structure mesh, for future expansion & lazy to change
    public:
        PointLight *containing_light;

        DrawingLight3D(): containing_light(nullptr) {} // empty consturct
        DrawingLight3D(PointLight *containing_light): containing_light(containing_light) {}
        ~DrawingLight3D() {}

        void draw_by(const Viewport &drawing_viewport);
    };


    static RenderServer *singleton;
    static bool is_deleting;

    Viewport *root = nullptr; 
    // DrawingTexture *texture_list = new DrawingTexture;  // head noded list
    DrawingMesh2D *mesh_list_2d = new DrawingMesh2D;           // all drawables in 2D
    DrawingMesh3D *mesh_list_3d = new DrawingMesh3D;           // all drawables in 3D
    DrawingLight3D *light_list_3d = new DrawingLight3D;           // all lights in 3D

    RenderServer(Viewport *root): root(root) {
        // texture_list->next = nullptr;
        mesh_list_2d->next = nullptr;
        mesh_list_3d->next = nullptr;
        light_list_3d->next = nullptr;
    }
};


#endif