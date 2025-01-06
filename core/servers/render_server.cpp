#include "render_server.h"
#include "../debugger/code_tester.h"

#include <GL/glut.h> // for swap buffer
#include <GL/glu.h> // for reset camera
#include <GL/gl.h> // for reset camera

RenderServer *RenderServer::singleton = nullptr;
bool RenderServer::is_deleting = false; // singleton deletion by destructor will call twice, must avoid delete `nullptr` 


// default: ~~TODO: adding in tree order~~, then let OpenGL do sorting  
const RenderServer::DrawingObject *RenderServer::new_mesh_instance_2d(Ref<Mesh> mesh, Node2D *containing_node) { 
    DrawingMesh2D *new_mesh = new DrawingMesh2D(mesh, containing_node);

    // just add in list, let OpenGL do the sorting
    new_mesh->before = mesh_list_2d;
    new_mesh->next = mesh_list_2d->next;
    if (mesh_list_3d->next != nullptr) {
        mesh_list_3d->next->before = new_mesh; 
    }
    mesh_list_2d->next = new_mesh;

    return new_mesh;
}


// really just add in list, let OpenGL do the sorting
const RenderServer::DrawingObject *RenderServer::new_mesh_instance_3d(Ref<Mesh> mesh, Node3D *containing_node) { 
    DrawingMesh3D *new_mesh = new DrawingMesh3D(mesh, containing_node);

    new_mesh->before = mesh_list_3d;
    new_mesh->next = mesh_list_3d->next;
    mesh_list_3d->next = new_mesh;

    return new_mesh;
}


void RenderServer::delete_drawing_object(const DrawingObject *mesh) { 
    mesh->before->next = mesh->next;
    if (mesh->next != nullptr) {
        mesh->next->before = mesh->before;
    }
    delete mesh;
}



void RenderServer::redraw() {
    
    Vector<Node *> dfs_stack; // use pointers for C-ish fast vector
    dfs_stack.push_back(static_cast<Node *>(root));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear back buffer & depth buffer

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
    //             .basis_x = (Vector3){.x = object_transform_2d.basis_x.x), .y = object_transform_2d.basis_x.y), .z = 0}, 
    //             .basis_y = (Vector3){.x = object_transform_2d.basis_y.x), .y = object_transform_2d.basis_y.y), .z = 0}, 
    //             .basis_z = (Vector3){.x = 0, .y = 0, .z = 0},   // project to z = 0
    //             .origin_offset = (Vector3){.x = object_transform_2d.origin_offset.x), .y = object_transform_2d.origin_offset.y), .z = 0}
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


    // draw 3D first (only has Mesh)
    if (root->get_camera_3d() != nullptr) {
        
        glEnable(GL_LIGHTING); // has light 
        glEnable(GL_COLOR_MATERIAL);  // when light, glColor become material
        glEnable(GL_DEPTH_TEST);  // Enable depth testing(sort by depth)

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0f, static_cast<double>(ProjectSetting::get_singleton().window_width) / static_cast<double>(ProjectSetting::get_singleton().window_height), 0.1f, 10.0f); // view angle 60 degree,
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        const Transform3D &camera_transform = root->get_camera_3d()->get_object_transform();
        Vector3 look_at_point = camera_transform.origin_offset - camera_transform.basis_z; // look at vector `camera -z` => look at point = `camera_pos` - `camera -z`
        gluLookAt(camera_transform.origin_offset.x, camera_transform.origin_offset.y, camera_transform.origin_offset.z,  // eye at `camera_pos`
                look_at_point.x, look_at_point.y, look_at_point.z,
                camera_transform.basis_y.x, camera_transform.basis_y.y, camera_transform.basis_y.z);  // up is `camera +y`


        DrawingMesh3D *now_mesh = static_cast<DrawingMesh3D *>(mesh_list_3d->next);
        while (now_mesh != nullptr) {
            now_mesh->mesh->draw(now_mesh->containing_node->get_object_transform()); // expend `draw_by` to reduce function call
            now_mesh = static_cast<DrawingMesh3D *>(now_mesh->next);
        }


        // reset camera
        glDisable(GL_LIGHTING); // 2D no light
        glDisable(GL_COLOR_MATERIAL);  // 2D no need material
        glDisable(GL_DEPTH_TEST);  // 2D no sort by depth
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-1, 1, -1, 1); // origin OpenGL setting
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0); // eye at (0, 0, 1), look at (0, 0, 0), up is (0, 1, 0)
    }

    // draw 2D in front
    DrawingObject *now_object = mesh_list_2d->next;
    while (now_object != nullptr) {
        now_object->draw_by(*root);
        now_object = now_object->next;
    }

    glutSwapBuffers();  // swap back buffer to front
}


void RenderServer::DrawingMesh2D::draw_by(const Viewport &drawing_viewport) {
    // 2D world coordinate (Node2D)
    Transform2D object_transform_2d = containing_node->get_object_transform();
    // to 3D
    Transform3D final_transform = {
        .basis_x = (Vector3){.x = object_transform_2d.basis_x.x, .y = object_transform_2d.basis_x.y, .z = 0}, 
        .basis_y = (Vector3){.x = object_transform_2d.basis_y.x, .y = object_transform_2d.basis_y.y, .z = 0}, 
        .basis_z = (Vector3){.x = 0, .y = 0, .z = 0},   // project to z = 0
        .origin_offset = (Vector3){.x = object_transform_2d.origin_offset.x, .y = object_transform_2d.origin_offset.y, .z = 0}
    };

    // draw
    mesh->draw(drawing_viewport.get_view_transform_2d() * final_transform);
}


void RenderServer::DrawingMesh3D::draw_by(const Viewport &drawing_viewport) {
    // mesh->draw(drawing_viewport.get_view_transform() * containing_node->get_object_transform());
    mesh->draw(containing_node->get_object_transform()); // let openGL do camera_transform + projection_transform 
}