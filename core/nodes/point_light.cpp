#include "point_light.h"
#include "../servers/render_server.h"
#include "../error/error_handler.h"
#include <utility> // std::move
#include <GL/glut.h>
#include <GL/gl.h>

int PointLight::total_light_count = 0;



PointLight::PointLight(): server_light(RenderServer::get_singleton().new_light_3d(this)) {
    tree_entered.add_listener<PointLight, &PointLight::on_tree_entered>(this);
}
PointLight::PointLight(float x, float y, float z, Color color, float ambient, float diffuse, float specular): 
Node3D(x, y, z),
ambient(ambient),
diffuse(diffuse),
specular(specular),
color(std::move(color)),
server_light(RenderServer::get_singleton().new_light_3d(this)) {
    tree_entered.add_listener<PointLight, &PointLight::on_tree_entered>(this);
}

PointLight::~PointLight() {
    RenderServer::get_singleton().delete_drawing_object(server_light);
    glDisable(GL_LIGHT0 + light_index);
    --total_light_count;
}

void PointLight::on_tree_entered() {
    ErrorHandler_FATAL_IF(total_light_count == 8, __FILE__, __LINE__); // reach OpenGL max light amount

    // index counting
    light_index = total_light_count; 
    ++total_light_count;
}


// light on, must update in every frame to ma
void PointLight::draw_light() {
    GLfloat light_ambient[] = {color.r * ambient, color.g * ambient, color.b * ambient, 1.0}; // hard written ambient, alpha only for speical blending light
    glLightfv(GL_LIGHT0 + light_index, GL_AMBIENT, light_ambient);
    GLfloat light_diffuse[] = {color.r * diffuse, color.g * diffuse, color.b * diffuse, 1.0}; // main contribution is diffuse, color is it!
    glLightfv(GL_LIGHT0 + light_index, GL_DIFFUSE, light_diffuse);
    GLfloat light_specular[] = {color.r * specular, color.g * specular, color.b * specular, 1.0}; // hard written specular(specular kinda hard to see, make it as strong as possible)
    glLightfv(GL_LIGHT0 + light_index, GL_SPECULAR, light_specular);
    
    Vector3 position = get_object_transform().origin_offset;
    GLfloat light_position[] = {position.x, position.y, position.z, 1.0}; // 1.0 > point light 
    glLightfv(GL_LIGHT0 + light_index, GL_POSITION, light_position);
    
    glEnable(GL_LIGHT0 + light_index);
}

void PointLight::change_color(Color new_color) {
    color = new_color;
    GLfloat light_diffuse[] = {color.r, color.g, color.b, 1.0}; // main contribution is diffuse, color is it!
    glLightfv(GL_LIGHT0 + light_index, GL_DIFFUSE, light_diffuse);
}
