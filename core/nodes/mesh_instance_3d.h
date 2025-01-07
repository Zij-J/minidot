// Node3D with a mesh stored, can draw mesh

#ifndef MESH_INSTANCE_3D_H
#define MESH_INSTANCE_3D_H

#include "node_3d.h"
#include "../templates/ref.h"
#include "../resources/mesh.h"
#include "../servers/render_server.h"


class MeshInstance3D: public Node3D {    
public:
    Ref<Mesh> mesh;    

    MeshInstance3D(Ref<Mesh> mesh)  // init with `Ref`, otherwise will got 2 seperate Ref in Node/Server
    : Node3D(0.0f, 0.0f),
    mesh(mesh), 
    server_mesh(RenderServer::get_singleton().new_mesh_instance_3d(mesh, this)) {}   

    MeshInstance3D(float x, float y, float z, Ref<Mesh> mesh)
    : Node3D(x, y, z),
    mesh(mesh), 
    server_mesh(RenderServer::get_singleton().new_mesh_instance_3d(mesh, this)) {}  
    
    ~MeshInstance3D() {RenderServer::get_singleton().delete_drawing_object(server_mesh);}

    // drawing function is implemented in Mesh, reducing a layer of function call 

private:    
    const ServerDrawingObject *server_mesh;
};


#endif