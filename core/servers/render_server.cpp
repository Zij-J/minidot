#include "render_server.h"

#include "../nodes/viewport.h"
#include "../nodes/sprite_2d.h"
#include "../nodes/mesh_instance_2d.h"
#include <typeinfo> // `typeid` keyword for C# `is` like behavior
#include <GL/glut.h> // for swap buffer

RenderServer *RenderServer::singleton = nullptr;
bool RenderServer::is_deleting = false; // singleton deletion by destructor will call twice, must avoid delete `nullptr` 


// default: ~~TODO: adding in tree order~~, then let OpenGL do sorting  
const RenderServer::DrawingMesh *RenderServer::new_mesh_instance_2d(Ref<Mesh> mesh, Node3D *containing_node) { 
    DrawingMesh *new_mesh = new DrawingMesh((DrawingMesh){ 
        .mesh = mesh,
        .containing_node = containing_node, 
        .before = nullptr,
        .next = nullptr,
    });

    // just add in list, let OpenGL do the sorting
    new_mesh->before = mesh_list_2d;
    new_mesh->next = mesh_list_2d->next;
    mesh_list_2d->next = new_mesh;

    return new_mesh;
}


void RenderServer::delete_mesh_instance(const DrawingMesh *mesh) { 
    mesh->before->next = mesh->next;
    if (mesh->next != nullptr) {
        mesh->next->before = mesh->before;
    }
    delete mesh;
}



void RenderServer::redraw() {
    
    Vector<Node *> dfs_stack; // use pointers for C-ish fast vector
    dfs_stack.push_back(static_cast<Node *>(root));

    glClear(GL_COLOR_BUFFER_BIT); // clear back buffer

    // draw with depth first search + leaves quene order
    // while (dfs_stack.size() != 0) {

    //     Node *now_node = dfs_stack.pop_back();
    //     for (int i = now_node->get_children().size() - 1; i >= 0; --i) { // leaves quene
    //         dfs_stack.push_back(now_node->get_children()[i]);
    //     }

    //     if (typeid(*now_node).hash_code() == typeid(MeshInstance2D).hash_code()) {  // draw MeshInstance2D 
    //         MeshInstance2D *drawing_node = static_cast<MeshInstance2D *>(now_node);
            
    //         // 2D world coordinate (Node2D)
    //         Transform2D object_transform_2d = drawing_node->get_object_transform();

    //         // -> 3D world coordinate (mesh)
    //         Transform3D final_transform = {
    //             .basis_x = (Vector3){.x = static_cast<float>(object_transform_2d.basis_x.x), .y = static_cast<float>(object_transform_2d.basis_x.y), .z = 0}, 
    //             .basis_y = (Vector3){.x = static_cast<float>(object_transform_2d.basis_y.x), .y = static_cast<float>(object_transform_2d.basis_y.y), .z = 0}, 
    //             .basis_z = (Vector3){.x = 0, .y = 0, .z = 0},   // project to z = 0
    //             .origin_offset = (Vector3){.x = static_cast<float>(object_transform_2d.origin_offset.x), .y = static_cast<float>(object_transform_2d.origin_offset.y), .z = 0}
    //         };

    //         // -> 2D viewport coordinate (camera)
    //         root->get_view_transform() *= final_transform;

    //         // -> 2D windows coordinate (windows) done by OpenGL
    //         drawing_node->mesh->draw(final_transform);     

    //     } else if (typeid(*now_node).hash_code() == typeid(Sprite2D).hash_code()) {  // draw Sprite2D
    //         Sprite2D *drawing_node = static_cast<Sprite2D *>(now_node);
    //         drawing_node->texture->draw(root->get_canva_transform() * drawing_node->get_object_transform());
    //     }
    // }

    // DrawingTexture *now_texture = texture_list->next;
    // while (now_texture != nullptr) {
    //     now_texture->texture->draw(*now_texture->global_position);
    //     now_texture = now_texture->next;
    // }


    // draw 3D first

    // draw 2D in front
    DrawingMesh *now_mesh = mesh_list_2d->next;
    while (now_mesh != nullptr) {
        now_mesh->mesh->draw(root->get_view_transform_2d() * now_mesh->containing_node->get_object_transform());
        now_mesh = now_mesh->next;
    }

    glutSwapBuffers();  // swap back buffer to front
}


