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
#include "shaders/global_shader.h"


// #define DEFAULT_VERTEX_SHADER "shaders/phong_vertex.glsl"
#define DEFAULT_VERTEX_SHADER "shaders/phong_vertex.glsl"
#define DEFAULT_FRAGMENT_SHADER "shaders/phong_fragment.glsl"


void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void idle(void);


// glut called `exit`, only C like global objects will call destructor when `exit`(tested). Sorry RAII
Viewport root;  
RenderServer render_server = RenderServer::initialize(&root);    // initialization can be executed outside `main`!

Node3D *world_center = new Node3D();
PointLight *global_light = new PointLight(0.0f, 0.1f, 0.5f, (Color){0.9, 0.9, 0.9}, 0.2, 0.3, 1);

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
    
    // world center & light init in node system
    root.add_child(world_center);
    root.add_child(global_light);

    // plane!
    {
        float plane_width = 10.0f;
        ArrayMesh* plane_mesh = new ArrayMesh();
        plane_mesh->colors.push_back((Color){.r = 0.1f, .g = 1.0f, .b = 0.1f});
        plane_mesh->normals.push_back({.x = 0.0f, .y = 1.0f, .z = 0.0f});
        plane_mesh->points.push_back({.x = -plane_width / 2, .y = 0.0f, .z = -plane_width / 2});
        plane_mesh->points.push_back({.x = plane_width / 2, .y = 0.0f, .z = -plane_width / 2});
        plane_mesh->points.push_back({.x = -plane_width / 2, .y = 0.0f, .z = plane_width / 2});
        plane_mesh->points.push_back({.x = -plane_width / 2, .y = 0.0f, .z = plane_width / 2});
        plane_mesh->points.push_back({.x = plane_width / 2, .y = 0.0f, .z = -plane_width / 2});
        plane_mesh->points.push_back({.x = plane_width / 2, .y = 0.0f, .z = plane_width / 2});
        root.add_child(new MeshInstance3D(plane_mesh));
    }
    root.add_child(new MeshInstance3D(0.45, 0.25, 0.4, new BoxMesh(0.25)));
    root.add_child(new MeshInstance3D(-0.35, 0.25, 0.4, new BoxMesh(0.2)));

    // camera (look at -z)
    Camera3D *global_camera = new Camera3D(0.0f, 1.0f, 1.0f);
    global_camera->rotate_x(-45.0f);
    world_center->add_child(global_camera);


    // let room has more light
    PointLight *ambient_light = new PointLight(0.0f, 0.0f, 0.0f, (Color){0.5, 0.5, 0.5}, 0.9, 0, 0);
    root.add_child(ambient_light);

    // red cross test
    Ref<LineMesh> red_line = new LineMesh(
        {0.5f, 0.0f, 0.0f}, 
        {-10, - 10, 0.0f},
        {10, 10, 0.0f},
        3
    );
    MeshInstance2D *line0 = new MeshInstance2D(static_cast<float>(ProjectSetting::get_singleton().design_width) / 2.0f, static_cast<float>(ProjectSetting::get_singleton().design_height) / 2.0f, red_line);
    MeshInstance2D *line1 = new MeshInstance2D(static_cast<float>(ProjectSetting::get_singleton().design_width) / 2.0f, static_cast<float>(ProjectSetting::get_singleton().design_height) / 2.0f, red_line);
    line1->rotate(90.0f);
    root.add_child(line0);
    root.add_child(line1);

    RenderServer::get_singleton().redraw();


    // game start!
    glutMainLoop();

    return 0;
}



void display() {
    RenderServer::get_singleton().redraw();
    // glClear(GL_COLOR_BUFFER_BIT); // gl + 名字, 清楚!

    // // glEnable();
    // glPointSize(10.0);  // 在畫 GL_POINT 才有效
    // glShadeModel(GL_SMOOTH);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    // glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;


    // // 怎麼包裝? 看我們!
    // glBegin(GL_TRIANGLES);
    //     glColor3f(1.0, 0.0, 0.0); //r
    //     glVertex2f(0.0, 0.0); // "low" left
    //     glColor3f(0.0, 1.0, 0.0); //g
    //     glVertex2f(-1.0, 1.0);  // "uppe" left
    //     glColor3f(0.0, 0.0, 1.0); //b
    //     glVertex2f(0.0, -1.0);  // lower left
    // glEnd();

    // glutSwapBuffers();
}


void reshape(int w, int h) {
    glViewport(0,0, w, h); // resize viewport *only*, content size will be fixed 
    ProjectSetting::get_singleton().window_width = w;
    ProjectSetting::get_singleton().window_height = h;

    // glViewport(0, 0, ProjectSetting::get_singleton().window_width, ProjectSetting::get_singleton().window_height);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    RenderServer::get_singleton().redraw();
    // display(); // refresh window.
}


// called once per input
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'q': 
        case 'Q' :
            exit(EXIT_SUCCESS); 
            break;
        case 'w' : 
            world_center->rotate_x(-1);
            break;
        case 's' : 
            world_center->rotate_x(1);
            break;
        case 'a' : 
            world_center->rotate_y(-1);
            break;
        case 'd' : 
            world_center->rotate_y(1);
            break;
        case 'W' :
            global_light->translate(0, 0.05, 0);
            break;
        case 'S' :
            global_light->translate(0, -0.05, 0);
            break;
        case 'A' :
            global_light->translate(-0.05, 0, 0);
            break;
        case 'D' :
            global_light->translate(0.05, 0, 0);
            break;
    }
}

void idle(void) {
    // t += dt;
    glutPostRedisplay(); // next MainLoop need redraw
}
