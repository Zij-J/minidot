#include "point_light.h"
#include "../error/error_handler.h"
#include <utility> // std::move
#include <GL/glut.h>
#include <GL/gl.h>

int PointLight::total_light_count = 0;


PointLight::PointLight() {
    tree_entered.add_listener<PointLight, on_tree_entered>(this);
}
PointLight::PointLight(float x, float y, float z, Color color): 
Node3D(x, y, z),
color(std::move(color)) {
    tree_entered.add_listener<PointLight, on_tree_entered>(this);
}

PointLight::~PointLight() {
    glDisable(GL_LIGHT0 + light_index);
    --total_light_count;
}

void PointLight::on_tree_entered() {
    ErrorHandler_FATAL_IF(total_light_count == 8, __FILE__, __LINE__); // reach OpenGL max light amount

    // index counting
    light_index = total_light_count; 
    ++total_light_count;

    // light on
    GLfloat light_ambient[] = {0.01, 0.01, 0.01, 1.0}; // hard written ambient, alpha only for speical blending light
    glLightfv(GL_LIGHT0 + light_index, GL_AMBIENT, light_ambient);
    GLfloat light_diffuse[] = {color.r, color.b, color.b, 1.0}; // main contribution is diffuse, color is it!
    glLightfv(GL_LIGHT0 + light_index, GL_DIFFUSE, light_diffuse);
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0}; // hard written specular(specular kinda hard to see, make it as strong as possible)
    glLightfv(GL_LIGHT0 + light_index, GL_SPECULAR, light_specular);
    
    Vector3 position = get_object_transform().origin_offset;
    GLfloat light_position[] = {position.x, position.y, position.z, 1.0}; // 1.0 > point light 
    glLightfv(GL_LIGHT0 + light_index, GL_POSITION, light_position);
    
    glEnable(GL_LIGHT0 + light_index);
}