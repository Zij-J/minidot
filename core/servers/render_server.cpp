#include "render_server.h"

#include "../nodes/viewport.h"
#include "../nodes/sprite_2d.h"
#include "../debugger/code_tester.h"
#include <typeinfo> // `typeid` keyword for C# `is` like behavior
#include <GL/glut.h> // for swap buffer

RenderServer *RenderServer::singleton = nullptr;
bool RenderServer::is_deleting = false; // singleton deletion by destructor will call twice, must avoid delete `nullptr` 


void RenderServer::redraw() {
    
    Vector<Node *> dfs_stack; // use pointers for C-ish fast vector
    dfs_stack.push_back(static_cast<Node *>(root));

    glClear(GL_COLOR_BUFFER_BIT); // clear back buffer

    // draw with depth first search + leaves quene order
    while (dfs_stack.size() != 0) {

        Node *now_node = dfs_stack.pop_back();
        for (int i = now_node->get_childs().size() - 1; i >= 0; --i) { // leaves quene
            dfs_stack.push_back(now_node->get_childs()[i]);
        }

        if (typeid(*now_node).hash_code() == typeid(MeshInstance2D).hash_code()) {  // draw MeshInstance2D 
            MeshInstance2D *drawing_node = static_cast<MeshInstance2D *>(now_node);
            
            // 2D world coordinate (Node2D)
            Transform2D object_transform_2d = drawing_node->get_object_transform();

            // -> 3D world coordinate (mesh)
            Transform3D final_transform = {
                .basis_x = (Vector3){.x = static_cast<float>(object_transform_2d.basis_x.x), .y = static_cast<float>(object_transform_2d.basis_x.y), .z = 0}, 
                .basis_y = (Vector3){.x = static_cast<float>(object_transform_2d.basis_y.x), .y = static_cast<float>(object_transform_2d.basis_y.y), .z = 0}, 
                .basis_z = (Vector3){.x = 0, .y = 0, .z = 0},   // project to z = 0
                .origin_offset = (Vector3){.x = static_cast<float>(object_transform_2d.origin_offset.x), .y = static_cast<float>(object_transform_2d.origin_offset.y), .z = 0}
            };

            // -> 2D viewport coordinate (camera)
            root->get_view_transform() *= final_transform;

            // -> 2D windows coordinate (windows) done by OpenGL
            drawing_node->mesh->draw(final_transform);     

        } else if (typeid(*now_node).hash_code() == typeid(Sprite2D).hash_code()) {  // draw Sprite2D
            Sprite2D *drawing_node = static_cast<Sprite2D *>(now_node);
            drawing_node->texture->draw(root->get_canva_transform() * drawing_node->get_object_transform());
        }
    }

    glutSwapBuffers();  // swap back buffer to front
}