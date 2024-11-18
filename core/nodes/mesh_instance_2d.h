// Node2D with a mesh stored, can draw mesh

#ifndef MESH_INSTANCE_2D_H
#define MESH_INSTANCE_2D_H

#include "node_2d.h"
#include "node_3d.h"
#include "../templates/ref.h"
#include "../resources/mesh.h"
#include "../servers/render_server.h"


class MeshInstance2D: public Node2D {    
public:
    Ref<Mesh> mesh;    

    MeshInstance2D(Ref<Mesh> mesh)  // init with `Ref`, otherwise will got 2 seperate Ref in Node/Server
    : Node2D(0.0f, 0.0f),
    mesh(mesh), 
    // server_node(0.0f, 0.0f, 0.0f), // update when enter tree
    server_mesh(RenderServer::get_singleton().new_mesh_instance_2d(mesh, &server_node)) {
        tree_entered.add_listener<MeshInstance2D, on_tree_entered>(this);
        server_node.transform_changed.add_listener<MeshInstance2D, on_server_node_transform_changed>(this);
    }   

    MeshInstance2D(float x, float y, Ref<Mesh> mesh)
    : Node2D(x, y),
    mesh(mesh), 
    // server_node(x, y, 0.0f),  // update when enter tree
    server_mesh(RenderServer::get_singleton().new_mesh_instance_2d(mesh, &server_node)) {
        tree_entered.add_listener<MeshInstance2D, on_tree_entered>(this);
        server_node.transform_changed.add_listener<MeshInstance2D, on_server_node_transform_changed>(this);
    }  
    
    ~MeshInstance2D() { RenderServer::get_singleton().delete_mesh_instance(server_mesh);}

    void on_tree_entered();
    void on_server_node_transform_changed();


    // drawing function is implemented in Mesh, reducing a layer of function call 

private:    
    Node3D server_node; // hack Node3D for workable MeshInstance"2D"
    const RenderServer::DrawingMesh *server_mesh;

    void _sync_server_node_transform() {
        Transform2D object_transform_2d = get_object_transform(); 
        server_node.set_local_transform((Transform3D){
            .basis_x = (Vector3){.x = static_cast<float>(object_transform_2d.basis_x.x), .y = static_cast<float>(object_transform_2d.basis_x.y), .z = 0}, 
            .basis_y = (Vector3){.x = static_cast<float>(object_transform_2d.basis_y.x), .y = static_cast<float>(object_transform_2d.basis_y.y), .z = 0}, 
            .basis_z = (Vector3){.x = 0, .y = 0, .z = 0},   // project to z = 0
            .origin_offset = (Vector3){.x = static_cast<float>(object_transform_2d.origin_offset.x), .y = static_cast<float>(object_transform_2d.origin_offset.y), .z = 0}
        });
    }
};


#endif