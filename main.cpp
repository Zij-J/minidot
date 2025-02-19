// entry point of this program

#include <GL/glew.h> // glew.h must included before any gl.h 
#include <GL/glut.h>
#include <GL/gl.h>

#include "core/config/project_setting.h"
#include "core/nodes/node.h"
#include "core/nodes/node_2d.h"
#include "core/nodes/mesh_instance_2d.h" 
#include "core/nodes/mesh_instance_3d.h" 
#include "core/nodes/viewport.h"
#include "core/nodes/sprite_2d.h"
#include "core/nodes/camera_3d.h"
#include "core/nodes/point_light.h"
#include "core/resources/point_mesh.h" 
#include "core/resources/line_mesh.h" 
#include "core/resources/array_mesh.h" 
#include "core/resources/box_mesh.h" 
#include "core/servers/render_server.h"
#include "core/thirdparty/sfm_loader/SMFLoader.h"
#include "shaders/global_shader.h"


#define DEFAULT_VERTEX_SHADER "shaders/phong_vertex.glsl"
#define DEFAULT_FRAGMENT_SHADER "shaders/phong_fragment.glsl"


void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void idle(void);



bool is_need_redraw = false;
bool is_first_person = false;
bool is_normal_light = true;


class Tank {
public:
    Node *root;
    MeshInstance3D *body;
    MeshInstance3D *head;
    Node3D *cannon_base;
    MeshInstance3D *cannon;
    Node3D *cannon_shoot_pos;

    Tank(Node *root, 
        Ref<BoxMesh> body_mesh = new BoxMesh((Color){.r = 0.6f, .g = 0.1f, .b = 0.1f}, 0.5f, 0.25f, 0.75f),
        Ref<BoxMesh> head_mesh = new BoxMesh((Color){.r = 0.8f, .g = 0.2f, .b = 0.2f}, 0.25f, 0.25f, 0.5f),
        Ref<BoxMesh> cannon_mesh = new BoxMesh((Color){.r = 0.9f, .g = 0.1f, .b = 0.1f}, 0.1f, 0.1f, 0.3f))
    : root(root), 
    body(new MeshInstance3D(0.0f, 0.125f, 0.0f, body_mesh)),
    head(new MeshInstance3D(0.0f, 0.15f, 0.0f, head_mesh)),
    cannon_base(new Node3D(0.0f, 0.05f, -0.2f)),
    cannon(new MeshInstance3D(0.0f, 0.0f, -0.15f, cannon_mesh)),
    cannon_shoot_pos(new Node3D(0.0f, 0.0f, -0.3f)) {
        root->add_child(body);
        body->add_child(head);
        head->add_child(cannon_base);
        cannon_base->add_child(cannon);
        cannon->add_child(cannon_shoot_pos);
    }

    ~Tank() {} // node tree will do the delete

    void alter_as_enemy() {
        // change color
        Ref<BoxMesh> now_mesh = body->mesh; 
        now_mesh->color.b = now_mesh->color.r;
        now_mesh->color.r = now_mesh->color.g;
        now_mesh = head->mesh; 
        now_mesh->color.b = now_mesh->color.r;
        now_mesh->color.r = now_mesh->color.g;
        now_mesh = cannon->mesh; 
        now_mesh->color.b = now_mesh->color.r;
        now_mesh->color.r = now_mesh->color.g;
    }

    void fire() {
        Node3D *start_pos = new Node3D(cannon_shoot_pos->get_object_transform());
        bullets[front] = new MeshInstance3D(bullet_mesh);
        root->add_child(start_pos);
        start_pos->add_child(bullets[front]);

        ++front;
        if (front == 16) {
            front = 0;
        }
    }

    void update_bullets() { 
        if (front == rear) { // quene empty
            return ;
        }

        is_need_redraw = true;
        for (int i = rear; i != front;) {
            bullets[i]->translate(0.0f, 0.0f, -0.01f); // cannon origin facing +x
            bullet_live_distances[i] += 0.01f;

            ++i;
            if (i == 16) {
                i = 0;
            }
        }

        if (bullet_live_distances[rear] > 5.0f) {
            delete bullets[rear]->get_parent();
            bullet_live_distances[rear] = 0;

            ++rear;
            if (rear == 16) {
                rear = 0;
            }
        }
    }

private:
    Ref<Mesh> bullet_mesh = new BoxMesh(0.075f);
    int front = 0;
    int rear = 0;
    MeshInstance3D *bullets[16];
    float bullet_live_distances[16];
    
};


