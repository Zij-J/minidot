// entry point of this program

#include "config/project_setting.h"
#include "nodes/node.h"
#include "nodes/node_2d.h"
#include "nodes/mesh_instance_2d.h" 
#include "nodes/mesh_instance_3d.h" 
#include "nodes/viewport.h"
#include "nodes/sprite_2d.h"
#include "nodes/camera_3d.h"
#include "nodes/point_light.h"
#include "resources/point_mesh.h" 
#include "resources/line_mesh.h" 
#include "resources/array_mesh.h" 
#include "resources/box_mesh.h" 
#include "servers/render_server.h"
#include <GL/glut.h>
#include <GL/gl.h>

void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void idle(void);


// glut called `exit`, only C like global objects will call destructor when `exit`(tested). Sorry RAII
Viewport root;  
RenderServer render_server = RenderServer::initialize(&root);    // initialization can be executed outside `main`!


int main(int argc, char **argv) {

    // glut init
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_DEPTH); // rgb + double buffer + multi sample anti-aliasing + sort by depth
    glutInit(&argc, argv);  // init all glut internal stuff 

    glutInitWindowSize(ProjectSetting::get_singleton().window_width , ProjectSetting::get_singleton().window_height);
    glutCreateWindow(ProjectSetting::get_singleton().window_name);


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glEnable(GL_COLOR_MATERIAL);  // when light, glColor become material


    // game init
    root.add_child(new MeshInstance2D(new PointMesh((Color){.r = 0, .g = 1, .b = 0}, 500, 250, 0, 2)));
    root.add_child(new Node);
    root.add_child(new Node2D);

    // Node *world = new Node2D(1000, 100);
    Node2D *a = new Node2D();
    
    root.add_child(a);
    root.add_child(new Node);
    root.add_child(new Node);
    a->add_child(new MeshInstance2D(500, 250, new LineMesh((Vector3){.x = 0, .y = 0}, (Vector3){.x = 100, .y = 100})));
    a->add_child(new MeshInstance2D(100, 250, new LineMesh((Color){.r = 1, .g = 0, .b = 0}, (Vector3){.x = 0, .y = 0}, (Vector3){.x = 100, .y = 100}, 10)));
    a->add_child(new MeshInstance2D(0, 250, new LineMesh((Color){.r = 1, .g = 0, .b = 0}, (Color){.r = 1, .g = 0, .b = 1}, (Vector3){.x = 0, .y = 0}, (Vector3){.x = 100, .y = 100})));
    // a->add_child(new Sprite2D(0, 0, new Texture("supermario.bmp", 512, 512)));
    root.add_child(new MeshInstance2D(new PointMesh(1000, 500)));
    a->translate(700, 0);
    
    ArrayMesh *array_mesh = new ArrayMesh();
    array_mesh->points.push_back({.x = 0, .y = 100});
    array_mesh->points.push_back({.x = 100, .y = 100});
    array_mesh->points.push_back({.x = 100, .y = 0});
    MeshInstance2D *b = new MeshInstance2D(array_mesh);
    root.add_child(b);
    // b->rotate(30);

    ArrayMesh *array_mesh2 = new ArrayMesh();
    array_mesh2->points.push_back({.x = 0, .y = 100});
    array_mesh2->points.push_back({.x = 100, .y = 100});
    array_mesh2->points.push_back({.x = 100, .y = 0});
    array_mesh2->colors.push_back({.r = 1, .g = 0, .b = 0});
    array_mesh2->colors.push_back({.r = 0, .g = 1, .b = 0});
    root.add_child(new MeshInstance2D(400, 40, array_mesh2));


    Node3D *tank = new Node3D;
    root.add_child(tank);
    // tank->translate(0.5, 0, 0); 
    // tank->translate(0, 0.5, 0); // up is y
    // tank->translate(0, 0, 0.5); 

    { // let `Ref<Mesh>` freeable (`glutMainLoop` won't finish `main` scope)
        float box_width = 0.5f; 
        ArrayMesh* box_mesh = new ArrayMesh();
        box_mesh->points.push_back({.x = -box_width / 2, .y = -box_width / 2, .z = -box_width / 2});
        box_mesh->points.push_back({.x = -box_width / 2, .y = box_width / 2, .z = -box_width / 2});
        box_mesh->points.push_back({.x = box_width / 2, .y = -box_width / 2, .z = -box_width / 2});
        box_mesh->points.push_back({.x = box_width / 2, .y = box_width / 2, .z = -box_width / 2});
        box_mesh->points.push_back({.x = -box_width / 2, .y = -box_width / 2, .z = box_width / 2});
        box_mesh->points.push_back({.x = -box_width / 2, .y = box_width / 2, .z = box_width / 2});
        box_mesh->points.push_back({.x = box_width / 2, .y = -box_width / 2, .z = box_width / 2});
        box_mesh->points.push_back({.x = box_width / 2, .y = box_width / 2, .z = box_width / 2});
        Ref<Mesh> ref_box_mesh = box_mesh;
        tank->add_child(new MeshInstance3D(ref_box_mesh));
        tank->add_child(new MeshInstance3D(1.5f, 0.0f, 0.0f, ref_box_mesh));
        // tank->rotate_x(45);
        tank->rotate_y(45);
        // tank->rotate_z(45);

        tank->add_child(new MeshInstance3D(-1.5f, 0.0f, 0.0f, new BoxMesh((Color){.r = 0, .g = 1, .b = 0}, 0.5f, 1.0f, 1.75f)));

        Camera3D *camera = new Camera3D(0, 0, 5);
        tank->add_child(camera);
        camera->translate(0.0f, 1.0f, 0);
        camera->rotate_x(-30.0f);
        tank->translate(0.5, 0, 0); 


        tank->add_child(new PointLight(0.0f, 0.0f, 0.0f, (Color){.r = 0, .g = 1, .b = 1}));
    }


    RenderServer::get_singleton().redraw();


    // game start!
    glutMainLoop();

    return 0;
}



void display() {
    // RenderServer::get_singleton().redraw();
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
        case 'W' :
            break;
    }
}

void idle(void) {
    // t += dt;
    glutPostRedisplay(); // next MainLoop need redraw
}
