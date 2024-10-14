// Node2D with a mesh stored, can draw mesh

#ifndef MESH_INSTANCE_2D_H
#define MESH_INSTANCE_2D_H

#include "node_2d.h"
#include "../resources/mesh.h"


class MeshInstance2D: public Node2D {    
public:
    Mesh *mesh;    

    MeshInstance2D(Mesh *mesh): mesh(mesh) { // C++17 guaranteed Copy Elision in (argument) assigment
    }   
    MeshInstance2D(int x, int y, Mesh *mesh): Node2D(x, y), mesh(mesh) {} 
    ~MeshInstance2D() { delete mesh; }


    // drawing function is implemented in Mesh, reducing a layer of function call 

private:    
};


#endif