// glut called `exit`, only C like global objects will call destructor when `exit`(tested). Sorry RAII
Viewport root;  
RenderServer render_server = RenderServer::initialize(&root);    // initialization can be executed outside `main`!
Tank player(&root);
Camera3D *camera;
PointLight *light;
MeshInstance2D *line0;
MeshInstance2D *line1;

int main(int argc, char **argv) {

    // glut & glew init
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_DEPTH); // rgb + double buffer + multi sample anti-aliasing + sort by depth
    glutInit(&argc, argv);  // init all glut internal stuff 

    glutInitWindowSize(ProjectSetting::get_singleton().window_width , ProjectSetting::get_singleton().window_height);
    glutCreateWindow(ProjectSetting::get_singleton().window_name);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glEnable(GL_COLOR_MATERIAL);  // when light, glColor become material

    glewInit();


    // shader init
    GlobalShader default_global_shader;
    default_global_shader.compile_and_link(DEFAULT_VERTEX_SHADER, GL_VERTEX_SHADER);
    default_global_shader.compile_and_link(DEFAULT_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
    default_global_shader.start_shading(); // replace fixed function pipeline to programmable pipeline.
    
    // // world center & light init in node system
    // root.add_child(world_center);
    // root.add_child(global_light);

    // plane!
    {
        float plane_width = 10.0f;
        ArrayMesh* plane_mesh = new ArrayMesh();
        plane_mesh->normals.push_back({.x = 0.0f, .y = 1.0f, .z = 0.0f});
        plane_mesh->points.push_back({.x = -plane_width / 2, .y = 0.0f, .z = -plane_width / 2});
        plane_mesh->points.push_back({.x = plane_width / 2, .y = 0.0f, .z = -plane_width / 2});
        plane_mesh->points.push_back({.x = -plane_width / 2, .y = 0.0f, .z = plane_width / 2});
        plane_mesh->points.push_back({.x = -plane_width / 2, .y = 0.0f, .z = plane_width / 2});
        plane_mesh->points.push_back({.x = plane_width / 2, .y = 0.0f, .z = -plane_width / 2});
        plane_mesh->points.push_back({.x = plane_width / 2, .y = 0.0f, .z = plane_width / 2});
        plane_mesh->colors.push_back((Color){.r = 0.1f, .g = 0.3f, .b = 0.1f});
        root.add_child(new MeshInstance3D(plane_mesh));
    }
    // light (not visible enought? because it's Gouraud shading...?)
    // root.add_child(new PointLight(0.0f, 0.5f, 0.0f, (Color){.r = 1.0f, .g = 1.0f, .b = 1.0f}));

    // enemy tank
    Tank *enemy = new Tank(&root); 
    enemy->alter_as_enemy();
    enemy->body->translate(3.0f, 0.0f, 0.0f);
    enemy->body->rotate_y(30.0f);
    enemy = new Tank(&root, enemy->body->mesh, enemy->head->mesh, enemy->cannon->mesh);
    enemy->body->translate(-2.0f, 0.0f, 0.0f);
    enemy->body->rotate_y(-30.0f);
    enemy = new Tank(&root, enemy->body->mesh, enemy->head->mesh, enemy->cannon->mesh);
    enemy->body->translate(0.0f, 0.0f, 1.0f);
    enemy->body->rotate_y(45.0f);


    // camera
    camera = new Camera3D(0.0f, 1.5f, 1.9f);
    camera->rotate_x(-30.0f);
    root.add_child(camera);

    // light
    light = new PointLight(0.0f, 1.9f, 0.0f, (Color){1.0f, 1.0f, 0.0f});
    root.add_child(light);


    // duck
    SMFLoader smf_loader;
    smf_loader.load("rubber_duck.smf");

    ArrayMesh *duck_mesh = new ArrayMesh();
    int faceCount = smf_loader.getFaceSize();
    for (int i = 0; i < faceCount; ++i) {
        float* faceVertices = smf_loader.getFace(i);
        float* faceColors = smf_loader.getFaceColor(i);
        float* faceNormal = smf_loader.getNormal(i);

        for (int j = 0; j < 3; ++j) { // Loop through the three vertices of the face
            if (faceColors) {
                duck_mesh->colors.push_back({faceColors[j * 3], faceColors[j * 3 + 1], faceColors[j * 3 + 2]});
            } else {
                duck_mesh->colors.push_back({1.0f, 1.0f, 1.0f});
            }
            if (faceNormal) {
                duck_mesh->normals.push_back({faceNormal[j * 3], faceNormal[j * 3 + 1], faceNormal[j * 3 + 2]});
            } else {
                duck_mesh->normals.push_back({0.0f, 1.0f, 0.0f});
            }
            duck_mesh->points.push_back({faceVertices[j * 3], faceVertices[j * 3 + 1], faceVertices[j * 3 + 2]});
        }

        delete[] faceVertices;
        if (faceColors) {
            delete[] faceColors;
        }
        if (faceNormal) {
            delete[] faceNormal;
        }
    }
    MeshInstance3D *duck = new MeshInstance3D(-1.0f, 0.0f, 0.0f, duck_mesh);
    duck->scale(0.001f);
    root.add_child(duck);


    RenderServer::get_singleton().redraw();


    // game start!
    glutMainLoop();

    return 0;
}



