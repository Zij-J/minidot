// entry point of this program

#include "config/project_setting.h"
#include "nodes/node.h"
#include "nodes/node_2d.h"
#include "nodes/mesh_instance_2d.h" 
#include "nodes/viewport.h"
#include "nodes/sprite_2d.h"
#include "resources/point_mesh.h" 
#include "resources/line_mesh.h" 
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
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE); // rgb + double buffer + multi sample anti-aliasing
    glutInit(&argc, argv);  // init all glut internal stuff 

    glutInitWindowSize(ProjectSetting::get_singleton().window_width , ProjectSetting::get_singleton().window_height);
    glutCreateWindow(ProjectSetting::get_singleton().window_name);


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);


    // game init
    root.add_child(new MeshInstance2D(new PointMesh(500, 250)));
    root.add_child(new Node);
    root.add_child(new Node2D);

    Node *a = new Node2D(1000, 100);
    
    
    root.add_child(a);
    root.add_child(new Node);
    root.add_child(new Node);
    a->add_child(new MeshInstance2D(500, 250, new LineMesh((Vector3){.x = 0, .y = 0}, (Vector3){.x = 100, .y = 100})));
    a->add_child(new Sprite2D(0, 0, new Texture("supermario.bmp", 512, 512)));
    root.add_child(new MeshInstance2D(new PointMesh(1000, 500)));

    // delete a;

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