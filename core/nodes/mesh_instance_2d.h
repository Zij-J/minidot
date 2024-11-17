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
    : Node2D(0.0f, 0.0f), // need remove!
    mesh(mesh), 
    server_node(0.0f, 0.0f, 0.0f),
    server_mesh(RenderServer::get_singleton().new_mesh_instance(mesh, &server_node)) {}   

    MeshInstance2D(int x, int y, Ref<Mesh> mesh)
    : Node2D(x, y), // need remove!
    mesh(mesh), 
    server_node(x, y, 0.0f),
    server_mesh(RenderServer::get_singleton().new_mesh_instance(mesh, &server_node)) {}  
    
    ~MeshInstance2D() { RenderServer::get_singleton().delete_mesh_instance(server_mesh);}

    void on_tree_entered() {
        
    }
    // drawing function is implemented in Mesh, reducing a layer of function call 

private:    
    Node3D server_node; // hack Node3D for workable MeshInstance"2D"
    const RenderServer::DrawingMesh *server_mesh;
};


#endif