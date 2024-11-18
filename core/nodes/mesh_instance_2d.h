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
    server_mesh(RenderServer::get_singleton().new_mesh_instance_2d(mesh, this)) {}   

    MeshInstance2D(float x, float y, Ref<Mesh> mesh)
    : Node2D(x, y),
    mesh(mesh), 
    server_mesh(RenderServer::get_singleton().new_mesh_instance_2d(mesh, this)) {}  
    
    ~MeshInstance2D() {RenderServer::get_singleton().delete_drawing_object(server_mesh);}

    // drawing function is implemented in Mesh, reducing a layer of function call 

private:    
    const RenderServer::DrawingObject *server_mesh;
};


#endif