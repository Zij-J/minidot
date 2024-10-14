#include "texture.h"
#include "../error/error_handler.h"

Texture::Texture(const char *image_file_path, int width, int height): size{.x = width, .y = height} {
    // open image file
    int file_width, file_height, in_file_channels; 
    unsigned char *image;
    image = stbi_load(image_file_path, &file_width, &file_height, &in_file_channels, 3);
    ErrorHandler_FATAL_IF(image == NULL, __FILE__, __LINE__);

    // generate texture 
    glGenTextures(1, &texture); // generate 1 texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, file_width, file_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);    // conserve the max quality of texture

    // close image file
    stbi_image_free(image);

    // calculate vertices coordinate if later needs
    is_vertices_dirty = true;  
}


void Texture::draw(const Transform2D &final_transform) {
    // transform vertices
    if (is_vertices_dirty) {
        calculate_vertices();
    }
    Vector2 final_vertices[4];
    for (int i = 0; i < 4; ++i) {
        final_vertices[i] = final_transform * vertices[i];
    }


    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);
    
    glBegin(GL_QUADS);
        glColor3f(1.0, 1.0, 1.0); // clear color
        glTexCoord2f(0, 0); glVertex2f(final_vertices[0].x, final_vertices[0].y);   // left-up
        glTexCoord2f(1, 0); glVertex2f(final_vertices[1].x, final_vertices[1].y);   // right-up
        glTexCoord2f(1, 1); glVertex2f(final_vertices[2].x, final_vertices[2].y);   // right-down
        glTexCoord2f(0, 1); glVertex2f(final_vertices[3].x, final_vertices[3].y);   // left-down
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0); // stop using texture
}


void Texture::calculate_vertices() {
    float half_x = static_cast<float>(size.x) / 2;
    float half_y = static_cast<float>(size.y) / 2;

    vertices[0].x = -half_x; // left-up 
    vertices[0].y = -half_y;
    vertices[1].x = half_x; // right-up
    vertices[1].y = -half_y;
    vertices[2].x = half_x; // right-down
    vertices[2].y = half_y;
    vertices[3].x = -half_x; // left-down
    vertices[3].y = half_y;

    is_vertices_dirty = false;
}