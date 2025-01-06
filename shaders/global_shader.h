// In Godot, shaders is resources instanced in material, which only affect the object it attaches. 
// I have no idea how to that! Hence shaders are outside of node system, for now.  
//      I know glUseProgram(0) maybe can reset, but that is invaild? It still reset in my opnegl anyway. I just confused...
// https://docs.godotengine.org/en/stable/tutorials/shaders/your_first_shader/your_first_2d_shader.html
// https://stackoverflow.com/questions/13546461/what-does-gluseprogram0-do

#ifndef GLOBAL_SHADER
#define GLOBAL_SHADER

#include <GL/glew.h>
#include <string>

using namespace std;


class GlobalShader
{
public:
    GlobalShader();
    ~GlobalShader();

    // file: path relative to exectuable, shader_type: pass in "glCreateShader"
    void compile_and_link(string file, GLuint shader_type); 

    void start_shading();

private:

    GLuint program;
};



#endif