void display() {
    RenderServer::get_singleton().redraw();
}


void reshape(int w, int h) {
    glViewport(0,0, w, h); // resize viewport *only*, content size will be fixed 
    ProjectSetting::get_singleton().window_width = w;
    ProjectSetting::get_singleton().window_height = h;

    RenderServer::get_singleton().redraw(); // refresh window.
}


// called once per input
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'a': 
        case 'A' :
            player.body->translate(-0.01f, 0.0f, 0.0f); // +x is right
            break;
        case 'd': 
        case 'D' :
            player.body->translate(0.01f, 0.0f, 0.0f); // +x is right
            break;
        case 's' : 
        case 'S' : 
            player.body->translate(0.0f, 0.0f, 0.01f); // -z is forward
            break;        
        case 'w' : 
        case 'W' :
            player.body->translate(0.0f, 0.0f, -0.01f); // -z is forward
            break;
        case ' ' :
            player.fire();
            break;
        case 'f' : 
        case 'F' :
            delete camera;
            camera = new Camera3D();
            if (is_first_person) {
                root.add_child(camera);
                camera->rotate_x(-30.0f);
                camera->translate(0.0f, 1.5f, 2.0f);

                delete line0;
                delete line1;
            } else {
                player.cannon_base->add_child(camera);
                camera->translate(0.0f, 0.1f, 0.0f);

                Ref<LineMesh> red_line = new LineMesh(
                    {0.5f, 0.0f, 0.0f}, 
                    {-10, - 10, 0.0f},
                    {10, 10, 0.0f},
                    3
                );
                line0 = new MeshInstance2D(static_cast<float>(ProjectSetting::get_singleton().design_width) / 2.0f, static_cast<float>(ProjectSetting::get_singleton().design_height) / 2.0f, red_line);
                line1 = new MeshInstance2D(static_cast<float>(ProjectSetting::get_singleton().design_width) / 2.0f, static_cast<float>(ProjectSetting::get_singleton().design_height) / 2.0f, red_line);
                line1->rotate(90.0f);
                root.add_child(line0);
                root.add_child(line1);
            }
            is_first_person = !is_first_person;
            break;
        case 'q' : 
        case 'Q' :
            if (!is_first_person) {
                camera->translate(0.0f, 0.0f, -0.01f);
            }
            break;
        case 'e' : 
        case 'E' :
            if (!is_first_person) {
                camera->translate(0.0f, 0.0f, 0.01f);
            }
            break;
        case 'z' : 
        case 'Z' :
            if (!is_first_person) {
                camera->translate(-0.01f, 0.0f, 0.0f);
            }
            break;
        case 'c' : 
        case 'C' :
            if (!is_first_person) {
                camera->translate(0.01f, 0.0f, 0.0f);
            }
            break;
        case 'r' : 
        case 'R' :
            if (!is_first_person) {
                camera->translate(0.0f, 0.01f, 0.0f);
            }
            break;
        case 'v' : 
        case 'V' :
            if (!is_first_person) {
                camera->translate(0.0f, -0.01f, 0.0f);
            }
            break;
        case 'x' : 
        case 'X' :
            if (is_normal_light) {
                light->change_color({1.0f, 0.0f, 1.0f});
            } else {
                light->change_color({1.0f, 1.0f, 1.0f});
            }
            is_normal_light = !is_normal_light;
            break;
    }
    is_need_redraw = true;
}
void special_keyboard(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            player.cannon_base->rotate_x(1.0f);
            break;
        case GLUT_KEY_DOWN:
            player.cannon_base->rotate_x(-1.0f);
            break;
        case GLUT_KEY_LEFT:
            player.head->rotate_y(1.0f);
            break;
        case GLUT_KEY_RIGHT:
            player.head->rotate_y(-1.0f);
            break;
    }
    // glutPostRedisplay(); // next MainLoop need redraw
    is_need_redraw = true;
}

void idle(void) {
    // t += dt;
    player.update_bullets();

    if (is_need_redraw) {
        glutPostRedisplay(); // next MainLoop need redraw
        is_need_redraw = false;
    }
}
